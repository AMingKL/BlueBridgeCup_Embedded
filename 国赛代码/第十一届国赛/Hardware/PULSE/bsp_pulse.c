/**
  *************************************************************************************************************************
  * @file    pluse.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   蓝桥杯嵌入式扩展板两路频率可调脉冲
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_pulse.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
__IO uint32_t TIM2_Freq1=0,TIM2_Freq2=0;

/********************************************************************************/
//频率缓冲区计数 TIM2的
uint8_t Period_yichu1=0;
uint8_t Period_yichu2=0;

uint8_t capture_number_CH2 = 0;
uint8_t  capture_number_CH3 = 0;

__IO uint16_t this_time_CH2 = 0; 
__IO uint16_t last_time_CH2 = 0; 

__IO uint16_t this_time_CH3 = 0; 
__IO uint16_t last_time_CH3 = 0; 

__IO uint32_t tmp16_CH2;
__IO uint32_t tmp16_CH3;
/********************************************************************************/


/**
 * @brief  
 * @param  无 参数
 * @retval 无
 */
void tim2_capture_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //中断配置
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; 
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
  NVIC_Init(&NVIC_InitStructure);	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  TIM_TimeBaseInitStructure.TIM_Prescaler=5;        
  TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;    
  TIM_TimeBaseInitStructure.TIM_Period=0xFFFF;            
  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;    
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;         
  TIM_ICInitStructure.TIM_ICFilter = 0x0;          
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
 	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
 	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	
  TIM_Cmd(TIM2, ENABLE);

	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE); 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 

  TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
}




void TIM2_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)  
	 {
		 TIM_ClearITPendingBit(TIM2,TIM_IT_Update); 
		if(capture_number_CH3 ==1)
	      {
		      Period_yichu1=1;
          } 
		  if(capture_number_CH2 ==1)
	      {
		     Period_yichu2=1;
          } 
      }		 
     if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET) 

    {         
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2); 
        if(capture_number_CH2 == 0) 
        {            
          capture_number_CH2 = 1;            
          last_time_CH2 = TIM_GetCapture2(TIM2); 
        } 
        else if(capture_number_CH2 == 1) 
        {             
           capture_number_CH2 = 0; 
           this_time_CH2 = TIM_GetCapture2(TIM2);            
         if(Period_yichu2==1)		
		    {
			    tmp16_CH2 = ((0xFFFF - last_time_CH2) + this_time_CH2); 
         }
         else
         {
			     tmp16_CH2 = (this_time_CH2 - last_time_CH2); 	
          }	 
			    Period_yichu2=0;
         TIM2_Freq1=12000000/tmp16_CH2;
						
           }
      }

    if(TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET) 
    {        
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3); 
        if(capture_number_CH3 == 0) 
        {             
            capture_number_CH3 = 1;            
            last_time_CH3 = TIM_GetCapture3(TIM2); 
        } 
        else if(capture_number_CH3 == 1) 
        {            
				 capture_number_CH3 = 0;            
				 this_time_CH3 = TIM_GetCapture3(TIM2);  
					if(Period_yichu1==1)		
				 {
					tmp16_CH3 = ((0xFFFF - last_time_CH3) + this_time_CH3); 
					}
				 else
				 {
					tmp16_CH3 = (this_time_CH3 - last_time_CH3); 	
				 }	
					Period_yichu1=0;		
					TIM2_Freq2=12000000/tmp16_CH3;	 	
        }
    } 	
}




/*****************************************************END OF FILE*********************************************************/	






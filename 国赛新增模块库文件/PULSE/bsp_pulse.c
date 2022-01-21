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
uint32_t ch1_val,ch1_duty;
uint32_t ch2_val,ch2_duty;
uint8_t ch1_mode=0,ch2_mode=0,mode;
/**
 * @brief  
 * @param  无 参数
 * @retval 无
 */
void tim2_capture_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  /* TIM3 channel 2 pin (PA.07) configuration */

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1|GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 71;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
  
  /* TIM enable counter */
  TIM_Cmd(TIM2, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM2, TIM_IT_CC2|TIM_IT_CC3, ENABLE);
}

void TIM2_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET) 
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		
		if(mode==1) //500ms刷新一次,分时复用
		{
		 switch(ch1_mode)    //判断当前的顺序,发生中断进来按我们想要顺序执行
		 {
			case 0:                          //第一次进来中断
					ch1_val=0;             //频率值清0
					ch1_duty=0;            //占空比清0
					TIM_SetCounter(TIM2,0);     //cnt清0
					TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_Low); //触发中断的极性改为低电平
					ch1_mode=1;    //顺序变为1
					break;

			case 1: ch1_duty=TIM_GetCounter (TIM2);   //获取计数值,该值就是高电平的时间
					TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_High); //再次改变中断触发的极性
					ch1_mode=2;    //顺序变为2
					break;

			case 2: ch1_val=TIM_GetCounter (TIM2);   //获取计时值,该值就是高电平时间加低电平时间,也就是说整个周期的时间
					TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_High); 
					ch1_mode=3;  //捕获成功,告诉主函数,进行数据解析
					break;

			default:break;
		 }
		}
			else
		{
			ch1_mode=0;
			TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_High);
		}
  }
	  if(TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET) 
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		
		if(mode==1) //500ms刷新一次,分时复用
		{
		 switch(ch2_mode)    //判断当前的顺序,发生中断进来按我们想要顺序执行
		 {
			case 0:                          //第一次进来中断
					ch2_val=0;             //频率值清0
					ch2_duty=0;            //占空比清0
					TIM_SetCounter(TIM2,0);     //cnt清0
					TIM_OC3PolarityConfig(TIM2,TIM_OCPolarity_Low); //触发中断的极性改为低电平
					ch2_mode=1;    //顺序变为1
					break;

			case 1: ch2_duty=TIM_GetCounter (TIM2);   //获取计数值,该值就是高电平的时间
					TIM_OC3PolarityConfig(TIM2,TIM_OCPolarity_High); //再次改变中断触发的极性
					ch2_mode=2;    //顺序变为2
					break;

			case 2: ch2_val=TIM_GetCounter (TIM2);   //获取计时值,该值就是高电平时间加低电平时间,也就是说整个周期的时间
					TIM_OC3PolarityConfig(TIM2,TIM_OCPolarity_High); 
					ch2_mode=3;  //捕获成功,告诉主函数,进行数据解析
					break;

			default:break;
		 }
		}
			else
		{
			ch2_mode=0;
			TIM_OC3PolarityConfig(TIM2,TIM_OCPolarity_High);
		}
  }
}


/*****************************************************END OF FILE*********************************************************/	






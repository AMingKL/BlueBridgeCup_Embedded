/**
  *************************************************************************************************************************
  * @file    pluse.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   ���ű�Ƕ��ʽ��չ����·Ƶ�ʿɵ�����
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_pulse.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/
uint32_t ch1_val,ch1_duty;
uint32_t ch2_val,ch2_duty;
uint8_t ch1_mode=0,ch2_mode=0,mode;
/**
 * @brief  
 * @param  �� ����
 * @retval ��
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
		
		if(mode==1) //500msˢ��һ��,��ʱ����
		{
		 switch(ch1_mode)    //�жϵ�ǰ��˳��,�����жϽ�����������Ҫ˳��ִ��
		 {
			case 0:                          //��һ�ν����ж�
					ch1_val=0;             //Ƶ��ֵ��0
					ch1_duty=0;            //ռ�ձ���0
					TIM_SetCounter(TIM2,0);     //cnt��0
					TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_Low); //�����жϵļ��Ը�Ϊ�͵�ƽ
					ch1_mode=1;    //˳���Ϊ1
					break;

			case 1: ch1_duty=TIM_GetCounter (TIM2);   //��ȡ����ֵ,��ֵ���Ǹߵ�ƽ��ʱ��
					TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_High); //�ٴθı��жϴ����ļ���
					ch1_mode=2;    //˳���Ϊ2
					break;

			case 2: ch1_val=TIM_GetCounter (TIM2);   //��ȡ��ʱֵ,��ֵ���Ǹߵ�ƽʱ��ӵ͵�ƽʱ��,Ҳ����˵�������ڵ�ʱ��
					TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_High); 
					ch1_mode=3;  //����ɹ�,����������,�������ݽ���
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
		
		if(mode==1) //500msˢ��һ��,��ʱ����
		{
		 switch(ch2_mode)    //�жϵ�ǰ��˳��,�����жϽ�����������Ҫ˳��ִ��
		 {
			case 0:                          //��һ�ν����ж�
					ch2_val=0;             //Ƶ��ֵ��0
					ch2_duty=0;            //ռ�ձ���0
					TIM_SetCounter(TIM2,0);     //cnt��0
					TIM_OC3PolarityConfig(TIM2,TIM_OCPolarity_Low); //�����жϵļ��Ը�Ϊ�͵�ƽ
					ch2_mode=1;    //˳���Ϊ1
					break;

			case 1: ch2_duty=TIM_GetCounter (TIM2);   //��ȡ����ֵ,��ֵ���Ǹߵ�ƽ��ʱ��
					TIM_OC3PolarityConfig(TIM2,TIM_OCPolarity_High); //�ٴθı��жϴ����ļ���
					ch2_mode=2;    //˳���Ϊ2
					break;

			case 2: ch2_val=TIM_GetCounter (TIM2);   //��ȡ��ʱֵ,��ֵ���Ǹߵ�ƽʱ��ӵ͵�ƽʱ��,Ҳ����˵�������ڵ�ʱ��
					TIM_OC3PolarityConfig(TIM2,TIM_OCPolarity_High); 
					ch2_mode=3;  //����ɹ�,����������,�������ݽ���
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






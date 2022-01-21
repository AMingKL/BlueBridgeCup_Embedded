/**
  *************************************************************************************************************************
  * @file    caputer.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   蓝桥杯嵌入式扩展板两路占空比可变pwm捕获
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include  "bsp_caputer.h" 
/* 定义 -----------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  
 * @param  无 
 * @retval 无
 */
void time3_capture(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 71;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(TIM3, &TIM_ICInitStructure);

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM3, TIM_IT_CC2 | TIM_IT_CC1, ENABLE);

}


u8 mode=0,ch2_mode,ch1_mode;
u32 ch2_val,ch2_duty,ch1_val,ch1_duty;

void TIM3_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET) 
  {
    /* Clear TIM3 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    if(mode==1) //500ms刷新一次,分时复用
	{
		switch(ch2_mode)
		{
			case 0: ch2_val=0;
					ch2_duty=0;
					TIM_OC2PolarityConfig(TIM3,TIM_OCPolarity_Low);
					ch2_mode=1;
					TIM_SetCounter(TIM3,0);
					break;

			case 1: ch2_duty=TIM_GetCounter(TIM3);
					TIM_OC2PolarityConfig(TIM3,TIM_OCPolarity_High);
					ch2_mode=2;
					break;

			case 2: ch2_val=TIM_GetCounter(TIM3);
					TIM_OC2PolarityConfig(TIM3,TIM_OCPolarity_High);
					ch2_mode=3;
					break;

			default: break;

		}
	}
	else
	{
	  ch2_mode=0;
	  TIM_OC2PolarityConfig(TIM3,TIM_OCPolarity_High);
	}
  }

  if(TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET) 
  {
    /* Clear TIM3 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    if(mode==0)			  //分时复用,每500ms切换一下
	{
		switch(ch1_mode)
		{
			case 0: ch1_val=0;
					ch1_duty=0;	
					TIM_OC1PolarityConfig(TIM3,TIM_OCPolarity_Low);
					ch1_mode=1;
					TIM_SetCounter(TIM3,0);
					break;

			case 1: ch1_duty=TIM_GetCounter(TIM3);
					TIM_OC1PolarityConfig(TIM3,TIM_OCPolarity_High);
					ch1_mode=2;
					break;

			case 2: ch1_val=TIM_GetCounter(TIM3);
					TIM_OC1PolarityConfig(TIM3,TIM_OCPolarity_High);
					ch1_mode=3;
					break;

			default: break;

		}
	}
	else
	{
	  ch1_mode=0;
	  TIM_OC1PolarityConfig(TIM3,TIM_OCPolarity_High);
	}
  }
}

/*****************************************************END OF FILE*********************************************************/	







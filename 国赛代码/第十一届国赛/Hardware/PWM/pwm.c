#include "pwm.h"


__IO uint32_t CCR2_Val = 1000000/2000;//2khz

float PA7_Duty = 0.50;
_Bool PA7_Flag = 0;


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void PWM_RCC_Configuration(void)
{
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
}

/**
  * @brief  Configure the TIM3 Pins.
  * @param  None
  * @retval None
  */
void PWM_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* GPIOA Configuration:TIM3 Channel1, 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void PWM_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void PWM_Init(uint8_t PA7)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t PrescalerValue = 0;
	
  /* System Clocks Configuration */
  PWM_RCC_Configuration();

  /* GPIO Configuration */
  PWM_GPIO_Configuration();

	TIM_DeInit(TIM3);
	
	/* NVIC Configuration */
	PWM_NVIC_Configuration();
	/* ---------------------------------------------------------------------------
		TIM3 Configuration: Output Compare Toggle Mode:
		TIM3CLK = SystemCoreClock / 2,
		The objective is to get TIM3 counter clock at 1 MHz:
		 - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
		CC2 update rate = TIM3 counter clock / CCR2_Val = 2000 Hz
	----------------------------------------------------------------------------*/
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	
	if(PA7 == 0)
	{
		TIM_OC2PolarityConfig(TIM3,TIM_OCPolarity_Low);
		TIM_ForcedOC2Config(TIM3,TIM_ForcedAction_Active);
	}
	else
	{
		PA7_Duty = PA7 * 0.01;
		/* Output Compare Toggle Mode configuration: Channel2 */
		TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

		TIM_OC2Init(TIM3, &TIM_OCInitStructure);

		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
		/* TIM IT enable */
		TIM_ITConfig(TIM3,TIM_IT_CC2, ENABLE);
	}
	
	/* TIM enable counter */
	TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	uint16_t capture = 0;
  /* TIM3_CH2 toggling with frequency = 2K Hz */
  if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    capture = TIM_GetCapture2(TIM3);
		if(PA7_Flag)
			TIM_SetCompare2(TIM3, capture + CCR2_Val * PA7_Duty );
		else
			TIM_SetCompare2(TIM3, capture + CCR2_Val * (1 - PA7_Duty) );
		  PA7_Flag = !PA7_Flag;
  }
}

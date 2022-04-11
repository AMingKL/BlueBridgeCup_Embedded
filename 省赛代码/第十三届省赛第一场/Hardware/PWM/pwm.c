/**
  *************************************************************************************************************************
  * @file    pwm.c
  * @author  amkl
  * @version V1.0
  * @date    2022-04-09
  * @brief   蓝桥杯嵌入式定时器PWM配置
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "pwm.h"  
/* 定义 -----------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  定时器2初始化
 * @param  无 
 * @retval 无
 */
void TIM2_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t PrescalerValue = 0;
	uint16_t CCR1_Val = 500;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;
	
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;					/* TIM_OCMode_PWM1：在向上计数时，一旦TIMx_CNT<TIMx_CCR1时通道1为有效电平，否则为无效电平；在向下计数时，一旦TIMx_CNT>TIMx_CCR1时通道1为无效电平(OC1REF=0)，否则为有效电平(OC1REF=1)*/
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		/* 输出状态：使能OC输出*/
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;							/* 设置脉冲宽度*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			/* 输出比较极性：高*/
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;					/* TIM_OCMode_PWM1：在向上计数时，一旦TIMx_CNT<TIMx_CCR1时通道1为有效电平，否则为无效电平；在向下计数时，一旦TIMx_CNT>TIMx_CCR1时通道1为无效电平(OC1REF=0)，否则为有效电平(OC1REF=1)*/
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		/* 输出状态：使能OC输出*/
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;							/* 设置脉冲宽度*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			/* 输出比较极性：高*/
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
}



/*****************************************************END OF FILE*********************************************************/	




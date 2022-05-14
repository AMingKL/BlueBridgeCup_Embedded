/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "tim.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  TIM4初始化函数
 * @param  无
 * @retval 无
 */
void Tim4_Init(void) 
{
	/* 定义一个NVIC_InitTypeDef类型的结构体 */
	NVIC_InitTypeDef   			 NVIC_InitStructure;
	
	/* 定义一个TIM_InitTypeDef类型的结构体 */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/* 使能TIM3时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	/* 中断向量配置 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;						/* 使能按TIM4所在的外部中断通道 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; /* 抢占优先级3                  */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				/* 子优先级0                    */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						/* 使能外部中断通道             */
	NVIC_Init(&NVIC_InitStructure);						
	
	/* 定时器初始化 */
	TIM_TimeBaseStructure.TIM_Period = 10000-1; 								  /* 自动装载值为10000-1 */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;									  /* 预分频数          */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* 计数模式 向上     */
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);						 
	
	/* TIM4预分频设置:1MHZ,APB1分频系数2，TIM4时钟为36MHzx2 = 72MHz */
	TIM_PrescalerConfig(TIM4,71, TIM_PSCReloadMode_Immediate);	
	
	/* 通用定时器TIM4中断使能 */
	TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE);	
	
	/* 定时器4使能 */
	TIM_Cmd(TIM4, ENABLE);
}




/*****************************************************END OF FILE*********************************************************/	

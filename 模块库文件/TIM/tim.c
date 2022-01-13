/**
  *************************************************************************************************************************
  * @file    tim.c
  * @author  刘洪明
  * @version V1.0
  * @date    2022-01-10
  * @brief   蓝桥嵌入式开发板定时器配置
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "tim.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  TIM3初始化函数
 * @param  无
 * @retval 无
 */
void Tim3_Init(void) 
{
	/* 定义一个NVIC_InitTypeDef类型的结构体 */
	NVIC_InitTypeDef   			 NVIC_InitStructure;
	
	/* 定义一个TIM_InitTypeDef类型的结构体 */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/* 使能TIM3时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* 中断向量配置 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;						/* 使能按TIM3所在的外部中断通道 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; /* 抢占优先级3                  */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				/* 子优先级0                    */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						/* 使能外部中断通道             */
	NVIC_Init(&NVIC_InitStructure);						
	
	/* 定时器初始化 */
	TIM_TimeBaseStructure.TIM_Period = 50000; 								  /* 自动装载值为50000 */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;									  /* 预分频数          */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* 计数模式 向上     */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);						 
	
	/* TIM3预分频设置:1MHZ,APB1分频系数2，TIM3时钟为36MHzx2 = 72MHz */
	TIM_PrescalerConfig(TIM3,71, TIM_PSCReloadMode_Immediate);	
	
	/* 通用定时器TIM3中断使能 */
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);	
	
	/* 定时器3使能 */
	TIM_Cmd(TIM3, ENABLE);
}


/**
 * @brief  定时器3中断服务函数
 * @param  无
 * @retval 无
 */
void TIM3_IRQHandler(void)
{
	static uint8_t times=0;
	
	/* 判断中断发生 */
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == 1) 
	{
		/* 清除中断标志 */
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		
		/* 中断一次时间为50ms */
		times++;
			
		/* 中断20次，为1s */
		if(times>=20) 
		{
			times=0;			
		}
	}
}

/*****************************************************END OF FILE*********************************************************/	

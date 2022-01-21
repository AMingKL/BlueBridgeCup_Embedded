/**
  *************************************************************************************************************************
  * @file    tim.c
  * @author  ������
  * @version V1.0
  * @date    2022-01-10
  * @brief   ����Ƕ��ʽ�����嶨ʱ������
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "tim.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  TIM3��ʼ������
 * @param  ��
 * @retval ��
 */
void Tim3_Init(void) 
{
	/* ����һ��NVIC_InitTypeDef���͵Ľṹ�� */
	NVIC_InitTypeDef   			 NVIC_InitStructure;
	
	/* ����һ��TIM_InitTypeDef���͵Ľṹ�� */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/* ʹ��TIM3ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* �ж��������� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;						/* ʹ�ܰ�TIM3���ڵ��ⲿ�ж�ͨ�� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; /* ��ռ���ȼ�3                  */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				/* �����ȼ�0                    */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						/* ʹ���ⲿ�ж�ͨ��             */
	NVIC_Init(&NVIC_InitStructure);						
	
	/* ��ʱ����ʼ�� */
	TIM_TimeBaseStructure.TIM_Period = 50000; 								  /* �Զ�װ��ֵΪ50000 */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;									  /* Ԥ��Ƶ��          */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* ����ģʽ ����     */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);						 
	
	/* TIM3Ԥ��Ƶ����:1MHZ,APB1��Ƶϵ��2��TIM3ʱ��Ϊ36MHzx2 = 72MHz */
	TIM_PrescalerConfig(TIM3,71, TIM_PSCReloadMode_Immediate);	
	
	/* ͨ�ö�ʱ��TIM3�ж�ʹ�� */
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);	
	
	/* ��ʱ��3ʹ�� */
	TIM_Cmd(TIM3, ENABLE);
}


/**
 * @brief  ��ʱ��3�жϷ�����
 * @param  ��
 * @retval ��
 */
void TIM3_IRQHandler(void)
{
	static uint8_t times=0;
	
	/* �ж��жϷ��� */
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == 1) 
	{
		/* ����жϱ�־ */
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		
		/* �ж�һ��ʱ��Ϊ50ms */
		times++;
			
		/* �ж�20�Σ�Ϊ1s */
		if(times>=20) 
		{
			times=0;			
		}
	}
}

/*****************************************************END OF FILE*********************************************************/	

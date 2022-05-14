/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "tim.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  TIM4��ʼ������
 * @param  ��
 * @retval ��
 */
void Tim4_Init(void) 
{
	/* ����һ��NVIC_InitTypeDef���͵Ľṹ�� */
	NVIC_InitTypeDef   			 NVIC_InitStructure;
	
	/* ����һ��TIM_InitTypeDef���͵Ľṹ�� */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/* ʹ��TIM3ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	/* �ж��������� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;						/* ʹ�ܰ�TIM4���ڵ��ⲿ�ж�ͨ�� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; /* ��ռ���ȼ�3                  */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				/* �����ȼ�0                    */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						/* ʹ���ⲿ�ж�ͨ��             */
	NVIC_Init(&NVIC_InitStructure);						
	
	/* ��ʱ����ʼ�� */
	TIM_TimeBaseStructure.TIM_Period = 10000-1; 								  /* �Զ�װ��ֵΪ10000-1 */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;									  /* Ԥ��Ƶ��          */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* ����ģʽ ����     */
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);						 
	
	/* TIM4Ԥ��Ƶ����:1MHZ,APB1��Ƶϵ��2��TIM4ʱ��Ϊ36MHzx2 = 72MHz */
	TIM_PrescalerConfig(TIM4,71, TIM_PSCReloadMode_Immediate);	
	
	/* ͨ�ö�ʱ��TIM4�ж�ʹ�� */
	TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE);	
	
	/* ��ʱ��4ʹ�� */
	TIM_Cmd(TIM4, ENABLE);
}




/*****************************************************END OF FILE*********************************************************/	

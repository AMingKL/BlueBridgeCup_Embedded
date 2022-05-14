/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "delay.h" 

/* ���� -----------------------------------------------------------------------------------------------------------------*/
uint32_t TimingDelay=0;

/**
 * @brief  ��ʱ������ʼ������ʼ��������SysTick�����������жϣ��жϼ��Ϊ1ms
 * @param  ��
 * @retval ��
 */
void Delay_Init(void)
{
	SysTick_Config(SystemCoreClock / 1000);
}	

/**
 * @brief  ��ʱnMs����ʱ����
 * @param  nTime����ʱʱ�䣬��λΪMs
 * @retval ��
 */
void Delay_Ms(uint32_t nTime)
{
	TimingDelay = nTime;
	while (TimingDelay != 0);
}

/**
 * @brief  ��ʱ���жϷ�����
 * @param  ��
 * @retval ��
 */
void SysTick_Handler(void)
{
	TimingDelay--;
}

/**
 * @brief  ��ʱ10ms������������ʱ
 * @param  ��
 * @retval ��
 */
void Delay_10ms(void)
{
	uint32_t i = 30000;    
	while (i--);
}

/*****************************************************END OF FILE*********************************************************/	

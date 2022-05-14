/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "delay.h" 

/* 定义 -----------------------------------------------------------------------------------------------------------------*/
uint32_t TimingDelay=0;

/**
 * @brief  延时函数初始化，初始化并启动SysTick计数器及其中断，中断间隔为1ms
 * @param  无
 * @retval 无
 */
void Delay_Init(void)
{
	SysTick_Config(SystemCoreClock / 1000);
}	

/**
 * @brief  延时nMs的延时函数
 * @param  nTime：延时时间，单位为Ms
 * @retval 无
 */
void Delay_Ms(uint32_t nTime)
{
	TimingDelay = nTime;
	while (TimingDelay != 0);
}

/**
 * @brief  定时器中断服务函数
 * @param  无
 * @retval 无
 */
void SysTick_Handler(void)
{
	TimingDelay--;
}

/**
 * @brief  延时10ms函数：空跑延时
 * @param  无
 * @retval 无
 */
void Delay_10ms(void)
{
	uint32_t i = 30000;    
	while (i--);
}

/*****************************************************END OF FILE*********************************************************/	

/*
		PROJECT:蓝桥杯嵌入式第十三届省赛第二场-购物车
		AUTHOR:AMKL
		TIME:2022/05/22
*/

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "myconfig.h"
/* 函数定义 -----------------------------------------------------------------------------------------------------------------*/
void All_Init(void);
/* 变量申明 -----------------------------------------------------------------------------------------------------------------*/
extern uint8_t Flash_Flag;

int main(void)
{	
	All_Init();
	while(1)
	{	
		/* key-10ms检测一次 */
		if(xDelay(TIMERINDEX1,1)  == TIMEISUP)
		{		
			Key_Proc();			
		}			
		/* lcd-100ms刷新一次*/
		if(xDelay(TIMERINDEX2,10)  == TIMEISUP)
		{
			Lcd_Proc();
		}	
		if((xDelay(TIMERINDEX3,10)  == TIMEISUP)&&(Flash_Flag==1))
		{
			LED_Toggle(2);
		}	
		Led_PWM_Proc();	 		
	}
}

void All_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();	
	LED_Init();
	i2c_init();
	Key_Init();
	LCD_Init();
	Tim4_Init();
	Usart2_Init(); 
	TIM2_PWM_Init();
	SystemPara_Init();	 
}


/*****************************************************END OF FILE*********************************************************/	


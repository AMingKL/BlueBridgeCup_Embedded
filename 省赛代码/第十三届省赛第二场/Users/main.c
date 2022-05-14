/*
		PROJECT:���ű�Ƕ��ʽ��ʮ����ʡ���ڶ���-���ﳵ
		AUTHOR:AMKL
		TIME:2022/05/22
*/

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "myconfig.h"
/* �������� -----------------------------------------------------------------------------------------------------------------*/
void All_Init(void);
/* �������� -----------------------------------------------------------------------------------------------------------------*/
extern uint8_t Flash_Flag;

int main(void)
{	
	All_Init();
	while(1)
	{	
		/* key-10ms���һ�� */
		if(xDelay(TIMERINDEX1,1)  == TIMEISUP)
		{		
			Key_Proc();			
		}			
		/* lcd-100msˢ��һ��*/
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


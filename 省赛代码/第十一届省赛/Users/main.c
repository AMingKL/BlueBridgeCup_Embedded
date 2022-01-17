/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-17
  * @brief   ����Ƕ��ʽ��ʮһ��ʡ����
  *************************************************************************************************************************
  * @attention
  *
  * 
  * 
  *************************************************************************************************************************
  */
/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "myconfig.h"
/* �������� -----------------------------------------------------------------------------------------------------------------*/
void Lcd_Proc(void);
void Key_Proc(void);
void Led_Proc(void);
void PWMOutput_Proc(void);
void SystemPara_Init(void);
/* LCDʱ����ʾ���� */
uint8_t Voltstring[20];
uint8_t PA6_Dutystring[20];
uint8_t PA7_Dutystring[20];


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();	
	LED_Init();
	Key_Init();
	LCD_Init();
	Tim4_Init();
	Adc_Init();	
	SystemPara_Init();
	while(1)
	{	
		Lcd_Proc();
		Led_Proc();	
	}
}

void Key_Proc(void)
{
  Key_refresh();
	if(key_falling==B1)//�����л���
	{
	  System.Interface=(System.Interface+1)%2;
	}
	else if(key_falling==B2)//PA6ռ�ձȲ������Ӽ�
	{
	  if(System.Interface==PARAM)
		{
		  (System.PA6_Duty==90)?(System.PA6_Duty=10):(System.PA6_Duty+=10);
		}
	}
	else if(key_falling==B3)//PA7ռ�ձȲ������Ӽ�
	{
		if(System.Interface==PARAM)
		{
		  (System.PA7_Duty==90)?(System.PA7_Duty=10):(System.PA7_Duty+=10);
		}
	}
	else if(key_falling==B4)//ģʽ�л���
	{
		System.Model=(System.Model+1)%2;
	}
}

void Lcd_Proc(void)
{
	System.VoltValue=(Get_AdcAverage()*(3.3/4095))*100;	
	if(System.Interface==DATA)//������ʾ����
	{
		/*                ��Ļ��һ��                          */
		LCD_DisplayStringLine(Line0,"LQB Di 11  Jie SaiTi");
		/*                ��Ļ�ڶ���                          */
		LCD_DisplayStringLine(Line2,"      Data          ");
		/*                ��Ļ������                          */
		sprintf((char*)Voltstring,"    V:%d.%.2dV        ",System.VoltValue/100,System.VoltValue%100);
		LCD_DisplayStringLine(Line4 ,Voltstring);
		/*                ��Ļ������                          */
		if(System.Model==AUTO)
		{
		  LCD_DisplayStringLine(Line5,"    Mode:AUTO       ");
		}
		else if(System.Model==MANU)
		{
		  LCD_DisplayStringLine(Line5,"    Mode:MANU       ");
		}	
		/*                ��Ļ�ھ���                          */
		LCD_DisplayStringLine(Line9," Author:LiuHongMing ");
	}
	else//������ʾ����
	{
		/*                ��Ļ��һ��                          */
		LCD_DisplayStringLine(Line0,"LQB Di 11  Jie SaiTi");
		/*                ��Ļ�ڶ���                          */
		LCD_DisplayStringLine(Line2,"      Para          ");			
		/*                ��Ļ������                          */
		sprintf((char*)PA6_Dutystring,"    PA6:%.2d%%       ",System.PA6_Duty);
		LCD_DisplayStringLine(Line4 ,PA6_Dutystring);
		/*                ��Ļ������                          */
		sprintf((char*)PA7_Dutystring,"    PA7:%.2d%%       ",System.PA7_Duty);
		LCD_DisplayStringLine(Line5 ,PA7_Dutystring);				
		/*                ��Ļ�ھ���                          */
		LCD_DisplayStringLine(Line9," Author:LiuHongMing ");
	}
}

void Led_Proc(void)
{
  if(System.Model==AUTO)
	{
	  LED_Control(LED1,ON);
	}
	else
	{
	 LED_Control(LED1,OFF);
	}
	if(System.Interface==DATA)
	{
	 LED_Control(LED2,ON);
	}
	else
	{
	 LED_Control(LED2,OFF);
	}	
}

void PWMOutput_Proc(void)
{
  if(System.Model==MANU)
	{
	  PWM_Init(System.PA6_Duty,System.PA7_Duty);
	}
	else if(System.Model==AUTO)
	{
	  System.AUTO_Duty=(System.VoltValue*30)/100;
		PWM_Init(System.AUTO_Duty,System.AUTO_Duty);		
	} 
}



void SystemPara_Init(void)
{
  System.Model=AUTO;
	System.Interface=DATA;
	System.PA6_Duty=10;
	System.PA7_Duty=10;
}


/**
 * @brief  ��ʱ��4�жϷ�����-1ms�ж�
 * @param  ��		
 * @retval ��
 */
void TIM4_IRQHandler(void)
{
	static uint8_t key_cnt,pwm_cnt=0;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		if(++key_cnt==10) 
		{
			key_cnt=0;
			Key_Proc();			
		}
		if(++pwm_cnt==5)
		{
			pwm_cnt=0;
		  PWMOutput_Proc();
		}
	}
}

/*****************************************************END OF FILE*********************************************************/	

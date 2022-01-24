/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-24
  * @brief   ����Ƕ��ʽ��ʮһ�������
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
void Key_Proc(void);
void Lcd_Proc(void);
void ADC_Proc(void);
void PWM_Proc(void);
void Led_Proc(void);
void Pulse_Proc(void);
void SystemPara_Init(void);
/* �������� -----------------------------------------------------------------------------------------------------------------*/
uint8_t lcdstring[20];

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();	
	LED_Init();
	Key_Init();
	LCD_Init();
	Tim4_Init();
	ADCx_Init();
	tim2_capture_init();	
	SystemPara_Init();
	PWM_Init(50);
	Delay_Ms(200);
	while(1)
	{	
		Led_Proc();
		Lcd_Proc();
		ADC_Proc();
		Key_Proc();
		PWM_Proc();
		Pulse_Proc();
	}
}

void PWM_Proc(void)
{
  if(System.Follow_Pulsx==PULS1)
	{
	 CCR2_Val=1000000/TIM2_Freq1;
	}
	else if(System.Follow_Pulsx==PULS2)
	{
	 CCR2_Val=1000000/TIM2_Freq2;
	}
}

void Led_Proc(void)
{
 if(System.VRp5_Value>System.VRp6_Value)
 {
	LED_Control(System.Volt_Led,ON);
 }
 else
 {
  LED_Control(System.Volt_Led,OFF);
 }
 if(System.F1>System.F2)
 {
 	LED_Control(System.Freq_Led,ON); 
 }
 else
 {
 	LED_Control(System.Freq_Led,OFF); 
 }
}

void Key_Proc(void)
{
  if(Flag.Read_KEY)
	{
	  Flag.Read_KEY=0;
		Key_refresh();
		if(key_falling==B1)//���ü�
		{
		  System.Interface=(System.Interface+1)%2;
			if(System.Interface==DATA)//�����趨ָʾ�Ʋ���
			{
				LED_Control(System.Volt_Led,OFF);
				LED_Control(System.Freq_Led,OFF); 
				System.Volt_Led=System.Volt_TempLed;
				System.Freq_Led=System.Freq_TempLed;
			}
		}
		else if(key_falling==B2)//��ѹ���ָʾ�Ƶ�����
		{
		  if(System.Interface==PARA)
			{
				if(System.Volt_TempLed!=System.Freq_TempLed-1)
				{
					if((System.Volt_TempLed==8)&&(System.Freq_TempLed==1))
					{
					 System.Volt_TempLed=2;
					}
					else
					{
					 (System.Volt_TempLed==8)?(System.Volt_TempLed=1):(System.Volt_TempLed++);
					}
				}
				else
				{
				 (System.Volt_TempLed==7)?(System.Volt_TempLed=1):(System.Volt_TempLed+=2);
				}
			}
		}
		else if(key_falling==B3)//Ƶ�����ָʾ�Ƶ�����
		{
		  if(System.Interface==PARA)
			{
				if(System.Freq_TempLed!=System.Volt_TempLed-1)
				{
					if((System.Freq_TempLed==8)&&(System.Volt_TempLed==1))
					{
					 System.Freq_TempLed=2;
					}
					else
					{
					 (System.Freq_TempLed==8)?(System.Freq_TempLed=1):(System.Freq_TempLed++);
					}
				}
				else
				{
				 (System.Freq_TempLed==7)?(System.Freq_TempLed=1):(System.Freq_TempLed+=2);
				}
			}		 
		}
		else if(key_falling==B4)//�л���
		{
		  System.Follow_Pulsx=(System.Follow_Pulsx+1)%2;
		}		
	}
}

void ADC_Proc(void)
{
 if(Flag.Read_ADC)
 {
	 Flag.Read_ADC=0;
	 System.VRp5_Value=Get_AnalogVoltage(VRP5)*3.3/4095;
	 System.VRp6_Value=Get_AnalogVoltage(VRP6)*3.3/4095;
 }
}

void Pulse_Proc(void)
{
 if(Flag.Read_PULS)
 {
	 Flag.Read_PULS=0;
	 System.F1=TIM2_Freq1; 
	 System.F2=TIM2_Freq2; 	 
 }
}

void Lcd_Proc(void)
{
  if(System.Interface==DATA)
	{
		/*                ��Ļ��һ��                          */
		LCD_DisplayStringLine(Line0,"Di 11th LQB GuoSaiTi");
		/*                ��Ļ�ڶ���                          */
		LCD_DisplayStringLine(Line2,"       Data         ");
		/*                ��Ļ������                          */
		sprintf((char*)lcdstring,"    V1:%.1fV            ",System.VRp5_Value);
		LCD_DisplayStringLine(Line3 ,lcdstring);
		/*                ��Ļ������                          */
		sprintf((char*)lcdstring,"    V2:%.1fV            ",System.VRp6_Value);
		LCD_DisplayStringLine(Line4 ,lcdstring);
		/*                ��Ļ������                          */
		sprintf((char*)lcdstring,"    F1:%dHz             ",System.F1);
		LCD_DisplayStringLine(Line5 ,lcdstring);
		/*                ��Ļ������                          */
		sprintf((char*)lcdstring,"    F2:%dHz             ",System.F2);
		LCD_DisplayStringLine(Line6 ,lcdstring);
		/*                ��Ļ�ھ���                          */
		LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");	
	}
	else if(System.Interface==PARA)
	{
		/*                ��Ļ��һ��                          */
		LCD_DisplayStringLine(Line0,"Di 11th LQB GuoSaiTi");
		/*                ��Ļ�ڶ���                          */
		LCD_DisplayStringLine(Line2,"       Para         ");
		/*                ��Ļ������                          */
		LCD_DisplayStringLine(Line3,"                    ");
		/*                ��Ļ������                          */
		sprintf((char*)lcdstring,"    VD:LD%d            ",System.Volt_TempLed);
		LCD_DisplayStringLine(Line4 ,lcdstring);
		/*                ��Ļ������                          */
		sprintf((char*)lcdstring,"    FD:LD%d            ",System.Freq_TempLed);
		LCD_DisplayStringLine(Line5 ,lcdstring);
		/*                ��Ļ������                          */
		LCD_DisplayStringLine(Line6,"                    ");
		/*                ��Ļ�ھ���                          */
		LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");	
	}
}

void SystemPara_Init(void)
{
  System.Interface=DATA;
	System.Follow_Pulsx=PULS1;
	System.Volt_Led=1;
	System.Freq_Led=2;
	System.Volt_TempLed=1;
	System.Freq_TempLed=2;
}

/**
 * @brief  ��ʱ��4�жϷ�����-1ms�ж�
 * @param  ��
 * @retval ��
 */
void TIM4_IRQHandler(void)
{
	static uint8_t  key_cnt;
	static uint16_t adc_cnt,puls_cnt;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		if(++adc_cnt==300) 
		{
			adc_cnt=0;	
			Flag.Read_ADC=1;
		}
		if(++puls_cnt==350) 
		{
			puls_cnt=0;	
			Flag.Read_PULS=1;
		}
		if(++key_cnt==10)
		{
			key_cnt=0;
			Flag.Read_KEY=1;
		}
	}
}

/*****************************************************END OF FILE*********************************************************/	


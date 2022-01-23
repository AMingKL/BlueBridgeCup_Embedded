/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-23
  * @brief   ����Ƕ��ʽ��ʮ�������
  *************************************************************************************************************************
  * @attention
  *
  * usart������ܳ�ͻ����:�Ҳ��÷�ʱ���ã������䶼����ɣ��������������Ǳ����������������������������ܣ�����������ȫʵ�֡�
  * 
  *************************************************************************************************************************
  */
/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "myconfig.h"
/* �������� -----------------------------------------------------------------------------------------------------------------*/
void Key_Proc(void);
void Led_Proc(void);
void Lcd_Proc(void);
void ADC_Proc(void);
void DS18B20_Proc(void);
void Usart_Proc(void);
void Seg_Proc(void);
void EEprom_Proc(void);
void Caputer_Proc(void);
void SystemPara_Init(void);

/* �������� -----------------------------------------------------------------------------------------------------------------*/
uint8_t lcdstring[20];
/* ������ر��� */
uint8_t RxCounter;
uint8_t RxoverFlag;
uint8_t RxBuffer[15];

uint8_t adc_flag,ds18b20_flag,seg_flag,key_flag,eeprom_write_flag,AutoReport_flag,AutoSend_flag,SendTemperature_flag,SendPara_flag,LedFlash_flag,Led_flag;

uint8_t B3_cnt,B4_cnt;
uint8_t ChannelTemp,Temperature_limitTemp;

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();	
	LED_Init();
	Key_Init();
	LCD_Init();
	Tim4_Init();	
	DS18B20_Init();	
	ADCx_Init();
	time3_capture();
	i2c_init();	
	SystemPara_Init();
	Delay_Ms(200);	
	while(1)
	{	
		Led_Proc();
		Lcd_Proc();
		ADC_Proc();
		Key_Proc();		
		Usart_Proc();
		DS18B20_Proc();	
		Caputer_Proc();
		EEprom_Proc();
		
	}
}

void EEprom_Proc(void)
{
  if(eeprom_write_flag==1)
	{
	  Write_eeprom(0x00,System.Set_ParaCnt);
		Delay_Ms(5);
	}
}

void Led_Proc(void)
{
	if(System.Temperature>System.Temperature_limit)
	{
	 LedFlash_flag=1;
	}
	else
	{
	 LedFlash_flag=0;
	 LED_Control(LED2,OFF);
	}
	
	if ((Led_flag)&&(LedFlash_flag))
	{
	  LED_Toggle(2);
	}
	else if((Led_flag==0)&&(LedFlash_flag))
	{
	  LED_Toggle(2);
	}
}


void Usart_Proc(void)
{
  if((AutoReport_flag)&&(AutoSend_flag))//�Զ��ϱ�
	{
		printf("$%.2f\r\n",System.Temperature);
	  AutoSend_flag=0;
	}
	if(SendTemperature_flag)
	{
	  SendTemperature_flag=0;
		printf("$%.2f\r\n",System.Temperature);
	}
	if(SendPara_flag)
	{
	  SendPara_flag=0;
		printf("#%d,AO%d\r\n",System.Temperature_limit,System.Choose_Channel);
	}
	if(RxoverFlag==1)//��Ϣ����
	{
		RxoverFlag=0;
    if((RxCounter==7)&&(RxBuffer[1]=='S')&&(RxBuffer[2]=='T')&&(RxBuffer[3]=='\\')&&(RxBuffer[4]=='r')&&(RxBuffer[5]=='\\')&&(RxBuffer[6]=='n'))
		{
     SendTemperature_flag=1;
		}
		if((RxCounter==9)&&(RxBuffer[1]=='P')&&(RxBuffer[2]=='A')&&(RxBuffer[3]=='R')&&(RxBuffer[4]=='A')&&(RxBuffer[5]=='\\')&&(RxBuffer[6]=='r')&&(RxBuffer[7]=='\\')&&(RxBuffer[8]=='n'))
		{
      SendPara_flag=1;
		}		
		RxCounter=0;
		memset(RxBuffer,0,15);//����������
	}
	
	Seg_Proc();
}

void Caputer_Proc(void)
{
 if(ch2_mode==3)
 {
	 System.PA7_DutyRatio=ch2_duty*100/ch2_val;
   ch2_mode=0;
 }
 if(System.Choose_Channel==AO1)
 {
	 if(System.VRP5_value>((System.PA7_DutyRatio*3.3)/100))
	 {
	   AutoReport_flag=1;
		 LED_Control(LED1,ON);
	 }
	 else
	 {
	  AutoReport_flag=0;
	  LED_Control(LED1,OFF);
	 }
 }
 else
 {
	 if(System.VRP6_value>((System.PA7_DutyRatio*3.3)/100))
	 {
	   AutoReport_flag=1;
		 LED_Control(LED1,ON);
	 } 
	 else
	 {
	  AutoReport_flag=0;
		LED_Control(LED1,OFF);
	 }	 
 }
}

void Key_Proc(void)
{		
	if(key_flag)
	{
		 Key_refresh();
		if(key_falling==B1)//���ü�
		{
			 System.Interface=(System.Interface+1)%2;
			 System.Choose_Para=0;
			if(System.Interface==MAIN)//�������ݽ��棬������仯��cnt��һ�����浽eeprom��
			{
			  if((ChannelTemp!=System.Choose_Channel)||(Temperature_limitTemp!=System.Temperature_limit))
				{
					ChannelTemp=System.Choose_Channel;
					Temperature_limitTemp=System.Temperature_limit;
					System.Set_ParaCnt+=1;
					eeprom_write_flag=1;
				}
			}
		}
		else if(key_falling==B2)//ѡ���
		{
			if(System.Interface==PARA)
			{
			  System.Choose_Para=(System.Choose_Para+1)%2;
			}
		}
		else if(key_falling==B3)//�Ӽ�
		{
			B3_cnt=0;
			if(System.Interface==PARA)
			{
				if(System.Choose_Para==0)//�¶�
				{
					(System.Temperature_limit==40)?(System.Temperature_limit=40):(System.Temperature_limit++);
				}
				else//�Ƚ�ͨ���л�
				{
				 System.Choose_Channel=(System.Choose_Channel%2)+1;
				}
			}
		}
		else if(key_falling==B4)//����
		{
			B4_cnt=0;
			if(System.Interface==PARA)
			{
				if(System.Choose_Para==0)//�¶�
				{
					(System.Temperature_limit==20)?(System.Temperature_limit=20):(System.Temperature_limit--);
				}
				else//�Ƚ�ͨ���л�
				{
				 System.Choose_Channel=(System.Choose_Channel%2)+1;
				}
			}		 
		}	
		if((key_state==B3)&&(System.Interface==PARA)&&(System.Choose_Para==0))
		{
			if(B3_cnt<10)
			{B3_cnt++;}
			else if(B3_cnt==10)
			{
			 B3_cnt=20;
			}
			if(B3_cnt==20)
			{
				(System.Temperature_limit==40)?(System.Temperature_limit=40):(System.Temperature_limit++);
			}
		}
		 else	if((key_state==B4)&&(System.Interface==PARA)&&(System.Choose_Para==0))
		{
			if(B4_cnt<10)
			{B4_cnt++;}
			else if(B4_cnt==10)
			{
			 B4_cnt=20;
			}
			if(B4_cnt==20)
			{
				(System.Temperature_limit==20)?(System.Temperature_limit=20):(System.Temperature_limit--);
			}
		}
		key_flag=0;
	}				
}

void ADC_Proc(void)
{
	if(adc_flag)
	{
	  System.VRP5_value=Get_AnalogVoltage(VRP5)*3.3/4095;
		System.VRP6_value=Get_AnalogVoltage(VRP6)*3.3/4095;
		adc_flag=0;
	}
}

void DS18B20_Proc(void)
{
 if(ds18b20_flag)
 {	 
	System.Temperature=DS18B20_Read();
  ds18b20_flag=0;
 }
}

void Seg_Proc(void)
{
	Seg_Init();
  Delay_Ms(5);
	
	if(seg_flag==1)//��ʾ�¶�
	Seg_Display(17,System.Temperature_limit/10,System.Temperature_limit%10);
	else
	Seg_Display(18,0,System.Choose_Channel);
	
	Usart2_Init();
	Delay_Ms(5);	
}

void  Lcd_Proc(void)
{
	if(System.Interface==MAIN)
	{
		/*                ��Ļ��һ��                          */
		LCD_DisplayStringLine(Line0,"The10th LQB GuoSaiTi");
		/*                ��Ļ�ڶ���                          */
		LCD_DisplayStringLine(Line2,"       Data         ");
		/*                ��Ļ������                          */
		sprintf((char*)lcdstring,"    AO1:%.2fV           ",System.VRP5_value);
		LCD_DisplayStringLine(Line3 ,lcdstring);
		/*                ��Ļ������                          */
		sprintf((char*)lcdstring,"    AO2:%.2fV          ",System.VRP6_value);
		LCD_DisplayStringLine(Line4 ,lcdstring);
		/*                ��Ļ������                          */
		sprintf((char*)lcdstring,"    PWM2:%d%%           ",System.PA7_DutyRatio);
		LCD_DisplayStringLine(Line5 ,lcdstring);
		/*                ��Ļ������                          */
		sprintf((char*)lcdstring,"    Temp:%.2fC    ",System.Temperature);
		LCD_DisplayStringLine(Line6 ,lcdstring);
		/*                ��Ļ������                          */
		sprintf((char*)lcdstring,"    N:%d   ",System.Set_ParaCnt);
		LCD_DisplayStringLine(Line7 ,lcdstring);
		/*                ��Ļ�ھ���                          */
		LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");	
	}
	else 
	{
		/*                ��Ļ��һ��                          */
		LCD_DisplayStringLine(Line0,"The10th LQB GuoSaiTi");
		/*                ��Ļ�ڶ���                          */
		LCD_DisplayStringLine(Line2,"       Para         ");
		/*                ��Ļ������                          */
		LCD_DisplayStringLine(Line3,"                    ");
		/*                ��Ļ������                          */
		LCD_DisplayStringLine(Line6,"                    ");
		/*                ��Ļ������                          */
		LCD_DisplayStringLine(Line7,"                    ");
		/*                ��Ļ�ھ���                          */
		LCD_DisplayStringLine(Line9," Author:LiuHongMing   ");	
		
		if(System.Choose_Para==0)
		{
			LCD_SetBackColor(Yellow); 
			sprintf((char*)lcdstring,"    T:%d             ",System.Temperature_limit);
			LCD_DisplayStringLine(Line4 ,lcdstring);	
			LCD_SetBackColor(Black);
			sprintf((char*)lcdstring,"    X:AO%d           ",System.Choose_Channel);
			LCD_DisplayStringLine(Line5 ,lcdstring);			
		}
		else
		{
			LCD_SetBackColor(Yellow); 
			sprintf((char*)lcdstring,"    X:AO%d           ",System.Choose_Channel);
			LCD_DisplayStringLine(Line5 ,lcdstring);		
			LCD_SetBackColor(Black);
			sprintf((char*)lcdstring,"    T:%d             ",System.Temperature_limit);
			LCD_DisplayStringLine(Line4 ,lcdstring);			
		}
	}
}

void SystemPara_Init(void)
{
  System.Temperature_limit=30;
	System.Choose_Channel=AO1;
	System.Interface=MAIN;
	ChannelTemp=System.Choose_Channel;
  Temperature_limitTemp=System.Temperature_limit;
	System.Set_ParaCnt=Read_eeprom(0x00);
}

/**
 * @brief  ��ʱ��4�жϷ�����-1ms�ж�
 * @param  ��
 * @retval ��
 */
void TIM4_IRQHandler(void)
{
	static uint16_t ds18b20_cnt,seg_cnt,autosend_cnt;
	static uint8_t  ket_cnt,led_cnt,adc_cnt;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		if(++adc_cnt==200) 
		{
			adc_cnt=0;			
			adc_flag=1;
		}
		if(++ds18b20_cnt==300)
		{
		 ds18b20_cnt=0;
		 ds18b20_flag=1;
		}
		if(++seg_cnt==2000)
		{
		  seg_cnt=0;
			seg_flag^=1;
		}
		if(++ket_cnt==10)
		{
		  ket_cnt=0;
			key_flag=1;
		}
		if(++autosend_cnt==1000)
		{
		  autosend_cnt=0;
			AutoSend_flag^=1;
		}
		if(++led_cnt==200)
		{
		  led_cnt=0;
			Led_flag^=1;
		}
	}
}

/**
 * @brief  ����2�����жϷ������
 * @param  ��
 * @retval ��
 */
void USART2_IRQHandler(void) 
{	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) !=RESET)
	{
	  RxBuffer[RxCounter++]=USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2,USART_IT_IDLE)!=RESET)
	{
		USART2->SR;
		USART2->DR;
		
		RxoverFlag=1;
	}
}


/*****************************************************END OF FILE*********************************************************/	


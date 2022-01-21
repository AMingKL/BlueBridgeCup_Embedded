/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-21
  * @brief   蓝桥嵌入式第八届国赛赛题-频率控制器
  *************************************************************************************************************************
  * @attention
  *
  * 
  * 
  *************************************************************************************************************************
  */
/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "myconfig.h"
/* 函数定义 -----------------------------------------------------------------------------------------------------------------*/
void Lcd_Proc(void);
void Key_Proc(void);
void Led_Proc(void);
void Pulse_Proc(void);
void Adc_Proc(void);
void EEprom_Proc(void);
void PWM_Out_Proc(void);
void FreqController_Init(void);
/* 变量定义 -----------------------------------------------------------------------------------------------------------------*/
uint8_t   lcdstr[20];
uint8_t   adc_flag,lcd_flag,pwm_out_flag,keyRead_flag,led_flag,eeprom_write_flag;
uint16_t  ao1_value,ao2_value;


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
  Delay_Init();	
	LED_Init();
	Key_Init();
	LCD_Init();
	Tim4_Init();
	ADC_TWO_Init();	
	tim2_capture_init();
	PWM_Init(50,50);
	i2c_init();
	FreqController_Init();
	Delay_Ms(200);
	while(1)
	{	
		Lcd_Proc();
		Pulse_Proc();
		Adc_Proc(); 
		PWM_Out_Proc();	
		Key_Proc();		
		Led_Proc();
		EEprom_Proc();		
	}	
}

void EEprom_Proc(void)
{
  if(eeprom_write_flag==1)
	{
		Write_eeprom(0x00,System.Divider);
		Delay_Ms(5);
		Write_eeprom(0x01,System.Multiplier);
		Delay_Ms(5);
		
		eeprom_write_flag=0;
	}	
}

void Key_Proc(void)
{
	if(keyRead_flag)
	{
	  keyRead_flag=0;
		Key_refresh();
		if(key_falling==B1)
		{
			System.Interface=(System.Interface+1)%2;
			if(System.Interface==MEASURE)
			{
				System.Divider=System.DividerTemp;
				System.Multiplier=System.MultiplierTemp;
				eeprom_write_flag=1;
			}
		}
		else if(key_falling==B2)
		{
		  if(System.Interface==CONFIG)
			{
			 System.ChooseTheParam=(System.ChooseTheParam+1)%2;
			}
		}
		else if(key_falling==B3)
		{
			if(System.Interface==CONFIG)
			{
			  if(System.ChooseTheParam==PARAM_DIV)
				{
					System.DividerTemp==4?System.DividerTemp=4:System.DividerTemp++;
				}
				else if(System.ChooseTheParam==PARAM_MUL)
				{
				 System.MultiplierTemp==4?System.MultiplierTemp=4:System.MultiplierTemp++;
				}
			}
		}
		else if(key_falling==B4)
		{
			if(System.Interface==CONFIG)
			{
			  if(System.ChooseTheParam==PARAM_DIV)
				{
				 System.DividerTemp==1?System.DividerTemp=1:System.DividerTemp--;
				}
				else if(System.ChooseTheParam==PARAM_MUL)
				{
				 System.MultiplierTemp==1?System.MultiplierTemp=1:System.MultiplierTemp--;
				}
			}
		}		
	}
}


void Adc_Proc(void)
{
	if(adc_flag)
	{
 		System.Rp5_adc_value = (get_2_adc(ADC_Channel_4)*3.3)/4095;
		System.Rp6_adc_value = (get_2_adc(ADC_Channel_5)*3.3)/4095;
		
		ao1_value=System.Rp5_adc_value*100;
		ao2_value=System.Rp6_adc_value*100;
		
		adc_flag=0;
	}
}

void Pulse_Proc(void)
{
  System.PULS1_Freq=TIM2_Freq1/1000;
	System.PULS2_Freq=TIM2_Freq2/1000;
}

void PWM_Out_Proc(void)
{
	if(pwm_out_flag==1)
	{
		if(System.Interface==MEASURE)
		{
			CCR1_Val=1000000/(System.PULS1_Freq*1000/System.Divider);
		  CCR2_Val=1000000/(System.PULS2_Freq*1000*System.Multiplier);
		}
		else if(System.Interface==CONFIG)
		{
		 PWM_Init(0,0);
		}
		pwm_out_flag=0;
	}
}

void Led_Proc(void)
{
	if(led_flag==1)
	{
		if(System.Interface==CONFIG)
		{
		 LED_Control(LED1,ON);
		}
		else
		{
			LED_Control(LED1,OFF);
		}
	 if(ao1_value>ao2_value)
	 {
	  LED_Control(LED8,ON);
	 }
	 else
	 {
	   LED_Control(LED8,OFF);
	 }
	  led_flag=0;
	}
	
}

void Lcd_Proc(void)
{
	if(lcd_flag==1)
	{
		if(System.Interface==MEASURE)
		{
			/*                屏幕第一行                          */
			LCD_DisplayStringLine(Line0,"The 8th LQB GuoSaiTi");
			/*                屏幕第二行                          */
			LCD_DisplayStringLine(Line2,"       Data         ");
			/*                屏幕第三行                          */
			sprintf((char*)lcdstr,"   PULS1:%dKHz           ",System.PULS1_Freq);
			LCD_DisplayStringLine(Line3 ,lcdstr);
			/*                屏幕第四行                          */
			sprintf((char*)lcdstr,"   PULS2:%dKHz           ",System.PULS2_Freq);
			LCD_DisplayStringLine(Line4 ,lcdstr);
			/*                屏幕第五行                          */
			sprintf((char*)lcdstr,"   AO1:%.2fV          ",System.Rp5_adc_value);
			LCD_DisplayStringLine(Line5 ,lcdstr);
			/*                屏幕第六行                          */
			sprintf((char*)lcdstr,"   AO2:%.2fV           ",System.Rp6_adc_value);
			LCD_DisplayStringLine(Line6 ,lcdstr);
			/*                屏幕第九行                          */
			LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");	
		}
		else if(System.Interface==CONFIG)
		{
			/*                屏幕第一行                          */
			LCD_DisplayStringLine(Line0,"The 8th LQB GuoSaiTi");
			/*                屏幕第二行                          */		
			LCD_DisplayStringLine(Line2,"       Config         ");
			/*                屏幕第三行                          */
			LCD_DisplayStringLine(Line3,"                       ");			
			/*                屏幕第六行                          */
			LCD_DisplayStringLine(Line6,"                       ");	
			/*                屏幕第九行                          */
			LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");	
			
			if(System.ChooseTheParam==PARAM_DIV)
			{
			  LCD_SetBackColor(Green); 	
				sprintf((char*)lcdstr,"   Divider:%d           ",System.DividerTemp);
				LCD_DisplayStringLine(Line4 ,lcdstr);
				LCD_SetBackColor(Black); 
				sprintf((char*)lcdstr,"   Multiplier:%d        ",System.MultiplierTemp);
			  LCD_DisplayStringLine(Line5 ,lcdstr);
			}
			else if(System.ChooseTheParam==PARAM_MUL)
			{
				LCD_SetBackColor(Green); 	
				sprintf((char*)lcdstr,"   Multiplier:%d        ",System.MultiplierTemp);
			  LCD_DisplayStringLine(Line5 ,lcdstr);
				LCD_SetBackColor(Black); 
				sprintf((char*)lcdstr,"   Divider:%d           ",System.DividerTemp);
				LCD_DisplayStringLine(Line4 ,lcdstr);
			}	
		}		
	 lcd_flag=0;
	}
}

void FreqController_Init(void)
{
	System.Interface=MEASURE;
	System.Divider=Read_eeprom(0x00);
	System.Multiplier=Read_eeprom(0x01);
	System.DividerTemp=Read_eeprom(0x00);
	System.MultiplierTemp=Read_eeprom(0x01);
	System.ChooseTheParam=PARAM_DIV;
	
	if(((System.Divider>4)||(System.Divider<1))||((System.Multiplier>4)||(System.Multiplier<1)))
	{
	  System.Divider=1;
		System.DividerTemp=1;
		System.Multiplier=1;
		System.MultiplierTemp=1;
	}
}

/**
 * @brief  定时器4中断服务函数-1ms中断
 * @param  无
 * @retval 无
 */
void TIM4_IRQHandler(void)
{
	static uint16_t pwm_cnt,adc_cnt,lcd_cnt,key_cnt,led_cnt;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		if(++pwm_cnt==200) 
		{
			pwm_cnt=0;
			pwm_out_flag=1;			
		}
		if(++adc_cnt==300)
		{
			adc_cnt=0;
		  adc_flag=1;
		}
		if(++lcd_cnt==100)
		{
			lcd_cnt=0;
			lcd_flag=1;
		}
		if(++key_cnt==10)
		{
		  key_cnt=0;
			keyRead_flag=1;
		}
		if(++led_cnt==100)
		{
		  led_cnt=0;
			led_flag=1;
		}
	}
}

/*****************************************************END OF FILE*********************************************************/	


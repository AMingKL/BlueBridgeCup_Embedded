/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-22
  * @brief   蓝桥杯嵌入式第九届国赛题-电子秤
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
void ADC_Proc(void);
void Lcd_Proc(void);
void Key_Proc(void);
void Led_Proc(void);
void Usart_Proc(void);
void EEprom_write_Proc(void);
void ElectronicScaleInit(void);
/* 变量定义 -----------------------------------------------------------------------------------------------------------------*/
uint8_t lcdstr[20];
float  AO1_VALUE=0;
uint8_t adc_flag,lcd_flag,adc_key_flag,send_billing_flag,eeprom_write_flag,send_para_flag,led1_flag,led2_flag;
float  goods1_temp,goods2_temp,goods3_temp;

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();	
	LED_Init();
	LCD_Init();
	Tim4_Init();
	Usart2_Init();
	i2c_init();
	ADCx_Init();	
	ElectronicScaleInit();
	Delay_Ms(200);	
	while(1)
	{	
		ADC_Proc();
		Lcd_Proc();
		Key_Proc();
		EEprom_write_Proc();
		Led_Proc();
		Usart_Proc();
	}
}


void ADC_Proc(void)
{
	if(adc_flag)
	{
		AO1_VALUE = (Get_AnalogVoltage(VRP5)*3.30/4095.0);
		
		if(AO1_VALUE>=3.283)//板子接触不良，不添加这句读取的电压在3.30V停留较短
		{
		 AO1_VALUE=3.30;
		}
		System.weight=AO1_VALUE;
		
		System.weight=AO1_VALUE*3.03;
		adc_flag=0;
	}
}

void Usart_Proc(void)
{
  if(send_billing_flag==1)
	{
	  send_billing_flag=0;	
		switch(System.number)//计算价格
		{
			case 1: System.totalprice=System.Goods1price*System.weight; printf("U.W.1:%.2f\n",System.Goods1price);break;
			case 2: System.totalprice=System.Goods2price*System.weight; printf("U.W.2:%.2f\n",System.Goods2price);break;
			case 3: System.totalprice=System.Goods3price*System.weight; printf("U.W.3:%.2f\n",System.Goods3price);break;
			default:break;
		}
		printf("G.W:%.2f\n",System.weight);
		printf("Total:%.2f\n",System.totalprice);
	}
	if(send_para_flag==1)
	{
		send_para_flag=0;
		printf("U.W.1:%.2f\n",System.Goods1price);
		printf("U.W.2:%.2f\n",System.Goods2price);
		printf("U.W.3:%.2f\n",System.Goods3price);
	}
}	

void EEprom_write_Proc(void)
{
 if(eeprom_write_flag)
 {
   eeprom_write_flag=0;
	 Write_eeprom(0x00,System.setnumcnt);
	 Delay_Ms(5);
	 Write_eeprom(0x01,(u16)(System.Goods1price*1000)/256);
	 Delay_Ms(5);
   Write_eeprom(0x02,(u16)(System.Goods1price*1000)%256);
	 Delay_Ms(5);
	 Write_eeprom(0x03,(u16)(System.Goods2price*1000)/256);
	 Delay_Ms(5);
   Write_eeprom(0x04,(u16)(System.Goods2price*1000)%256);
	 Delay_Ms(5);
	 Write_eeprom(0x05,(u16)(System.Goods3price*1000)/256);
	 Delay_Ms(5);
   Write_eeprom(0x06,(u16)(System.Goods3price*1000)%256);
	 Delay_Ms(5);
	 eeprom_write_flag=0;
 }
}

void Led_Proc(void)
{
  if(System.Interface==DATA)
	{
	  if(led1_flag)
		{
		 LED_Control(LED1,ON);
		}
		else
		{
			LED_Control(LED1,OFF);
	  }	
	}
	else if(System.Interface==PARA)
	{
	  if(led2_flag)
		{
		 LED_Control(LED1,ON);
		}
		else
		{
			LED_Control(LED1,OFF);
	  }		 
	}
}

void Key_Proc(void)
{
		if(Key.Value != 0xff)// 
		{				
			switch(Key.Value)
			{
				case 1://设置键
					if(Key.ShortPress == TRUE)
					{
						Key.ShortPress = FALSE;  
						System.Interface=(System.Interface+1)%2;
						System.ChooseGoods=0;
						if(System.Interface==DATA)
						{
							send_para_flag=1;
              if((goods1_temp!=System.Goods1price)||(goods2_temp!=System.Goods2price)||(goods3_temp!=System.Goods3price))
							{
							  System.setnumcnt+=1;
								eeprom_write_flag=1;
								goods1_temp=System.Goods1price;
								goods2_temp=System.Goods2price;
								goods3_temp=System.Goods3price;
							}
						}
					}
					break;
				case 2:	//加键	
						if(System.Interface==PARA)
						{
						    if(Key.ShortPress == TRUE)
								{
									Key.ShortPress = FALSE;
									switch(System.ChooseGoods)
									{
										case 0:(System.Goods1price>=10.00)?(System.Goods1price=10.00):(System.Goods1price+=0.01);break;
										case 1:(System.Goods2price>=10.00)?(System.Goods2price=10.00):(System.Goods2price+=0.01);break;
										case 2:(System.Goods3price>=10.00)?(System.Goods3price=10.00):(System.Goods3price+=0.01);break;
										default:break;
									}
								}							 
               if(Key.LongPress==TRUE)
							 {					 
									 switch(System.ChooseGoods)
									{
										case 0:(System.Goods1price>=10.00)?(System.Goods1price=10.00):(System.Goods1price+=0.01);break;
										case 1:(System.Goods2price>=10.00)?(System.Goods2price=10.00):(System.Goods2price+=0.01);break;
										case 2:(System.Goods3price>=10.00)?(System.Goods3price=10.00):(System.Goods3price+=0.01);break;
										default:break;
									}
							}	
						}							
					break;
				case 3:	//减键
						   if(System.Interface==PARA)
							{
								if(Key.ShortPress == TRUE)
								{
									Key.ShortPress = FALSE;
									  switch(System.ChooseGoods)
										{
										  case 0:(System.Goods1price<0.01)?(System.Goods1price=0):(System.Goods1price-=0.01);break;
											case 1:(System.Goods2price<0.01)?(System.Goods2price=0):(System.Goods2price-=0.01);break;
											case 2:(System.Goods3price<0.01)?(System.Goods3price=0):(System.Goods3price-=0.01);break;
											default:break;
										}
								}
               if(Key.LongPress==TRUE)
							 {
									  switch(System.ChooseGoods)
										{
										  case 0:(System.Goods1price<0.01)?(System.Goods1price=0):(System.Goods1price-=0.01);break;
											case 1:(System.Goods2price<0.01)?(System.Goods2price=0):(System.Goods2price-=0.01);break;
											case 2:(System.Goods3price<0.01)?(System.Goods3price=0):(System.Goods3price-=0.01);break;
											default:break;
										}
							 }	
						 }
					break;
				case 4:	//切换键
								if(Key.ShortPress == TRUE)
								{
									Key.ShortPress = FALSE;
									if(System.Interface==PARA)
									{
									  System.ChooseGoods=(System.ChooseGoods+1)%3;
									}
								}					
				 break;
				case 5://货物1				
								if(Key.ShortPress == TRUE)
								{
									Key.ShortPress = FALSE;
									System.number=1;
								}
				 break;
	      case 6:	//货物2	
					      if(Key.ShortPress == TRUE)
								{
									Key.ShortPress = FALSE;
									System.number=2;
								}
				 break;
			  case 7:	//货物3
					      if(Key.ShortPress == TRUE)
								{
									Key.ShortPress = FALSE;
									System.number=3;
								}					
				 break;
			  case 8:	//计费键
								if(Key.ShortPress == TRUE)
								{
									Key.ShortPress = FALSE;   //！！！手动清除短按标志位
									send_billing_flag=1;
								}					
				 break;			
			}
			Key.Value = 0xff;  //在此清除的目的在于 不至于在状态机中有按键值，而在按键处理函数检测时已经被状态机刷新销毁，上面switch判断后才能清除！
		}
		
	}

void Lcd_Proc(void)
{
	if(lcd_flag==1)
	{
		if(System.Interface==DATA)
		{
			switch(System.number)
			{
				case 1:	sprintf((char*)lcdstr," Price:%.2f/kg          ",System.Goods1price);
								LCD_DisplayStringLine(Line4 ,lcdstr);break;
				case 2:	sprintf((char*)lcdstr," Price:%.2f/kg          ",System.Goods2price);
								LCD_DisplayStringLine(Line4 ,lcdstr);break;
				case 3:	sprintf((char*)lcdstr," Price:%.2f/kg          ",System.Goods3price);
								LCD_DisplayStringLine(Line4 ,lcdstr);break;
				default:break;
				
			}
			/*                屏幕第一行                          */
			LCD_DisplayStringLine(Line0,"The 9th LQB GuoSaiTi");
			/*                屏幕第二行                          */
			LCD_DisplayStringLine(Line2,"       Data         ");
			/*                屏幕第三行                          */
			sprintf((char*)lcdstr," Number:%d           ",System.number);
			LCD_DisplayStringLine(Line3 ,lcdstr);
			/*                屏幕第五行                          */
			sprintf((char*)lcdstr," Weight:%.2fkg          ",System.weight);
			LCD_DisplayStringLine(Line5 ,lcdstr);
			/*                屏幕第六行                          */
			sprintf((char*)lcdstr," TotalPrice:%.2fyuan    ",System.totalprice);
			LCD_DisplayStringLine(Line6 ,lcdstr);
			/*                屏幕第九行                          */
			LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");	
		}
		else if(System.Interface==PARA)
		{
			switch(System.ChooseGoods)
			{
				case 0:	LCD_SetBackColor(Green);
								sprintf((char*)lcdstr," Goods1:%.2fyuan/kg           ",System.Goods1price);
								LCD_DisplayStringLine(Line3 ,lcdstr);
								LCD_SetBackColor(Black);
								sprintf((char*)lcdstr," Goods2:%.2fyuan/kg          ",System.Goods2price);
				        LCD_DisplayStringLine(Line4 ,lcdstr);
								sprintf((char*)lcdstr," Goods3:%.2fyuan/kg          ",System.Goods3price);
								LCD_DisplayStringLine(Line5 ,lcdstr);
								break;
				case 1:LCD_SetBackColor(Green);
								sprintf((char*)lcdstr," Goods2:%.2fyuan/kg          ",System.Goods2price);
				        LCD_DisplayStringLine(Line4 ,lcdstr);
								LCD_SetBackColor(Black);
								sprintf((char*)lcdstr," Goods1:%.2fyuan/kg           ",System.Goods1price);
								LCD_DisplayStringLine(Line3 ,lcdstr);
								sprintf((char*)lcdstr," Goods3:%.2fyuan/kg          ",System.Goods3price);
								LCD_DisplayStringLine(Line5 ,lcdstr);
								break;
				case 2:LCD_SetBackColor(Green);
								sprintf((char*)lcdstr," Goods3:%.2fyuan/kg          ",System.Goods3price);
								LCD_DisplayStringLine(Line5 ,lcdstr);
								LCD_SetBackColor(Black);
								sprintf((char*)lcdstr," Goods2:%.2fyuan/kg          ",System.Goods2price);
				        LCD_DisplayStringLine(Line4 ,lcdstr);
								sprintf((char*)lcdstr," Goods1:%.2fyuan/kg           ",System.Goods1price);
								LCD_DisplayStringLine(Line3 ,lcdstr);
								break;
				default:break;
			}		
			/*                屏幕第一行                          */
			LCD_DisplayStringLine(Line0,"The 9th LQB GuoSaiTi");
			/*                屏幕第二行                          */
			LCD_DisplayStringLine(Line2,"       Para         ");
			/*                屏幕第六行                          */
			sprintf((char*)lcdstr," SetCnt:%d                    ",System.setnumcnt);
			LCD_DisplayStringLine(Line6 ,lcdstr);
			/*                屏幕第九行                          */
			LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");	
		}
			lcd_flag=0;
	}
}




void ElectronicScaleInit(void)
{
	 System.number=1;
	
	 System.setnumcnt  =  Read_eeprom(0x00);
	 System.Goods1price = (Read_eeprom(0x01)*256+Read_eeprom(0x02))/1000.0f;
	 System.Goods2price = (Read_eeprom(0x03)*256+Read_eeprom(0x04))/1000.0f;
	 System.Goods3price = (Read_eeprom(0x05)*256+Read_eeprom(0x06))/1000.0f;

	 if((System.Goods1price>10.0)||(System.Goods1price<=0))
	 {
		 System.Goods1price=0.20;
	 }
	 if((System.Goods2price>10.0)||(System.Goods2price<=0))
	 {
		 System.Goods2price=0.20;
	 }
	 if((System.Goods3price>10.0)||(System.Goods3price<=0))
	 {
		 System.Goods3price=0.30;
	 }
	 goods1_temp=System.Goods1price;
	 goods2_temp=System.Goods2price;
	 goods3_temp=System.Goods3price;
	 
}

/**
 * @brief  定时器4中断服务函数-1ms中断
 * @param  无
 * @retval 无
 */
void TIM4_IRQHandler(void)
{
	static uint16_t adc_cnt,lcd_cnt,adc_ket_cnt,led1_cnt,led2_cnt;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
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
		if(++adc_ket_cnt==5)//5ms刷新一次
		{
			adc_ket_cnt=0;
			Key_Interrup();
		}
		if(++led1_cnt==800)
		{
			led1_cnt=0;
		  led1_flag^=1;
		}
		if(++led2_cnt==400)
		{
			led2_cnt=0;
		  led2_flag^=1;
		}
	}
}


/*****************************************************END OF FILE*********************************************************/	


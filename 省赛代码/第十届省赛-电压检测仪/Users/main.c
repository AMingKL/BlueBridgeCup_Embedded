/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-16
  * @brief   蓝桥嵌入式第十届省赛题-电压检测仪
  *************************************************************************************************************************
  * @attention
  *
  * 
  * 
  *************************************************************************************************************************
  */
/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "myconfig.h"
/* 函数定义 -------------------------------------------------------------------------------------------------------------*/
void Lcd_Proc(void);
void Key_Proc(void);
void Led_Proc(void);
void VoltState_Proc(void);
void Highlight_Proc(void);
void VoltSystem_Init(void);
void Threshold_Check(void);
/* LCD时间显示数组 */
uint8_t Voltstring[20];
uint8_t MaxVoltstring[20];
uint8_t MinVoltstring[20];
uint8_t UpperLedstring[20];
uint8_t LowerLedstring[20];
/* 变量定义 ---------------------------------------------------------------------------------------------------------------*/
uint8_t led_flag=0;


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();	
	LED_Init();
	Key_Init();
	LCD_Init();
	Tim4_Init(); 
	Adc_Init(); 
	VoltSystem_Init();
	while(1)
	{	
		Lcd_Proc();
		Led_Proc();
		VoltState_Proc();
		Highlight_Proc();
	}
}

void Key_Proc(void)
{
  Key_refresh();
	if(key_falling==B1)//界面切换键
	{
		if(VoltSystem.Interface==Main)
		{
		 VoltSystem.Interface=Setting;
		 VoltSystem.Highlight=0;
		}
		else
		 Threshold_Check();		
	}
	else if(key_falling==B2)//切换选择参数键
	{
		if(VoltSystem.Interface==Setting)
		{
		  VoltSystem.Highlight=(VoltSystem.Highlight+1)%4;
		}
	}
	else if(key_falling==B3)//"加"按键 1.选择电压参数，当前电压加0.3V   2.选择指示灯参数，LED指示灯序号加1  3.电压范围0~3.3v之间不循环led范围led1~led8不循环
	{
		if(VoltSystem.Interface==Setting)
		{
			switch(VoltSystem.Highlight)
			{
				case 0:	(VoltSystem.MaxVoltValue==330)?(VoltSystem.MaxVoltValue=330):(VoltSystem.MaxVoltValue=VoltSystem.MaxVoltValue+30);break;
				case 1:	(VoltSystem.MinVoltValue==330)?(VoltSystem.MinVoltValue=330):(VoltSystem.MinVoltValue=VoltSystem.MinVoltValue+30);break;
				case 2:	(VoltSystem.UpperLed==8)?(VoltSystem.UpperLed=8):(VoltSystem.UpperLed=VoltSystem.UpperLed+1);break;
				case 3:	(VoltSystem.LowerLed==8)?(VoltSystem.LowerLed=8):(VoltSystem.LowerLed=VoltSystem.LowerLed+1);break;
				default:break;
			}
		}
	}	
	else if(key_falling==B4)//"减"按键 1.选择电压参数，当前电压减0.3V   2.选择指示灯参数，LED指示灯序号减1  3.电压范围0~3.3v之间不循环led范围led1~led8不循环
	{
		if(VoltSystem.Interface==Setting)
		{
			switch(VoltSystem.Highlight)
			{
				case 0:	(VoltSystem.MaxVoltValue==0)?(VoltSystem.MaxVoltValue=0):(VoltSystem.MaxVoltValue=VoltSystem.MaxVoltValue-30);break;
				case 1:	(VoltSystem.MinVoltValue==0)?(VoltSystem.MinVoltValue=0):(VoltSystem.MinVoltValue=VoltSystem.MinVoltValue-30);break;
				case 2:	(VoltSystem.UpperLed==1)?(VoltSystem.UpperLed=1):(VoltSystem.UpperLed=VoltSystem.UpperLed-1);break;
				case 3:	(VoltSystem.LowerLed==1)?(VoltSystem.LowerLed=1):(VoltSystem.LowerLed=VoltSystem.LowerLed-1);break;
				default:break;
			}
		}
	}
}

void Lcd_Proc(void)
{
  if(VoltSystem.Interface==Main)
	{
		VoltSystem.VoltValue = (Get_AdcAverage()*(3.3/4095))*100;	
		
		/*                屏幕第一行                          */
    LCD_DisplayStringLine(Line0,"LQB Di Shi Jie SaiTi");
		/*                屏幕第二行                          */
    LCD_DisplayStringLine(Line2,"        Main        ");
		/*                屏幕第四行                          */
		sprintf((char*)Voltstring,"  Volt: %d.%.2dV        ",VoltSystem.VoltValue/100,VoltSystem.VoltValue%100);
	  LCD_DisplayStringLine(Line4 ,Voltstring);
		/*                屏幕第五行                          */
    LCD_DisplayStringLine(Line5,"                    ");
		/*                屏幕第六行                          */
		switch (VoltSystem.state)
		{
			case 0:LCD_DisplayStringLine(Line6,"  State: Upper      ");	break;
			case 1:LCD_DisplayStringLine(Line6,"  State: Lower      ");break;
			case 2:LCD_DisplayStringLine(Line6,"  State: Normal      ");break;
			default:break;
		}
		/*                屏幕第七行                          */
    LCD_DisplayStringLine(Line7,"                    ");		
		/*                屏幕第九行                          */
	  LCD_DisplayStringLine(Line9," Author:LiuHongMing ");
	}
	else if(VoltSystem.Interface==Setting)
	{
		/*                屏幕第一行                          */
    LCD_DisplayStringLine(Line0,"LQB Di Shi Jie SaiTi");
		/*                屏幕第二行                          */
    LCD_DisplayStringLine(Line2,"       Setting      ");
		/*                屏幕第四行                          */
		sprintf((char*)MaxVoltstring,"  Max Volt: %d.%.2dV   ",VoltSystem.MaxVoltValue/100,VoltSystem.MaxVoltValue%100);
		/*                屏幕第五行                          */
		sprintf((char*)MinVoltstring,"  Min Volt: %d.%.2dV   ",VoltSystem.MinVoltValue/100,VoltSystem.MinVoltValue%100);
		/*                屏幕第六行                          */
		sprintf((char*)UpperLedstring,"  Upper: LD%d         ",VoltSystem.UpperLed);
		/*                屏幕第七行                          */
		sprintf((char*)LowerLedstring,"  Lower: LD%d         ",VoltSystem.LowerLed);
		/*                屏幕第九行                          */
	  LCD_DisplayStringLine(Line9," Author:LiuHongMing ");		
		LCD_SetBackColor(Black); 
	}	
}


void VoltSystem_Init(void)
{
	VoltSystem.UpperLed=1;
	VoltSystem.LowerLed=2;
  VoltSystem.MaxVoltValue=240;
	VoltSystem.MinVoltValue=120;
}

void VoltState_Proc(void)
{
  if(VoltSystem.Interface==Main)
	{
	  if(VoltSystem.VoltValue>VoltSystem.MaxVoltValue)
		{
		 VoltSystem.state=Upper;
		}
		else if(VoltSystem.VoltValue<VoltSystem.MinVoltValue)
		{
		 VoltSystem.state=Lower;
		}
		else
		{
		 VoltSystem.state=Normal;
		}
	}
}

void Led_Proc(void)
{
	if(VoltSystem.Interface==Main)
	{
	   switch (VoltSystem.state)
		{
			case 0:if(led_flag) { led_flag=0; LED_Toggle(VoltSystem.UpperLed);	} break;
			case 1:if(led_flag) { led_flag=0; LED_Toggle(VoltSystem.LowerLed);	} break;
			case 2:if(led_flag) { led_flag=0; LED_ALL_OFF();	} break;
			default:LED_ALL_OFF();break;
		} 
	}
	else
	{
	 LED_ALL_OFF();
	}
}

void Threshold_Check(void)
{
  if(VoltSystem.Interface==Setting)
	{
	  if((VoltSystem.MaxVoltValue<=VoltSystem.MinVoltValue)||(VoltSystem.UpperLed==VoltSystem.LowerLed))
		{
		 VoltSystem.Interface=Setting;
		}
	  else
		{
		 VoltSystem.Interface=Main;
		}
	}
}

void Highlight_Proc(void)
{
  if(VoltSystem.Interface==Setting)
	{
		switch(VoltSystem.Highlight)
		{
			case 0:	LCD_SetBackColor(Green); LCD_DisplayStringLine(Line4 ,MaxVoltstring);LCD_SetBackColor(Black);\
				      LCD_DisplayStringLine(Line5 ,MinVoltstring); \
			        LCD_DisplayStringLine(Line6 ,UpperLedstring); \
			        LCD_DisplayStringLine(Line7 ,LowerLedstring); break;
			case 1:	LCD_SetBackColor(Green); LCD_DisplayStringLine(Line5 ,MinVoltstring);LCD_SetBackColor(Black);\
							LCD_DisplayStringLine(Line4 ,MaxVoltstring); \
			        LCD_DisplayStringLine(Line6 ,UpperLedstring); \
			        LCD_DisplayStringLine(Line7 ,LowerLedstring); break;
			case 2:	LCD_SetBackColor(Green); LCD_DisplayStringLine(Line6 ,UpperLedstring);LCD_SetBackColor(Black);\
				      LCD_DisplayStringLine(Line5 ,MinVoltstring); \
			        LCD_DisplayStringLine(Line4 ,MaxVoltstring); \
			        LCD_DisplayStringLine(Line7 ,LowerLedstring); break;
			case 3:	LCD_SetBackColor(Green); LCD_DisplayStringLine(Line7 ,LowerLedstring);LCD_SetBackColor(Black);\
				      LCD_DisplayStringLine(Line5 ,MinVoltstring); \
			        LCD_DisplayStringLine(Line6 ,UpperLedstring); \
			        LCD_DisplayStringLine(Line4 ,MaxVoltstring); break;
			default:break;
		}
	}
}
/**
 * @brief  定时器4中断服务函数-1ms中断
 * @param  无
 * @retval 无
 */
void TIM4_IRQHandler(void)
{
	static uint8_t key_cnt=0,lcd_cnt=0;
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		if(++key_cnt == 10) 
		{
			key_cnt=0;
			Key_Proc();			
		}
		if(++lcd_cnt == 200)
		{
		  lcd_cnt=0;
			led_flag=1;
		}			
	}
}

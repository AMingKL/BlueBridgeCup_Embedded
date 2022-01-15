/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2021-01-15
  * @brief   蓝桥嵌入式第九届省赛题-电子定时器
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
void Time_load(void);
void Led_PWM_Proc(void);
void E2prom_storage(void);
/* LCD时间显示数组 */
uint8_t Timestring[20];
/* 变量定义 -----------------------------------------------------------------------------------------------------------------*/
uint8_t E2prom_storage_flag;//eeprom存储标志位
uint8_t TimerState;//定时器状态
uint8_t TimerChoose;//选择设置时间
uint8_t TimerNum=1;//定时器存储编号
uint16_t Led_cnt;//LED闪烁刷新时间变量
uint16_t Timer_cnt;//电子定时器刷新时间变量
uint8_t Led_flag;//LED闪烁标志位
uint8_t B2_cnt;//B2按键长短按时间变量
uint8_t B3_cnt;//B3按键长短按时间变量
uint8_t B4_cnt;//B4按键长短按时间变量

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();	
	LED_Init();
	i2c_init();
	Key_Init();
	LCD_Init();
	Tim4_Init();
	pwm_init(0);
	LED_ALL_OFF();
	Time_load();
	while(1)
	{	
    Lcd_Proc();
		Led_PWM_Proc();
		E2prom_storage();
	}
}

void Key_Proc(void)
{
	Key_refresh();
	if(key_falling == B1)//存储位置切换键
	{
		TimerNum=(TimerNum%5)+1;
		Time_load();
	}
	if(key_state == B2)//短按时间位置（时、分、秒）切换键，长按存储键
	{
		if(B2_cnt<80)
		{
		 B2_cnt++;
		}
		else if(B2_cnt==80)//长按，状态切换，时间保存
		{
		 B2_cnt=100;
		 TimerState=Standby;
		 E2prom_storage_flag=1;//eeprom存储
		 Timer_Formal.hour=Timer_Temp.hour;
		 Timer_Formal.min=Timer_Temp.min;
		 Timer_Formal.sec=Timer_Temp.min;
		}
	}
	else if(B2_cnt&&key_state!=B2)//表示B2按下，且按键已松开
	{
	  if(B2_cnt!=100)//短按
		{
			if(TimerState!=Setting)
			{
			 TimerState=Setting;//当前为时间设置状态
			 TimerChoose=2;//切换到选择小时设置
		   Timer_Temp.hour=0;//临时时间清零
			 Timer_Temp.min=0;
			 Timer_Temp.sec=0;
			}
			else 
			{
			 B2_cnt=0;	
			 TimerChoose=(TimerChoose+1)%3;	
			}
		}
		else//长按，计数清零
		B2_cnt=0;		
	}
	
	if((key_falling==B3)&&(TimerState==Setting))
	{
		B3_cnt=0;
	 		switch(TimerChoose)
		 {
			 case 0:(Timer_Temp.hour>= 23)?(Timer_Temp.hour = 0 ):( Timer_Temp.hour = Timer_Temp.hour + 1);break;
			 case 1:(Timer_Temp.min>= 59)?(Timer_Temp.min = 0 ):( Timer_Temp.min = Timer_Temp.min + 1);break;	
			 case 2:(Timer_Temp.sec>= 59)?(Timer_Temp.sec = 0 ):( Timer_Temp.sec = Timer_Temp.sec + 1);break;
			 default:break;		 
		 }
	}
	if((key_state == B3)&&(TimerState==Setting))//时、分、秒数字增加键，长按快速增加数字
	{
    if(B3_cnt<80)
		{
			B3_cnt++;
		}
		else if(B3_cnt==80)//长按，对应设定位置时间快速加减
		{
		 B3_cnt=100;
		}
		if(B3_cnt==100)
		{
		 switch(TimerChoose)
		 {
			 case 0:(Timer_Temp.hour>= 23)?(Timer_Temp.hour = 0 ):( Timer_Temp.hour = Timer_Temp.hour + 1);break;
			 case 1:(Timer_Temp.min>= 59)?(Timer_Temp.min = 0 ):( Timer_Temp.min = Timer_Temp.min + 1);break;	
			 case 2:(Timer_Temp.sec>= 59)?(Timer_Temp.sec = 0 ):( Timer_Temp.sec = Timer_Temp.sec + 1);break;
			 default:break;		 
		 }
		}
	}
	if((key_rising==B3)&&(TimerState==Setting))
	{
	 B3_cnt=0;
	}
		
	if(key_state == B4)//电子定时器启动键
	{
		if(B4_cnt<80)
		{
		 B4_cnt++;
		}
		else if(B4_cnt==80)  //长按退出，并重新装载定时初值，
		{                     //1.储存位置已有数值，未有临时数值，定时取消后，装载储存位置初值。
		 B4_cnt=100;          //2.储存位置未有数值，使用的临时数值定时，定时取消后，无法启动定时器
		 TimerState=Standby;  //3.存储位置有数值，使用的临时数值定时，定时取消后，装载存储数值
		 Time_load();
		}
	}
	else if(B4_cnt&&key_state!=B4)//表示B4按下，且按键已松开
	{
	  if(B4_cnt!=100)//短按
		{
			if(TimerState == Running)
			{
       TimerState = Pause;
			}
			else  //1.处于取消运行状态，装载存储值 2.处于设置状态，装载临时值
			{
			 if(TimerState == Standby)
			 {
			  Time_load();
			 }
			 else if(TimerState == Setting)//未改变值,无存储
			 {
			  Timer_Formal.hour=Timer_Temp.hour;
				Timer_Formal.min=Timer_Temp.min;
				Timer_Formal.sec=Timer_Temp.min;
			 }
			if(Timer_Formal.hour||Timer_Formal.min||Timer_Formal.sec)
			 {
				 TimerState = Running;
			 }
			}
		}
		B4_cnt=0;		
	}
}

void Time_load(void)
{
  Timer_Formal.hour=Read_eeprom(TimerNum*3+0);
	Timer_Formal.min=Read_eeprom(TimerNum*3+1);
	Timer_Formal.sec=Read_eeprom(TimerNum*3+2);
	
	if(Timer_Formal.hour>23||Timer_Formal.min>59||Timer_Formal.sec>59)//消除原存储位置的数值大于时间的最大值，避免启动出现BUG
	{
		Timer_Formal.hour=0;
		Timer_Formal.min=0;
		Timer_Formal.sec=0;
	}
}

void E2prom_storage(void)
{
	if(E2prom_storage_flag==1)
	{
	  E2prom_storage_flag=0;
		Write_eeprom(TimerNum*3+0,Timer_Temp.hour);Delay_Ms(5);
		Write_eeprom(TimerNum*3+1,Timer_Temp.min);Delay_Ms(5);
		Write_eeprom(TimerNum*3+2,Timer_Temp.sec);Delay_Ms(5);
	}
}

void Led_PWM_Proc(void)
{
	if((TimerState==Running)&&(Led_flag))
	{
	  LED_Control(LED1,ON);
		pwm_init(80);		
	}
	else if((TimerState==Running)&&(!Led_flag))
	{
	 LED_Control(LED1,OFF);
	 pwm_init(80);		
	}
	if(TimerState!=Running)
	{
	 LED_Control(LED1,OFF);
	 pwm_init(0);		
	}	
}

void Lcd_Proc(void)
{
	/*                屏幕第一行                          */
  LCD_DisplayStringLine(Line0,"LQB Di Jiu Jie SaiTi     ");
	/*                屏幕第二行                          */ 
	LCD_DisplayStringLine(Line1,"  NO.");
	LCD_DisplayChar(Line1,224,TimerNum+0x30);
	/*                屏幕第四行                          */
	if(TimerState==Setting)
	{
		sprintf((char*)Timestring,"     %.2d: %.2d: %.2d         ",Timer_Temp.hour,Timer_Temp.min,Timer_Temp.sec);
		switch(TimerChoose)
		{
			case 0:highlight(Timestring,5);break;
			case 1:highlight(Timestring,9);break;
			case 2:highlight(Timestring,13);break;
			default:	break;
		}
	}
	else
	{
		sprintf((char*)Timestring,"     %.2d: %.2d: %.2d         ",Timer_Formal.hour,Timer_Formal.min,Timer_Formal.sec);
	  LCD_DisplayStringLine(Line4 ,Timestring);
	}
	/*                屏幕第七行                          */
	switch(TimerState)
	{
		case 0: LCD_DisplayStringLine(Line7,"       Standby    ");break;
		case 1: LCD_DisplayStringLine(Line7,"       Setting    ");break;
		case 2: LCD_DisplayStringLine(Line7,"       Running    ");break;
		case 3: LCD_DisplayStringLine(Line7,"        Pause     ");break;
		default:	break;
	}	
	/*                屏幕第九行                          */
	LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");		
}
/**
 * @brief  定时器4中断服务函数-1ms中断
 * @param  无
 * @retval 无
 */
void TIM4_IRQHandler(void)
{
	static uint16_t key_cnt=0;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		if(++key_cnt==10)
		{
		  key_cnt=0;
			Key_Proc();
		}
    if(++Led_cnt==500)
		{
		 Led_flag=!Led_flag;
		 Led_cnt=0;
		}
		if((TimerState==Running)&&(++Timer_cnt==1000))
		{
		  Timer_cnt=0;
			if(Timer_Formal.sec!=0)
			{
			 Timer_Formal.sec--;
			}
			else if(Timer_Formal.min!=0)
			{
			 Timer_Formal.min--;
			 Timer_Formal.sec=59;
			}
			else if(Timer_Formal.hour!=0)
			{
			 Timer_Formal.hour--;
			 Timer_Formal.min=59;
			 Timer_Formal.sec=59;
			}
			if((Timer_Formal.hour&&Timer_Formal.min&&Timer_Formal.sec)==0)
			{
			 TimerState=Standby;
			}
		}			
	}
}

/*****************************************************END OF FILE*********************************************************/	

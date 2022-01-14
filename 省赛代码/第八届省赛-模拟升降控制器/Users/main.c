/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  ������
  * @version V1.0
  * @date    2021-01-10
  * @brief   ����Ƕ��ʽ�ڰ˽�ʡ����-ģ������������
  *************************************************************************************************************************
  * @attention
  *
  * 
  * 
  *************************************************************************************************************************
  */
/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "delay.h" 
#include "lcd.h"
#include "tim.h"
#include "pwm.h" 
#include "rtc.h"
#include "stdio.h"
#include "string.h"
/* �궨�� -----------------------------------------------------------------------------------------------------------------*/
/* ��������¥��궨�� */
#define 		F1			0x0100 
#define 		F2			0x0200
#define 		F3			0x0400
#define 		F4			0x0800
/* �������з���궨�� */
#define			UP			1
#define			DOWN		0
/* ��������״̬�궨�� */
#define 		OFF					0
#define 		STOP				1
#define 		OPENNING		2
#define 		CLOSING			3
#define 		MOVING			4
/* LCD��ʾ���� */
uint8_t Timestring[20];
uint8_t Platstring[20];
/*	��������  */
uint16_t Current_platform=F1;//��ǰ¥��
uint8_t  direction = UP;//������ʻ����
uint16_t aim_floor = 0x0000;//Ŀ��¥��
uint16_t lcd_floor=0x0000;
uint8_t  state = OFF;//��������״̬

uint8_t  BlinkyLed_flag = OFF;//��ˮ�Ʊ�־
uint16_t led_cnt = 0;//ledˢ��ʱ�����
uint16_t BlinkyLed = 0x1000;//��ˮ�Ʊ�������ʼled5����

uint8_t  key_cnt = 0;//����ˢ��ʱ�����

uint8_t  ElevatorButton_flag=0;//���ݰ�ť��־λ 0δ���£�1����
uint16_t Elevator_cnt=0;//����ˢ��ʱ�����
uint16_t ElevatorRun_cnt=0;//����������ʱ�����

uint8_t  lcd_flash_flag=0;//����Ŀ��¥��lcd��˸��־λ
uint8_t  lcd_flag=0;
/*  ��������  */
void lcd_proc(void);
void key_proc(void);
void led_proc(void);
void pwm_proc(void);
void state_Check(void);
void Update_the_floor(void);
void RunningDirection_Check(void);
void EevatorGpio_init(void);
void Eevator_proc(void);



int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_Init();//��ʱ������ʼ��
	Delay_Ms(200);
	LCD_Init();//LCD��ʼ��
	Tim4_Init(); //��ʱ����ʼ��
	RTC_Init();//RTCʱ�ӳ�ʼ��
	LED_Init();//LED��ʼ��
	Key_Init();//������ʼ��
	EevatorGpio_init();//������GPIO��ʼ��
	PWM_Init(0,0);//PWM��ʼ��
	while(1)
	{	
		lcd_proc();	
		led_proc();
		pwm_proc();
		Eevator_proc();
	}
}

void lcd_proc(void)
{
 		LCD_DisplayStringLine(Line0,"LQB Di Ba Jie Sai Ti    ");

		LCD_DisplayStringLine(Line2,"  Current platform      ");
	
		if(lcd_flash_flag==0)
		{
			sprintf((char*)Platstring,"         %d",(Current_platform / 256 / 2 + 1)== 5 ? 4: (Current_platform / 256 / 2 + 1));
			LCD_DisplayStringLine(Line4,Platstring);
		}		
		else
		{
		  LCD_DisplayStringLine(Line4,"                       ");
		}
    
	   if((direction == UP)&&state==CLOSING)
		 {
			LCD_DisplayStringLine(Line6," Elevator_State:UP     ");	
		 }
		 else if((direction == DOWN)&&state==CLOSING)
		 {
			 LCD_DisplayStringLine(Line6," Elevator_State:DOWN   ");	
		 }
	  
			switch(state)
		{
			case 0:LCD_DisplayStringLine(Line7," Door_State:OFF    ");LCD_DisplayStringLine(Line6," Elevator_State:WAIT     ");break;
			case 1:LCD_DisplayStringLine(Line7," Door_State:STOP   ");break;
			case 2:LCD_DisplayStringLine(Line7," Door_State:OPEN   ");break;
			case 3:LCD_DisplayStringLine(Line7," Door_State:CLOS   ");break;
			case 4:LCD_DisplayStringLine(Line7," Door_State:MOVE   ");break;
			default:break;	
		}

		sprintf((char*)Timestring,"     %.2d: %.2d: %.2d",HOUR,MINUTE,SECOND);
		LCD_DisplayStringLine(Line8,Timestring);
	
		LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");		
}

void pwm_proc(void)
{
  static uint8_t pwm_state=0;
	if(pwm_state!=state)//���ݴ�������״̬
	{
	  if(state==MOVING)
		{
		  if(direction == UP)
				PWM_Init(80,0);
			else
				PWM_Init(60,0);
		}
		else if(state == OPENNING)
		{
		  PWM_Init(0,60);
		}
		else if(state == CLOSING)
		{
			PWM_Init(0,50);
		}
		else
		{
			PWM_Init(0,0);
		}
		pwm_state = state;
	}	
}

void led_proc(void)
{	
  if(MOVING != state)
	{
	 LED_Control(0xF000,OFF);//���еƹر�
	}
	else if(BlinkyLed_flag)
	{
	  BlinkyLed_flag=0;
	  LED_Control(BlinkyLed,ON);
		LED_Control(~(BlinkyLed)&0xF000 ,OFF);
		if(direction == UP)
		{
			if(BlinkyLed != 0x8000) 
				BlinkyLed <<= 1;
			else
				BlinkyLed = 0x1000;
		}
		else
		{
			if(BlinkyLed != 0x1000) 
				BlinkyLed >>= 1;
			else
				BlinkyLed = 0x8000;
		}
		
	}
		LED_Control(aim_floor,ON);
		LED_Control(~(aim_floor)&0x0F00 ,OFF);	//1111 1110   &   1111  1111= 1111 1110 
}                                           //1111 1101   &   1111  1111= 1111 1101 

void key_proc(void)
{
	if(state == OFF || state == STOP)
	{
		Key_refresh();
		if(key_falling == B1)
		{
      if(Current_platform != F1)
			{
			 aim_floor ^= F1;//��ͬΪ1����ͬΪ0     0x0000^0x0100=0x0100    0x0100^0x0100=0x0000
			 ElevatorButton_flag=1;
			 Elevator_cnt=0;
			}
		}
		else if(key_falling == B2)
		{
			if(Current_platform != F2)
			{
				aim_floor ^= F2;
				ElevatorButton_flag=1;
				Elevator_cnt=0;
			}
		}
		else if(key_falling == B3)
		{
			if(Current_platform != F3)
			{
				aim_floor ^= F3;
				ElevatorButton_flag=1;
				Elevator_cnt=0;
			}
		}
		else if(key_falling == B4)
		{
			if(Current_platform != F4)
			{
				aim_floor ^= F4;
				ElevatorButton_flag=1;
				Elevator_cnt=0;
			}
		}
		lcd_floor&=aim_floor;
	}
}

void RunningDirection_Check(void)
{
  if(aim_floor>Current_platform)
	{
	 direction = UP;
	}
	else
	 direction = DOWN;
}

void Update_the_floor(void)
{
  if(direction==UP)
	{
	 Current_platform<<=1;
	}
	else
	{
	 Current_platform>>=1;
	}
	
	aim_floor &= ~Current_platform;//Ŀ��¥����£�ȥ����ǰ¥��
	RunningDirection_Check();//���з�����
}

void state_Check(void)
{
	if(state==STOP)
	{
	 state=CLOSING;
	 ElevatorRun_cnt=4000;//����4s
	}
	else if(state==CLOSING)
	{
	 state=MOVING;
	 ElevatorRun_cnt=6000;//�������ϻ�������ʱ��6s
	}
	else if(state==MOVING)
	{
	 state=OPENNING;
	 ElevatorRun_cnt=4000;//����4s
	}
	else if(state==OPENNING)
	{
	 	Update_the_floor();//���µ�ǰ¥��
		
		if((lcd_floor&&aim_floor)==0)
		{
		 lcd_flag=1;//LCD��־λ��һ
		}	
		if(aim_floor == 0x0000)
			state = OFF;// ���û��Ŀ��ƽ̨����ô����ֹͣ����
		else 
			state = STOP;// �����ʣ��Ŀ��¥�㣬��ô���ݼ�������
		
		ElevatorRun_cnt = 2000;// ����ͣ������	
	}
}

void EevatorGpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5);	
}

void Eevator_proc(void)
{
	if(direction == UP)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
	
	if(state == OFF || state == STOP)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	}
}

void TIM4_IRQHandler(void)	
{
	static uint16_t lcd_cnt=0;//lcdˢ��ʱ�����
	static uint16_t tick_cnt = 0;//����������ʱ��Ƚϱ���
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);		
		
		if((state == MOVING) && ++led_cnt == 400)//����ʱÿ0.4Sˢ��led
		{
			led_cnt = 0;
			BlinkyLed_flag = 1;
		}
		if(++key_cnt==10)
		{
			key_cnt=0;
			key_proc();
		}
		if(ElevatorButton_flag && (++Elevator_cnt==1000))//��ť�����£��ȴ�1sδ���°�������ִ�У����к��������з����顣
		{
		 ElevatorButton_flag=0;
		 Elevator_cnt=0;
		 state=CLOSING;//�м����µ��ݹ���
		 ElevatorRun_cnt=4000;
		 RunningDirection_Check();
		}
		if(ElevatorButton_flag == 0 &&  state != OFF && ++tick_cnt >= ElevatorRun_cnt)//δ�а�ť���£��ҵ��ݲ�����ֹͣ����״̬���ȴ���������ʱ�䣬���к���
		{
			tick_cnt = 0;
			state_Check();
		}
		if((lcd_flag)&&(++lcd_cnt%250)==0)
		{
		 if(lcd_cnt==1000)
		 {
		   lcd_cnt=0;
			 lcd_flag=0;
			 lcd_flash_flag=0;
		 }
		 else
		 {
		  lcd_flash_flag=!lcd_flash_flag;
		 }		
		}
	}
}
/*****************************************************END OF FILE*********************************************************/	

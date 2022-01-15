/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2021-01-15
  * @brief   ����Ƕ��ʽ�ھŽ�ʡ����-���Ӷ�ʱ��
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
void Time_load(void);
void Led_PWM_Proc(void);
void E2prom_storage(void);
/* LCDʱ����ʾ���� */
uint8_t Timestring[20];
/* �������� -----------------------------------------------------------------------------------------------------------------*/
uint8_t E2prom_storage_flag;//eeprom�洢��־λ
uint8_t TimerState;//��ʱ��״̬
uint8_t TimerChoose;//ѡ������ʱ��
uint8_t TimerNum=1;//��ʱ���洢���
uint16_t Led_cnt;//LED��˸ˢ��ʱ�����
uint16_t Timer_cnt;//���Ӷ�ʱ��ˢ��ʱ�����
uint8_t Led_flag;//LED��˸��־λ
uint8_t B2_cnt;//B2�������̰�ʱ�����
uint8_t B3_cnt;//B3�������̰�ʱ�����
uint8_t B4_cnt;//B4�������̰�ʱ�����

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
	if(key_falling == B1)//�洢λ���л���
	{
		TimerNum=(TimerNum%5)+1;
		Time_load();
	}
	if(key_state == B2)//�̰�ʱ��λ�ã�ʱ���֡��룩�л����������洢��
	{
		if(B2_cnt<80)
		{
		 B2_cnt++;
		}
		else if(B2_cnt==80)//������״̬�л���ʱ�䱣��
		{
		 B2_cnt=100;
		 TimerState=Standby;
		 E2prom_storage_flag=1;//eeprom�洢
		 Timer_Formal.hour=Timer_Temp.hour;
		 Timer_Formal.min=Timer_Temp.min;
		 Timer_Formal.sec=Timer_Temp.min;
		}
	}
	else if(B2_cnt&&key_state!=B2)//��ʾB2���£��Ұ������ɿ�
	{
	  if(B2_cnt!=100)//�̰�
		{
			if(TimerState!=Setting)
			{
			 TimerState=Setting;//��ǰΪʱ������״̬
			 TimerChoose=2;//�л���ѡ��Сʱ����
		   Timer_Temp.hour=0;//��ʱʱ������
			 Timer_Temp.min=0;
			 Timer_Temp.sec=0;
			}
			else 
			{
			 B2_cnt=0;	
			 TimerChoose=(TimerChoose+1)%3;	
			}
		}
		else//��������������
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
	if((key_state == B3)&&(TimerState==Setting))//ʱ���֡����������Ӽ�������������������
	{
    if(B3_cnt<80)
		{
			B3_cnt++;
		}
		else if(B3_cnt==80)//��������Ӧ�趨λ��ʱ����ټӼ�
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
		
	if(key_state == B4)//���Ӷ�ʱ��������
	{
		if(B4_cnt<80)
		{
		 B4_cnt++;
		}
		else if(B4_cnt==80)  //�����˳���������װ�ض�ʱ��ֵ��
		{                     //1.����λ��������ֵ��δ����ʱ��ֵ����ʱȡ����װ�ش���λ�ó�ֵ��
		 B4_cnt=100;          //2.����λ��δ����ֵ��ʹ�õ���ʱ��ֵ��ʱ����ʱȡ�����޷�������ʱ��
		 TimerState=Standby;  //3.�洢λ������ֵ��ʹ�õ���ʱ��ֵ��ʱ����ʱȡ����װ�ش洢��ֵ
		 Time_load();
		}
	}
	else if(B4_cnt&&key_state!=B4)//��ʾB4���£��Ұ������ɿ�
	{
	  if(B4_cnt!=100)//�̰�
		{
			if(TimerState == Running)
			{
       TimerState = Pause;
			}
			else  //1.����ȡ������״̬��װ�ش洢ֵ 2.��������״̬��װ����ʱֵ
			{
			 if(TimerState == Standby)
			 {
			  Time_load();
			 }
			 else if(TimerState == Setting)//δ�ı�ֵ,�޴洢
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
	
	if(Timer_Formal.hour>23||Timer_Formal.min>59||Timer_Formal.sec>59)//����ԭ�洢λ�õ���ֵ����ʱ������ֵ��������������BUG
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
	/*                ��Ļ��һ��                          */
  LCD_DisplayStringLine(Line0,"LQB Di Jiu Jie SaiTi     ");
	/*                ��Ļ�ڶ���                          */ 
	LCD_DisplayStringLine(Line1,"  NO.");
	LCD_DisplayChar(Line1,224,TimerNum+0x30);
	/*                ��Ļ������                          */
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
	/*                ��Ļ������                          */
	switch(TimerState)
	{
		case 0: LCD_DisplayStringLine(Line7,"       Standby    ");break;
		case 1: LCD_DisplayStringLine(Line7,"       Setting    ");break;
		case 2: LCD_DisplayStringLine(Line7,"       Running    ");break;
		case 3: LCD_DisplayStringLine(Line7,"        Pause     ");break;
		default:	break;
	}	
	/*                ��Ļ�ھ���                          */
	LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");		
}
/**
 * @brief  ��ʱ��4�жϷ�����-1ms�ж�
 * @param  ��
 * @retval ��
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

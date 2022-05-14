/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef __MYCONFIG_H
#define	__MYCONFIG_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "delay.h"
#include "lcd.h"
#include "usart.h"
#include "pwm.h"
#include "tim.h"
#include "i2c.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/
#define TIMERCOUNT  (3)//���������ʱ����
#define TIMERINDEX1 (0)//�¼�1    
#define TIMERINDEX2 (1)//�¼�2
#define TIMERINDEX3 (2)	 		

#define TIMEISUP		(1)//ʱ�䵽
#define TIMEISNOTUP	(0)//ʱ��û��

#define SHOP		(0)//����
#define PRICE		(1)//�۸�
#define REP			(2)//���




/* �ṩ������C�ļ����õĺ��� --------------------------------------------------------------------------------------------*/
unsigned char xDelay(unsigned char xIndex,unsigned char xTime);
void Lcd_Proc(void);
void Led_PWM_Proc(void);
void Key_Proc(void);
void StringCheck(void);
void Usart_Proc(void);
void E2prom_Proc(void);
void SystemPara_Init(void);
#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

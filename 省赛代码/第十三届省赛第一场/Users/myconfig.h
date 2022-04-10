/**
  *************************************************************************************************************************
  * @file    myconfig.h
  * @author  amkl
  * @version V1.0
  * @date    2022-04-10
  * @brief   ����Ƕ��ʽ�������������ţ����ò���,ͷ�ļ�
  *************************************************************************************************************************
  * @attention
  * 
  * 
  * 
  *************************************************************************************************************************
  */


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
#include "stdio.h"
#include "math.h"
#include "string.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/
#define TIMERCOUNT  (4)//���������ʱ����
#define TIMERINDEX1 (0)//�¼�1    
#define TIMERINDEX2 (1)//�¼�2
#define TIMERINDEX3 (2)//�¼�3 		
#define TIMERINDEX4 (3)//�¼�4 

#define TIMEISUP		(1)//ʱ�䵽
#define TIMEISNOTUP	(0)//ʱ��û��

#define PSD  (0)
#define STA  (1)

typedef struct
{
  uint8_t  Index;
	uint8_t  Password[3];
	uint8_t  InputPassword[3];
	uint8_t  UartPassword[3];
	uint8_t  PasswordRight;
	uint8_t  PasswordError;
	uint8_t  B1Reset;
	uint8_t  B2Reset;
	uint8_t  B3Reset;
	uint8_t  Time5s_flag;
	uint8_t  ErrorCnt;
	uint8_t  ErrorTime5s_flag;
	
	uint8_t  Duty;
	uint16_t Freq;
}System_InitTypeDef;

extern System_InitTypeDef Variable;

/* �ṩ������C�ļ����õĺ��� --------------------------------------------------------------------------------------------*/
unsigned char xDelay(unsigned char xIndex,unsigned char xTime);
void Lcd_Proc(void);
void Led2_Proc(void);
void Key_Proc(void);
void Password_Check(void);
void Pwm_Proc(void);
void Usart_Proc(void);
void StringCheck(void);
void SystemPara_Init(void);
#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

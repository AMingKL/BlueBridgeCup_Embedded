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
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
#define TIMERCOUNT  (3)//定义软件定时个数
#define TIMERINDEX1 (0)//事件1    
#define TIMERINDEX2 (1)//事件2
#define TIMERINDEX3 (2)	 		

#define TIMEISUP		(1)//时间到
#define TIMEISNOTUP	(0)//时间没到

#define SHOP		(0)//购买
#define PRICE		(1)//价格
#define REP			(2)//库存




/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
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

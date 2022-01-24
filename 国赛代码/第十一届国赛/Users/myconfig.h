/**
  *************************************************************************************************************************
  * @file    myconfig.h
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-15
  * @brief   蓝桥嵌入式开发板外设引脚，配置参数,头文件
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
#include "lcd.h"
#include "pwm.h"
#include "tim.h"
#include "i2c.h"
#include "delay.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_adc.h"
#include "bsp_pulse.h"
#include "bsp_AnalogVoltage.h"
#include "string.h"
#include "stdio.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
#define DATA (0)
#define PARA (1)

#define PULS1 (0)
#define PULS2 (1)



typedef struct
{
  uint8_t Interface;//LCD界面
	
	float VRp5_Value;//rp5电压值
	float VRp6_Value;//rp6电压值
	
	uint32_t F1;//Puls1输出频率值
	uint32_t F2;//Puls2输出频率值
	
	uint8_t Freq_Led;//频率指示灯
	uint8_t Volt_Led;//电压指示灯	
	uint8_t Freq_TempLed;//频率临时指示灯
	uint8_t Volt_TempLed;//电压临时指示灯
	
	uint8_t Follow_Pulsx;//跟随Pulsex输出，x:1 or 2
		
}System_InitTypeDef;

System_InitTypeDef System;

typedef struct
{
  uint8_t Read_ADC;//adc读取标志位
	uint8_t Read_KEY;//key读取标志位
	uint8_t Read_PULS;//puls读取标志位		
}Flag_InitTypeDef;

Flag_InitTypeDef Flag;



/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

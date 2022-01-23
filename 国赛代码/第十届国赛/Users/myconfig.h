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
#include "bsp_led.h"
#include "bsp_key.h"
#include "delay.h"
#include "bsp_ds18b20.h"
#include "bsp_adc.h"
#include "bsp_AnalogVoltage.h"
#include "bsp_digital.h"
#include "bsp_caputer.h" 
#include "lcd.h"
#include "usart.h"
#include "tim.h"
#include "i2c.h"
#include "string.h"
#include "stdio.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
#define MAIN (0)
#define PARA (1)

#define AO1 (1)
#define AO2 (2)


typedef struct
{
  uint8_t  Interface;//lcd界面
	
	float VRP5_value;//Vrp5读取值
	float VRP6_value;//Vrp6读取值
	
	
	float Temperature;//温度值
	
	uint16_t Set_ParaCnt;//参数变动次数变量
	
	uint8_t Temperature_limit;//温度超限值
	uint8_t Choose_Channel;//选择比较通道变量
	uint8_t Choose_Para;//选择参数变量
	
	uint16_t PA7_DutyRatio;
	
	
	
}MeasuringInstrument;


MeasuringInstrument System;

/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

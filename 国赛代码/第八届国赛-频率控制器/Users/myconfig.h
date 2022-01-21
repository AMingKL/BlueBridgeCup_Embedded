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
#include "lcd.h"
#include "tim.h"
#include "i2c.h"
#include "pwm.h" 
#include "bsp_pulse.h"
#include "bsp_adctwo.h"
#include "stdio.h"
#include "string.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
#define MEASURE (0)
#define CONFIG  (1)

#define PARAM_DIV  (0)
#define PARAM_MUL  (1)

typedef struct
{
  uint8_t  Interface;//lcd界面
	
	uint32_t  PULS1_Freq;
	uint32_t  PULS2_Freq;
	
	float   Rp5_adc_value;
	float   Rp6_adc_value;
	
	uint8_t Divider;
	uint8_t Multiplier;
	
	uint8_t DividerTemp;
	uint8_t MultiplierTemp;
	
	uint8_t ChooseTheParam;
	
}FrequencyController;

FrequencyController System;



/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

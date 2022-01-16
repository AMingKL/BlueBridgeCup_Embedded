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
#include "bsp_adc.h"
#include "lcd.h"
#include "tim.h"
#include "stdio.h"
#include "string.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
#define Main    (0)
#define Setting (1)

#define Upper  	(0)
#define Lower  	(1)
#define Normal  (2)

typedef struct
{
	uint8_t  UpperLed;//上限led指示灯
	uint8_t  LowerLed;//下限led指示灯
	uint8_t  state;//电压状态
	uint8_t  Interface;//Lcd界面
	uint8_t  Highlight;//高亮选择参数
	uint16_t VoltValue;//电压测量值
	uint16_t MaxVoltValue;//电压上限值
	uint16_t MinVoltValue;//电压下限值
}VoltageDetector;

VoltageDetector VoltSystem;

/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

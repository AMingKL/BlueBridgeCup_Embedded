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
#include "delay.h"
#include "lcd.h"
#include "usart.h"
#include "tim.h"
#include "i2c.h"
#include "bsp_adc.h"
#include "bsp_adc_key.h"
#include "bsp_AnalogVoltage.h"
#include "string.h"
#include "stdio.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
#define DATA (0)
#define PARA (1)


typedef struct
{
  uint8_t Interface;//lcd界面
	
	uint8_t  number;//货物编号
	float   Goods1price;//货物1单价
	float   Goods2price;//货物2单价
	float   Goods3price;//货物3单价
	float   weight;//货物重量
	float   totalprice;//货物总价
	
	uint8_t  setnumcnt;//设置次数
	
	uint8_t  ChooseGoods;//选择货物键
	
}ElectronicScale;

ElectronicScale System;



/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

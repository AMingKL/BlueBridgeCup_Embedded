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
#include "usart.h"
#include "pwm.h"
#include "tim.h"
#include "stdio.h"
#include "string.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
#define CarShow 	(0)//车位显示
#define RateSet 	(1)//费率设置

#define PULSE    	(0)//脉冲
#define LOWLEVEL	(1)//低电平

#define EMPTY   	(0)//空
#define FULL	    (1)//满


typedef struct
{
	uint8_t  Interface;//Lcd界面
	
	uint8_t	 PA7_state;//PA7输出状态
	
	uint16_t CarStopTime;//停车时间
	uint32_t ParkingFee;//停车费用
	
	uint16_t CNBR_Rate;//CNBR停车的费率
	uint16_t VNBR_Rate;//VNBR停车的费率
	uint16_t CNBR_RateTemp;//CNBR停车的临时费率
	uint16_t VNBR_RateTemp;//VNBR停车的临时费率
	
	uint8_t  CNBR_Number;//CNBR停车数量
	uint8_t  VNBR_Number;//VNBR停车数量
	uint8_t  IDLE_Number;//空闲车位数量

}ParkingBillingSystem;

ParkingBillingSystem System;


typedef struct
{
  uint8_t cartype[5];//车辆类型
	uint8_t carid[5];//车俩编号
	uint8_t yearin;
	uint8_t monthin;
	uint8_t dayin;
	uint8_t hourin;
	uint8_t minin;
	uint8_t secin;
	uint8_t state;

}StorageSystem;//信息储存系统

StorageSystem car[8] = {0};


/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

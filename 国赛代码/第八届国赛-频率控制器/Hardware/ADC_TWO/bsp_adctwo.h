/**
  *************************************************************************************************************************
  * @file    adctwo.h
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   蓝桥杯嵌入式扩展板两路AD模块头文件配置
  *************************************************************************************************************************
  * @attention
  * 
  * 
  * 
  *************************************************************************************************************************
  */


/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef __BSP_ADCTWO_H
#define	__BSP_ADCTWO_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "stm32f10x.h"

/* 定义 -----------------------------------------------------------------------------------------------------------------*/

/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
void ADC_TWO_Init(void);
uint16_t get_2_adc(uint8_t Channel);
#endif /* __BSP_ADCTWO_H */

/*****************************************************END OF FILE*********************************************************/	



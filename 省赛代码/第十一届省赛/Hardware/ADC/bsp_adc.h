/**
  *************************************************************************************************************************
  * @file    adc.h
  * @author  刘刘洪明
  * @version V1.0
  * @date    2022-01-10
  * @brief   蓝桥杯嵌入式开发板ADC配置
  *************************************************************************************************************************
  * @attention
  * 
  * ADC接口说明：
	*    PB0口采集电压值
  * 
  *************************************************************************************************************************
  */

/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef __BSP_ADC_H
#define __BSP_ADC_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "stm32f10x.h"

/* 定义 -----------------------------------------------------------------------------------------------------------------*/
/* ADC值 */
extern float adc_temp;

/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
void Adc_Init(void);   			   /* ADC初始化函数 */
uint16_t Get_Adc(void);				 /* 获得ADC值 */
uint16_t Get_AdcAverage(void); /* 多次采集求平均值 */

#endif /* __BSP_ADC_H */

/*****************************************************END OF FILE*********************************************************/	

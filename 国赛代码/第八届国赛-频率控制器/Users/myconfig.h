/**
  *************************************************************************************************************************
  * @file    myconfig.h
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-15
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
#include "tim.h"
#include "i2c.h"
#include "pwm.h" 
#include "bsp_pulse.h"
#include "bsp_adctwo.h"
#include "stdio.h"
#include "string.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/
#define MEASURE (0)
#define CONFIG  (1)

#define PARAM_DIV  (0)
#define PARAM_MUL  (1)

typedef struct
{
  uint8_t  Interface;//lcd����
	
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



/* �ṩ������C�ļ����õĺ��� --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

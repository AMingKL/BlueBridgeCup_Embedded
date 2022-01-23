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
/* ���� -----------------------------------------------------------------------------------------------------------------*/
#define MAIN (0)
#define PARA (1)

#define AO1 (1)
#define AO2 (2)


typedef struct
{
  uint8_t  Interface;//lcd����
	
	float VRP5_value;//Vrp5��ȡֵ
	float VRP6_value;//Vrp6��ȡֵ
	
	
	float Temperature;//�¶�ֵ
	
	uint16_t Set_ParaCnt;//�����䶯��������
	
	uint8_t Temperature_limit;//�¶ȳ���ֵ
	uint8_t Choose_Channel;//ѡ��Ƚ�ͨ������
	uint8_t Choose_Para;//ѡ���������
	
	uint16_t PA7_DutyRatio;
	
	
	
}MeasuringInstrument;


MeasuringInstrument System;

/* �ṩ������C�ļ����õĺ��� --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

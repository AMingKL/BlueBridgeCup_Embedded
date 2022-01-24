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
/* ���� -----------------------------------------------------------------------------------------------------------------*/
#define DATA (0)
#define PARA (1)

#define PULS1 (0)
#define PULS2 (1)



typedef struct
{
  uint8_t Interface;//LCD����
	
	float VRp5_Value;//rp5��ѹֵ
	float VRp6_Value;//rp6��ѹֵ
	
	uint32_t F1;//Puls1���Ƶ��ֵ
	uint32_t F2;//Puls2���Ƶ��ֵ
	
	uint8_t Freq_Led;//Ƶ��ָʾ��
	uint8_t Volt_Led;//��ѹָʾ��	
	uint8_t Freq_TempLed;//Ƶ����ʱָʾ��
	uint8_t Volt_TempLed;//��ѹ��ʱָʾ��
	
	uint8_t Follow_Pulsx;//����Pulsex�����x:1 or 2
		
}System_InitTypeDef;

System_InitTypeDef System;

typedef struct
{
  uint8_t Read_ADC;//adc��ȡ��־λ
	uint8_t Read_KEY;//key��ȡ��־λ
	uint8_t Read_PULS;//puls��ȡ��־λ		
}Flag_InitTypeDef;

Flag_InitTypeDef Flag;



/* �ṩ������C�ļ����õĺ��� --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

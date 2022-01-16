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
#include "bsp_adc.h"
#include "lcd.h"
#include "tim.h"
#include "stdio.h"
#include "string.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/
#define Main    (0)
#define Setting (1)

#define Upper  	(0)
#define Lower  	(1)
#define Normal  (2)

typedef struct
{
	uint8_t  UpperLed;//����ledָʾ��
	uint8_t  LowerLed;//����ledָʾ��
	uint8_t  state;//��ѹ״̬
	uint8_t  Interface;//Lcd����
	uint8_t  Highlight;//����ѡ�����
	uint16_t VoltValue;//��ѹ����ֵ
	uint16_t MaxVoltValue;//��ѹ����ֵ
	uint16_t MinVoltValue;//��ѹ����ֵ
}VoltageDetector;

VoltageDetector VoltSystem;

/* �ṩ������C�ļ����õĺ��� --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

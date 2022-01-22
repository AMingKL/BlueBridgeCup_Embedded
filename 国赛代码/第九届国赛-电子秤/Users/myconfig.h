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
/* ���� -----------------------------------------------------------------------------------------------------------------*/
#define DATA (0)
#define PARA (1)


typedef struct
{
  uint8_t Interface;//lcd����
	
	uint8_t  number;//������
	float   Goods1price;//����1����
	float   Goods2price;//����2����
	float   Goods3price;//����3����
	float   weight;//��������
	float   totalprice;//�����ܼ�
	
	uint8_t  setnumcnt;//���ô���
	
	uint8_t  ChooseGoods;//ѡ������
	
}ElectronicScale;

ElectronicScale System;



/* �ṩ������C�ļ����õĺ��� --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

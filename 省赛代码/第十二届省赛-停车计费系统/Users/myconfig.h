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
#include "usart.h"
#include "pwm.h"
#include "tim.h"
#include "stdio.h"
#include "string.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/
#define CarShow 	(0)//��λ��ʾ
#define RateSet 	(1)//��������

#define PULSE    	(0)//����
#define LOWLEVEL	(1)//�͵�ƽ

#define EMPTY   	(0)//��
#define FULL	    (1)//��


typedef struct
{
	uint8_t  Interface;//Lcd����
	
	uint8_t	 PA7_state;//PA7���״̬
	
	uint16_t CarStopTime;//ͣ��ʱ��
	uint32_t ParkingFee;//ͣ������
	
	uint16_t CNBR_Rate;//CNBRͣ���ķ���
	uint16_t VNBR_Rate;//VNBRͣ���ķ���
	uint16_t CNBR_RateTemp;//CNBRͣ������ʱ����
	uint16_t VNBR_RateTemp;//VNBRͣ������ʱ����
	
	uint8_t  CNBR_Number;//CNBRͣ������
	uint8_t  VNBR_Number;//VNBRͣ������
	uint8_t  IDLE_Number;//���г�λ����

}ParkingBillingSystem;

ParkingBillingSystem System;


typedef struct
{
  uint8_t cartype[5];//��������
	uint8_t carid[5];//�������
	uint8_t yearin;
	uint8_t monthin;
	uint8_t dayin;
	uint8_t hourin;
	uint8_t minin;
	uint8_t secin;
	uint8_t state;

}StorageSystem;//��Ϣ����ϵͳ

StorageSystem car[8] = {0};


/* �ṩ������C�ļ����õĺ��� --------------------------------------------------------------------------------------------*/

#endif /* __MYCONFIG_H */

/*****************************************************END OF FILE*********************************************************/	

/**
  *************************************************************************************************************************
  * @file    i2c.h
  * @author  ������
  * @version V1.0
  * @date    2022-01-10
  * @brief   ���ű�Ƕ��ʽ������I2Cģ��
  *************************************************************************************************************************
  * @attention
  * 
  * CT117EǶ��ʽ������GPIOģ��I2C������������ �����ű��ٷ��ṩ
  * ģ��I2C�ӿ�˵�� 
	*     SDA--PB7
  *     SCL--PB6	
	*
  *************************************************************************************************************************
  */
	
/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef  __I2C_H__
#define  __I2C_H__

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "stm32f10x.h"

/** I2C ���߽ӿ� */
#define I2C_PORT GPIOB
#define SDA_Pin	GPIO_Pin_7
#define SCL_Pin GPIO_Pin_6

#define FAILURE 0
#define SUCCESS 1

void delay_iic(unsigned int n);

void I2CStart(void);
void I2CStop(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
unsigned char I2CWaitAck(void);

void I2CSendByte(unsigned char cSendByte);
unsigned char I2CReceiveByte(void);

/* �ṩ������C�ļ����õĺ��� --------------------------------------------------------------------------------------------*/
void i2c_init(void);
void Write_eeprom(unsigned char add,unsigned char dat);
unsigned char Read_eeprom(unsigned char add);

#endif


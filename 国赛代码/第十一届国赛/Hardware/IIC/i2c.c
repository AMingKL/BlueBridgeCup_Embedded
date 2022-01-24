/**
  *************************************************************************************************************************
  * @file    i2c.c
  * @author  ������
  * @version V1.0
  * @date    2022-01-10
  * @brief   ���ű�Ƕ��ʽ������I2Cģ��
  *************************************************************************************************************************
  * @attention
  *
  * CT117EǶ��ʽ������GPIOģ��I2C������������ �����ű��ٷ��ṩ
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "i2c.h"


//����SDA�ź���Ϊ����ģʽ
void SDA_Input_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 

	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

//����SDA�ź���Ϊ���ģʽ
void SDA_Output_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

//
void SDA_Output( uint16_t val )
{
	if ( val ) 
	{
		GPIO_SetBits(I2C_PORT,SDA_Pin);
	} 
	else 
	{
		GPIO_ResetBits(I2C_PORT,SDA_Pin);
	}
}

//
void SCL_Output( uint16_t val )
{
	if ( val ) 
	{
		GPIO_SetBits(I2C_PORT,SCL_Pin);
	} 
	else 
	{
		GPIO_ResetBits(I2C_PORT,SCL_Pin);
	}
}

//
uint8_t SDA_Input()
{
	return GPIO_ReadInputDataBit( I2C_PORT, SDA_Pin);
}

//��ʱ����
void delay_iic(unsigned int n)
{
	unsigned int i;
	for ( i=0;i<n;++i);
}

//I2C��������
void I2CStart(void)
{
	SDA_Output(1);delay_iic(500);
	SCL_Output(1);delay_iic(500);
	SDA_Output(0);delay_iic(500);
	SCL_Output(0);delay_iic(500);
}

//I2C����ֹͣ
void I2CStop(void)
{
	SCL_Output(0); delay_iic(500);
	SDA_Output(0); delay_iic(500);
	SCL_Output(1); delay_iic(500);
	SDA_Output(1); delay_iic(500);

}

//�ȴ�Ӧ��
unsigned char I2CWaitAck(void)
{
	unsigned short cErrTime = 5;
	SDA_Input_Mode(); 
	delay_iic(500);
	SCL_Output(1);delay_iic(500);
	while(SDA_Input())
	{
		cErrTime--;
		delay_iic(500);
		if (0 == cErrTime)
		{
			SDA_Output_Mode();
			I2CStop();
			return FAILURE;
		}
	}
	SDA_Output_Mode();
	SCL_Output(0);delay_iic(500); 
	return SUCCESS;
}

//����Ӧ��λ
void I2CSendAck(void)
{
	SDA_Output(0);delay_iic(500);
	delay_iic(500);
	SCL_Output(1); delay_iic(500);
	SCL_Output(0); delay_iic(500);

}

//
void I2CSendNotAck(void)
{
	SDA_Output(1);
	delay_iic(500);
	SCL_Output(1); delay_iic(500);
	SCL_Output(0); delay_iic(500);

}

//ͨ��I2C���߷���һ���ֽ�����
void I2CSendByte(unsigned char cSendByte)
{
	unsigned char  i = 8;
	while (i--)
	{
		SCL_Output(0);delay_iic(500); 
		SDA_Output(cSendByte & 0x80); delay_iic(500);
		cSendByte += cSendByte;
		delay_iic(500); 
		SCL_Output(1);delay_iic(500); 
	}
	SCL_Output(0);delay_iic(500); 
}

//��I2C���߽���һ���ֽ�����
unsigned char I2CReceiveByte(void)
{
	unsigned char i = 8;
	unsigned char cR_Byte = 0;
	SDA_Input_Mode(); 
	while (i--)
	{
		cR_Byte += cR_Byte;
		SCL_Output(0);delay_iic(500); 
		delay_iic(500); 
		SCL_Output(1);delay_iic(500); 
		cR_Byte |=  SDA_Input(); 
	}
	SCL_Output(0);delay_iic(500); 
	SDA_Output_Mode();
	return cR_Byte;
}

//I2C���߳�ʼ��
void i2c_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SDA_Pin | SCL_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 // **

	GPIO_Init(I2C_PORT, &GPIO_InitStructure);

}

//void Write_eeprom(unsigned char add,unsigned char dat)
//{

//}
//unsigned char Read_eeprom(unsigned char add)
//{

//}
//��Ҫ�Լ�д�ĳ��򣬶�дAT24C02
/**
  * @˵��     ��AT24C02ָ����ַд��һ���ֽ�����
  * @����     add:AT24C02�ڲ��洢��ַ
  * @����     dat:д������
  * @����ֵ   None
  */
void Write_eeprom(unsigned char add,unsigned char dat)
{
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	
	I2CSendByte(add);
	I2CWaitAck();
	I2CSendByte(dat);
	I2CWaitAck();
	I2CStop();
}
/**
  * @˵��     ��AT24C02ָ����ַ����һ���ֽ�����
  * @����     add:AT24C02�ڲ��洢��ַ
  * @����ֵ   dat:��������
  */
unsigned char Read_eeprom(unsigned char add)
{
	unsigned char dat;
	
	I2CStart();
	I2CSendByte(0xA0);
	I2CWaitAck();
	
	I2CSendByte(add);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(0xA1);
	I2CWaitAck();
	dat = I2CReceiveByte();
	I2CSendNotAck();
	I2CStop();
	
	return dat;
}
/*****************************************************END OF FILE*********************************************************/	

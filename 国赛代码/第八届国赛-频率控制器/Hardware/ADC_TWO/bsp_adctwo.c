/**
  *************************************************************************************************************************
  * @file    adctwo.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   蓝桥杯嵌入式扩展板两路AD模块
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_adctwo.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  
 * @param  无 参数
 * @retval 无
 */



//
void ADC_TWO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//蓝桥杯扩展板因为都是ADC1采集，所以配置独立通道即可
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 2;//双路AD采集通道
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channels configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_239Cycles5);
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}



uint16_t get_2_adc(uint8_t Channel)  
{
	uint8_t i,j;
	uint16_t t[50];
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	ADC_RegularChannelConfig(ADC1, Channel, 1, ADC_SampleTime_239Cycles5);  
	for(i=0;i<50;i++)
	{
		t[i]=ADC_GetConversionValue (ADC1);
	}
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
	for(i=0;i<50-1;i++)   //滤波
	{
		for(j=0;j<50-1-i;j++)
		{
			if(t[j]>t[j+1])
			{
				t[j]=t[j] ^ t[j+1];
				t[j+1]=t[j] ^ t[j+1];
				t[j]=t[j] ^ t[j+1];
			}
		}
	}
	return (t[24]+t[25])/2;
}	

#if 0

uint8_t  buff[20];
uint16_t x,y;

ADC_TWO_Init();
while(1)
{
		x = get_2_adc(ADC_Channel_4);
		y = get_2_adc(ADC_Channel_5);
				
		snprintf((char *)buff, sizeof(buff), " VRp5:%3.2fV", x/4095.*3.3);
		LCD_DisplayStringLine(Line6, buff);
		snprintf((char *)buff, sizeof(buff), " VRp6:%3.2fV", y/4095.*3.3);
		LCD_DisplayStringLine(Line7, buff);	
		Delay_Ms(200);
}

#endif 

/*****************************************************END OF FILE*********************************************************/	

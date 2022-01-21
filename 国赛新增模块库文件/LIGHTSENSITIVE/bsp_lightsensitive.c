/**
  *************************************************************************************************************************
  * @file    lightsensitive.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   蓝桥杯嵌入式扩展板光敏电阻开关量模拟量配置
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_lightsensitive.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  
 * @param  无 
 * @retval 无
 */
void lightsensitive_ao_do_init(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

  /* Configure PC.01, PC.02 and PC.04 (ADC Channel11, Channel12 and Channel14)
    as analog input ----------------------------------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
   /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channels configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5); //通道四
  
  ADC_Cmd(ADC1, ENABLE);
  
  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));	
}

uint16_t get_adcval(void)//AO
{
	uint8_t i;
	uint16_t t[10];
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
	for(i=0;i<10;i++)
	{
		t[i]=ADC_GetConversionValue (ADC1);
	}
	for(i=1;i<10;i++)
	{
		t[0] +=t[i];
	}	 
	
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);		  
	return t[0]/10;
}
uint8_t get_status(void)//DO   1:灯关太暗	 0:灯关太亮  ,这个暗和亮的程度主要靠RP7来调节
{
	uint8_t t;
	t=GPIO_ReadInputDataBit (GPIOA,GPIO_Pin_3);
	return t;
} 



/*****************************************************END OF FILE*********************************************************/	

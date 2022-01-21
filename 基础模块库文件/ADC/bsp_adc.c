/**
  *************************************************************************************************************************
  * @file    adc.c
  * @author  刘洪明
  * @version V1.0
  * @date    2022-01-10
  * @brief   蓝桥杯嵌入式开发板ADC配置
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_adc.h"
#include "delay.h"

/* 定义 -----------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  ADC初始化函数
 * @param  无
 * @retval 无
 */
void Adc_Init(void) 
{
	/* 定义一个ADC_InitTypeDef类型的结构体 */
	ADC_InitTypeDef ADC_InitStructure; 
	
	/* 定义一个GPIO_InitTypeDef类型的结构体 */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ADC1时钟使能，GPIOB时钟使能 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE); 
	
	/* 设置PB0为模拟输入 */
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 设置ADC分频因子6：72M/6=12,ADC最大时间不能超过14M */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   
	
	/* 复位ADC1 */
	ADC_DeInit(ADC1);  
	
	/* ADC初始化 */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			/* ADC工作模式:ADC1和ADC2工作在独立模式 */
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						/* 模数转换工作在单通道模式 */
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			/* 模数转换工作在单次转换模式 */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	 /* 转换由软件而不是外部触发启动 */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	/* ADC数据右对齐 */
	ADC_InitStructure.ADC_NbrOfChannel = 1;									/* 顺序进行规则转换的ADC通道的数目 */
	ADC_Init(ADC1, &ADC_InitStructure);				
	
	/* 使能ADC1 */
	ADC_Cmd(ADC1, ENABLE);	
	
	/* 使能复位校准 */  
	ADC_ResetCalibration(ADC1);	
	
	/* 等待复位校准结束	*/
	while( ADC_GetResetCalibrationStatus(ADC1));	
	
	/* 开启AD校准 */
	ADC_StartCalibration(ADC1);	 
	
	/* 等待校准结束 */
	while (ADC_GetCalibrationStatus(ADC1));	
}

/**
 * @brief  获得ADC值
 * @param  无
 * @retval 最近一次ADC1规则组的转换结果
 */
uint16_t Get_Adc(void)   
{
  /* 设置指定ADC的规则组通道，一个序列，采样时间：ADC1,ADC通道,采样时间为239.5周期 */
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8 , 1, ADC_SampleTime_239Cycles5 );	//	  			    
  
	/* 使能指定的ADC1的软件转换启动功能	 */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
	 
	/* 等待转换结束 */ 
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); 

	/* 返回最近一次ADC1规则组的转换结果 */
	return ADC_GetConversionValue(ADC1);
}

/**
 * @brief  多次采集求平均值
 * @param  无
 * @retval 最近一次ADC1规则组的转换结果
 */
uint16_t Get_AdcAverage(void)
{
	/* ADC值 */
	uint32_t temp_val = 0;
	
	/* 采集次数 */
	uint8_t times;
	
	for(times = 0; times < 10; times++)
	{
		temp_val += Get_Adc();
		Delay_10ms();
	}
	
	/* 返回采集到的的平均值 */
	return temp_val / 10;
}	
/*****************************************************END OF FILE*********************************************************/	

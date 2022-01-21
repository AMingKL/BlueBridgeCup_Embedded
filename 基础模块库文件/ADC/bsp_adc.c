/**
  *************************************************************************************************************************
  * @file    adc.c
  * @author  ������
  * @version V1.0
  * @date    2022-01-10
  * @brief   ���ű�Ƕ��ʽ������ADC����
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

/* ���� -----------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  ADC��ʼ������
 * @param  ��
 * @retval ��
 */
void Adc_Init(void) 
{
	/* ����һ��ADC_InitTypeDef���͵Ľṹ�� */
	ADC_InitTypeDef ADC_InitStructure; 
	
	/* ����һ��GPIO_InitTypeDef���͵Ľṹ�� */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ADC1ʱ��ʹ�ܣ�GPIOBʱ��ʹ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE); 
	
	/* ����PB0Ϊģ������ */
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ����ADC��Ƶ����6��72M/6=12,ADC���ʱ�䲻�ܳ���14M */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   
	
	/* ��λADC1 */
	ADC_DeInit(ADC1);  
	
	/* ADC��ʼ�� */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			/* ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ */
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						/* ģ��ת�������ڵ�ͨ��ģʽ */
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			/* ģ��ת�������ڵ���ת��ģʽ */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	 /* ת��������������ⲿ�������� */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	/* ADC�����Ҷ��� */
	ADC_InitStructure.ADC_NbrOfChannel = 1;									/* ˳����й���ת����ADCͨ������Ŀ */
	ADC_Init(ADC1, &ADC_InitStructure);				
	
	/* ʹ��ADC1 */
	ADC_Cmd(ADC1, ENABLE);	
	
	/* ʹ�ܸ�λУ׼ */  
	ADC_ResetCalibration(ADC1);	
	
	/* �ȴ���λУ׼����	*/
	while( ADC_GetResetCalibrationStatus(ADC1));	
	
	/* ����ADУ׼ */
	ADC_StartCalibration(ADC1);	 
	
	/* �ȴ�У׼���� */
	while (ADC_GetCalibrationStatus(ADC1));	
}

/**
 * @brief  ���ADCֵ
 * @param  ��
 * @retval ���һ��ADC1�������ת�����
 */
uint16_t Get_Adc(void)   
{
  /* ����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ�䣺ADC1,ADCͨ��,����ʱ��Ϊ239.5���� */
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8 , 1, ADC_SampleTime_239Cycles5 );	//	  			    
  
	/* ʹ��ָ����ADC1�����ת����������	 */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
	 
	/* �ȴ�ת������ */ 
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); 

	/* �������һ��ADC1�������ת����� */
	return ADC_GetConversionValue(ADC1);
}

/**
 * @brief  ��βɼ���ƽ��ֵ
 * @param  ��
 * @retval ���һ��ADC1�������ת�����
 */
uint16_t Get_AdcAverage(void)
{
	/* ADCֵ */
	uint32_t temp_val = 0;
	
	/* �ɼ����� */
	uint8_t times;
	
	for(times = 0; times < 10; times++)
	{
		temp_val += Get_Adc();
		Delay_10ms();
	}
	
	/* ���زɼ����ĵ�ƽ��ֵ */
	return temp_val / 10;
}	
/*****************************************************END OF FILE*********************************************************/	

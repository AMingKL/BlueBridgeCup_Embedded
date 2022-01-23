/**
  *************************************************************************************************************************
  * @file    adc.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   ���ű�Ƕ��ʽ������ADCģ��
  *************************************************************************************************************************
  * @attention
  *
	* ����˵����
  *          1.������R37��λ���� PB0-ADC1 channel 8   ADC1 12λ�ֱ��� 0xfff��2^12  = 4096
	*          2.������RP5��λ���� PA4-ADC1 channel 4 
	*		       3.������RP6��λ���� PA5-ADC1 channel 5 
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_adc.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/
__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0};  

/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	RCC_APB2PeriphClockCmd( ADC_GPIO_CLK, ENABLE );
	
	// ���� ADC IO ����ģʽ
	// ����Ϊģ������
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// ��ʼ�� ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);				
}
static void ADCx_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	// ��DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel1);					// ��λDMA������  ADC1 ��Ӧ DMA1ͨ��1,ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	// ��λDMA������
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue;    // �洢����ַ 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;   // ����Դ��������
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;   // ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;   // �洢����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  // �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  // �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  // ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   // ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);   // ��ʼ��DMA
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);		// ʹ�� DMA ͨ��
}
/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	

	// ��ADCʱ��
	RCC_APB2PeriphClockCmd( ADC_CLK, ENABLE );
	
	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڶ���ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 

	// ����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 

	// �����ⲿ����ת���������������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ��x��
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	
		
	// ��ʼ��ADC
	ADC_Init(ADCx, &ADC_InitStructure);
	
	// ����ADCʱ��ΪPCLK2��8��Ƶ��72/8 ��9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// ���� ADC ͨ��ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADCx, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5);
//	ADC_AutoInjectedConvCmd(ADCx,ENABLE);
	
	// ʹ��ADC DMA ����
	ADC_DMACmd(ADCx, ENABLE);
	// ����ADC ������ʼת��
	ADC_Cmd(ADCx, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADCx);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADCx);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADCx));
	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}
/**
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
  * @attention   ������main.c���ó�ʼ��
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_DMA_Config();
	ADCx_Mode_Config();
}
/*****************************************************END OF FILE*********************************************************/	






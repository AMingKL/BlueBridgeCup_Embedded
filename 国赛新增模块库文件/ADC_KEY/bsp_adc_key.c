/**
  *************************************************************************************************************************
  * @file    adc_key.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   ���ű�Ƕ��ʽ��չ��adc����ģ��
  *************************************************************************************************************************
  * @attention
  *����������Ҫ��������,��ʱ����ÿ50ms�ͻ�Ѱ�����־λ��1,Ȼ������������,�ж������־λ,�ٵ��ð�����ȡ����,�ﵽ������Ч����
  *           ��������:
  * 		P4 5
  * 		   |
  * 		P5 5(AKEY)
  * 
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_adc_key.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/
uint16_t adcx_val;


/**
 * @brief  
 * @param  �� ����
 * @retval ��
 */
void Adc_Key_Init(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

  /* Configure PC.01, PC.02 and PC.04 (ADC Channel11, Channel12 and Channel14)
    as analog input ----------------------------------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	// ADC1 ����ģʽ����
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channels configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_239Cycles5);  
  
  ADC_Cmd(ADC1, ENABLE);
  
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));  
}

uint16_t  Adc_GetButtonVal(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));

	ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
	return ADC_GetConversionValue(ADC1);
}

uint16_t get_adc(void)  //�Ի�ȡ��adcֵ�����˲�,ð������ȡ�м�ֵ
{
	uint16_t adc_buff[50];
	uint8_t i,j;
	
	for(i=0;i<50;i++)
	{
		adc_buff[i]=Adc_GetButtonVal();
	}

	for(i=0;i<50-1;i++)
	{
		for(j=0;j<50-i-1;j++)
		{
			if(adc_buff[j]>adc_buff[j+1])
			{
				adc_buff[j]=adc_buff[j] ^ adc_buff[j+1];
				adc_buff[j+1]=adc_buff[j] ^ adc_buff[j+1];
				adc_buff[j]=adc_buff[j] ^ adc_buff[j+1];
			}
		}
	}
	adcx_val	= (adc_buff[24] + adc_buff[25])/2;
	return adcx_val;
			
}


uint8_t Adc_Key_Read(void)  //�ж����ĸ���������
{
	uint16_t temp = 0;
	temp = get_adc();
	
	if(temp <= 80){//ʵ�� 0x0000��0x0001  0-1֮��
		return 1;
	}else if((temp >= 100) && (temp <= 300)){//ʵ�� 0x00CD��0x00CE  205-206֮��
		return 2; 
	}else if((temp >= 500) && (temp <= 700)){//ʵ�� 0x0264��0x0265  612-613֮��
		return 3;
	}else if((temp >= 900) && (temp <= 1200)){//ʵ�� 0x03FF��0x0400  1023-1024֮��
		return 4;
	}else if((temp >= 1300) && (temp <= 1500)){//ʵ�� 0x0580��0x0584  1408-1412֮��
		return 5;
	}else if((temp >= 1700) && (temp <= 1900)){//ʵ�� 0x0744��0x0749  1860-1865֮��
		return 6;
	}else if((temp >= 2200) && (temp <= 2400)){//ʵ�� 0x0919��0x091A  2329-2330֮��
		return 7;
	}else if((temp >= 2700) && (temp <= 2900)){//ʵ�� 0x0B34��0x0B35  2868-2869֮��
		return 8;
	}else{
		return 0;	//error status & no key  //4095
		}
}
						
/*****************************************************END OF FILE*********************************************************/	


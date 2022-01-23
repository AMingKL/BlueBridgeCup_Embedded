/**
  *************************************************************************************************************************
  * @file    adc.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   蓝桥杯嵌入式开发板ADC模块
  *************************************************************************************************************************
  * @attention
  *
	* 程序说明：
  *          1.开发板R37电位器： PB0-ADC1 channel 8   ADC1 12位分辨率 0xfff：2^12  = 4096
	*          2.开发板RP5电位器： PA4-ADC1 channel 4 
	*		       3.开发板RP6电位器： PA5-ADC1 channel 5 
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_adc.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0};  

/**
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	RCC_APB2PeriphClockCmd( ADC_GPIO_CLK, ENABLE );
	
	// 配置 ADC IO 引脚模式
	// 必须为模拟输入
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// 初始化 ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);				
}
static void ADCx_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	// 打开DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel1);					// 复位DMA控制器  ADC1 对应 DMA1通道1,ADC3对应DMA2通道5，ADC2没有DMA功能
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	// 复位DMA控制器
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue;    // 存储器地址 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;   // 数据源来自外设
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;   // 缓冲区大小，应该等于数据目的地的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;   // 存储器地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  // 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  // 内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  // 循环传输模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;  // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   // 禁止存储器到存储器模式，因为是从外设到存储器
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);   // 初始化DMA
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);		// 使能 DMA 通道
}
/**
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
  */
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	

	// 打开ADC时钟
	RCC_APB2PeriphClockCmd( ADC_CLK, ENABLE );
	
	// ADC 模式配置
	// 只使用一个ADC，属于独立模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 

	// 连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 

	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// 转换通道x个
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	
		
	// 初始化ADC
	ADC_Init(ADCx, &ADC_InitStructure);
	
	// 配置ADC时钟为PCLK2的8分频，72/8 即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// 配置 ADC 通道转换顺序和采样时间
	ADC_RegularChannelConfig(ADCx, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5);
//	ADC_AutoInjectedConvCmd(ADCx,ENABLE);
	
	// 使能ADC DMA 请求
	ADC_DMACmd(ADCx, ENABLE);
	// 开启ADC ，并开始转换
	ADC_Cmd(ADCx, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADCx);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	// ADC开始校准
	ADC_StartCalibration(ADCx);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADCx));
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}
/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  * @attention   别忘了main.c调用初始化
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_DMA_Config();
	ADCx_Mode_Config();
}
/*****************************************************END OF FILE*********************************************************/	






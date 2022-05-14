/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "rtc.h"

/* ���� -----------------------------------------------------------------------------------------------------------------*/
/* ����ʱ�������ֵ */
unsigned int time_value = 0;

/* ����ʵʱʱ��ֵ */
unsigned int HOUR;
unsigned int MINUTE;
unsigned int SECOND;

/**
 * @brief  RTC��ʼ��
 * @param  ��
 * @retval ��
 */
void RTC_Init(void)
{
	/* ����һ��NVIC_InitTypeDef���͵Ľṹ�� */
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ʹ�ܵ�Դʱ�Ӻͱ�������ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE); 
	
	/* ʹ�ܺ󱸼Ĵ������ʣ�ȡ��������д���� */
	PWR_BackupAccessCmd(ENABLE);
	
	/* ��λ�������� */
	BKP_DeInit();
	
	/* ʹ���ڲ����پ���LSI�����ȴ�LSI�������  */
	RCC_LSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	
	/* ѡȡLSI��ΪRTCʱ�ӣ���ʹ�� */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ�RTC�Ĵ�����RTC��APBʱ��ͬ�� */
	RTC_WaitForSynchro();
	
	/* �ȴ����һ�ζ�RTC�Ĵ���д��������� */
	RTC_WaitForLastTask();
	
	/* ʹ��RTC���ж� */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* �ȴ����һ�ζ�RTC�Ĵ���д��������� */
	RTC_WaitForLastTask();
	
	/* ����RTCԤ��Ƶ��ֵ  RTC period = RTCCLK/RTC_PR = (40 KHz)/(39999+1) */
	RTC_SetPrescaler(39999); 
	
	/* �ȴ����һ�ζ�RTC�Ĵ���д��������� */
	RTC_WaitForLastTask();	
	
	/* ����RTC��������ֵ��Ĭ��Ϊ00:00:00 */
	time_value = HOURINIT * 3600 + MINUTEINIT * 60 + SECONDINIT;
	RTC_SetCounter(time_value); 
	
	/* �ȴ����һ�ζ�RTC�Ĵ���д��������� */
	RTC_WaitForLastTask();		
	
	/* �����ж����������� */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;  /* ѡ��RTC�ж�ͨ�� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; /* ʹ��ͨ�� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  /* ��ռ���ȼ�Ϊ3 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				 /* �����ȼ�Ϊ3 */
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  RTC�жϴ�����
 * @param  ��
 * @retval ��
 */
void RTC_IRQHandler(void)
{
	/* �ж�RTC���жϵ��жϱ�־λ */
	if (RTC_GetITStatus(RTC_IT_SEC) != 0)	
	{
		/* ���RTC���жϵ��ж�λ */
		RTC_ClearITPendingBit(RTC_IT_SEC);
		
		/* ʱ�����,��1 */
		time_value++;
       		
		/* �ȴ����һ�ζ�RTC�Ĵ���д��������� */
		RTC_WaitForLastTask();
		
		/* ��24Сʱ���� 0x00015180Ϊ23:59:59 */
		if (RTC_GetCounter() == 0x00015180)
		{
			/* ʱ������ */
			time_value = 0;
			RTC_SetCounter(time_value);
			
		  /* �ȴ����һ�ζ�RTC�Ĵ���д��������� */
		  RTC_WaitForLastTask();
		}
		
	/* ����Сʱ */
	HOUR = time_value / 3600;
	/* ������� */
	MINUTE = (time_value % 3600) / 60;
	/* �������� */
	SECOND = (time_value % 3600) % 60;
	}
}

/*****************************************************END OF FILE*********************************************************/	

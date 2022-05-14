/**
  *************************************************************************************************************************
  * @file    rtc.c
  * @author  ������
  * @version V1.0
  * @date    2022-01-10
  * @brief   ������RTCģ��
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "rtc.h"

uint8_t THH = 0, TMM = 0, TSS = 0;

void RTC_Init(void)
{
  /* ʹ�ܵ�Դʱ�Ӻͱ�������ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* ʹ�ܺ󱸼Ĵ������ʣ�ȡ��������д���� */
  PWR_BackupAccessCmd(ENABLE);

  /* ��λ�������� */
  BKP_DeInit();

  /* ʹ���ڲ����پ���LSI�����ȴ�LSI�������  */
  RCC_LSICmd(ENABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}
  /* ѡȡLSI��ΪRTCʱ�ӣ���ʹ�� */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  RCC_RTCCLKCmd(ENABLE);

  /* �ȴ�RTC�Ĵ�����RTC��APBʱ��ͬ�� */
  RTC_WaitForSynchro();

  /* �ȴ����һ�ζ�RTC�Ĵ���д��������� */
  RTC_WaitForLastTask();

  /* ʹ��RTC���ж� */
  // RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* �ȴ����һ�ζ�RTC�Ĵ���д��������� */
  RTC_WaitForLastTask();

  /* ����RTCԤ��Ƶ��ֵ  RTC period = RTCCLK/RTC_PR = (40 KHz)/(39999+1) */
  RTC_SetPrescaler(40000);

  /* �ȴ����һ�ζ�RTC�Ĵ���д��������� */
  RTC_WaitForLastTask();

  /* To output second signal on Tamper pin, the tamper functionality
       must be disabled (by default this functionality is disabled) */
  BKP_TamperPinCmd(DISABLE);

  /* Enable the RTC Second Output on Tamper Pin */
  // BKP_RTCOutputConfig(BKP_RTCOutputSource_Second);
}

/**
  * @brief  Adjusts time.
  * @param  None
  * @retval None
  */
void Time_Adjust(uint8_t Tmp_HH,uint8_t Tmp_MM,uint8_t Tmp_SS)
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

void Time_Reresh(void)
{
	uint32_t TimeVar = RTC_GetCounter();
	/* ��24Сʱ���� 0x00015180Ϊ23:59:59 */
  if (TimeVar == 0x00015180)
  {
		/* ʱ������ */
     RTC_SetCounter(0x0);
     /* �ȴ����һ�ζ�RTC�Ĵ���д��������� */
     RTC_WaitForLastTask();
  }
  
  /* ����Сʱ */
  THH = TimeVar / 3600;
  /* ������� */
  TMM = (TimeVar % 3600) / 60;
  /* �������� */
  TSS = (TimeVar % 3600) % 60;
}


/*****************************************************END OF FILE*********************************************************/	

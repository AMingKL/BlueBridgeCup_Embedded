/**
  *************************************************************************************************************************
  * @file    rtc.c
  * @author  刘洪明
  * @version V1.0
  * @date    2022-01-10
  * @brief   开发板RTC模块
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
  /* 使能电源时钟和备份区域时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* 使能后备寄存器访问，取消备份区写保护 */
  PWR_BackupAccessCmd(ENABLE);

  /* 复位备份区域 */
  BKP_DeInit();

  /* 使能内部低速晶振LSI，并等待LSI晶振就绪  */
  RCC_LSICmd(ENABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}
  /* 选取LSI作为RTC时钟，并使能 */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  RCC_RTCCLKCmd(ENABLE);

  /* 等待RTC寄存器与RTC的APB时钟同步 */
  RTC_WaitForSynchro();

  /* 等待最近一次对RTC寄存器写操作的完成 */
  RTC_WaitForLastTask();

  /* 使能RTC秒中断 */
  // RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* 等待最近一次对RTC寄存器写操作的完成 */
  RTC_WaitForLastTask();

  /* 设置RTC预分频的值  RTC period = RTCCLK/RTC_PR = (40 KHz)/(39999+1) */
  RTC_SetPrescaler(40000);

  /* 等待最近一次对RTC寄存器写操作的完成 */
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
	/* 满24小时清零 0x00015180为23:59:59 */
  if (TimeVar == 0x00015180)
  {
		/* 时间清零 */
     RTC_SetCounter(0x0);
     /* 等待最近一次对RTC寄存器写操作的完成 */
     RTC_WaitForLastTask();
  }
  
  /* 计算小时 */
  THH = TimeVar / 3600;
  /* 计算分钟 */
  TMM = (TimeVar % 3600) / 60;
  /* 计算秒钟 */
  TSS = (TimeVar % 3600) % 60;
}


/*****************************************************END OF FILE*********************************************************/	

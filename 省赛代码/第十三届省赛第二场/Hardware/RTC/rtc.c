/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "rtc.h"

/* 定义 -----------------------------------------------------------------------------------------------------------------*/
/* 定义时间更新数值 */
unsigned int time_value = 0;

/* 定义实时时间值 */
unsigned int HOUR;
unsigned int MINUTE;
unsigned int SECOND;

/**
 * @brief  RTC初始化
 * @param  无
 * @retval 无
 */
void RTC_Init(void)
{
	/* 定义一个NVIC_InitTypeDef类型的结构体 */
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 使能电源时钟和备份区域时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE); 
	
	/* 使能后备寄存器访问，取消备份区写保护 */
	PWR_BackupAccessCmd(ENABLE);
	
	/* 复位备份区域 */
	BKP_DeInit();
	
	/* 使能内部低速晶振LSI，并等待LSI晶振就绪  */
	RCC_LSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	
	/* 选取LSI作为RTC时钟，并使能 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE);
	
	/* 等待RTC寄存器与RTC的APB时钟同步 */
	RTC_WaitForSynchro();
	
	/* 等待最近一次对RTC寄存器写操作的完成 */
	RTC_WaitForLastTask();
	
	/* 使能RTC秒中断 */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* 等待最近一次对RTC寄存器写操作的完成 */
	RTC_WaitForLastTask();
	
	/* 设置RTC预分频的值  RTC period = RTCCLK/RTC_PR = (40 KHz)/(39999+1) */
	RTC_SetPrescaler(39999); 
	
	/* 等待最近一次对RTC寄存器写操作的完成 */
	RTC_WaitForLastTask();	
	
	/* 设置RTC计算器的值，默认为00:00:00 */
	time_value = HOURINIT * 3600 + MINUTEINIT * 60 + SECONDINIT;
	RTC_SetCounter(time_value); 
	
	/* 等待最近一次对RTC寄存器写操作的完成 */
	RTC_WaitForLastTask();		
	
	/* 配置中断向量控制器 */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;  /* 选择RTC中断通道 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; /* 使能通道 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  /* 抢占优先级为3 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				 /* 从优先级为3 */
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  RTC中断处理函数
 * @param  无
 * @retval 无
 */
void RTC_IRQHandler(void)
{
	/* 判断RTC秒中断的中断标志位 */
	if (RTC_GetITStatus(RTC_IT_SEC) != 0)	
	{
		/* 清除RTC秒中断的中断位 */
		RTC_ClearITPendingBit(RTC_IT_SEC);
		
		/* 时间更新,加1 */
		time_value++;
       		
		/* 等待最近一次对RTC寄存器写操作的完成 */
		RTC_WaitForLastTask();
		
		/* 满24小时清零 0x00015180为23:59:59 */
		if (RTC_GetCounter() == 0x00015180)
		{
			/* 时间清零 */
			time_value = 0;
			RTC_SetCounter(time_value);
			
		  /* 等待最近一次对RTC寄存器写操作的完成 */
		  RTC_WaitForLastTask();
		}
		
	/* 计算小时 */
	HOUR = time_value / 3600;
	/* 计算分钟 */
	MINUTE = (time_value % 3600) / 60;
	/* 计算秒钟 */
	SECOND = (time_value % 3600) % 60;
	}
}

/*****************************************************END OF FILE*********************************************************/	

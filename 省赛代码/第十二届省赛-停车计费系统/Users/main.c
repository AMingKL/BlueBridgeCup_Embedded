/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-18
  * @brief   蓝桥杯嵌入式第十二届省赛题-停车计费系统
  *************************************************************************************************************************
  * @attention
  *
	*难点:1.串口处理，接收不定长信息
	*     2.PA7低电平的配置，强制输出
	*     3.车位选择，空车位的判定
 	* 
	*逻辑部分:1.出库的车的时间，不能比入库时早
	          2.车编号唯一，不会有俩个车辆类型而编号相同
  *************************************************************************************************************************
  */
/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "myconfig.h"
/* 函数定义 -----------------------------------------------------------------------------------------------------------------*/
void Lcd_Proc(void);
void Key_Proc(void);
void Led_Proc(void);
void Usart2_Proc(void);
void cleanidtype(void);
void CarInStorage(void);
void CarOutStorage(void);
void ChargingTheCar(void);
void PA7_OutputProc(void);
void RxbufferToEmpty(void);
void BillingSystem_Init(void);
void substr(uint8_t* d_str,uint8_t* s_str,uint8_t locate,uint8_t length);
uint8_t StringCheck(void);
uint8_t EmptyStorage(void);
uint8_t isExist(uint8_t* str);
uint8_t CheckTheCocation(void);
uint8_t JudgeTheCar_In_or_Out(void);
/* 变量定义 -----------------------------------------------------------------------------------------------------------------*/
/* LCD显示数组 */
uint8_t CNBRstring[20];
uint8_t VNBRstring[20];
uint8_t IDLEstring[20];
uint8_t CNBRRatestring[20];
uint8_t VNBRRatestring[20];
/* 串口相关变量 */
uint8_t RxCounter;
uint8_t RxoverFlag;
uint8_t RxBuffer[25];
/* 变量 */
uint8_t  car_id[5];
uint8_t  car_type[5];
uint8_t  locate;
uint32_t CarStopTime;
uint32_t Money;
uint8_t  year_temp,month_temp,day_temp,hour_temp,min_temp,sec_temp;


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();	
	LED_Init();
	Key_Init();
	LCD_Init();
	Tim4_Init();
	Usart2_Init();
	BillingSystem_Init();	
	while(1)
	{	
		Lcd_Proc();
		Led_Proc();
		PA7_OutputProc();
		Usart2_Proc();
	}
}


/**
 * @brief  检查字符串函数
 * @attention 判断数据长度，格式，时间范围是否正确
 * @retval 0-错误 1-正确
 */
uint8_t StringCheck(void)  
{
	uint8_t i;
	if(RxCounter!=22)//长度不正确
	{
	 return 0;
	}
	if((RxBuffer[0]=='C'||RxBuffer[0]=='V')&&(RxBuffer[1]=='N')&&(RxBuffer[2]=='B')&&(RxBuffer[3]=='R')&&(RxBuffer[4]==':')&&(RxBuffer[9]==':'))
	{
		for(i=10;i<22;i++)                            
		{                                              
			if(RxBuffer[i]>'9'||RxBuffer[i]<'0')
			return 0;
		}
		
		year_temp  = (RxBuffer[10] - '0') * 10 + (RxBuffer[11] - '0');
		month_temp = (RxBuffer[12] - '0') * 10 + (RxBuffer[13] - '0');
		day_temp   = (RxBuffer[14] - '0') * 10 + (RxBuffer[15] - '0');
		hour_temp  = (RxBuffer[16] - '0') * 10 + (RxBuffer[17] - '0');
		min_temp   = (RxBuffer[18] - '0') * 10 + (RxBuffer[19] - '0');
		sec_temp   = (RxBuffer[20] - '0') * 10 + (RxBuffer[21] - '0');
		
		if(year_temp>99||month_temp>12||day_temp>31||hour_temp>23||min_temp>59||sec_temp>59)
		{
		 return 0;
		}	
		return 1;
	}
	return 0;
}

/**
 * @brief  字符串数组清零函数
 * @attention  清空缓冲区，防止造成干扰
 * @retval 无
 */
void RxbufferToEmpty(void)
{
  uint8_t i;
	for(i=0;i<25;i++)
	{
	 RxBuffer[i]=0;
	}
}

/**
 * @brief  串口数据处理函数
 * @attention  	
 * 检查字符串信息正确与否，判断有无空闲车位，有如下情况:   
 *1.无车位报错
 *2.有车位，判断车辆入库还是出库，车辆入库，保存入库时间信息，保存车类型，编号。相应车类型数量加一，空闲车位数量减一
 *3.有车位，车辆出库，找到第几号车位的车出库，判断是否符合逻辑(时间，车牌号和类型一致)，不符合时间逻辑，报错，车不出库。符合逻辑，串口传输收费消息，清除入库信息，相应车类型数量减一，空闲车位数量加一。
 * @retval 无
 */ 
void Usart2_Proc(void)
{
	if(RxoverFlag==1)
	{
	  RxoverFlag=0;
		
		if(StringCheck())
		{  
				if(JudgeTheCar_In_or_Out()==0)
				{
					CarInStorage();//入库
				}
				else if(JudgeTheCar_In_or_Out()==1)
				{
				 printf("Error\n");//报错
				}
				else if(JudgeTheCar_In_or_Out()==2)
				{
				 CarOutStorage();//出库
				}
		}
		else
		{
			printf("Error\n");
		}	
		RxbufferToEmpty();
		RxCounter=0;
	}	
}
/**
 * @brief      车辆出库函数
 * @attention  无
 * @retval     无
 */
void CarOutStorage(void)
{
	if(car[locate].yearin>year_temp||car[locate].monthin>month_temp||car[locate].dayin>day_temp||car[locate].hourin>hour_temp||car[locate].minin>min_temp||car[locate].secin>sec_temp)
	{
	  printf("Error\n");//出库时间小于入库时间，逻辑错误
	}
	else//符合逻辑，串口传输收费消息，清除入库信息，相应车类型数量减一，空闲车位数量加一。
	{
		if(car[locate].cartype[0]=='C')
		{
			System.CNBR_Number-=1;
			System.IDLE_Number+=1;
		}
		else
		{
			System.VNBR_Number-=1;
			System.IDLE_Number+=1;
		}	
    ChargingTheCar();//收费函数
	}
   cleanidtype();
}

/**
 * @brief      检查空车位号函数
 * @attention  无
 * @retval     无
 */
uint8_t EmptyStorage(void)
{
  uint8_t m;
	for(m=0;m<8;m++)
	{
	 if(car[m].state==EMPTY)
	  return m;
	}
	return 0xFF;
}

/**
 * @brief      车辆入库函数
 * @attention  如当前车位被占，找空车位。
 * @retval     无
 */
void CarInStorage(void)
{
	uint8_t i;
	
	if(car[8-System.IDLE_Number].state==FULL)//当前车位被占，找一个空的车位
	{		
	  //找一个空的车位
			for(i=0;i<4;i++)//保存车类型
		{
		 car[EmptyStorage()].cartype[i]=RxBuffer[i];
		}
		for(i=0;i<4;i++)//保存车编号
		{
		 car[EmptyStorage()].carid[i]=RxBuffer[i+5];
		}
		car[EmptyStorage()].yearin	=(RxBuffer[10] - '0') * 10 + (RxBuffer[11] - '0');
		car[EmptyStorage()].monthin	=(RxBuffer[12] - '0') * 10 + (RxBuffer[13] - '0');
		car[EmptyStorage()].dayin		=(RxBuffer[14] - '0') * 10 + (RxBuffer[15] - '0');
		car[EmptyStorage()].hourin	=(RxBuffer[16] - '0') * 10 + (RxBuffer[17] - '0');
		car[EmptyStorage()].minin		=(RxBuffer[18] - '0') * 10 + (RxBuffer[19] - '0');
		car[EmptyStorage()].secin		=(RxBuffer[20] - '0') * 10 + (RxBuffer[21] - '0');
		
		if(RxBuffer[0]=='C')
		{
			System.CNBR_Number+=1;
			System.IDLE_Number-=1;
		}
		else
		{
			System.VNBR_Number+=1;
			System.IDLE_Number-=1;
		}
		car[EmptyStorage()].state=FULL;
	}
	else//使用当前车位
	{
		for(i=0;i<4;i++)//保存车类型
		{
		 car[8-System.IDLE_Number].cartype[i]=RxBuffer[i];
		}
		for(i=0;i<4;i++)//保存车编号
		{
		 car[8-System.IDLE_Number].carid[i]=RxBuffer[i+5];
		}
		car[8-System.IDLE_Number].yearin	=(RxBuffer[10] - '0') * 10 + (RxBuffer[11] - '0');
		car[8-System.IDLE_Number].monthin	=(RxBuffer[12] - '0') * 10 + (RxBuffer[13] - '0');
		car[8-System.IDLE_Number].dayin		=(RxBuffer[14] - '0') * 10 + (RxBuffer[15] - '0');
		car[8-System.IDLE_Number].hourin	=(RxBuffer[16] - '0') * 10 + (RxBuffer[17] - '0');
		car[8-System.IDLE_Number].minin		=(RxBuffer[18] - '0') * 10 + (RxBuffer[19] - '0');
		car[8-System.IDLE_Number].secin		=(RxBuffer[20] - '0') * 10 + (RxBuffer[21] - '0');
		
		if(RxBuffer[0]=='C')
		{
			System.CNBR_Number+=1;
			System.IDLE_Number-=1;
		}
		else
		{
			System.VNBR_Number+=1;
			System.IDLE_Number-=1;
		}
		car[8-System.IDLE_Number].state=FULL;
	}
}

/**
 * @brief      判断车辆入库出库函数
 * @attention  无车位，且为新车辆需报错
 * @retval     无
 */
uint8_t JudgeTheCar_In_or_Out(void)
{	
  if(System.IDLE_Number==8)//车位充足，入库
	{
	 return 0;
	}
	else
	{
		substr(car_id,RxBuffer,5,4);//将车辆的编号信息提取到car_id
		substr(car_type,RxBuffer,0,4);//将车辆的类型信息提取到car_type
		locate = isExist(car_id);//查询这辆车是否在车库从存在
		if(locate != 0xFF)//这辆车在车库中存在
		{
			if(strcmp((const char *)car_type,(const char *)car[locate].cartype))		// 如果车辆的id和车辆的类型不同，则表示信息错误
			{
				return 1;//报错
			}
			else
			{
			 return 2;//出库
			}
		}
		else// 这辆车在车库中不存在
		{
		  if(System.IDLE_Number==0)//没有车位
				return 1;//报错
			else
				return 0;//入库
		}
	}
}

/**
 * @brief      清除存储信息函数
 * @attention  清除车辆入库存储信息
 * @retval     无
 */
void cleanidtype(void)
{
	uint8_t i;		
	for(i=0;i<4;i++)//清空
	{
	 car[locate].cartype[i]=0;
	 car[locate].carid[i]=0;
	}
		car[locate].yearin=0;
		car[locate].monthin=0;
		car[locate].dayin=0;
		car[locate].hourin=0;
		car[locate].minin=0;
		car[locate].secin=0;
		car[locate].state=EMPTY;
}
/**
 * @brief      按键处理函数
 * @attention  注意在费率设置下的费率不能生效
 * @retval     无
 */
void Key_Proc(void)
{
	Key_refresh();
	if(key_falling==B1)//界面切换按键
	{
		System.Interface=(System.Interface+1)%2;
		if(System.Interface==CarShow)
		{
		  System.CNBR_Rate=System.CNBR_RateTemp;
			System.VNBR_Rate=System.VNBR_RateTemp;
		}
	}
	else	if(key_falling==B2)//费率增加键，按下加0.5元
	{
	  if(System.Interface==RateSet)//题中未涉及费率上限，我定为20元
		{
		  (System.CNBR_RateTemp==2000)?(System.CNBR_RateTemp=2000):(System.CNBR_RateTemp+=50);
			(System.VNBR_RateTemp==1850)?(System.VNBR_RateTemp=1850):(System.VNBR_RateTemp+=50);
		}
	}
	else	if(key_falling==B3)//费率减少键，按下减0.5元
	{
	 	if(System.Interface==RateSet)//费率下限，我定为0元
		{
		  (System.CNBR_RateTemp==150)?(System.CNBR_RateTemp=150):(System.CNBR_RateTemp-=50);
			(System.VNBR_RateTemp==0)?(System.VNBR_RateTemp=0):(System.VNBR_RateTemp-=50);
		}
	}
	else if(key_falling==B4)//控制按键 0. PA7:2KHZ,20%占空比  1. PA7:低电平
	{
	  System.PA7_state=(System.PA7_state+1)%2;
	}
}
/**
 * @brief      led函数
 * @attention  无
 * @retval     无
 */
void Led_Proc(void)
{
  if(System.IDLE_Number!=0)
	{
	  LED_Control(LED1,ON);
	}
	else
	{
		LED_Control(LED1,OFF);
	}
	
	if(System.PA7_state==PULSE)
	{
		LED_Control(LED2,ON);
	}
	else
	{
		LED_Control(LED2,OFF);
	}
}
/**
 * @brief      PA7输出函数
 * @attention  无
 * @retval     无
 */
void PA7_OutputProc(void)
{
   if(System.PA7_state==PULSE)
	 {
	  PWM_Init(20);
	 }
	 else if(System.PA7_state==LOWLEVEL)
	 {
	  PWM_Init(0);
	 }
}
/**
 * @brief      LCD函数
 * @attention  无
 * @retval     无
 */
void Lcd_Proc(void)
{
	if(System.Interface==CarShow)
	{		
		/*                屏幕第一行                          */
		LCD_DisplayStringLine(Line0," LQB Di 12 Jie SaiTi");
		/*                屏幕第二行                          */
		LCD_DisplayStringLine(Line2,"       Data         ");
		/*                屏幕第三行                          */
		sprintf((char*)CNBRstring,"   CNBR:%d           ",System.CNBR_Number);
		LCD_DisplayStringLine(Line3 ,CNBRstring);
		/*                屏幕第五行                          */
		sprintf((char*)VNBRstring,"   VNBR:%d           ",System.VNBR_Number);
		LCD_DisplayStringLine(Line5 ,VNBRstring);
		/*                屏幕第七行                          */
		sprintf((char*)IDLEstring,"   IDLE:%d           ",System.IDLE_Number);
		LCD_DisplayStringLine(Line7 ,IDLEstring);
		/*                屏幕第九行                          */
		LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");	
	}
  else if(System.Interface==RateSet)
	{
		/*                屏幕第一行                          */
		LCD_DisplayStringLine(Line0," LQB Di 12 Jie SaiTi");
		/*                屏幕第二行                          */
		LCD_DisplayStringLine(Line2,"       Para         ");
		/*                屏幕第三行                          */
		sprintf((char*)CNBRRatestring,"   CNBR:%d.%.2d        ",System.CNBR_RateTemp/100,System.CNBR_RateTemp%100);
		LCD_DisplayStringLine(Line3 ,CNBRRatestring);
		/*                屏幕第五行                          */
		sprintf((char*)VNBRRatestring,"   VNBR:%d.%.2d        ",System.VNBR_RateTemp/100,System.VNBR_RateTemp%100);
		LCD_DisplayStringLine(Line5 ,VNBRRatestring);
		/*                屏幕第七行                          */
		LCD_DisplayStringLine(Line7,"                       ");	
		/*                屏幕第九行                          */
		LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");	
	}
}
/**
 * @brief      停车系统初始函数
 * @attention  初始化相关变量
 * @retval     无
 */
void BillingSystem_Init(void)
{
	System.PA7_state=LOWLEVEL;
	System.Interface=CarShow;
	System.CNBR_Rate=350;
	System.VNBR_Rate=200;
	System.CNBR_RateTemp=350;
	System.VNBR_RateTemp=200;
	System.IDLE_Number=8;
}

/**
 * @brief      字符串提取函数
 * @param       * d_str;存储传递的字符串的数组 * s_str:传递字符串的数组  locate:传递数组的起始位 length:传输数据的长度
 * @attention  无
 * @retval     无
 */
void substr(uint8_t* d_str,uint8_t* s_str,uint8_t locate,uint8_t length)
{
	uint8_t i = 0;
	for(i = 0; i < length; i++)
	{
		d_str[i] = s_str[locate + i];
	}
	d_str[length] = '\0';
}


/**
 * @brief      判断车辆函数
 * @attention  将存储库中的信息与当前车辆对比 1.是否存在 2.返回车位号 
 * @retval     存在，i为返回车位号  0xFF，不存在
 */
uint8_t isExist(uint8_t* str)		
{
	uint8_t i = 0;
	for(i = 0; i < 8; i++)
	{
		if(strcmp((const char*)str,(const char*)car[i].carid) == 0)
		{
			return i;		
		}
	}
	return 0xFF;	
}
/**
 * @brief      车辆费用计算函数
 * @attention  并输出费用等信息
 * @retval     无
 */
void ChargingTheCar(void)
{
  // 假设一年365天,一个月31天，秒为单位
	CarStopTime=(year_temp-car[locate].yearin)*365*24*3600+(month_temp-car[locate].monthin)*31*24*3600+(day_temp-car[locate].dayin)*24*3600\
	             +(hour_temp-car[locate].hourin)*3600+(min_temp-car[locate].minin)*60+(sec_temp-car[locate].secin);
	//换算成小时，不足一小时按一个小时算
	CarStopTime=(CarStopTime+3599)/3600;
	
	if(RxBuffer[0] == 'C')
	{
	 Money=CarStopTime*System.CNBR_Rate;
	}
	else
	{
	 Money=CarStopTime*System.VNBR_Rate;
	}
	// 输出计费信息
	printf("%s:%s:%d:%d.%.2d\r\n",car[locate].cartype,car[locate].carid,CarStopTime,Money/100,Money%100);
}



/**
 * @brief  串口2接收中断服务程序
 * @param  无
 * @retval 无
 */
void USART2_IRQHandler(void) 
{	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) !=RESET)
	{
	  RxBuffer[RxCounter++]=USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2,USART_IT_IDLE)!=RESET)
	{
		USART2->SR;
		USART2->DR;
		
		RxoverFlag=1;
	}
}

/**
 * @brief  定时器4中断服务函数-1ms中断
 * @param  无
 * @retval 无
 */
void TIM4_IRQHandler(void)
{
	static uint8_t key_cnt=0;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		if(++key_cnt==20) 
		{
			key_cnt=0;
			Key_Proc();
		}
	}
}

/*****************************************************END OF FILE*********************************************************/	


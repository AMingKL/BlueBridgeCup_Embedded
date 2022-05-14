#include "myconfig.h"


/* 变量定义 -----------------------------------------------------------------------------------------------------------------*/
unsigned char  Timers[TIMERCOUNT];//软件定时器数组
unsigned char  LcdString[20];//lcd显示数组

uint8_t Index;//界面

uint8_t X_goumaiNum;//x购买数量
uint8_t X_danjia;//x单价
uint8_t X_kucunNum;//x库存数量
uint8_t X_Oleddanjia;//x之前单价
uint8_t X_OledkucunNum;//x之前库存数量

uint8_t Y_goumaiNum;//y购买数量
uint8_t Y_danjia;//y单价
uint8_t Y_kucunNum;//y库存数量	
uint8_t Y_Oleddanjia;//Y之前单价
uint8_t Y_OledkucunNum;//Y之前库存数量

uint8_t Queren_flag;//确认购买
uint8_t flag_5s,Flash_Flag;//led闪烁标志

uint8_t verify;//eeprom检验

uint8_t RxCounter;//串口接收数据长度
uint8_t RxoverFlag;//串口接收是否完成标志位
uint8_t RxBuffer[25];//数据缓冲区



/**
 * @brief  软件定时器
 * @param  xIndex-事件x，xTime-延时时间
 * @retval 无
 */
unsigned char xDelay(unsigned char xIndex,unsigned char xTime)
{
	unsigned char RetValue=TIMEISNOTUP;

	if(Timers[xIndex]==0)//判断指定软件定时器的计数器是否为 0
	{
		Timers[xIndex]=xTime;//得装计数器的值
		RetValue=TIMEISUP;	 //赋值返回值为：时间到	
	}
	return(RetValue);
}

/**
 * @brief  LCD显示函数
 * @attention 显示数据等信息
 * @retval  无
 */
void Lcd_Proc(void)
{
	if(Index==SHOP)
	{
		/*屏幕第二行*/
		 LCD_DisplayStringLine(Line1,"        SHOP        ");	
		/*屏幕第四行*/				   
		sprintf((char*)LcdString,"     X:%d           ",X_goumaiNum);
		LCD_DisplayStringLine(Line3 ,LcdString); 
		/*屏幕第五行*/
		sprintf((char*)LcdString,"     Y:%d           ",Y_goumaiNum);
		LCD_DisplayStringLine(Line4 ,LcdString); 	 
		
	}
	else if(Index==PRICE)
	{
		/*屏幕第二行*/
		 LCD_DisplayStringLine(Line1,"        PRICE       ");	
		/*屏幕第四行*/				      
		sprintf((char*)LcdString,"     X:%d.%d           ",X_danjia/10,X_danjia%10);
		LCD_DisplayStringLine(Line3 ,LcdString); 
		/*屏幕第五行*/
		sprintf((char*)LcdString,"     Y:%d.%d           ",Y_danjia/10,Y_danjia%10);
		LCD_DisplayStringLine(Line4 ,LcdString); 		 
	}
	else if(Index==REP)
	{
		/*屏幕第二行*/
		 LCD_DisplayStringLine(Line1,"        REP         ");	
		/*屏幕第四行*/				      
		sprintf((char*)LcdString,"     X:%d           ",X_kucunNum);
		LCD_DisplayStringLine(Line3 ,LcdString); 
		/*屏幕第五行*/
		sprintf((char*)LcdString,"     Y:%d           ",Y_kucunNum);
		LCD_DisplayStringLine(Line4 ,LcdString); 	 
	}
}


/**
 * @brief  串口处理函数
 * @attention 无
 * @retval  无
 */
void Usart_Proc(void)
{
	if(RxoverFlag==1)
	{
	  RxoverFlag=0;
		StringCheck();		
		memset(RxBuffer,0,sizeof(RxBuffer));
		RxCounter=0;
	}	
}


/**
 * @brief  字符串检查函数
 * @attention 判断数据长度，格式是否正确
 * @retval  无
 */
void  StringCheck(void)
{
 	uint8_t i=1;
	
	while(i)
	{
		if(RxCounter!=1)//长度不正确
		{
		 break;
		}
		if(RxBuffer[0]=='?')
		{
			printf("X:%d.%d,Y:%d.%d\r\n",X_danjia/10,X_danjia%10,Y_danjia/10,Y_danjia%10);      
		}
	 break;
	}
}


/**
 * @brief      按键处理函数
 * @attention  无
 * @retval     无
 */
void Key_Proc(void)
{
	Key_refresh();
	if(key_falling==B1)//B1按键-界面切换按键
	{
		Index=(Index+1)%3;
	}
	
	else	if(key_falling==B2)//B2按键-“商品 X”
	{
		if(Index==SHOP)
		{
			(X_goumaiNum==X_kucunNum)?(X_goumaiNum=0):(X_goumaiNum++);
		}
		else if(Index==PRICE)
		{
		   X_Oleddanjia=X_danjia;
		  (X_danjia==20)?(X_danjia=10):(X_danjia++);
			if( X_danjia!=X_Oleddanjia)//代表单价有变化
			{
			  Write_eeprom(2,X_danjia);//保存
				Delay_Ms(5);
			}
		}
		else if(Index==REP)
		{
			X_OledkucunNum=X_kucunNum;
		  X_kucunNum++;
			if( X_OledkucunNum!=X_kucunNum)//代表单价有变化
			{
			  Write_eeprom(0,X_kucunNum);//保存
				Delay_Ms(5);
			}
		}
	}
	
	
	else	if(key_falling==B3)//B3按键-“商品 Y”
	{
		if(Index==SHOP)
		{
			(Y_goumaiNum==Y_kucunNum)?(Y_goumaiNum=0):(Y_goumaiNum++);
		}
		else if(Index==PRICE)
		{
			 Y_Oleddanjia=Y_danjia;
		  (Y_danjia==20)?(Y_danjia=10):(Y_danjia++);
			if( Y_danjia!=Y_Oleddanjia)//代表单价有变化
			{
				Write_eeprom(3,Y_danjia);//保存
				Delay_Ms(5);
			}
		}
		
		else if(Index==REP)
		{
			Y_OledkucunNum=Y_kucunNum;
		  Y_kucunNum++;
			if( Y_OledkucunNum!=Y_kucunNum)//代表单价有变化
			{
				Write_eeprom(1,Y_kucunNum);//保存
				Delay_Ms(5);
			}
		}
	}
	
	
	else if(key_falling==B4)//B4按键 -"确认"
	{
		if(Index==SHOP)
		{
			Queren_flag=1;
			flag_5s=1;
			Y_kucunNum=Y_kucunNum-Y_goumaiNum;//更新库存数量
			X_kucunNum=X_kucunNum-X_goumaiNum;//更新库存数量
			
			// 输出计费信息
			printf("X:%d,Y:%d,Z:%d.%d\r\n",X_goumaiNum,Y_goumaiNum,\
			((X_goumaiNum*X_danjia)+(Y_goumaiNum*Y_danjia))/10,\
			((X_goumaiNum*X_danjia)+(Y_goumaiNum*Y_danjia))%10);
			
			if(X_goumaiNum!=0)//代表数量有变化
			{
				Write_eeprom(0,X_kucunNum);//保存
				Delay_Ms(5);
			}
			if(Y_goumaiNum!=0)//代表数量有变化
			{	
				Write_eeprom(1,Y_kucunNum);//保存
				Delay_Ms(5);			
			}				
			
			X_goumaiNum=0;//商品购买界面下xy清0
			Y_goumaiNum=0;//商品购买界面下xy清0
		}
		
	}
}

/**
 * @brief  led&pwm输出配置函数
 * @param  无
 * @retval 无
 */
void Led_PWM_Proc(void)
{
	if(flag_5s==1)
	{
		LED_Control(LED1,ON);
		TIM_SetAutoreload(TIM2,500 - 1);//设置频率为2KHz
		TIM_SetCompare2(TIM2,150);//设置占空比30%
  }
	else
	{
		LED_Control(LED1,OFF);
		TIM_SetAutoreload(TIM2,500 - 1);//设置频率为2KHz
		TIM_SetCompare2(TIM2,25);//设置占空比5%
	}
	if((X_kucunNum==0)&&(Y_kucunNum==0))
	{
	 Flash_Flag=1;
	}
	else
	{
	 Flash_Flag=0;
	 LED_Control(LED2,OFF);
	}	
}



/*
 * @brief  系统参数初始化函数
 * @param  无
 * @retval 无
 */
void SystemPara_Init(void)
{
	verify=Read_eeprom(4);
	
	if(verify!=10)//随便设定一个数，总没有那么巧吧，哈哈哈，不等就认为第一次上电
	{
		Write_eeprom(0,10);//保存
		Delay_Ms(5);
		Write_eeprom(1,10);//保存
		Delay_Ms(5);	
		Write_eeprom(2,10);//保存
		Delay_Ms(5);
		Write_eeprom(3,10);//保存
		Delay_Ms(5);	 
		Write_eeprom(4,10);//赋值为10，此段函数只执行一次	
		Delay_Ms(5);	
	}	
	
	X_kucunNum=Read_eeprom(0);
	Y_kucunNum=Read_eeprom(1);
	X_danjia	=Read_eeprom(2);
	Y_danjia	=Read_eeprom(3);
  	
	Index=SHOP;
	
	X_goumaiNum=0;
	Y_goumaiNum=0;
}

/**
 * @brief  定时器4中断服务函数-10ms中断
 * @param  无
 * @retval 无
 */
void TIM4_IRQHandler(void)
{
	static uint8_t Index;
	static uint16_t TimeCnt;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		
		//为每一个软件定时器中的中断次数计数减 1，直到其值为 0
		for(Index=0;Index<TIMERCOUNT;Index++)
		if(Timers[Index]>0)
		Timers[Index]--;
		
		if(flag_5s==1)
		{				
		 if(++TimeCnt==500)
			{		
				TimeCnt=0;
				flag_5s=0;
		  }
		}		
	}
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
		Usart_Proc();
	}
}

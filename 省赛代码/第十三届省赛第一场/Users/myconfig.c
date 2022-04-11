/**
  *************************************************************************************************************************
  * @file    myconfig.c
  * @author  AMKL
  * @version V1.0
  * @date    2022-04-10
  * @brief   配置函数
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "myconfig.h"
/* 变量定义 -----------------------------------------------------------------------------------------------------------------*/
uint8_t Timers[TIMERCOUNT];//软件定时器数组
System_InitTypeDef Variable;//变量结构体声明
uint8_t  LcdString[20];//lcd显示数组
/* 串口相关变量 */
uint8_t RxCounter;//接收数量计数
uint8_t RxoverFlag;//接收完成标志
uint8_t RxBuffer[7];//接收数组



/**
 * @brief  软件定时器
 * @param  xIndex-事件x，xTime-延时时间
 * @retval 无
 */
uint8_t xDelay(uint8_t xIndex,uint8_t xTime)
{
	uint8_t RetValue=TIMEISNOTUP;

	if(Timers[xIndex]==0)//判断指定软件定时器的计数器是否为 0
	{
		Timers[xIndex]=xTime;//得装计数器的值
		RetValue=TIMEISUP;	 //赋值返回值为：时间到	
	}
	return(RetValue);
}

/**
 * @brief  按键处理函数
 * @param  无
 * @retval 无
 */
void Key_Proc(void)
{
	static uint8_t i=0;
 
  Key_refresh();	 
	if(key_falling==B1)//密码输入界面下B1值
	{
    if((Variable.Index==PSD)&&(Variable.B1Reset==0))
		{
		  Variable.B1Reset=1;
		}
		else if((Variable.Index==PSD)&&(Variable.B1Reset==1))
		{
			(Variable.InputPassword[0]==9)?(Variable.InputPassword[0]=0):(Variable.InputPassword[0]++);
		}
				
	}
	else	if(key_falling==B2)//密码输入界面下B2值
	{
    if((Variable.Index==PSD)&&(Variable.B2Reset==0))
		{
		  Variable.B2Reset=1;
		}
		else if((Variable.Index==PSD)&&(Variable.B2Reset==1))
		{
			(Variable.InputPassword[1]==9)?(Variable.InputPassword[1]=0):(Variable.InputPassword[1]++);
		}
	}
	else	if(key_falling==B3)//密码输入界面下B3值
	{
    if((Variable.Index==PSD)&&(Variable.B3Reset==0))
		{
		  Variable.B3Reset=1;
		}
		else if((Variable.Index==PSD)&&(Variable.B3Reset==1))
		{
			(Variable.InputPassword[2]==9)?(Variable.InputPassword[2]=0):(Variable.InputPassword[2]++);
		}
	}
	else if(key_falling==B4)//密码输入界面下确认键
	{
		for(i=0;i<3;i++)
		{
		  if(Variable.Password[i]==Variable.InputPassword[i])
			{
				Variable.PasswordRight=1;//密码正确
				Variable.Time5s_flag=1;//开启5s计时				
			}
			else
			{
			 Variable.PasswordError=1;//密码错误
			}
		}
		 Password_Check();//密码核对函数
	}
}

/**
 * @brief  密码核对函数
 * @param  无
 * @retval 无
 */
void Password_Check(void)
{
  if((Variable.PasswordRight==1)&&(Variable.PasswordError==0))//密码正确 
	{
	 Variable.Index=STA;
	 Variable.PasswordRight=0;
	 Variable.ErrorCnt=0;//错误次数清零
	}
	else//密码错误
	{
	 Variable.Index=PSD;
	 Variable.B1Reset=0;
	 Variable.B2Reset=0;
	 Variable.B3Reset=0;
	 Variable.InputPassword[0]=0;		
	 Variable.InputPassword[1]=0;
	 Variable.InputPassword[2]=0;
	 Variable.PasswordRight=0;
	 Variable.PasswordError=0;
	 Variable.ErrorCnt++;
		if(Variable.ErrorCnt>=3)//超过3次5s报警开启
		{
			Variable.ErrorTime5s_flag=1;
		}			
	}
}

/**
 * @brief  Led2处理函数
 * @param  无
 * @retval 无
 */
void Led2_Proc(void)
{
  if(Variable.ErrorCnt>=3)
	{
		if(Variable.ErrorTime5s_flag==1)//开启报警
		{
		  LED_Toggle(2);//主程序中0.1s扫描一次此函数，实现了0.1s亮灭
		}
		else
		{
		 LED_Control(LED2,OFF);
		}
	}
	else
	{
	 LED_Control(LED2,OFF);
	}
}

/**
 * @brief  pwm处理函数
 * @param  无
 * @retval 无
 */
void Pwm_Proc(void)
{
  if(Variable.Index==STA)
	{
	 	TIM_SetAutoreload(TIM2,500 - 1);//设置频率为2KHz
		TIM_SetCompare2(TIM2,100);//设置占空比10%
		LED_Control(LED1,ON);//在输出界面保持led1亮
	}
	else
	{
		TIM_SetAutoreload(TIM2,1000 - 1);	//设置PWM频率为1KHz.
	  TIM_SetCompare2(TIM2,500);//设置占空比50%
		LED_Control(LED1,OFF);
	} 
	if(Variable.Time5s_flag==2)//5s时间到，复位密码，切换回原界面
	{
		Variable.Time5s_flag=0;
	  Variable.Index=PSD;
		Variable.B1Reset=0;
	  Variable.B2Reset=0;
	  Variable.B3Reset=0;
	  Variable.InputPassword[0]=0;		
	  Variable.InputPassword[1]=0;
	  Variable.InputPassword[2]=0;
	}
}

/**
 * @brief  Usart处理函数
 * @param  无
 * @retval 无
 */
void Usart_Proc(void)
{
 	if(RxoverFlag==1)
	{
	  RxoverFlag=0;
		StringCheck();
		RxCounter=0;
		memset(RxBuffer,0,sizeof(RxBuffer));//清空缓冲区
	}
}

/**
 * @brief  字符串检查函数
 * @attention 判断数据长度，格式，范围是否正确，正确后修改密码
 * @retval  无
 */
void  StringCheck(void)
{
 	uint8_t i=1;
	
	while(i)
	{
		if(RxCounter!=7)//长度不正确
		{
		 break;
		}
		if(RxBuffer[3]=='-')
		{
			for(i=0;i<3;i++)                            
			{                                              
				if(RxBuffer[i]>'9'||RxBuffer[i]<'0')
				break;
			}
			for(i=4;i<6;i++)                            
			{                                              
				if(RxBuffer[i]>'9'||RxBuffer[i]<'0')
				break;
			}
					
			Variable.UartPassword[0] = RxBuffer[0] - '0';
			Variable.UartPassword[1] = RxBuffer[1] - '0';
			Variable.UartPassword[2] = RxBuffer[2] - '0';
			
			if((Variable.UartPassword[0]!=Variable.Password[0])||\
				 (Variable.UartPassword[1]!=Variable.Password[1])||\
				 (Variable.UartPassword[2]!=Variable.Password[2]))
			break;
			
			Variable.Password[0] = RxBuffer[4] - '0';
			Variable.Password[1] = RxBuffer[5] - '0';
			Variable.Password[2] = RxBuffer[6] - '0';		
		}
	 break;
	}
}

/**
 * @brief  LCD显示函数
 * @param  无
 * @retval 无
 */
void Lcd_Proc(void)
{
	if(Variable.Index==PSD)
	{
		/*屏幕第二行*/
		 LCD_DisplayStringLine(Line1,"       PSD          ");
		/*屏幕第四行*/
		if(Variable.B1Reset==1)
		{
			sprintf((char*)LcdString,"    B1:%d",Variable.InputPassword[0]);
			LCD_DisplayStringLine(Line3 ,LcdString);
		}
		else
		{
		 LCD_DisplayStringLine(Line3,"    B1:@             ");
		}
		/*屏幕第五行*/
		if(Variable.B2Reset==1)
		{
			sprintf((char*)LcdString,"    B2:%d",Variable.InputPassword[1]);
			LCD_DisplayStringLine(Line4 ,LcdString);		 
		}
		else
		{
		 LCD_DisplayStringLine(Line4,"    B2:@             ");
		}
		/*屏幕第六行*/
		if(Variable.B3Reset==1)
		{
		sprintf((char*)LcdString,"    B3:%d",Variable.InputPassword[2]);
		LCD_DisplayStringLine(Line5 ,LcdString);		
		}
		else
		{
		 LCD_DisplayStringLine(Line5,"    B3:@             ");
		}
	}
	else if(Variable.Index==STA)
	{
		/*屏幕第二行*/
		 LCD_DisplayStringLine(Line1,"       STA          ");
		/*屏幕第四行*/
		LCD_DisplayStringLine(Line3,"    F:2000Hz          ");
		/*屏幕第五行*/
		LCD_DisplayStringLine(Line4,"    D:10%             ");
		/*屏幕第六行*/
		 LCD_DisplayStringLine(Line5,"                   ");		
	}
}

/**
 * @brief  系统参数初始化函数
 * @param  无
 * @retval 无
 */
void SystemPara_Init(void)
{
  Variable.Password[0]=1;
	Variable.Password[1]=2;
	Variable.Password[2]=3;
	Variable.Index=PSD;
	TIM_SetAutoreload(TIM2,1000 - 1);//PWM频率为1KHz.
	TIM_SetCompare2(TIM2,500);//占空比50%
}

/**
 * @brief  定时器4中断服务函数-10ms中断
 * @param  无
 * @retval 无
 */
void TIM4_IRQHandler(void)
{
	static uint8_t Index;
	static uint16_t TimeCnt,ErrorTimeCnt;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		
		//为每一个软件定时器中的中断次数计数减 1，直到其值为 0
		for(Index=0;Index<TIMERCOUNT;Index++)
		if(Timers[Index]>0)
		Timers[Index]--;
		
		if(Variable.Time5s_flag==1)
		{	
			if(++TimeCnt==500)
			{
			  TimeCnt=0;
			  Variable.Time5s_flag=2;
			}
		}	
		if(Variable.ErrorTime5s_flag==1)
		{				
		 if(++ErrorTimeCnt==500)
			{		
				ErrorTimeCnt=0;
				Variable.ErrorTime5s_flag=0;
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
	}
}

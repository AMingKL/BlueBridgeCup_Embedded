/**
  *************************************************************************************************************************
  * @file    myconfig.c
  * @author  AMKL
  * @version V1.0
  * @date    2022-04-10
  * @brief   ���ú���
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "myconfig.h"
/* �������� -----------------------------------------------------------------------------------------------------------------*/
uint8_t Timers[TIMERCOUNT];//�����ʱ������
System_InitTypeDef Variable;//�����ṹ������
uint8_t  LcdString[20];//lcd��ʾ����
/* ������ر��� */
uint8_t RxCounter;//������������
uint8_t RxoverFlag;//������ɱ�־
uint8_t RxBuffer[7];//��������



/**
 * @brief  �����ʱ��
 * @param  xIndex-�¼�x��xTime-��ʱʱ��
 * @retval ��
 */
uint8_t xDelay(uint8_t xIndex,uint8_t xTime)
{
	uint8_t RetValue=TIMEISNOTUP;

	if(Timers[xIndex]==0)//�ж�ָ�������ʱ���ļ������Ƿ�Ϊ 0
	{
		Timers[xIndex]=xTime;//��װ��������ֵ
		RetValue=TIMEISUP;	 //��ֵ����ֵΪ��ʱ�䵽	
	}
	return(RetValue);
}

/**
 * @brief  ����������
 * @param  ��
 * @retval ��
 */
void Key_Proc(void)
{
	static uint8_t i=0;
 
  Key_refresh();	 
	if(key_falling==B1)//�������������B1ֵ
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
	else	if(key_falling==B2)//�������������B2ֵ
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
	else	if(key_falling==B3)//�������������B3ֵ
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
	else if(key_falling==B4)//�������������ȷ�ϼ�
	{
		for(i=0;i<3;i++)
		{
		  if(Variable.Password[i]==Variable.InputPassword[i])
			{
				Variable.PasswordRight=1;//������ȷ
				Variable.Time5s_flag=1;//����5s��ʱ				
			}
			else
			{
			 Variable.PasswordError=1;//�������
			}
		}
		 Password_Check();//����˶Ժ���
	}
}

/**
 * @brief  ����˶Ժ���
 * @param  ��
 * @retval ��
 */
void Password_Check(void)
{
  if((Variable.PasswordRight==1)&&(Variable.PasswordError==0))//������ȷ 
	{
	 Variable.Index=STA;
	 Variable.PasswordRight=0;
	 Variable.ErrorCnt=0;//�����������
	}
	else//�������
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
		if(Variable.ErrorCnt>=3)//����3��5s��������
		{
			Variable.ErrorTime5s_flag=1;
		}			
	}
}

/**
 * @brief  Led2������
 * @param  ��
 * @retval ��
 */
void Led2_Proc(void)
{
  if(Variable.ErrorCnt>=3)
	{
		if(Variable.ErrorTime5s_flag==1)//��������
		{
		  LED_Toggle(2);//��������0.1sɨ��һ�δ˺�����ʵ����0.1s����
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
 * @brief  pwm������
 * @param  ��
 * @retval ��
 */
void Pwm_Proc(void)
{
  if(Variable.Index==STA)
	{
	 	TIM_SetAutoreload(TIM2,500 - 1);//����Ƶ��Ϊ2KHz
		TIM_SetCompare2(TIM2,100);//����ռ�ձ�10%
		LED_Control(LED1,ON);//��������汣��led1��
	}
	else
	{
		TIM_SetAutoreload(TIM2,1000 - 1);	//����PWMƵ��Ϊ1KHz.
	  TIM_SetCompare2(TIM2,500);//����ռ�ձ�50%
		LED_Control(LED1,OFF);
	} 
	if(Variable.Time5s_flag==2)//5sʱ�䵽����λ���룬�л���ԭ����
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
 * @brief  Usart������
 * @param  ��
 * @retval ��
 */
void Usart_Proc(void)
{
 	if(RxoverFlag==1)
	{
	  RxoverFlag=0;
		StringCheck();
		RxCounter=0;
		memset(RxBuffer,0,sizeof(RxBuffer));//��ջ�����
	}
}

/**
 * @brief  �ַ�����麯��
 * @attention �ж����ݳ��ȣ���ʽ����Χ�Ƿ���ȷ����ȷ���޸�����
 * @retval  ��
 */
void  StringCheck(void)
{
 	uint8_t i=1;
	
	while(i)
	{
		if(RxCounter!=7)//���Ȳ���ȷ
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
 * @brief  LCD��ʾ����
 * @param  ��
 * @retval ��
 */
void Lcd_Proc(void)
{
	if(Variable.Index==PSD)
	{
		/*��Ļ�ڶ���*/
		 LCD_DisplayStringLine(Line1,"       PSD          ");
		/*��Ļ������*/
		if(Variable.B1Reset==1)
		{
			sprintf((char*)LcdString,"    B1:%d",Variable.InputPassword[0]);
			LCD_DisplayStringLine(Line3 ,LcdString);
		}
		else
		{
		 LCD_DisplayStringLine(Line3,"    B1:@             ");
		}
		/*��Ļ������*/
		if(Variable.B2Reset==1)
		{
			sprintf((char*)LcdString,"    B2:%d",Variable.InputPassword[1]);
			LCD_DisplayStringLine(Line4 ,LcdString);		 
		}
		else
		{
		 LCD_DisplayStringLine(Line4,"    B2:@             ");
		}
		/*��Ļ������*/
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
		/*��Ļ�ڶ���*/
		 LCD_DisplayStringLine(Line1,"       STA          ");
		/*��Ļ������*/
		LCD_DisplayStringLine(Line3,"    F:2000Hz          ");
		/*��Ļ������*/
		LCD_DisplayStringLine(Line4,"    D:10%             ");
		/*��Ļ������*/
		 LCD_DisplayStringLine(Line5,"                   ");		
	}
}

/**
 * @brief  ϵͳ������ʼ������
 * @param  ��
 * @retval ��
 */
void SystemPara_Init(void)
{
  Variable.Password[0]=1;
	Variable.Password[1]=2;
	Variable.Password[2]=3;
	Variable.Index=PSD;
	TIM_SetAutoreload(TIM2,1000 - 1);//PWMƵ��Ϊ1KHz.
	TIM_SetCompare2(TIM2,500);//ռ�ձ�50%
}

/**
 * @brief  ��ʱ��4�жϷ�����-10ms�ж�
 * @param  ��
 * @retval ��
 */
void TIM4_IRQHandler(void)
{
	static uint8_t Index;
	static uint16_t TimeCnt,ErrorTimeCnt;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		
		//Ϊÿһ�������ʱ���е��жϴ��������� 1��ֱ����ֵΪ 0
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
 * @brief  ����2�����жϷ������
 * @param  ��
 * @retval ��
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

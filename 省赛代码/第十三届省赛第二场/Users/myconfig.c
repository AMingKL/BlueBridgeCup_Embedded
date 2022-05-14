#include "myconfig.h"


/* �������� -----------------------------------------------------------------------------------------------------------------*/
unsigned char  Timers[TIMERCOUNT];//�����ʱ������
unsigned char  LcdString[20];//lcd��ʾ����

uint8_t Index;//����

uint8_t X_goumaiNum;//x��������
uint8_t X_danjia;//x����
uint8_t X_kucunNum;//x�������
uint8_t X_Oleddanjia;//x֮ǰ����
uint8_t X_OledkucunNum;//x֮ǰ�������

uint8_t Y_goumaiNum;//y��������
uint8_t Y_danjia;//y����
uint8_t Y_kucunNum;//y�������	
uint8_t Y_Oleddanjia;//Y֮ǰ����
uint8_t Y_OledkucunNum;//Y֮ǰ�������

uint8_t Queren_flag;//ȷ�Ϲ���
uint8_t flag_5s,Flash_Flag;//led��˸��־

uint8_t verify;//eeprom����

uint8_t RxCounter;//���ڽ������ݳ���
uint8_t RxoverFlag;//���ڽ����Ƿ���ɱ�־λ
uint8_t RxBuffer[25];//���ݻ�����



/**
 * @brief  �����ʱ��
 * @param  xIndex-�¼�x��xTime-��ʱʱ��
 * @retval ��
 */
unsigned char xDelay(unsigned char xIndex,unsigned char xTime)
{
	unsigned char RetValue=TIMEISNOTUP;

	if(Timers[xIndex]==0)//�ж�ָ�������ʱ���ļ������Ƿ�Ϊ 0
	{
		Timers[xIndex]=xTime;//��װ��������ֵ
		RetValue=TIMEISUP;	 //��ֵ����ֵΪ��ʱ�䵽	
	}
	return(RetValue);
}

/**
 * @brief  LCD��ʾ����
 * @attention ��ʾ���ݵ���Ϣ
 * @retval  ��
 */
void Lcd_Proc(void)
{
	if(Index==SHOP)
	{
		/*��Ļ�ڶ���*/
		 LCD_DisplayStringLine(Line1,"        SHOP        ");	
		/*��Ļ������*/				   
		sprintf((char*)LcdString,"     X:%d           ",X_goumaiNum);
		LCD_DisplayStringLine(Line3 ,LcdString); 
		/*��Ļ������*/
		sprintf((char*)LcdString,"     Y:%d           ",Y_goumaiNum);
		LCD_DisplayStringLine(Line4 ,LcdString); 	 
		
	}
	else if(Index==PRICE)
	{
		/*��Ļ�ڶ���*/
		 LCD_DisplayStringLine(Line1,"        PRICE       ");	
		/*��Ļ������*/				      
		sprintf((char*)LcdString,"     X:%d.%d           ",X_danjia/10,X_danjia%10);
		LCD_DisplayStringLine(Line3 ,LcdString); 
		/*��Ļ������*/
		sprintf((char*)LcdString,"     Y:%d.%d           ",Y_danjia/10,Y_danjia%10);
		LCD_DisplayStringLine(Line4 ,LcdString); 		 
	}
	else if(Index==REP)
	{
		/*��Ļ�ڶ���*/
		 LCD_DisplayStringLine(Line1,"        REP         ");	
		/*��Ļ������*/				      
		sprintf((char*)LcdString,"     X:%d           ",X_kucunNum);
		LCD_DisplayStringLine(Line3 ,LcdString); 
		/*��Ļ������*/
		sprintf((char*)LcdString,"     Y:%d           ",Y_kucunNum);
		LCD_DisplayStringLine(Line4 ,LcdString); 	 
	}
}


/**
 * @brief  ���ڴ�����
 * @attention ��
 * @retval  ��
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
 * @brief  �ַ�����麯��
 * @attention �ж����ݳ��ȣ���ʽ�Ƿ���ȷ
 * @retval  ��
 */
void  StringCheck(void)
{
 	uint8_t i=1;
	
	while(i)
	{
		if(RxCounter!=1)//���Ȳ���ȷ
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
 * @brief      ����������
 * @attention  ��
 * @retval     ��
 */
void Key_Proc(void)
{
	Key_refresh();
	if(key_falling==B1)//B1����-�����л�����
	{
		Index=(Index+1)%3;
	}
	
	else	if(key_falling==B2)//B2����-����Ʒ X��
	{
		if(Index==SHOP)
		{
			(X_goumaiNum==X_kucunNum)?(X_goumaiNum=0):(X_goumaiNum++);
		}
		else if(Index==PRICE)
		{
		   X_Oleddanjia=X_danjia;
		  (X_danjia==20)?(X_danjia=10):(X_danjia++);
			if( X_danjia!=X_Oleddanjia)//�������б仯
			{
			  Write_eeprom(2,X_danjia);//����
				Delay_Ms(5);
			}
		}
		else if(Index==REP)
		{
			X_OledkucunNum=X_kucunNum;
		  X_kucunNum++;
			if( X_OledkucunNum!=X_kucunNum)//�������б仯
			{
			  Write_eeprom(0,X_kucunNum);//����
				Delay_Ms(5);
			}
		}
	}
	
	
	else	if(key_falling==B3)//B3����-����Ʒ Y��
	{
		if(Index==SHOP)
		{
			(Y_goumaiNum==Y_kucunNum)?(Y_goumaiNum=0):(Y_goumaiNum++);
		}
		else if(Index==PRICE)
		{
			 Y_Oleddanjia=Y_danjia;
		  (Y_danjia==20)?(Y_danjia=10):(Y_danjia++);
			if( Y_danjia!=Y_Oleddanjia)//�������б仯
			{
				Write_eeprom(3,Y_danjia);//����
				Delay_Ms(5);
			}
		}
		
		else if(Index==REP)
		{
			Y_OledkucunNum=Y_kucunNum;
		  Y_kucunNum++;
			if( Y_OledkucunNum!=Y_kucunNum)//�������б仯
			{
				Write_eeprom(1,Y_kucunNum);//����
				Delay_Ms(5);
			}
		}
	}
	
	
	else if(key_falling==B4)//B4���� -"ȷ��"
	{
		if(Index==SHOP)
		{
			Queren_flag=1;
			flag_5s=1;
			Y_kucunNum=Y_kucunNum-Y_goumaiNum;//���¿������
			X_kucunNum=X_kucunNum-X_goumaiNum;//���¿������
			
			// ����Ʒ���Ϣ
			printf("X:%d,Y:%d,Z:%d.%d\r\n",X_goumaiNum,Y_goumaiNum,\
			((X_goumaiNum*X_danjia)+(Y_goumaiNum*Y_danjia))/10,\
			((X_goumaiNum*X_danjia)+(Y_goumaiNum*Y_danjia))%10);
			
			if(X_goumaiNum!=0)//���������б仯
			{
				Write_eeprom(0,X_kucunNum);//����
				Delay_Ms(5);
			}
			if(Y_goumaiNum!=0)//���������б仯
			{	
				Write_eeprom(1,Y_kucunNum);//����
				Delay_Ms(5);			
			}				
			
			X_goumaiNum=0;//��Ʒ���������xy��0
			Y_goumaiNum=0;//��Ʒ���������xy��0
		}
		
	}
}

/**
 * @brief  led&pwm������ú���
 * @param  ��
 * @retval ��
 */
void Led_PWM_Proc(void)
{
	if(flag_5s==1)
	{
		LED_Control(LED1,ON);
		TIM_SetAutoreload(TIM2,500 - 1);//����Ƶ��Ϊ2KHz
		TIM_SetCompare2(TIM2,150);//����ռ�ձ�30%
  }
	else
	{
		LED_Control(LED1,OFF);
		TIM_SetAutoreload(TIM2,500 - 1);//����Ƶ��Ϊ2KHz
		TIM_SetCompare2(TIM2,25);//����ռ�ձ�5%
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
 * @brief  ϵͳ������ʼ������
 * @param  ��
 * @retval ��
 */
void SystemPara_Init(void)
{
	verify=Read_eeprom(4);
	
	if(verify!=10)//����趨һ��������û����ô�ɰɣ������������Ⱦ���Ϊ��һ���ϵ�
	{
		Write_eeprom(0,10);//����
		Delay_Ms(5);
		Write_eeprom(1,10);//����
		Delay_Ms(5);	
		Write_eeprom(2,10);//����
		Delay_Ms(5);
		Write_eeprom(3,10);//����
		Delay_Ms(5);	 
		Write_eeprom(4,10);//��ֵΪ10���˶κ���ִֻ��һ��	
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
 * @brief  ��ʱ��4�жϷ�����-10ms�ж�
 * @param  ��
 * @retval ��
 */
void TIM4_IRQHandler(void)
{
	static uint8_t Index;
	static uint16_t TimeCnt;
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		
		//Ϊÿһ�������ʱ���е��жϴ��������� 1��ֱ����ֵΪ 0
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
		Usart_Proc();
	}
}

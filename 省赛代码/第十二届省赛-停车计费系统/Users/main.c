/**
  *************************************************************************************************************************
  * @file    main.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-18
  * @brief   ���ű�Ƕ��ʽ��ʮ����ʡ����-ͣ���Ʒ�ϵͳ
  *************************************************************************************************************************
  * @attention
  *
	*�ѵ�:1.���ڴ������ղ�������Ϣ
	*     2.PA7�͵�ƽ�����ã�ǿ�����
	*     3.��λѡ�񣬿ճ�λ���ж�
 	* 
	*�߼�����:1.����ĳ���ʱ�䣬���ܱ����ʱ��
	          2.�����Ψһ�������������������Ͷ������ͬ
  *************************************************************************************************************************
  */
/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "myconfig.h"
/* �������� -----------------------------------------------------------------------------------------------------------------*/
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
/* �������� -----------------------------------------------------------------------------------------------------------------*/
/* LCD��ʾ���� */
uint8_t CNBRstring[20];
uint8_t VNBRstring[20];
uint8_t IDLEstring[20];
uint8_t CNBRRatestring[20];
uint8_t VNBRRatestring[20];
/* ������ر��� */
uint8_t RxCounter;
uint8_t RxoverFlag;
uint8_t RxBuffer[25];
/* ���� */
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
 * @brief  ����ַ�������
 * @attention �ж����ݳ��ȣ���ʽ��ʱ�䷶Χ�Ƿ���ȷ
 * @retval 0-���� 1-��ȷ
 */
uint8_t StringCheck(void)  
{
	uint8_t i;
	if(RxCounter!=22)//���Ȳ���ȷ
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
 * @brief  �ַ����������㺯��
 * @attention  ��ջ���������ֹ��ɸ���
 * @retval ��
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
 * @brief  �������ݴ�����
 * @attention  	
 * ����ַ�����Ϣ��ȷ����ж����޿��г�λ�����������:   
 *1.�޳�λ����
 *2.�г�λ���жϳ�����⻹�ǳ��⣬������⣬�������ʱ����Ϣ�����泵���ͣ���š���Ӧ������������һ�����г�λ������һ
 *3.�г�λ���������⣬�ҵ��ڼ��ų�λ�ĳ����⣬�ж��Ƿ�����߼�(ʱ�䣬���ƺź�����һ��)��������ʱ���߼��������������⡣�����߼������ڴ����շ���Ϣ����������Ϣ����Ӧ������������һ�����г�λ������һ��
 * @retval ��
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
					CarInStorage();//���
				}
				else if(JudgeTheCar_In_or_Out()==1)
				{
				 printf("Error\n");//����
				}
				else if(JudgeTheCar_In_or_Out()==2)
				{
				 CarOutStorage();//����
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
 * @brief      �������⺯��
 * @attention  ��
 * @retval     ��
 */
void CarOutStorage(void)
{
	if(car[locate].yearin>year_temp||car[locate].monthin>month_temp||car[locate].dayin>day_temp||car[locate].hourin>hour_temp||car[locate].minin>min_temp||car[locate].secin>sec_temp)
	{
	  printf("Error\n");//����ʱ��С�����ʱ�䣬�߼�����
	}
	else//�����߼������ڴ����շ���Ϣ����������Ϣ����Ӧ������������һ�����г�λ������һ��
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
    ChargingTheCar();//�շѺ���
	}
   cleanidtype();
}

/**
 * @brief      ���ճ�λ�ź���
 * @attention  ��
 * @retval     ��
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
 * @brief      ������⺯��
 * @attention  �統ǰ��λ��ռ���ҿճ�λ��
 * @retval     ��
 */
void CarInStorage(void)
{
	uint8_t i;
	
	if(car[8-System.IDLE_Number].state==FULL)//��ǰ��λ��ռ����һ���յĳ�λ
	{		
	  //��һ���յĳ�λ
			for(i=0;i<4;i++)//���泵����
		{
		 car[EmptyStorage()].cartype[i]=RxBuffer[i];
		}
		for(i=0;i<4;i++)//���泵���
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
	else//ʹ�õ�ǰ��λ
	{
		for(i=0;i<4;i++)//���泵����
		{
		 car[8-System.IDLE_Number].cartype[i]=RxBuffer[i];
		}
		for(i=0;i<4;i++)//���泵���
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
 * @brief      �жϳ��������⺯��
 * @attention  �޳�λ����Ϊ�³����豨��
 * @retval     ��
 */
uint8_t JudgeTheCar_In_or_Out(void)
{	
  if(System.IDLE_Number==8)//��λ���㣬���
	{
	 return 0;
	}
	else
	{
		substr(car_id,RxBuffer,5,4);//�������ı����Ϣ��ȡ��car_id
		substr(car_type,RxBuffer,0,4);//��������������Ϣ��ȡ��car_type
		locate = isExist(car_id);//��ѯ�������Ƿ��ڳ���Ӵ���
		if(locate != 0xFF)//�������ڳ����д���
		{
			if(strcmp((const char *)car_type,(const char *)car[locate].cartype))		// ���������id�ͳ��������Ͳ�ͬ�����ʾ��Ϣ����
			{
				return 1;//����
			}
			else
			{
			 return 2;//����
			}
		}
		else// �������ڳ����в�����
		{
		  if(System.IDLE_Number==0)//û�г�λ
				return 1;//����
			else
				return 0;//���
		}
	}
}

/**
 * @brief      ����洢��Ϣ����
 * @attention  ����������洢��Ϣ
 * @retval     ��
 */
void cleanidtype(void)
{
	uint8_t i;		
	for(i=0;i<4;i++)//���
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
 * @brief      ����������
 * @attention  ע���ڷ��������µķ��ʲ�����Ч
 * @retval     ��
 */
void Key_Proc(void)
{
	Key_refresh();
	if(key_falling==B1)//�����л�����
	{
		System.Interface=(System.Interface+1)%2;
		if(System.Interface==CarShow)
		{
		  System.CNBR_Rate=System.CNBR_RateTemp;
			System.VNBR_Rate=System.VNBR_RateTemp;
		}
	}
	else	if(key_falling==B2)//�������Ӽ������¼�0.5Ԫ
	{
	  if(System.Interface==RateSet)//����δ�漰�������ޣ��Ҷ�Ϊ20Ԫ
		{
		  (System.CNBR_RateTemp==2000)?(System.CNBR_RateTemp=2000):(System.CNBR_RateTemp+=50);
			(System.VNBR_RateTemp==1850)?(System.VNBR_RateTemp=1850):(System.VNBR_RateTemp+=50);
		}
	}
	else	if(key_falling==B3)//���ʼ��ټ������¼�0.5Ԫ
	{
	 	if(System.Interface==RateSet)//�������ޣ��Ҷ�Ϊ0Ԫ
		{
		  (System.CNBR_RateTemp==150)?(System.CNBR_RateTemp=150):(System.CNBR_RateTemp-=50);
			(System.VNBR_RateTemp==0)?(System.VNBR_RateTemp=0):(System.VNBR_RateTemp-=50);
		}
	}
	else if(key_falling==B4)//���ư��� 0. PA7:2KHZ,20%ռ�ձ�  1. PA7:�͵�ƽ
	{
	  System.PA7_state=(System.PA7_state+1)%2;
	}
}
/**
 * @brief      led����
 * @attention  ��
 * @retval     ��
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
 * @brief      PA7�������
 * @attention  ��
 * @retval     ��
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
 * @brief      LCD����
 * @attention  ��
 * @retval     ��
 */
void Lcd_Proc(void)
{
	if(System.Interface==CarShow)
	{		
		/*                ��Ļ��һ��                          */
		LCD_DisplayStringLine(Line0," LQB Di 12 Jie SaiTi");
		/*                ��Ļ�ڶ���                          */
		LCD_DisplayStringLine(Line2,"       Data         ");
		/*                ��Ļ������                          */
		sprintf((char*)CNBRstring,"   CNBR:%d           ",System.CNBR_Number);
		LCD_DisplayStringLine(Line3 ,CNBRstring);
		/*                ��Ļ������                          */
		sprintf((char*)VNBRstring,"   VNBR:%d           ",System.VNBR_Number);
		LCD_DisplayStringLine(Line5 ,VNBRstring);
		/*                ��Ļ������                          */
		sprintf((char*)IDLEstring,"   IDLE:%d           ",System.IDLE_Number);
		LCD_DisplayStringLine(Line7 ,IDLEstring);
		/*                ��Ļ�ھ���                          */
		LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");	
	}
  else if(System.Interface==RateSet)
	{
		/*                ��Ļ��һ��                          */
		LCD_DisplayStringLine(Line0," LQB Di 12 Jie SaiTi");
		/*                ��Ļ�ڶ���                          */
		LCD_DisplayStringLine(Line2,"       Para         ");
		/*                ��Ļ������                          */
		sprintf((char*)CNBRRatestring,"   CNBR:%d.%.2d        ",System.CNBR_RateTemp/100,System.CNBR_RateTemp%100);
		LCD_DisplayStringLine(Line3 ,CNBRRatestring);
		/*                ��Ļ������                          */
		sprintf((char*)VNBRRatestring,"   VNBR:%d.%.2d        ",System.VNBR_RateTemp/100,System.VNBR_RateTemp%100);
		LCD_DisplayStringLine(Line5 ,VNBRRatestring);
		/*                ��Ļ������                          */
		LCD_DisplayStringLine(Line7,"                       ");	
		/*                ��Ļ�ھ���                          */
		LCD_DisplayStringLine(Line9," Author:LiuHongMing    ");	
	}
}
/**
 * @brief      ͣ��ϵͳ��ʼ����
 * @attention  ��ʼ����ر���
 * @retval     ��
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
 * @brief      �ַ�����ȡ����
 * @param       * d_str;�洢���ݵ��ַ��������� * s_str:�����ַ���������  locate:�����������ʼλ length:�������ݵĳ���
 * @attention  ��
 * @retval     ��
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
 * @brief      �жϳ�������
 * @attention  ���洢���е���Ϣ�뵱ǰ�����Ա� 1.�Ƿ���� 2.���س�λ�� 
 * @retval     ���ڣ�iΪ���س�λ��  0xFF��������
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
 * @brief      �������ü��㺯��
 * @attention  ��������õ���Ϣ
 * @retval     ��
 */
void ChargingTheCar(void)
{
  // ����һ��365��,һ����31�죬��Ϊ��λ
	CarStopTime=(year_temp-car[locate].yearin)*365*24*3600+(month_temp-car[locate].monthin)*31*24*3600+(day_temp-car[locate].dayin)*24*3600\
	             +(hour_temp-car[locate].hourin)*3600+(min_temp-car[locate].minin)*60+(sec_temp-car[locate].secin);
	//�����Сʱ������һСʱ��һ��Сʱ��
	CarStopTime=(CarStopTime+3599)/3600;
	
	if(RxBuffer[0] == 'C')
	{
	 Money=CarStopTime*System.CNBR_Rate;
	}
	else
	{
	 Money=CarStopTime*System.VNBR_Rate;
	}
	// ����Ʒ���Ϣ
	printf("%s:%s:%d:%d.%.2d\r\n",car[locate].cartype,car[locate].carid,CarStopTime,Money/100,Money%100);
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

/**
 * @brief  ��ʱ��4�жϷ�����-1ms�ж�
 * @param  ��
 * @retval ��
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


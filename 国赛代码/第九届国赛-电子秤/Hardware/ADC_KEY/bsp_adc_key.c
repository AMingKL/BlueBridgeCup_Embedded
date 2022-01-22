/**
  *************************************************************************************************************************
  * @file    adc_key.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   ���ű�Ƕ��ʽ��չ��AD����ģ��
  *************************************************************************************************************************
  * @attention
  *��չ��AD��������  PA5  - ADC1  CH5
  * 
	* ����˵����    �����ɿ� -- 3.3V   |4095
  *	  
  *  
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_adc_key.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL]; 
uint8_t   keyCheck = 0;
uint8_t   keyState = 0;
uint16_t  last_keyValue = 0xff;//��һ�εļ�ֵ
uint32_t  keyLongCheck = 0;
uint16_t  Key_LongTime = 0;
uint8_t   keyCountTime = 0;
uint16_t   DoubleClickTime=0;

KEY_TypeDef Key = {0xff,FALSE,FALSE,FALSE,FALSE}; 
/**
 * @brief        ����ɨ�躯��
 * @attention  ���ݵ����ѹ����ȡ����ͬ�ĵ�ѹֵ��Ӧ��������
 * @retval ��
 */
uint8_t Adc_Key_Read(void)  //�ж����ĸ���������
{
	uint16_t temp = 0;
	temp = Get_AnalogVoltage(VRP6);
	if(temp <= 80){//ʵ�� 0x0000��0x0001  0-1֮��
		return 1;
	}else if((temp >= 100) && (temp <= 300)){//ʵ�� 0x00CD��0x00CE  205-206֮��
		return 2; 
	}else if((temp >= 500) && (temp <= 700)){//ʵ�� 0x0264��0x0265  612-613֮��
		return 3;
	}else if((temp >= 900) && (temp <= 1200)){//ʵ�� 0x03FF��0x0400  1023-1024֮��
		return 4;
	}else if((temp >= 1300) && (temp <= 1500)){//ʵ�� 0x0580��0x0584  1408-1412֮��
		return 5;
	}else if((temp >= 1700) && (temp <= 1900)){//ʵ�� 0x0744��0x0749  1860-1865֮��
		return 6;
	}else if((temp >= 2200) && (temp <= 2400)){//ʵ�� 0x0919��0x091A  2329-2330֮��
		return 7;
	}else if((temp >= 2700) && (temp <= 2900)){//ʵ�� 0x0B34��0x0B35  2868-2869֮��
		return 8;
	}else
	{
		return 0;	//error status & no key  //4095		
	}
}


/**
  * @˵��     ����״̬������
  * @����     none
  * @����ֵ   None
  * @ע��   1.����״̬��ֻ����״̬�� ��������״̬
            2.Key.Value ��Key.ShortPress 
              ��ʹ�ú���Ҫ��Ϊ��λ0xff,FALSE���������ܱ�֤ÿһ�ε�״̬���ܱ�����
            3.Key_Interrup()������Ҫ���붨ʱ���жϣ����ж�ʱ��С��10MS�����Ϊ10�Ĺ�Լ������1��2��5MS
**/
void Key_Interrup(void)      
{
	uint8_t keyPress = 0xff;
	keyCountTime ++;
	if(keyCountTime >= KEY_EliShaking_Time) //�������
	{
		keyCountTime = 0;
		keyCheck = 1;
	}
	if(keyCheck == 1)
	{
		keyCheck = 0;
		keyPress = Adc_Key_Read();
		switch(keyState)
		{
			case 0://����δ����̬
				if(keyPress != 0xff)//��ʾ�а�������
				{
					last_keyValue = keyPress; //��¼��ǰ����״̬
					keyState = 1;
				}
				else
				{
					keyState = 0;
				}
			break;
			case 1://��ʾ�а������£��жϵ�ǰֵ����һ�ε�ֵ�Ƿ�һ��������һ����Ϊ������
				if(keyPress == last_keyValue)//���Ƕ���
				{
					keyState = 2;
				}else
				{
					keyState = 0; //�Ƕ�����������һ��
					last_keyValue = 0xff;
				}	 
				break;
			case 2:
				if(keyPress != last_keyValue)//��ʾ�������ɿ���ֻ�г���״̬�������������ʱ�������̰�״̬��������ȴ��û�ʹ�ò����
				{		
					if(keyLongCheck < KEY_LONGSTATUS_Time)//�̰�
					{
						Key.ShortPress = TRUE;						
						Key.Value = last_keyValue;
					}
					Key.LongPress = FALSE;
					last_keyValue = 0xff;
					keyState = 0;// �����������س�ʼ״̬��
					keyLongCheck = 0;//������ʱ��0
					Key_LongTime = 0;
				}
				else
				{					
					keyLongCheck++;
					if(keyLongCheck >= KEY_LONGSTATUS_Time)//����ʱ�䳬�� ����ʱ�� ,����ֱ���ۼӣ���Ϊ��������ʱ��֮��ÿ��ɨ��ʱ�䣨10MS���Ϳ��Է���һ�ΰ���ֵ�����Ұ���ֵ�ù�������
					{
						keyLongCheck = KEY_LONGSTATUS_Time + 1;
	
						Key.Value = last_keyValue;//����ֵ����ĸ��������ڵ��ú���������Կ���ֱ�������ۼӹ���
						Key.ShortPress = FALSE;
						Key.LongPress = TRUE;						
					}
				}
				break;
			default : break;
		}
	}
	if(Key.LongPress == TRUE)//��������ˢ�£����г��������ۼ����ۼ�ʹ�ã���Ϊ�趨����ۼ�һ��
	{
		Key_LongTime ++;
		if(Key_LongTime >= Key_LongTimeSet)
		{
			Key_LongTime = 0;
			Key.LongFresh = TRUE;
		}
	}
}

/*****************************************************END OF FILE*********************************************************/	

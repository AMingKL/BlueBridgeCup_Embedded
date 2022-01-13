/**
  *************************************************************************************************************************
  * @file    led.c
  * @author  ������
  * @version V1.0
  * @date    2021-12-28
  * @brief   ����Ƕ��ʽ�����尴������
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_key.h"
#include  "bsp_led.h"

/* ���� -----------------------------------------------------------------------------------------------------------------*/
/* ����������ر��� */
static uint8_t Key_Trg_Falling;//�����½���
static uint8_t Key_Trg_Rising;//����������
static uint8_t key_State;//����״̬��δ����Ϊ0��key1����0x01��key2����0x02��key3����0x04��key4����0x08.
static uint16_t key_time = 0;//�жϰ�����ǰ״̬-1.δ���� 2.�̰� 3.����
static uint16_t key_pressed_time = 0;//��������ʱ��
static uint16_t key1Time = 0;//�ж�KEY1�����Ƿ�˫������ʱ��
static uint8_t  key1PressNumber = 0;//�ж�KEY1������˫������

/**
 * @brief  ������ʼ��
 * @param  ��
 * @retval ��
 */
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief  ����ɨ��
 * @param  	// ���1��key1������һ�ΰ��£���ʱkey_State��Key_Trg = 0x01
	          // ���2��key1�����������£��ڶ���ɨ���ʱ�򣬴�ʱkey_State = 0x01��Key_Trg = 0
	          // ���3�����û�а������£���key_Value = 0��Key_Trg = 0
	          // �����ϵķ������Կ�����Key_Trgֻ���ڰ������½�����һʱ����ֵ��
 * @retval key_State:��ֵ
 */
void Key_Read(void)
{
	uint8_t key_Value = 0xF0;
	key_Value |= GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) | (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) << 1) | \
					    (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) << 2) | (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) << 3);
	key_Value = key_Value ^ 0xff;		// key1���£�key_state = 0x01,���û�а������£���key_Value = 0
	Key_Trg_Rising = (!key_Value) * (key_Value^key_State); // ��ⰴ�����½���	
	Key_Trg_Falling = key_Value & (key_Value^key_State); // ��ⰴ�����Ͻ���
	key_State = key_Value;//key_State���������ɨ�����İ�����ֵ
}
/**
 * @brief  ��������
 * @param  ��
 * @retval ����������ʹ�ö�ʱ��ÿ10msɨ��ú���
 */
void Key_Scan(void)
{
	Key_Read();
	
	if(key1PressNumber == 1)	// ��⵽һ�ΰ����������أ���ʼ��ʱ
	{
		key1Time += 10;			// ���ڰ���������10msɨ��һ�Σ����������10ms
		if(key1Time >= 150)	// �������1500ms����ʾ�����ǵ�������ʼ��˫�����
		{
			// begin
			// ��������Ӱ��������Ĵ���
			// end
			key1Time = 0;
			key1PressNumber = 0;
		}
	}
	
	if(Key_Trg_Rising == 0x01)	// �����������أ��������ɿ�
	{
		if(key1PressNumber == 0)	// ��ʾ�����ǵ�һ�ΰ�����������
		{
			key1PressNumber = 1;		// �������
		}
	}
	
	if(Key_Trg_Falling == 0x01)	// ��⵽�������½���
	{
		LED_Control(LED1,ON);		
		if(key1PressNumber == 1)	// ����Ѿ�������һ�ΰ����������أ��������ڻ�û�г�ʱ����ô�����һ��˫���Ķ���
		{
			// begin
			// �����������˫����Ҫ����ĳ���
			LED_Control(LED5,ON);
			// end
			key1Time = 0;
			key1PressNumber = 0;		// ��ʼ������˫����صı���
		}		
	}
	else if(Key_Trg_Falling == 0x02)
	{
		LED_Control(LED2,ON);
	}
	else if(Key_Trg_Falling == 0x04)
	{
		LED_Control(LED3,ON);
	}
	else if(Key_Trg_Falling == 0x08)
	{
		LED_Control(LED4,ON);
	}
	
	if(key_State == 0x01)
	{
		key_pressed_time+=10;
		if(key_time != 0xffff && ++key_time >= 50)
		{
			key_time = 0xffff;			
		}
	}
	
	if(key_State == 0 && Key_Trg_Falling == 0)	// �����ɿ�
	{
		key_pressed_time = 0;
		key_time = 0;// ��ֹ�ظ�����γ������ɿ�������ʱ�򣬽�key_time����	
		LED_Control(LED_ALL,OFF);
	}
}

/*****************************************************END OF FILE*********************************************************/	

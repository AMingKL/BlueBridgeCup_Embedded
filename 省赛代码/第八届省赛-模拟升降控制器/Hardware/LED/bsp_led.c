/**
  *************************************************************************************************************************
  * @file    led.c
  * @author  ������
  * @version V1.0
  * @date    2021-12-28
  * @brief   ����Ƕ��ʽ������LED����
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include  "bsp_led.h"
void  LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=LED_ALL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	/* LED�ܽ�Ĭ��Ϊ�ߵ�ƽ��LEDȫ�� */
	GPIO_SetBits(GPIOC,LED_ALL);
	/* LED�������ܽ���Ϊ�͵�ƽ������˲���������˱仯 */
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);	
}

/**
 * @brief  LED�ƿ��ƺ���
 * @param  led:LED1~LED8,state:ON-����OFF��
 * @retval �������ƣ�����Ӱ��
 */
//void LED_Control(u16 led, u8 state)
//{
//	if(state)
//	{		
//		GPIO_ResetBits(GPIOC,led);
//		GPIO_SetBits(GPIOD,GPIO_Pin_2);
//		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
//	}
//	else
//	{
//		GPIO_SetBits(GPIOC,led);
//		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
//		GPIO_SetBits(GPIOD,GPIO_Pin_2);
//	}
//}

void LED_Control(u16 led, u8 state)
{
	if(state)		
		GPIOC->ODR &= ~led ;
	else
		GPIOC->ODR |= led;
	
	GPIOD->ODR |= GPIO_Pin_2;
	GPIOD->ODR &= ~GPIO_Pin_2;
}
/**
 * @brief  LED����˸����
 * @param  ledx:1~8��Ӧled1~led8
 * @retval �������ƣ�����Ӱ��
 */
void LED_Toggle(uint16_t LEDx)
{
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	GPIOC->ODR ^= (LEDx << 8);
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
}
/**
 * @brief  LED��Ϩ����
 * @param  ��
 * @retval LEDȫ��
 */
void LED_ALL_OFF(void)
{
	
	GPIOD-> ODR |= (1<<2);
	GPIOC-> ODR = 0XFFFF;
	GPIOD-> ODR &=~(1<<2);
}
/*****************************************************END OF FILE*********************************************************/	



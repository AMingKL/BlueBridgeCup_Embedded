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
	LED_ALL_OFF();	
}

/**
 * @brief  LED����˸����
 * @param  ledx:1~8��Ӧled1~led8
 * @retval �������ƣ�����Ӱ��
 */
void LED_Toggle(uint8_t LEDx)
{
	   LEDx=LEDx+7;
     GPIOC-> ODR ^= (1<<LEDx);
     GPIOD-> ODR |= (1<<2);
     GPIOD-> ODR &=~(1<<2);
}
/**
 * @brief  LED�ƿ��ƺ���
 * @param  led:LED1~LED8,state:ON-����OFF��
 * @retval �������ƣ�����Ӱ��
 */
void LED_Control(u8 ledx,u8 state)   //����led������,ledxȡֵ��Χ:8-15
{
	ledx=ledx+7;
	if(state)
	{
			GPIOC->ODR &=~(1<<ledx);    
   		GPIOD->ODR |=1<<2;
   		GPIOD->ODR &=~(1<<2);	
	}
	else
	{
	  GPIOC->ODR |=1<<ledx;
   	GPIOD->ODR |=1<<2;
   	GPIOD->ODR &=~(1<<2);
	}
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



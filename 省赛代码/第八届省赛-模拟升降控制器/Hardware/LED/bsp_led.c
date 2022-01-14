/**
  *************************************************************************************************************************
  * @file    led.c
  * @author  刘洪明
  * @version V1.0
  * @date    2021-12-28
  * @brief   蓝桥嵌入式开发板LED配置
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
	/* LED管脚默认为高电平，LED全灭 */
	GPIO_SetBits(GPIOC,LED_ALL);
	/* LED锁存器管脚设为低电平，输出端不会随输入端变化 */
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);	
}

/**
 * @brief  LED灯控制函数
 * @param  led:LED1~LED8,state:ON-亮，OFF灭
 * @retval 独立控制，互不影响
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
 * @brief  LED灯闪烁函数
 * @param  ledx:1~8对应led1~led8
 * @retval 独立控制，互不影响
 */
void LED_Toggle(uint16_t LEDx)
{
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	GPIOC->ODR ^= (LEDx << 8);
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
}
/**
 * @brief  LED灯熄灭函数
 * @param  无
 * @retval LED全灭
 */
void LED_ALL_OFF(void)
{
	
	GPIOD-> ODR |= (1<<2);
	GPIOC-> ODR = 0XFFFF;
	GPIOD-> ODR &=~(1<<2);
}
/*****************************************************END OF FILE*********************************************************/	



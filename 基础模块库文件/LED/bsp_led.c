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

void  led_Init(void)
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
void led_Control(u16 led, u8 state)
{
	if(state)
	{
		GPIOD-> ODR |= (1<<2);
		GPIO_ResetBits(GPIOC,led);
		GPIOD-> ODR &=~(1<<2);
	}
	else
	{
		GPIOD-> ODR |= (1<<2);
		GPIO_SetBits(GPIOC,led);
		GPIOD-> ODR &=~(1<<2);
	}

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



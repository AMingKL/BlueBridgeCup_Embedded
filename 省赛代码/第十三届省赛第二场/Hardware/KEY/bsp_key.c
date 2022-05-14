/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_key.h"

/* 定义 -----------------------------------------------------------------------------------------------------------------*/

/* 按键处理相关变量 */
uint8_t key_state = 0;
uint8_t key_falling = 0;
uint8_t key_rising = 0;


/**
 * @brief  按键初始化
 * @param  无
 * @retval 无
 */
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Key_refresh(void)
{
	uint8_t key_temp = 0xF0;
	key_temp |= KEY_STATE_GET;
	key_temp ^= 0xFF;
	key_falling = key_temp & (key_state ^ key_temp);
	key_rising = (!key_temp) * (key_state ^ key_temp);//(!key_temp) & (key_state ^ key_temp)
	key_state = key_temp;
}

/*****************************************************END OF FILE*********************************************************/	

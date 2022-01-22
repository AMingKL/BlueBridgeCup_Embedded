/**
  *************************************************************************************************************************
  * @file    key.h
  * @author  刘洪明
  * @version V1.0
  * @date    2021-12-28
  * @brief   蓝桥嵌入式开发板按键模块
  *************************************************************************************************************************
  * @attention
  * 
  * 按键接口说明：
	*     B1--PA0
	*     B2--PA8	   
	*     B3--PB1
	*     B4--PB2
  * 按键IO口默认为高，当IO口为低时，说明有按键按下 
	*
  *************************************************************************************************************************
  */

/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef _BSP_KEY_H
#define _BSP_KEY_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "stm32f10x.h"

#define				B1								0x01
#define				B2								0x02
#define				B3								0x04
#define				B4								0x08

#define				KEY_STATE_GET			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) << 0 | \
																GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) << 1 | \
																GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) << 2 | \
																GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) << 3;

extern uint8_t key_state;
extern uint8_t key_falling;
extern uint8_t key_rising;
/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
void Key_Init(void);
void Key_refresh(void);


#endif /*_BSP_KEY_H*/

/*****************************************************END OF FILE*********************************************************/	

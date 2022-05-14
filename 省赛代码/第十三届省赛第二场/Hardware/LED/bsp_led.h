/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef _BSP_LED_H
#define _BSP_LED_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "stm32f10x.h"

/* ���� -----------------------------------------------------------------------------------------------------------------*/
/* LED_GPIO�˿� */

#define LED_ALL (GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15)

#define BEEP_ON()	GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define BEEP_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)

#define LED1 GPIO_Pin_8
#define LED2 GPIO_Pin_9
#define LED3 GPIO_Pin_10
#define LED4 GPIO_Pin_11
#define LED5 GPIO_Pin_12
#define LED6 GPIO_Pin_13
#define LED7 GPIO_Pin_14
#define LED8 GPIO_Pin_15

/* LED����궨�� */
#define ON  1
#define OFF 0

/* �ṩ������C�ļ����õĺ��� --------------------------------------------------------------------------------------------*/
void LED_Init(void);                  /* LED��ʼ������ */
void LED_Control(u16 led, u8 state);  /*  LED���ƺ���  */  
void LED_Toggle(uint16_t LEDx);
void LED_ALL_OFF(void);             	/*  LEDȫ����  */

void Buzzer_Init(void);

#endif   /*_BSP_LED_H*/

/*****************************************************END OF FILE*********************************************************/	

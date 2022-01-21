/**
  *************************************************************************************************************************
  * @file    digital.c
  * @author  LiuhHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   蓝桥杯嵌入式扩展板数码管模块
  *************************************************************************************************************************
  * @attention
  *
  * 跳线配置:
	*   P3:1 2 3(SER RCK SCK)
  *      | | |
	*   P4:1 2 3
	*
	*   因为数码管使用的引脚与串口2的引脚重复。所以串口2不能使用
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_digital.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
//共阴极段码表       0    1   2     3    4    5    6    7    8    9    a    b   c    d     e    f    灭 
uint8_t Seg[17] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x4f,0x79,0x78,0x00};


	
	
/**
 * @brief  数码管对应引脚初始化
 * @param  无 
 * @retval 无
 */	
void Seg_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief  数码管显示函数
 * @param  bitx:第x位数码管
 * @retval 无
 */
void Seg_Display(uint8_t bit1,uint8_t bit2,uint8_t bit3)
{
	uint8_t i;
	uint8_t seg_temp;
	seg_temp=Seg[bit3];//看数码管原理图,先显示第三位数码管
	for(i=0;i<8;i++)
	{
		if(seg_temp & 0x80)//取8位数据的最高位,判断是1 还是0 ,再存数据进芯片74ls595
		{
			SER_H;//SER制高
		}
		else
		{
			SER_L;//SER制低
		}
		SCK_H;//SCLK引脚制高，同意数据移位进入芯片74ls595
		seg_temp=seg_temp<<1;//数据左移一位
		SCK_L;//SCLK引脚制低
	}

	seg_temp=Seg[bit2];//显示第二位数码管				
	for(i=0;i<8;i++)
	{
		if(seg_temp & 0x80)
		{
			SER_H;
		}
		else
		{
			SER_L;
		}
		SCK_H;
		seg_temp=seg_temp<<1;
		SCK_L;
	}

	seg_temp=Seg[bit1];//显示第一位数码管	
	for(i=0;i<8;i++)
	{
		if(seg_temp & 0x80)
		{
			SER_H;
		}
		else
		{
			SER_L;
		}
		SCK_H;
		seg_temp=seg_temp<<1;
		SCK_L;
	}
	RCK_H;//同意锁存的数据传到数码管
	RCK_L;
}

/*****************************************************END OF FILE*********************************************************/	
  

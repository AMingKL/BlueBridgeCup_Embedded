/**
  *************************************************************************************************************************
  * @file    digital.c
  * @author  LiuhHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   ���ű�Ƕ��ʽ��չ�������ģ��
  *************************************************************************************************************************
  * @attention
  *
  * ��������:
	*   P3:1 2 3(SER RCK SCK)
  *      | | |
	*   P4:1 2 3
	*
	*   ��Ϊ�����ʹ�õ������봮��2�������ظ������Դ���2����ʹ��
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_digital.h"
/* ���� -----------------------------------------------------------------------------------------------------------------*/
//�����������       0    1   2     3    4    5    6    7    8    9    a    b   c    d     e    f    �� 
uint8_t Seg[17] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x4f,0x79,0x78,0x00};


	
	
/**
 * @brief  ����ܶ�Ӧ���ų�ʼ��
 * @param  �� 
 * @retval ��
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
 * @brief  �������ʾ����
 * @param  bitx:��xλ�����
 * @retval ��
 */
void Seg_Display(uint8_t bit1,uint8_t bit2,uint8_t bit3)
{
	uint8_t i;
	uint8_t seg_temp;
	seg_temp=Seg[bit3];//�������ԭ��ͼ,����ʾ����λ�����
	for(i=0;i<8;i++)
	{
		if(seg_temp & 0x80)//ȡ8λ���ݵ����λ,�ж���1 ����0 ,�ٴ����ݽ�оƬ74ls595
		{
			SER_H;//SER�Ƹ�
		}
		else
		{
			SER_L;//SER�Ƶ�
		}
		SCK_H;//SCLK�����Ƹߣ�ͬ��������λ����оƬ74ls595
		seg_temp=seg_temp<<1;//��������һλ
		SCK_L;//SCLK�����Ƶ�
	}

	seg_temp=Seg[bit2];//��ʾ�ڶ�λ�����				
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

	seg_temp=Seg[bit1];//��ʾ��һλ�����	
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
	RCK_H;//ͬ����������ݴ��������
	RCK_L;
}

/*****************************************************END OF FILE*********************************************************/	
  

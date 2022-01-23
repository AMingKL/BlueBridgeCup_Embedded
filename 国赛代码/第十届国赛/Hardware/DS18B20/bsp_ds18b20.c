/**
  *************************************************************************************************************************
  * @file    ds18b20.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   蓝桥杯嵌入式扩展板ds18b20模块
  *************************************************************************************************************************
  * @attention
  *
  * 	跳线配置:
  * 		P4 6
  * 		   |
  * 		P3 6(TDQ)
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_ds18b20.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
#define delay_us(X)  delay((X)*72/5)
/**
 * @brief  
 * @param  无 
 * @retval 无
 */
void delay(unsigned int n)
{
	while(n--);
}

void DS18B20_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	/* Enable  clock */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  , ENABLE);
  
  	/* Configure Ports */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//
void mode_input1(void )
{
  	GPIO_InitTypeDef GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void mode_output1(void )
{
  	GPIO_InitTypeDef GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//
uint8_t ow_reset(void)
{ 
	uint8_t err;

   	OW_DIR_OUT(); // pull OW-Pin low for 480us
   	OW_OUT_LOW(); // disable internal pull-up (maybe on from parasite)

   	delay_us(400);	  //about 480us
   
   	// set Pin as input - wait for clients to pull low
   	OW_DIR_IN(); // input

   	delay_us(66);
   	err = OW_GET_IN();		// no presence detect
   	// nobody pulled to low, still high


   	// after a delay the clients should release the line
  	// and input-pin gets back to high due to pull-up-resistor
   	delay_us(480-66);
   	if( OW_GET_IN() == 0 )		// short circuit
      err = 1;

   	return err;
 }

uint8_t ow_bit_io( uint8_t b )
{ 
 	OW_DIR_OUT(); // drive bus low
 	OW_OUT_LOW();	
 	delay_us(1); // Recovery-Time wuffwuff was 1

 	if ( b ) OW_DIR_IN(); // if bit is 1 set bus high (by ext. pull-up)
	
#define  OW_CONF_DELAYOFFSET  5
 	delay_us(15-1-OW_CONF_DELAYOFFSET);
      
 	if( OW_GET_IN() == 0 ) b = 0;  // sample at end of read-timeslot
	
 	delay_us(60-15);
 	OW_DIR_IN();

 	return b;
}

uint8_t ow_byte_wr( uint8_t b )
{ 	
	uint8_t i = 8, j;
   	do 
    { 
		j = ow_bit_io( b & 1 );
      	b >>= 1;
      	if( j ) b |= 0x80;
    } 
   	while( --i );
   	return b;
}

//
uint8_t ow_byte_rd( void )
{
   	return ow_byte_wr( 0xFF ); 
}
float DS18B20_Read(void)  
{
	u8 i;
	s16 x;
	u8 val[2];
	u8 s;
	float temp;
	

	ow_reset();
	ow_byte_wr(OW_SKIP_ROM);
	ow_byte_wr(DS18B20_CONVERT);
//	delay_us(750000);

	ow_reset();
	ow_byte_wr(OW_SKIP_ROM);
	ow_byte_wr(DS18B20_READ);

	for(i=0;i<2;i++)
	{
		val[i]=ow_byte_rd();
	}

	if(val[1]>7)    //零下温度处理
	{
		val[0]=~val[0];
		val[1]=~val[1];
		s=1;
	}
	else
	{
		s=0;
	}

	x=val[1];
	x=x<<8;
	x=x | val[0];
	//x=x&0x7ff;
	temp=(float)x/16.0;   

	if(s)
	{
		return -temp;
	}
	else
	{
		return temp;
	}
}


/*****************************************************END OF FILE*********************************************************/	











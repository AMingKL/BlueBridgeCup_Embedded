/**
  *************************************************************************************************************************
  * @file    led.c
  * @author  刘洪明
  * @version V1.0
  * @date    2021-12-28
  * @brief   蓝桥嵌入式开发板按键配置
  *************************************************************************************************************************
  * @attention
  *
  * 
  *
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_key.h"
#include  "bsp_led.h"

/* 定义 -----------------------------------------------------------------------------------------------------------------*/
/* 按键处理相关变量 */
static uint8_t Key_Trg_Falling;//按键下降沿
static uint8_t Key_Trg_Rising;//按键上升沿
static uint8_t key_State;//按键状态，未按下为0，key1按下0x01，key2按下0x02，key3按下0x04，key4按下0x08.
static uint16_t key_time = 0;//判断按键当前状态-1.未按下 2.短按 3.长按
static uint16_t key_pressed_time = 0;//按键按下时间
static uint16_t key1Time = 0;//判断KEY1按键是否双击，记时。
static uint8_t  key1PressNumber = 0;//判断KEY1按键单双击变量

/**
 * @brief  按键初始化
 * @param  无
 * @retval 无
 */
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief  按键扫描
 * @param  	// 情况1：key1按键第一次按下，此时key_State，Key_Trg = 0x01
	          // 情况2：key1按键持续按下，第二次扫描的时候，此时key_State = 0x01，Key_Trg = 0
	          // 情况3：如果没有按键按下，则key_Value = 0，Key_Trg = 0
	          // 由以上的分析可以看出，Key_Trg只有在按键的下降沿那一时刻有值。
 * @retval key_State:键值
 */
void Key_Read(void)
{
	uint8_t key_Value = 0xF0;
	key_Value |= GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) | (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) << 1) | \
					    (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) << 2) | (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) << 3);
	key_Value = key_Value ^ 0xff;		// key1按下，key_state = 0x01,如果没有按键按下，则key_Value = 0
	Key_Trg_Rising = (!key_Value) * (key_Value^key_State); // 检测按键的下降沿	
	Key_Trg_Falling = key_Value & (key_Value^key_State); // 检测按键的上降沿
	key_State = key_Value;//key_State即保存的是扫描过后的按键的值
}
/**
 * @brief  按键处理
 * @param  无
 * @retval 在主函数中使用定时器每10ms扫描该函数
 */
void Key_Scan(void)
{
	Key_Read();
	
	if(key1PressNumber == 1)	// 检测到一次按键的上升沿，开始计时
	{
		key1Time += 10;			// 由于按键程序是10ms扫描一次，所以这里加10ms
		if(key1Time >= 150)	// 如果超过1500ms，表示按键是单击，初始化双击检测
		{
			// begin
			// 在这里添加按键单击的代码
			// end
			key1Time = 0;
			key1PressNumber = 0;
		}
	}
	
	if(Key_Trg_Rising == 0x01)	// 按键的上升沿，即按键松开
	{
		if(key1PressNumber == 0)	// 表示按键是第一次按键的上升沿
		{
			key1PressNumber = 1;		// 做个标记
		}
	}
	
	if(Key_Trg_Falling == 0x01)	// 检测到按键的下降沿
	{
		LED_Control(LED1,ON);		
		if(key1PressNumber == 1)	// 如果已经捕获到了一次按键的上升沿，并且现在还没有超时，那么这就是一个双击的动作
		{
			// begin
			// （在这里添加双击需要处理的程序）
			LED_Control(LED5,ON);
			// end
			key1Time = 0;
			key1PressNumber = 0;		// 初始化按键双击相关的变量
		}		
	}
	else if(Key_Trg_Falling == 0x02)
	{
		LED_Control(LED2,ON);
	}
	else if(Key_Trg_Falling == 0x04)
	{
		LED_Control(LED3,ON);
	}
	else if(Key_Trg_Falling == 0x08)
	{
		LED_Control(LED4,ON);
	}
	
	if(key_State == 0x01)
	{
		key_pressed_time+=10;
		if(key_time != 0xffff && ++key_time >= 50)
		{
			key_time = 0xffff;			
		}
	}
	
	if(key_State == 0 && Key_Trg_Falling == 0)	// 按键松开
	{
		key_pressed_time = 0;
		key_time = 0;// 防止重复进入次程序，在松开按键的时候，将key_time置零	
		LED_Control(LED_ALL,OFF);
	}
}

/*****************************************************END OF FILE*********************************************************/	

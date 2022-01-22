/**
  *************************************************************************************************************************
  * @file    adc_key.c
  * @author  LiuHongMing
  * @version V1.0
  * @date    2022-01-20
  * @brief   蓝桥杯嵌入式扩展板AD按键模块
  *************************************************************************************************************************
  * @attention
  *扩展版AD按键函数  PA5  - ADC1  CH5
  * 
	* 程序说明：    按键松开 -- 3.3V   |4095
  *	  
  *  
  *************************************************************************************************************************
  */

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "bsp_adc_key.h"
/* 定义 -----------------------------------------------------------------------------------------------------------------*/
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL]; 
uint8_t   keyCheck = 0;
uint8_t   keyState = 0;
uint16_t  last_keyValue = 0xff;//上一次的键值
uint32_t  keyLongCheck = 0;
uint16_t  Key_LongTime = 0;
uint8_t   keyCountTime = 0;
uint16_t   DoubleClickTime=0;

KEY_TypeDef Key = {0xff,FALSE,FALSE,FALSE,FALSE}; 
/**
 * @brief        按键扫描函数
 * @attention  根据电阻分压，读取到不同的电压值对应各个按键
 * @retval 无
 */
uint8_t Adc_Key_Read(void)  //判断是哪个按键按下
{
	uint16_t temp = 0;
	temp = Get_AnalogVoltage(VRP6);
	if(temp <= 80){//实测 0x0000—0x0001  0-1之间
		return 1;
	}else if((temp >= 100) && (temp <= 300)){//实测 0x00CD—0x00CE  205-206之间
		return 2; 
	}else if((temp >= 500) && (temp <= 700)){//实测 0x0264—0x0265  612-613之间
		return 3;
	}else if((temp >= 900) && (temp <= 1200)){//实测 0x03FF—0x0400  1023-1024之间
		return 4;
	}else if((temp >= 1300) && (temp <= 1500)){//实测 0x0580—0x0584  1408-1412之间
		return 5;
	}else if((temp >= 1700) && (temp <= 1900)){//实测 0x0744—0x0749  1860-1865之间
		return 6;
	}else if((temp >= 2200) && (temp <= 2400)){//实测 0x0919—0x091A  2329-2330之间
		return 7;
	}else if((temp >= 2700) && (temp <= 2900)){//实测 0x0B34—0x0B35  2868-2869之间
		return 8;
	}else
	{
		return 0;	//error status & no key  //4095		
	}
}


/**
  * @说明     按键状态机函数
  * @参数     none
  * @返回值   None
  * @注意   1.按键状态机只做出状态， 但不消除状态
            2.Key.Value ，Key.ShortPress 
              被使用后需要人为复位0xff,FALSE，这样才能保证每一次的状态都能被处理
            3.Key_Interrup()函数需要放入定时器中断，且中断时间小于10MS，最好为10的公约数：如1，2，5MS
**/
void Key_Interrup(void)      
{
	uint8_t keyPress = 0xff;
	keyCountTime ++;
	if(keyCountTime >= KEY_EliShaking_Time) //消抖完成
	{
		keyCountTime = 0;
		keyCheck = 1;
	}
	if(keyCheck == 1)
	{
		keyCheck = 0;
		keyPress = Adc_Key_Read();
		switch(keyState)
		{
			case 0://按键未按下态
				if(keyPress != 0xff)//表示有按键按下
				{
					last_keyValue = keyPress; //记录当前按键状态
					keyState = 1;
				}
				else
				{
					keyState = 0;
				}
			break;
			case 1://表示有按键按下，判断当前值和上一次的值是否一样，若不一样则为抖动！
				if(keyPress == last_keyValue)//不是抖动
				{
					keyState = 2;
				}else
				{
					keyState = 0; //是抖动！返回上一层
					last_keyValue = 0xff;
				}	 
				break;
			case 2:
				if(keyPress != last_keyValue)//表示按键已松开，只有长按状态被清除，长按计时结束，短按状态不清除，等待用户使用并清除
				{		
					if(keyLongCheck < KEY_LONGSTATUS_Time)//短按
					{
						Key.ShortPress = TRUE;						
						Key.Value = last_keyValue;
					}
					Key.LongPress = FALSE;
					last_keyValue = 0xff;
					keyState = 0;// 按键结束返回初始状态机
					keyLongCheck = 0;//长按计时清0
					Key_LongTime = 0;
				}
				else
				{					
					keyLongCheck++;
					if(keyLongCheck >= KEY_LONGSTATUS_Time)//按下时间超过 长按时间 ,可以直接累加，因为超过长按时间之后每隔扫描时间（10MS）就可以返回一次按键值，而且按键值用过后会清除
					{
						keyLongCheck = KEY_LONGSTATUS_Time + 1;
	
						Key.Value = last_keyValue;//返回值标记哪个按键，在调用后清除，所以可以直接做到累加功能
						Key.ShortPress = FALSE;
						Key.LongPress = TRUE;						
					}
				}
				break;
			default : break;
		}
	}
	if(Key.LongPress == TRUE)//按键长按刷新，进行长按快速累减或累加使用，人为设定多久累加一次
	{
		Key_LongTime ++;
		if(Key_LongTime >= Key_LongTimeSet)
		{
			Key_LongTime = 0;
			Key.LongFresh = TRUE;
		}
	}
}

/*****************************************************END OF FILE*********************************************************/	

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include  "usart.h"
#include  "stdio.h"

/* 定义 -----------------------------------------------------------------------------------------------------------------*/
/**
 * @brief  串口初始化函数
 * @param  无
 * @retval 无
 */
void Usart2_Init(void)  
{
	GPIO_InitTypeDef   GPIO_InitStructure;	
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	// 允许 GPIOA 复用 和 USART2 时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
	
	// PA2-TX2 复用推挽输出
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;        
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	// PA3-TX2 浮空输入（复位状态，可以省略）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
	// 初始化 USART2（波特率 BAUDRATE可调，默认 8 个数据位， 1 个停止位，无校验，允许 Rx 和 Tx）
	USART_InitStructure.USART_BaudRate   = BAUDRATE; 						
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;		
	USART_InitStructure.USART_Parity     = USART_Parity_No;	 		
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_Init(USART2, &USART_InitStructure);
	
	// 允许 USART2 使能
	USART_Cmd(USART2, ENABLE);
	
	// 允许USART2接收中断，空闲中断
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	
	
	// 允许 NVIC USART2 中断
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;        
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		    
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);	
}

/**
 * @brief  串口发送字符
 * @param  dat：要发送的字符
 * @retval 无
 */
void USART_SendChar(unsigned char dat)
{
	while (!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
	
	USART_SendData(USART2, dat);
	
}	

/**
 * @brief  串口发送字符串
 * @param  *str：要发送的数组的首地址
 * @retval 无
 */
void USART_SendString(int8_t *str)  
{
	uint8_t i;
	for (i = 0; i < 20; i++)
	{
		USART_SendData(USART2, str[i]);
		
		/* 等待发送完成 */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == 0); 
	} 
}



///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(USART2, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/*****************************************************END OF FILE*********************************************************/	



/* Includes -------------------------------------------------------------------------------------------------------------*/
#include  "usart.h"
#include  "stdio.h"

/* ���� -----------------------------------------------------------------------------------------------------------------*/
/**
 * @brief  ���ڳ�ʼ������
 * @param  ��
 * @retval ��
 */
void Usart2_Init(void)  
{
	GPIO_InitTypeDef   GPIO_InitStructure;	
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	// ���� GPIOA ���� �� USART2 ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
	
	// PA2-TX2 �����������
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;        
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	// PA3-TX2 �������루��λ״̬������ʡ�ԣ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
	// ��ʼ�� USART2�������� BAUDRATE�ɵ���Ĭ�� 8 ������λ�� 1 ��ֹͣλ����У�飬���� Rx �� Tx��
	USART_InitStructure.USART_BaudRate   = BAUDRATE; 						
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;		
	USART_InitStructure.USART_Parity     = USART_Parity_No;	 		
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_Init(USART2, &USART_InitStructure);
	
	// ���� USART2 ʹ��
	USART_Cmd(USART2, ENABLE);
	
	// ����USART2�����жϣ������ж�
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	
	
	// ���� NVIC USART2 �ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;        
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		    
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						
	NVIC_Init(&NVIC_InitStructure);	
}

/**
 * @brief  ���ڷ����ַ�
 * @param  dat��Ҫ���͵��ַ�
 * @retval ��
 */
void USART_SendChar(unsigned char dat)
{
	while (!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
	
	USART_SendData(USART2, dat);
	
}	

/**
 * @brief  ���ڷ����ַ���
 * @param  *str��Ҫ���͵�������׵�ַ
 * @retval ��
 */
void USART_SendString(int8_t *str)  
{
	uint8_t i;
	for (i = 0; i < 20; i++)
	{
		USART_SendData(USART2, str[i]);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == 0); 
	} 
}



///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART2, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/*****************************************************END OF FILE*********************************************************/	



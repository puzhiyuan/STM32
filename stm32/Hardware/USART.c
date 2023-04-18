#include "stm32f10x.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "delay.h"

char Usart_RxPacket[100];				//"@MSG\r\n"
uint8_t Usart_RxFlag;


/**
 * @description: 初始化串口1
 * @param {unsigned int} baud 波特率
 * @return {*}
 */
void Usart1_Init(unsigned int baud)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//PA9	TXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//PA10	RXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//接收和发送
	USART_InitStructure.USART_Parity = USART_Parity_No;									//无校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//8位数据位
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能接收中断
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
    USART_Cmd(USART1, ENABLE);//使能串口
}


/**
 * @description: 初始化串口2
 * @param {unsigned int} baud 波特率
 * @return {*}
 */
void Usart2_Init(unsigned int baud)
{
	GPIO_InitTypeDef gpioInitStruct;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//PA2	TXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_2;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInitStruct);
	
	//PA3	RXD
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInitStruct.GPIO_Pin = GPIO_Pin_3;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInitStruct);
	
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//接收和发送
	USART_InitStructure.USART_Parity = USART_Parity_No;									//无校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//8位数据位
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//使能接收中断
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	USART_Cmd(USART2, ENABLE);//使能串口
}



/**
 * @description: 串口发送字节
 * @param {USART_TypeDef} *USARTx 串口号
 * @param {uint8_t} Byte 字节数据
 * @return {*}
 */
void Usart_SendByte(USART_TypeDef *USARTx, uint8_t Byte)
{
    Usart_SendByte(USARTx, Byte);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

/**
 * @description: 
 * @param {USART_TypeDef} *USARTx
 * @param {uint8_t} *Array
 * @param {uint16_t} Length
 * @return {*}
 */
void Usart_SendArray(USART_TypeDef *USARTx, uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Usart_SendByte(USARTx, Array[i]);
	}
}

/**
 * @description: 串口发送字符串
 * @param {USART_TypeDef} *USARTx 串口号
 * @param {unsigned char} *str 需要发送的数据
 * @param {unsigned short} len 数据长度
 * @return {*}
 */
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{
	unsigned short count = 0;
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}

}

/**
 * @description: 格式化打印输出
 * @param {USART_TypeDef} *USARTx 串口号
 * @param {char} *fmt
 * @return {*}
 */
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{
	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);	//格式化
	va_end(ap);
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}

}

/**
 * @description: 串口1收发中断
 * @return {*}
 */
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		
		if (RxState == 0)
		{
			if (RxData == '@' && Usart_RxFlag == 0)
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)
		{
			if (RxData == '\r')
			{
				RxState = 2;
			}
			else
			{
				Usart_RxPacket[pRxPacket] = RxData;
				pRxPacket ++;
			}
		}
		else if (RxState == 2)
		{
			if (RxData == '\n')
			{
				RxState = 0;
				Usart_RxPacket[pRxPacket] = '\0';
				Usart_RxFlag = 1;
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}

}

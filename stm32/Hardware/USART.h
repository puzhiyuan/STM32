#ifndef _USART_H_
#define _USART_H_


//usart1设置为串口调试
#define USART_DEBUG		USART1
void Usart1_Init(unsigned int baud);
void Usart2_Init(unsigned int baud);
void Usart_SendByte(USART_TypeDef *USARTx, uint8_t Byte);
void Usart_SendArray(USART_TypeDef *USARTx, uint8_t *Array, uint16_t Length);
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

#endif

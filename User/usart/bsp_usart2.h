#ifndef __USART2_H
#define	__USART2_H


#include "stm32f4xx.h"
#include <stdio.h>

#define		UART2_RX_LEN       100
#define		UART2_TX_LEN       100

extern uint8_t Uart2_Rx_Buf[UART2_RX_LEN];
extern uint8_t Uart2_Tx_Buf[UART2_TX_LEN];
extern uint8_t Uart2_Rx_Data_Len;
extern uint8_t Uart2_Tx_Data_Len;
extern uint8_t Uart2_Rx_F;

/**************************USART参数定义********************************/
#define             macUSARTx                                USART2
#define             macUSART_BAUD_RATE                       115200
#define             macUSART_xClock_FUN                      RCC_APB1PeriphClockCmd
#define             macUSART_CLK                             RCC_APB1Periph_USART2

#define             macUSART_GPIO_xClock_FUN                 RCC_AHB1PeriphClockCmd
#define             macUSART_GPIO_CLK                        RCC_AHB1Periph_GPIOA     
#define             macUSART_TX_PORT                         GPIOA   
#define             macUSART_TX_PIN                          GPIO_Pin_2
#define             macUSART_TX_AF                           GPIO_AF_USART2
#define             macUSART_TX_SOURCE                       GPIO_PinSource2
#define             macUSART_RX_PORT                         GPIOA 
#define             macUSART_RX_PIN                          GPIO_Pin_3
#define             macUSART_RX_AF                           GPIO_AF_USART2
#define             macUSART_RX_SOURCE                       GPIO_PinSource3

#define             macUSART_IRQ                             USART2_IRQn
#define             macUSART_INT_FUN                         USART2_IRQHandler



void	USART2_Config	( u32 band );
void	Uart2_Process	(void);


#endif /* __USART2_H */


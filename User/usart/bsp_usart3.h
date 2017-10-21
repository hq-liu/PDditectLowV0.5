#ifndef __USART3_H
#define	__USART3_H


#include "stm32f4xx.h"
#include <stdio.h>

#define		UART3_RX_LEN       100
#define		UART3_TX_LEN       100

extern uint8_t Uart3_Rx_Buf[UART3_RX_LEN];
extern uint8_t Uart3_Tx_Buf[UART3_TX_LEN];
extern uint8_t Uart3_Rx_Data_Len;
extern uint8_t Uart3_Tx_Data_Len;
extern uint8_t Uart3_Rx_F;

void	USART3_Config	(u32 baud );
void USART3_SStr(uint8_t *SData,uint8_t S_Len);
void receive_command(void);

#endif /* __USART3_H */


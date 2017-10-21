#ifndef __USART1_H
#define	__USART1_H


#include "stm32f4xx.h"
#include <stdio.h>



void USART1_SendByte(u8 ch);
void	USART1_Config	(u32 band);
void	Uart1_Process	(void);
void USART1_Sendword(u16 ch);

#endif /* __USART2_H */


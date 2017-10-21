/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   usart1应用bsp  波特率升级为10Mb 用于板间传输数据
  ******************************************************************************
  * @attention
  *
  * 实验平台:正点原子阿波罗f429 开发板 
  * 说明，为使得波特率支持 10Mb，需要更改文件 stm32f4xx_usart.h中，Line 354
  * 			的波特率验证代码
  *				增加调用8位过采样率设置函数
  ******************************************************************************
  */
  
#include "global.h"

// 配置USART2接收中断
static void NVIC_USART1_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */

void USART1_Config(u32 band)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);	
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	/* USART1 mode config */
	USART_OverSampling8Cmd(USART1 , ENABLE); 
	USART_InitStructure.USART_BaudRate = band;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	NVIC_USART1_Configuration();
	
	/* 使能串口中断*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启接收中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); //开启空闲中断
	
	USART_Cmd(USART1, ENABLE);
}

void USART1_SendByte(u8 ch)
{
	USART1->DR = (uint16_t) ch;
}
void USART1_Sendword(u16 ch)
{	
	USART1->DR = ((uint16_t) ch >> 8) & 0x0FF;
	while ((USART1->SR & USART_FLAG_TXE) == 0);
	USART1->DR = (uint16_t) ch & 0x0FF;
	while ((USART1->SR & USART_FLAG_TXE) == 0);
}

///*********************************************END OF FILE**********************/


//串口1中断服务函数
void USART1_IRQHandler(void)
{

}
/*********************************************END OF FILE**********************/



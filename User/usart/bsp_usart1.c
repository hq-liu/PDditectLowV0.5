/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   usart1Ӧ��bsp  ����������Ϊ10Mb ���ڰ�䴫������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����ԭ�Ӱ�����f429 ������ 
  * ˵����Ϊʹ�ò�����֧�� 10Mb����Ҫ�����ļ� stm32f4xx_usart.h�У�Line 354
  * 			�Ĳ�������֤����
  *				���ӵ���8λ�����������ú���
  ******************************************************************************
  */
  
#include "global.h"

// ����USART2�����ж�
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
  * @brief  USART1 GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
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
	
	/* ʹ�ܴ����ж�*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //���������ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); //���������ж�
	
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


//����1�жϷ�����
void USART1_IRQHandler(void)
{

}
/*********************************************END OF FILE**********************/



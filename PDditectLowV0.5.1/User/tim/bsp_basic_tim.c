/**
  ******************************************************************************
  * @file    bsp_basic_tim.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   ���ļ���ʼ���˻�����ʱ��TIM6������ʱ�䴥��
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
#include "global.h"


	/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */	
void  TIM6_DAC_IRQHandler (void)  //ʱ�䴥��
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		num=SPI_ADS7946_READ();
		//printf("%d ",num);	
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//		USART_SendData(USART1,0x55);
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//		USART_SendData(USART1,(num&0XFF00)>>8);//��ǰ���ݵĸ߰�λ
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//		USART_SendData(USART1,num&0XFF);//��ǰ���ݵĵͰ�λ
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//		USART_SendData(USART1,0x55);
		TIM6->SR = ~0x01;		
	}		 	
}


 /**
  * @brief  ������ʱ�� TIMx,x[6,7]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
void TIM6_Mode_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	 NVIC_InitStructure.NVIC_IRQChannel =TIM6_DAC_IRQn; 	
		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;	 
	  // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 500-1;       	
	//��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
  //				PCLK1 = HCLK / 4 
  //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
    TIM_TimeBaseStructure.TIM_Prescaler = 9000-1;	
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6, ENABLE);	
}
/*********************************************END OF FILE**********************/

/**
  ******************************************************************************
  * @file    bsp_basic_io.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   ���ļ���ʼ���˵ײ�����õ���GPIO����
						 PB1����LED�ƣ�������ʾ��ǰ��ʱ�䴥������λ�ƴ���
						 PB0�������ڽ��շ�ת�źţ��Ѿ�ȡ����
						 ���ڴ���ng�źţ�flag_ngΪ1ʱ��GPIOΪ1����ʱû�м��룩
						 ���ڽ���ԭ���źţ���ʱ�ð������棩
  ******************************************************************************
  * @attention ��

  ******************************************************************************
  */


#include "global.h"

void BasicIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
  
  
	/* ѡ�񰴼�1������ */ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  /* ��������Ϊ����ģʽ */ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    		
  /* �������Ų�����Ҳ������ */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  /* ʹ������Ľṹ���ʼ������ */
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0); //�����ⲿ�ж�
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	 NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  /* ��������Ϊ����ģʽ */ 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	    		
  /* �������Ų�����Ҳ������ */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  /* ʹ������Ľṹ���ʼ������ */
	GPIOB->BSRRL = GPIO_Pin_1 ;  
  GPIO_Init(GPIOB, &GPIO_InitStructure); 	
}

void EXTI0_IRQHandler(void)
{

if((EXTI->PR & EXTI_Line0) != (uint32_t)RESET)
{
	Turn=GPIOB->IDR & GPIO_Pin_0;
	
	EXTI->PR = EXTI_Line0;   //����жϱ�־λ  �Ĵ�����ʽ 
	//EXTI_ClearITPendingBit(EXTI_Line0);    //����жϱ�־λ  �⺯����ʽ
	
}
} 

/*********************************************END OF FILE**********************/

/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "global.h"


static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  
  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  /* ������ռ���ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* ���������ȼ���1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}

 /**
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
  
	/*��������GPIO�ڵ�ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH ,ENABLE);
  
  /* ʹ�� SYSCFG ʱ�� ��ʹ��GPIO�ⲿ�ж�ʱ����ʹ��SYSCFGʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* ���� NVIC */
  NVIC_Configuration();
  
	/* ѡ�񰴼�1������ */ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  /* ��������Ϊ����ģʽ */ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    		
  /* �������Ų�����Ҳ������ */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  /* ʹ������Ľṹ���ʼ������ */
  GPIO_Init(GPIOH, &GPIO_InitStructure); 

	/* ���� EXTI �ж�Դ ��key1���� */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOH,EXTI_PinSource13);

  /* ѡ�� EXTI �ж�Դ */
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  /* �ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  /* �½��ش��� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  /* ʹ���ж�/�¼��� */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

}

void EXTI15_10_IRQHandler(void)
{
  //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(EXTI_Line13) != RESET) 
	{		
		printf("encoder%d\n",TIM3->CNT);	
		EXTI_ClearITPendingBit(EXTI_Line13);     
	}  
}
/*********************************************END OF FILE**********************/

/**
  ******************************************************************************
  * @file    exti.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   ���ļ������˰����жϣ�����ģ��ԭ���źţ�֮��ȥ��
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#include "global.h"
								  

//�ⲿ�ж�0�������
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);	//����
//	if(WK_UP==1)	 
//	{
//				TIM3->CNT=0;
//				TIM4->CNT=0;
//				TIM5->CNT=0;
//	}		 
//	EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
//}	
////�ⲿ�ж�2�������
//void EXTI2_IRQHandler(void)
//{
//	delay_ms(10);	//����
//	if(KEY1==0)	  
//	{	 
//		LED1=!LED1; 			 
//	}		 
//	EXTI->PR=1<<2;  //���LINE2�ϵ��жϱ�־λ  
//}
////�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	delay_ms(10);	//����
	if(KEY0==0)	 
	{
		TIM4->CNT=0;
		//TIM3->CNT=0;
		TIM5->ARR=position;  //TIM���ֵ����ΪS1
		TIM5->CNT=10;
		Tim5State=0;
		TIM5->CR1 |= TIM_CR1_CEN;  //����TIM5
	}		 
	EXTI->PR=1<<3;  //���LINE3�ϵ��жϱ�־λ  
}
//�ⲿ�ж�10~15�������
//void EXTI15_10_IRQHandler(void)
//{
//	delay_ms(10);	//����
//	if(KEY2==0)	 
//	{		
//		LED0=!LED0;			
//	}		 
//	EXTI->PR=1<<13;  //���LINE13�ϵ��жϱ�־λ  
//}
//	   
//�ⲿ�жϳ�ʼ������
//��ʼ��PH2,PH3,PC13,PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
	KEY_Init(); 
	Ex_NVIC_Config(GPIO_C,13,FTIR);		//�½��ش���
	Ex_NVIC_Config(GPIO_H,2,FTIR); 		//�½��ش���
	Ex_NVIC_Config(GPIO_H,3,FTIR); 		//�½��ش���
 	Ex_NVIC_Config(GPIO_A,0,RTIR); 	 	//�����ش��� 
	MY_NVIC_Init(3,2,EXTI2_IRQn,2);		//��ռ3�������ȼ�2����2
	MY_NVIC_Init(2,2,EXTI3_IRQn,2);		//��ռ2�������ȼ�2����2	   
	MY_NVIC_Init(1,2,EXTI15_10_IRQn,2);	//��ռ1�������ȼ�2����2	   
	MY_NVIC_Init(0,2,EXTI0_IRQn,2);		//��ռ0�������ȼ�2����2	   
}













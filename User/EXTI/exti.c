/**
  ******************************************************************************
  * @file    exti.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   该文件配置了按键中断，用于模拟原点信号，之后将去掉
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#include "global.h"
								  

//外部中断0服务程序
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);	//消抖
//	if(WK_UP==1)	 
//	{
//				TIM3->CNT=0;
//				TIM4->CNT=0;
//				TIM5->CNT=0;
//	}		 
//	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
//}	
////外部中断2服务程序
//void EXTI2_IRQHandler(void)
//{
//	delay_ms(10);	//消抖
//	if(KEY1==0)	  
//	{	 
//		LED1=!LED1; 			 
//	}		 
//	EXTI->PR=1<<2;  //清除LINE2上的中断标志位  
//}
////外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(KEY0==0)	 
	{
		TIM4->CNT=0;
		//TIM3->CNT=0;
		TIM5->ARR=position;  //TIM溢出值设置为S1
		TIM5->CNT=10;
		Tim5State=0;
		TIM5->CR1 |= TIM_CR1_CEN;  //开启TIM5
	}		 
	EXTI->PR=1<<3;  //清除LINE3上的中断标志位  
}
//外部中断10~15服务程序
//void EXTI15_10_IRQHandler(void)
//{
//	delay_ms(10);	//消抖
//	if(KEY2==0)	 
//	{		
//		LED0=!LED0;			
//	}		 
//	EXTI->PR=1<<13;  //清除LINE13上的中断标志位  
//}
//	   
//外部中断初始化程序
//初始化PH2,PH3,PC13,PA0为中断输入.
void EXTIX_Init(void)
{
	KEY_Init(); 
	Ex_NVIC_Config(GPIO_C,13,FTIR);		//下降沿触发
	Ex_NVIC_Config(GPIO_H,2,FTIR); 		//下降沿触发
	Ex_NVIC_Config(GPIO_H,3,FTIR); 		//下降沿触发
 	Ex_NVIC_Config(GPIO_A,0,RTIR); 	 	//上升沿触发 
	MY_NVIC_Init(3,2,EXTI2_IRQn,2);		//抢占3，子优先级2，组2
	MY_NVIC_Init(2,2,EXTI3_IRQn,2);		//抢占2，子优先级2，组2	   
	MY_NVIC_Init(1,2,EXTI15_10_IRQn,2);	//抢占1，子优先级2，组2	   
	MY_NVIC_Init(0,2,EXTI0_IRQn,2);		//抢占0，子优先级2，组2	   
}













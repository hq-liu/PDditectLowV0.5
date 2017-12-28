/**
  ******************************************************************************
  * @file    bsp_basic_io.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   该文件初始化了底层板所用到的GPIO引脚
						 PB1——LED灯，用于显示当前是时间触发还是位移触发
						 PB0——用于接收反转信号（已经取消）
						 用于触发ng信号，flag_ng为1时该GPIO为1（暂时没有加入）
						 用于接收原点信号（暂时用按键代替）
  ******************************************************************************
  * @attention 无

  ******************************************************************************
  */


#include "global.h"

void BasicIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
  
  
	/* 选择按键1的引脚 */ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  /* 设置引脚为输入模式 */ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    		
  /* 设置引脚不上拉也不下拉 */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  /* 使用上面的结构体初始化按键 */
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0); //配置外部中断
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
  /* 设置引脚为输入模式 */ 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	    		
  /* 设置引脚不上拉也不下拉 */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  /* 使用上面的结构体初始化按键 */
	GPIOB->BSRRL = GPIO_Pin_1 ;  
  GPIO_Init(GPIOB, &GPIO_InitStructure); 	
}

void EXTI0_IRQHandler(void)
{

if((EXTI->PR & EXTI_Line0) != (uint32_t)RESET)
{
	Turn=GPIOB->IDR & GPIO_Pin_0;
	
	EXTI->PR = EXTI_Line0;   //清除中断标志位  寄存器方式 
	//EXTI_ClearITPendingBit(EXTI_Line0);    //清除中断标志位  库函数方式
	
}
} 

/*********************************************END OF FILE**********************/

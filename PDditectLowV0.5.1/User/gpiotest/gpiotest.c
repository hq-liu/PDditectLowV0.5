/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./gpiotest/gpiotest.h"   

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void GPIOTest_Config(void)
{		
		
		GPIO_InitTypeDef GPIO_InitStructure;/*定义一个GPIO_InitTypeDef类型的结构体*/
		
		RCC_AHB1PeriphClockCmd ( Test_GPIO_CLK1, ENABLE); /*开启LED相关的GPIO外设时钟*/

		GPIO_InitStructure.GPIO_Pin = TestGPIO_PIN1;	/*选择要控制的GPIO引脚*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   /*设置引脚模式为输出模式*/ 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*设置引脚的输出类型为推挽输出*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;/*设置引脚为上拉模式，默认LED亮*/
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 		/*设置引脚速率为50MHz */   
		
		GPIO_Init(TestGPIO_PORT1, &GPIO_InitStructure);		/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/    
}
/*********************************************END OF FILE**********************/

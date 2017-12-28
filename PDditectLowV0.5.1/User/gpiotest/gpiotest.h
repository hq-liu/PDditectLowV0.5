#ifndef __LED_H
#define	__LED_H

#include "stm32f4xx.h"

//引脚定义
/*******************************************************/
//R 红色灯
#define TestGPIO_PIN1              GPIO_Pin_12                 
#define TestGPIO_PORT1             GPIOH                      
#define Test_GPIO_CLK1             RCC_AHB1Periph_GPIOH

\
/** 控制LED灯亮灭的宏，
	* LED低电平亮，设置ON=0，OFF=1
	* 若LED高电平亮，把宏设置成ON=1 ，OFF=0 即可
	*/
#define ON  0
#define OFF 1

/* 带参宏，可以像内联函数一样使用 */
#define TestGPIO(a)	if (a)	\
					GPIO_SetBits(TestGPIO_PORT1,TestGPIO_PIN1);\
					else		\
					GPIO_ResetBits(TestGPIO_PORT1,TestGPIO_PIN1)

\
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			  //设置为高电平		
#define digitalLo(p,i)			{p->BSRRH=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态
//使用示例
//digitalToggle(TestGPIO_PORT1,TestGPIO_PIN1)




void GPIOTest_Config(void);

#endif /* __LED_H */

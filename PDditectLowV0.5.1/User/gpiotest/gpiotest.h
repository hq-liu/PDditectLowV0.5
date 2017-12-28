#ifndef __LED_H
#define	__LED_H

#include "stm32f4xx.h"

//���Ŷ���
/*******************************************************/
//R ��ɫ��
#define TestGPIO_PIN1              GPIO_Pin_12                 
#define TestGPIO_PORT1             GPIOH                      
#define Test_GPIO_CLK1             RCC_AHB1Periph_GPIOH

\
/** ����LED������ĺ꣬
	* LED�͵�ƽ��������ON=0��OFF=1
	* ��LED�ߵ�ƽ�����Ѻ����ó�ON=1 ��OFF=0 ����
	*/
#define ON  0
#define OFF 1

/* ���κ꣬��������������һ��ʹ�� */
#define TestGPIO(a)	if (a)	\
					GPIO_SetBits(TestGPIO_PORT1,TestGPIO_PIN1);\
					else		\
					GPIO_ResetBits(TestGPIO_PORT1,TestGPIO_PIN1)

\
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRRH=i;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬
//ʹ��ʾ��
//digitalToggle(TestGPIO_PORT1,TestGPIO_PIN1)




void GPIOTest_Config(void);

#endif /* __LED_H */

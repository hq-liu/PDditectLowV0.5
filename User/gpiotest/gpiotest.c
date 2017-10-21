/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./gpiotest/gpiotest.h"   

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void GPIOTest_Config(void)
{		
		
		GPIO_InitTypeDef GPIO_InitStructure;/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		
		RCC_AHB1PeriphClockCmd ( Test_GPIO_CLK1, ENABLE); /*����LED��ص�GPIO����ʱ��*/

		GPIO_InitStructure.GPIO_Pin = TestGPIO_PIN1;	/*ѡ��Ҫ���Ƶ�GPIO����*/	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   /*��������ģʽΪ���ģʽ*/ 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;/*��������Ϊ����ģʽ��Ĭ��LED��*/
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 		/*������������Ϊ50MHz */   
		
		GPIO_Init(TestGPIO_PORT1, &GPIO_InitStructure);		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/    
}
/*********************************************END OF FILE**********************/

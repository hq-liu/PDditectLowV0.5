/**
  ******************************************************************************
  * @file    main.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   先进行初始化工作，之后进入主循环
						 主函数主要做四步判断
						 1.判断是否有新的命令接收完成，如果有则调用receive_command()进行配置；没有则跳过
						 2.判断已经对比的点数是否小于接收的点数，如果是则进行对比；如果否则跳过等待新的数据到来
						 3.判断发送数组SendData中的数据是否小于已经对比的数据，如果是则填充SendData；如果否则跳过等待新的对比数据到来
						 4.判断SendData中已经发送的数据是否小于已经填充的数据，如果是则通过串口1发送数据；如果否则等待新的数据填充完成
  ******************************************************************************
  * @attention
  *	是否需要发送与对比主要通过以下指针完成：recive_num（初始值为-1，来一个数据+1）；
	* compared_num（初始值为0，对比完一个数据+1）；
	* DataPointer（初始值为0，每次增加的值为Gap）
	* cntt（初始值为0，每次填充完发送数组至少+2）
	* datasend（初始值为0，发送完一个数据+1）
  * 目前小规模测试中发现发数据应该是没有问题，但是没有细算，很可能漏发1、2位数据
  ******************************************************************************
  */
#include "global.h"

int main(void)
{
	//进行初始化
	Stm32_Clock_Init(320,25,2,8);
	delay_init(160);
	/* Configure the NVIC Preemption Priority Bits */ 
	//配置中断优先级组，整个程序仅需调用一次
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置中断优先级分组
	USART2_Config(115200);
	USART1_Config(256000);
	//USART1_Config(115200);
	USART3_Config(115200);
	SDRAM_Init();
	SPI1_Init1();
	//TIM6_Mode_Config();
	TIM4_Mode_Config(sampling);
	TIM3_Mode_Config(0xFFFF);
	TIM5_Mode_Config();
	TIM5->ARR=position;  //TIM5的ARR寄存器用于修改中断溢出值	
	//GPIOTest_Config();
	EXTIX_Init();
	BasicIO_Config();
	get_envelope_up (envelope_up,EnvelopeU_len);
	get_envelope_below (envelope_below,EnvelopeB_len); //初始化对比区间
	//TIM_Cmd(TIM6,ENABLE);
	Gap=saving/sampling;
	Data_Len=10000/Gap;  //获取要发送的数据长度 
		
  while(1)
	{
		 if(Uart3_Rx_F)  //如果有命令参数需要配置
		 {
			receive_command();  //配置命令参数
			Uart3_Rx_F = 0;
			Uart3_Rx_Data_Len=0;
		 }
			//开始对比
			if(compared_num < recive_num)//开始对比算法
			{
				if(( save_line[compared_num] > y_up[compared_num] ) || ( save_line[compared_num] < y_below[compared_num] )) //如果采集信号在包络线之外，则输出NG信号，跳出循环
				{
					flag_ng = 1; //记录下了recive_num值，recive_num值是x的值，用作后面的输出				
				}
				else{
					GPIOB->BSRRH = GPIO_Pin_0 ;  //触发NG信号
				}
				compared_num++;
			}
			
			if(DataPointer < compared_num)  //对比完开始填充需要发送的数组
			{
					if((save_line[DataPointer]&0xFF00)>>8 == 0x55) //低八位为0x55
					{
						SendData[cntt] = 0xAA;cntt++;
						SendData[cntt] = 0x5A;cntt++;
					}
					else if((save_line[DataPointer]&0xFF00)>>8 == 0xAA) //低八位为0xAA
					{
						SendData[cntt] = 0xAA;cntt++;
						SendData[cntt] = 0xA5;cntt++;
					}
					else
					{
						SendData[cntt] = (save_line[DataPointer]&0xFF00)>>8;cntt++;
					}
					
					if(((save_line[DataPointer]&0xFF)) == 0x55)//高八位为0x55
					{
						SendData[cntt] = 0xAA;cntt++;
						SendData[cntt] = 0x5A;cntt++;
					}
					else if(((save_line[DataPointer]&0xFF)) == 0xAA)//高八位为0x55
					{
						SendData[cntt] = 0xAA;cntt++;
						SendData[cntt] = 0xA5;cntt++;
					}
					else
					{
						SendData[cntt] = (save_line[DataPointer]&0xFF);cntt++;
					}
					DataPointer=DataPointer + Gap;
					//DataPointer=DataPointer + 30;
			}
			
			if(datasend < cntt)
				{
				if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
					{
						USART1->DR = SendData[datasend];
						USART2->DR = SendData[datasend];
						datasend++;
					}	
				}
			//printf("%d,",DataPointer);
 }
}
	

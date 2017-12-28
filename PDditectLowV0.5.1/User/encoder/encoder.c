/**
  ******************************************************************************
  * @file    encoder.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   该文件初始化了TIM3，TIM4，TIM5三个定时器
						 TIM4与TIM5的中断
						 TIM4每触发一次中断读取一次AD值（压力值），并将其存入真实数据数组
						 TIM5通过中断控制当前状态为时间触发还是位移触发
  ******************************************************************************
  * @attention 无

  ******************************************************************************
  */


#include "global.h"


//void TIM3_IRQHandler(void)
//{ 	
//	
//}
void TIM4_IRQHandler(void)
{ 
	if((TIM4->SR & 0x01) && (TIM4->DIER & 0x01))//溢出中断 
	{
		if(recive_num<=10000)
		{
				num=SPI_ADS7946_READ();
				//save_line[recive_num]=num;  //经一次中断save数组填充一位
				save_line[recive_num]=recive_num;
				recive_num++;  //记录采集点个数的参数加一,recive_num++相当于x值			
		}
  }
	TIM4->SR &= ~0x01;	
}
void TIM5_IRQHandler(void)
{ 
	if((TIM5->SR & 0x01) && (TIM5->DIER & 0x01))//溢出中断 
	{
		if(Tim5State==0)  //S1阶段进入S2阶段
		{
				//TIM_Cmd(TIM6,DISABLE);//关闭时间触发
				//TIM6->CR1 &= (uint16_t)~TIM_CR1_CEN;
				Tim5State=1;  //切换当前状态
				GPIOB->BSRRH = GPIO_Pin_1 ; //进入位移触发DS0点亮
				TIM5->ARR=depth;  //TIM溢出值设置为S2
				TIM5->CNT=0;
				TIM4->CR1 |= TIM_CR1_CEN;  //开启TIM4,准备位移触发采集
				TIM4->CNT=0;
				//处理数据头并发送	
				//数据代码，0x01为实时模式，0x02为单次模式
				HeadCnt=0;
				DataHead[HeadCnt]=0x55;HeadCnt++;
				if(WorkMode==0x01) {DataHead[HeadCnt]=0x01;HeadCnt++;}
				else if(WorkMode==0x03) {DataHead[HeadCnt]=0x02;HeadCnt++;} 
				//数据长度，根据DataLen得出，分为高八位和低八位发送
				if(((Data_Len&0XFF00)>>8)==0x55) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0x5A;HeadCnt++;}
				else if(((Data_Len&0XFF00)>>8)==0xAA) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0xA5;HeadCnt++;}
				else {DataHead[HeadCnt]=(Data_Len&0XFF00)>>8;HeadCnt++;}
				if((Data_Len&0XFF)==0x55) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0x5A;HeadCnt++;}
				else if((Data_Len&0XFF)==0xAA) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0xA5;HeadCnt++;}
				else {DataHead[HeadCnt]=Data_Len&0XFF;HeadCnt++;}
				
				//数据序号，根据SerialNumber得出
				SerialNumber++;
				if(((SerialNumber&0XFF00)>>8)==0x55) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0x5A;HeadCnt++;}
				else if(((SerialNumber&0XFF00)>>8)==0xAA) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0xA5;HeadCnt++;}
				else {DataHead[HeadCnt]=(SerialNumber&0XFF00)>>8;HeadCnt++;}
				if((SerialNumber&0XFF)==0x55) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0x5A;HeadCnt++;}
				else if((SerialNumber&0XFF)==0xAA) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0xA5;HeadCnt++;}
				else {DataHead[HeadCnt]=SerialNumber&0XFF;HeadCnt++;}
				
				for(s=0;s<HeadCnt;s++) //把DataHead数组的内容发送出去
				{
						while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);
						USART1->DR=DataHead[s];
				}
				send_num=1;
				datasend=0;   //初始化下一轮数据采集
				recive_num=0;  
				compared_num=0;
				DataPointer=0;
				cntt=0;
		}
		else if(Tim5State==1) //S2状态切换到S3状态
		{
				Tim5State=2;  //切换当前状态
				TIM4->CR1 &= (uint16_t)~TIM_CR1_CEN; //关闭位移触发
				GPIOB->BSRRL = GPIO_Pin_1 ;  //关闭LED
				TIM5->CR1 &= (uint16_t)~TIM_CR1_CEN; //关闭TIM5
				//处理数据尾并发送
				//发送数据类型，OK数据发送0x00,NG数据发送0x01,ALL数据发送0x02
				TailCnt=0;
				AccumulateCheck=0;
				if(!flag_ng) {DataTail[TailCnt]=0x00;TailCnt++;}
				else {DataTail[TailCnt]=0x01;TailCnt++;}
				for(s=0;s<HeadCnt;s++)
				 AccumulateCheck=AccumulateCheck+DataHead[s];
				for(s=0;s<cntt;s++)
					AccumulateCheck=AccumulateCheck+SendData[s];
				AccumulateCheck=AccumulateCheck+DataTail[0];
				if(AccumulateCheck==0x55){DataTail[TailCnt]=0xAA;TailCnt++;DataTail[TailCnt]=0x5A;TailCnt++;}
				else if(AccumulateCheck==0xAA){DataTail[TailCnt]=0xAA;TailCnt++;DataTail[TailCnt]=0xA5;TailCnt++;}
				else {DataTail[TailCnt]=AccumulateCheck;TailCnt++;}
				DataTail[TailCnt]=0x55;TailCnt++;
				for(s=0;s<TailCnt;s++)
				{
						while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);
						USART1->DR=DataTail[s];
				}
		}
  }
	TIM5->SR &= ~0x01;	
}

void TIM4_Mode_Config(u16 Period4)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	//PB0 ch1 ,PB1 ch2 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能TIM4时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	
	GPIO_StructInit(&GPIO_InitStructure);//将GPIO_InitStruct中的参数按缺省值输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//PA0 PA1浮空输入	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
		
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn; 	
	// 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_DeInit(TIM4);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = Period4-1;  //设定计数器重装值 
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3时钟预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//设置时钟分割 T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
                 
	/*----------编码器模式设置---------*/
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//使用编码器模式1，上升下降都计数
	TIM_ICStructInit(&TIM_ICInitStructure);//将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = 15;  //选择输入比较滤波器 
	TIM_ICInit(TIM4, &TIM_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM2
	
	//TIM_ARRPreloadConfig(TIM4, ENABLE);//使能预装载 2017.2.8 试验后发现此句应当注销 原因未查
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM2的更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//运行更新中断
	//Reset counter
	TIM4->CNT = 0;//
	//TIM_Cmd(TIM4, ENABLE);   //启动TIM3定时器 需要检测时才开启定时器

}
void TIM3_Mode_Config(u16 temp)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	//PA0 ch1 ,PA1 ch2 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能TIM4时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	
	GPIO_StructInit(&GPIO_InitStructure);//将GPIO_InitStruct中的参数按缺省值输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//PA0 PA1浮空输入	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);   
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);

	TIM_DeInit(TIM3);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = temp;  //设定计数器重装值 
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3时钟预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//设置时钟分割 T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              
                 
	/*----------编码器模式设置---------*/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//使用编码器模式1，上升下降都计数
	TIM_ICStructInit(&TIM_ICInitStructure);//将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = 14;  //选择输入比较滤波器 
	TIM_ICInit(TIM3, &TIM_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM3

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM2的更新标志位
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//运行更新中断
	TIM3->CNT = 0;//
	TIM_Cmd(TIM3, ENABLE);   //启动TIM3定时器
}

void TIM5_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//使能TIM5时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);//使能GPIOA时钟
	
	GPIO_StructInit(&GPIO_InitStructure);//将GPIO_InitStruct中的参数按缺省值输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOH, &GPIO_InitStructure);   
	
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource10,GPIO_AF_TIM5); //TIM5 CH1
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource11,GPIO_AF_TIM5);  //TIM5 CH2 
		
		// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn; 	
	// 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	// 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
 	

	TIM_DeInit(TIM5);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  //设定计数器重装值 
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM5时钟预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//设置时钟分割 T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数 
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);              
                 
	/*----------编码器模式设置---------*/
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);//将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = 0;  //选择输入比较滤波器 
	TIM_ICInit(TIM5, &TIM_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM2
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);//清除TIM2的更新标志位
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);//运行更新中断
	//Reset counter
	TIM5->CNT = 10;//
	TIM_Cmd(TIM5, ENABLE);   //启动TIM2定时器

}

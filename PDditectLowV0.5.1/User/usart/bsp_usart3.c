
#include "global.h"
#include "./usart/bsp_usart3.h"
#include "crc.h"


uint8_t Uart3_Rx_Buf[UART3_RX_LEN];
uint8_t Uart3_Tx_Buf[UART3_TX_LEN];
uint8_t Uart3_Rx_Data_Len = 0;
uint8_t Uart3_Tx_Data_Len = 0;
uint8_t Uart3_Rx_F = 0;
u8 Uart3_SendCurP = 0;

static void SendAll(void) //接收到0x24命令，将sive_line全部数据发送到上位机
{
		u16 cnt1 = 0;
		u16 cnt2 = 0;  //计数变量
	
		while(cnt2 < 10000)
		{
			if(!(cnt1 & 0x01)) //cnt为偶数发送低八位
			{
				if((save_line[cnt2] & 0XFF) == 0x55)  //低八位为0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = 0xAA ;
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = 0x5A ;
							cnt1++;
						}
				}
				else if((save_line[cnt2] & 0XFF) == 0xAA)  //低八位为0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = 0xAA ;
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = 0xA5 ;
							cnt1++;
						}
				}
				else
				{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = save_line[cnt2]&0XFF ;//数据序列的低八位
							cnt1++;
						}
				}
			}
			else  //cnt为偶数发送高八位
			{
				if(((save_line[cnt2]&0XFF00)>>8) == 0x55)  //高八位为0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = 0xAA ;
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = 0x5A ;
							cnt1++;
							cnt2++;
						}
				}
				else if(((save_line[cnt2]&0XFF00)>>8) == 0xAA)  //高八位为0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = 0xAA ;
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = 0xA5 ;
							cnt1++;
							cnt2++;
						}
				}
				else
				{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (save_line[cnt2]&0XFF00)>>8;//高八位
							cnt1++;
							cnt2++;
						}
				}
			}
		}
}
void receive_command(void)  //接收数据函数，由主函数调用
{
	u16 i; //临时变量
	u16 crc;  //crc校验，采用CCITT
	u16 crc_rec;  //接收到的两字节crc校验码，转换成16位，方便比较
	
	u8 ecnt = 0;
	//各个参量在命令数据包里的偏移量
	u16 EnvelopeUstart = 20;
	u16 EnvelopeUend = 0;
	u16 EnvelopeBstart = 0;
	u16 EnvelopeBend = 0;
	u16 Rctanglestart = 0;
	u16 Rctangleend = 0;
	u16 Intervalstart = 0;
	u16 Intervalend = 0;
	if( (Uart3_Rx_Buf[0] == 0xEF) && 
			check_data(Uart3_Rx_Buf,Uart3_Rx_Data_Len) )  //数据头正确
	{
		//构造发送数据包，并发送数据
		{
			ecnt = 0;
			Uart3_Tx_Buf[ecnt++] = 0xEF;
			Uart3_Tx_Buf[ecnt++] = 0x00;
			Uart3_Tx_Buf[ecnt++] = 0x01;
			Uart3_Tx_Buf[ecnt++] = Uart3_Rx_Buf[1];
			crc = crc16(Uart3_Tx_Buf , 4);
			Uart3_Tx_Buf[ecnt++] = crc;
			Uart3_Tx_Buf[ecnt++] = crc>>8;
			USART3_SStr(Uart3_Tx_Buf , ecnt);
		}		
		
		switch (Uart3_Rx_Buf[1]) //根据命令代码跳转分支
		{
			case 0x01: //工作模式切换命令
				switch (Uart3_Rx_Buf[4])
				{
					case 0x01:	//设定为正常工作模式
						TIM5->ARR=position;  //起始位置根据参数确定
						TIM5->CNT=0;
						EXTI->IMR |= 1<<3;
						break;
					case 0x02:	//设定为停止模式
						TIM5->ARR=0xFFFF;  //起始触发区间为最大
						TIM5->CNT=0;
						EXTI->IMR &= ~(1<<3);
						//TIM_Cmd(TIM6,ENABLE); //设定为定时触发状态
						break;
					case 0x03:	//设定为停止模式 2017.3.21 暂无此状态
						break;
					default:
						break;
				}
				break;
			case 0x02: //工作模式配置
				//配置通用参量
				{
					
					//配置插针深度
					depth = (Uart3_Rx_Buf[4]<<24) +  
									(Uart3_Rx_Buf[5]<<16) +
									(Uart3_Rx_Buf[6]<<8)  +
									(Uart3_Rx_Buf[7]);
					//配置插针与插件的初始距离
					position= (Uart3_Rx_Buf[8]<<24) +
										(Uart3_Rx_Buf[9]<<16) +
										(Uart3_Rx_Buf[10]<<8)  +
										(Uart3_Rx_Buf[11]);
					//配置采集数据的间隔
					sampling = (Uart3_Rx_Buf[12]<<24) +
										 (Uart3_Rx_Buf[13]<<16) +
										 (Uart3_Rx_Buf[14]<<8)  +
										 (Uart3_Rx_Buf[15]);
					//配置保存数据的间隔,为sampling的整数倍
					saving= (Uart3_Rx_Buf[16]<<24) +
									(Uart3_Rx_Buf[17]<<16) +
									(Uart3_Rx_Buf[18]<<8)  +
									(Uart3_Rx_Buf[19]);
					Gap=saving/sampling;
					Data_Len=10000/Gap;  //获取要发送的数据长度 
				}
				//包络线算法数据
				{
					
					//上包络线的点数
					EnvelopeU_len = (Uart3_Rx_Buf[EnvelopeUstart  ]<<24) +
													(Uart3_Rx_Buf[EnvelopeUstart+1]<<16) +
													(Uart3_Rx_Buf[EnvelopeUstart+2]<<8)  +
													(Uart3_Rx_Buf[EnvelopeUstart+3]);
					EnvelopeUstart = 20;
					EnvelopeUend = EnvelopeUstart + 
												 4 +
												 EnvelopeU_len * 4;
												 
					//配置上包络坐标点
					ecnt=0;
					for(i = EnvelopeUstart+4 ; i<EnvelopeUend ; i++)
					{
						envelope_up[ecnt] = (Uart3_Rx_Buf[i]<<8) +
																	(Uart3_Rx_Buf[i+1]);
						i++;
						ecnt++;
					}
					EnvelopeBstart = EnvelopeUend;
					//下包络线的点数
					EnvelopeB_len = (Uart3_Rx_Buf[EnvelopeBstart  ]<<24) +
													(Uart3_Rx_Buf[EnvelopeBstart+1]<<16) +
													(Uart3_Rx_Buf[EnvelopeBstart+2]<<8)  +
													(Uart3_Rx_Buf[EnvelopeBstart+3]);
					EnvelopeBend = EnvelopeBstart + 
												 4 +
												 EnvelopeB_len * 4;
					//配置下包络坐标点
					ecnt = 0;
					for(i = EnvelopeBstart+4 ; i<EnvelopeBend ; i++)
					{
						envelope_below[ecnt] = (Uart3_Rx_Buf[i]<<8) +
																	(Uart3_Rx_Buf[i+1]);
						i++;
						ecnt++;
					}
				}
				//区域对比算法数据
				{
					Rctanglestart = EnvelopeBend;
					//矩形区域的数量
					Rctangle_len = (Uart3_Rx_Buf[Rctanglestart  ]<<24) +
												 (Uart3_Rx_Buf[Rctanglestart+1]<<16) +
												 (Uart3_Rx_Buf[Rctanglestart+2]<<8)  +
												 (Uart3_Rx_Buf[Rctanglestart+3]);
					Rctangleend = 	Rctanglestart + 
												 4 +
												 EnvelopeU_len * 4;
					//配置各个矩形区域的左上右下顶点坐标
					ecnt = 0;
					for(i = Rctanglestart+4 ; i<Rctangleend ; i++)
					{
						rectangleXY[ecnt] = (Uart3_Rx_Buf[i]<<8) +
																	(Uart3_Rx_Buf[i+1]);
						i++;
						ecnt++;
					}
				}			
				//极值对比算法数据
				{
					Intervalstart = Rctangleend;
					//区域的数量
					Interval_len = (Uart3_Rx_Buf[Intervalstart  ]<<24) +
												 (Uart3_Rx_Buf[Intervalstart+1]<<16) +
												 (Uart3_Rx_Buf[Intervalstart+2]<<8)  +
												 (Uart3_Rx_Buf[Intervalstart+3]);
					Intervalend = 	Intervalstart + 
												 4 +
												 Interval_len * 4;
					//配置各个间隔区域的两点坐标
					ecnt = 0;
					for(i = Intervalstart+4 ; i<Intervalend ; i++)
					{
						intervalXY[ecnt] = (Uart3_Rx_Buf[i]<<8) +
																	(Uart3_Rx_Buf[i+1]);
						i++;
						ecnt++;
					}
				}
				break;
			case 0x03: //传感器参数选择 触发采集方式选择
				break;
			case 0x04: //发送当前全部数据
				SendAll();  //发送全部数据
				break;
			case 0x05: //对比算法切换
				switch (Uart3_Rx_Buf[4])
				{
					case 0x01:
						get_envelope_up (envelope_up,EnvelopeU_len);//p_up获取上包络线的y值
						get_envelope_below (envelope_below,EnvelopeB_len); //p_below获取下包络线的值
						break;
					case 0x02:
						get_rectangle_up (rectangleXY,Rctangle_len);//p_up获取上包络线的y值
						get_rectangle_below (rectangleXY,Rctangle_len); //p_below获取下包络线的值
						break;
					case 0x03:
						get_interval_up (intervalXY,Interval_len);//p_up获取上包络线的y值
						get_interval_below (intervalXY,Interval_len); //p_below获取下包络线的值
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
}

void USART3_IRQHandler(void)
{
	u8 a = 0;
	//接收数据中断
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{		
		Uart3_Rx_Buf[Uart3_Rx_Data_Len] = USART3->DR; 
		Uart3_Rx_Data_Len++;
	}
	//空闲中断
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  
  {
		a = USART3->SR;  
    a = USART3->DR; //清除IDLE标志     
		Uart3_Rx_F = 1;
	}
	//发送中端处理
	if(USART_GetITStatus( USART3, USART_IT_TC ) != RESET)
	{	
		USART_ClearFlag(USART3,USART_FLAG_TC);
		Uart3_SendCurP++;
		if(Uart3_SendCurP < Uart3_Tx_Data_Len)
		{
			USART3->DR = Uart3_Tx_Buf[Uart3_SendCurP];
		}		
	}
}

void USART3_SStr(uint8_t *SData,uint8_t S_Len)
{
	uint8_t cnt = 0;
	for(;cnt<S_Len;cnt++)
	{
		Uart3_Tx_Buf[cnt] = SData[cnt];
	}
	Uart3_Tx_Data_Len = S_Len;
	Uart3_SendCurP = 0;
	USART_ClearFlag(USART3,USART_FLAG_TC);
	//USART_SendData(USART3, Uart3_Tx_Buf[Uart3_SendCurP] );
	USART3->DR = Uart3_Tx_Buf[Uart3_SendCurP];
}

 /**
  * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USART3_Config(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* config USART1 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);	
	
	/* USART3 GPIO config */
	/* Configure USART3 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USART3 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	

	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 使能串口中断*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启接收中断
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); //开启空闲中断
	USART_ClearFlag(USART3,USART_FLAG_TC);
	USART_ITConfig(USART3,USART_IT_TC,ENABLE);//开启发送中断
	
	USART_Cmd(USART3, ENABLE);
}




/*********************************************END OF FILE**********************/




#include "global.h"
#include "./usart/bsp_usart3.h"
#include "crc.h"


uint8_t Uart3_Rx_Buf[UART3_RX_LEN];
uint8_t Uart3_Tx_Buf[UART3_TX_LEN];
uint8_t Uart3_Rx_Data_Len = 0;
uint8_t Uart3_Tx_Data_Len = 0;
uint8_t Uart3_Rx_F = 0;
u8 Uart3_SendCurP = 0;

static void SendAll(void) //���յ�0x24�����sive_lineȫ�����ݷ��͵���λ��
{
		u16 cnt1 = 0;
		u16 cnt2 = 0;  //��������
	
		while(cnt2 < 10000)
		{
			if(!(cnt1 & 0x01)) //cntΪż�����͵Ͱ�λ
			{
				if((save_line[cnt2] & 0XFF) == 0x55)  //�Ͱ�λΪ0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = 0xAA ;
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = 0x5A ;
							cnt1++;
						}
				}
				else if((save_line[cnt2] & 0XFF) == 0xAA)  //�Ͱ�λΪ0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = 0xAA ;
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = 0xA5 ;
							cnt1++;
						}
				}
				else
				{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = save_line[cnt2]&0XFF ;//�������еĵͰ�λ
							cnt1++;
						}
				}
			}
			else  //cntΪż�����͸߰�λ
			{
				if(((save_line[cnt2]&0XFF00)>>8) == 0x55)  //�߰�λΪ0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = 0xAA ;
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = 0x5A ;
							cnt1++;
							cnt2++;
						}
				}
				else if(((save_line[cnt2]&0XFF00)>>8) == 0xAA)  //�߰�λΪ0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = 0xAA ;
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = 0xA5 ;
							cnt1++;
							cnt2++;
						}
				}
				else
				{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (save_line[cnt2]&0XFF00)>>8;//�߰�λ
							cnt1++;
							cnt2++;
						}
				}
			}
		}
}
void receive_command(void)  //�������ݺ�����������������
{
	u16 i; //��ʱ����
	u16 crc;  //crcУ�飬����CCITT
	u16 crc_rec;  //���յ������ֽ�crcУ���룬ת����16λ������Ƚ�
	
	u8 ecnt = 0;
	//�����������������ݰ����ƫ����
	u16 EnvelopeUstart = 20;
	u16 EnvelopeUend = 0;
	u16 EnvelopeBstart = 0;
	u16 EnvelopeBend = 0;
	u16 Rctanglestart = 0;
	u16 Rctangleend = 0;
	u16 Intervalstart = 0;
	u16 Intervalend = 0;
	if( (Uart3_Rx_Buf[0] == 0xEF) && 
			check_data(Uart3_Rx_Buf,Uart3_Rx_Data_Len) )  //����ͷ��ȷ
	{
		//���췢�����ݰ�������������
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
		
		switch (Uart3_Rx_Buf[1]) //�������������ת��֧
		{
			case 0x01: //����ģʽ�л�����
				switch (Uart3_Rx_Buf[4])
				{
					case 0x01:	//�趨Ϊ��������ģʽ
						TIM5->ARR=position;  //��ʼλ�ø��ݲ���ȷ��
						TIM5->CNT=0;
						EXTI->IMR |= 1<<3;
						break;
					case 0x02:	//�趨Ϊֹͣģʽ
						TIM5->ARR=0xFFFF;  //��ʼ��������Ϊ���
						TIM5->CNT=0;
						EXTI->IMR &= ~(1<<3);
						//TIM_Cmd(TIM6,ENABLE); //�趨Ϊ��ʱ����״̬
						break;
					case 0x03:	//�趨Ϊֹͣģʽ 2017.3.21 ���޴�״̬
						break;
					default:
						break;
				}
				break;
			case 0x02: //����ģʽ����
				//����ͨ�ò���
				{
					
					//���ò������
					depth = (Uart3_Rx_Buf[4]<<24) +  
									(Uart3_Rx_Buf[5]<<16) +
									(Uart3_Rx_Buf[6]<<8)  +
									(Uart3_Rx_Buf[7]);
					//���ò��������ĳ�ʼ����
					position= (Uart3_Rx_Buf[8]<<24) +
										(Uart3_Rx_Buf[9]<<16) +
										(Uart3_Rx_Buf[10]<<8)  +
										(Uart3_Rx_Buf[11]);
					//���òɼ����ݵļ��
					sampling = (Uart3_Rx_Buf[12]<<24) +
										 (Uart3_Rx_Buf[13]<<16) +
										 (Uart3_Rx_Buf[14]<<8)  +
										 (Uart3_Rx_Buf[15]);
					//���ñ������ݵļ��,Ϊsampling��������
					saving= (Uart3_Rx_Buf[16]<<24) +
									(Uart3_Rx_Buf[17]<<16) +
									(Uart3_Rx_Buf[18]<<8)  +
									(Uart3_Rx_Buf[19]);
					Gap=saving/sampling;
					Data_Len=10000/Gap;  //��ȡҪ���͵����ݳ��� 
				}
				//�������㷨����
				{
					
					//�ϰ����ߵĵ���
					EnvelopeU_len = (Uart3_Rx_Buf[EnvelopeUstart  ]<<24) +
													(Uart3_Rx_Buf[EnvelopeUstart+1]<<16) +
													(Uart3_Rx_Buf[EnvelopeUstart+2]<<8)  +
													(Uart3_Rx_Buf[EnvelopeUstart+3]);
					EnvelopeUstart = 20;
					EnvelopeUend = EnvelopeUstart + 
												 4 +
												 EnvelopeU_len * 4;
												 
					//�����ϰ��������
					ecnt=0;
					for(i = EnvelopeUstart+4 ; i<EnvelopeUend ; i++)
					{
						envelope_up[ecnt] = (Uart3_Rx_Buf[i]<<8) +
																	(Uart3_Rx_Buf[i+1]);
						i++;
						ecnt++;
					}
					EnvelopeBstart = EnvelopeUend;
					//�°����ߵĵ���
					EnvelopeB_len = (Uart3_Rx_Buf[EnvelopeBstart  ]<<24) +
													(Uart3_Rx_Buf[EnvelopeBstart+1]<<16) +
													(Uart3_Rx_Buf[EnvelopeBstart+2]<<8)  +
													(Uart3_Rx_Buf[EnvelopeBstart+3]);
					EnvelopeBend = EnvelopeBstart + 
												 4 +
												 EnvelopeB_len * 4;
					//�����°��������
					ecnt = 0;
					for(i = EnvelopeBstart+4 ; i<EnvelopeBend ; i++)
					{
						envelope_below[ecnt] = (Uart3_Rx_Buf[i]<<8) +
																	(Uart3_Rx_Buf[i+1]);
						i++;
						ecnt++;
					}
				}
				//����Ա��㷨����
				{
					Rctanglestart = EnvelopeBend;
					//�������������
					Rctangle_len = (Uart3_Rx_Buf[Rctanglestart  ]<<24) +
												 (Uart3_Rx_Buf[Rctanglestart+1]<<16) +
												 (Uart3_Rx_Buf[Rctanglestart+2]<<8)  +
												 (Uart3_Rx_Buf[Rctanglestart+3]);
					Rctangleend = 	Rctanglestart + 
												 4 +
												 EnvelopeU_len * 4;
					//���ø�������������������¶�������
					ecnt = 0;
					for(i = Rctanglestart+4 ; i<Rctangleend ; i++)
					{
						rectangleXY[ecnt] = (Uart3_Rx_Buf[i]<<8) +
																	(Uart3_Rx_Buf[i+1]);
						i++;
						ecnt++;
					}
				}			
				//��ֵ�Ա��㷨����
				{
					Intervalstart = Rctangleend;
					//���������
					Interval_len = (Uart3_Rx_Buf[Intervalstart  ]<<24) +
												 (Uart3_Rx_Buf[Intervalstart+1]<<16) +
												 (Uart3_Rx_Buf[Intervalstart+2]<<8)  +
												 (Uart3_Rx_Buf[Intervalstart+3]);
					Intervalend = 	Intervalstart + 
												 4 +
												 Interval_len * 4;
					//���ø�������������������
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
			case 0x03: //����������ѡ�� �����ɼ���ʽѡ��
				break;
			case 0x04: //���͵�ǰȫ������
				SendAll();  //����ȫ������
				break;
			case 0x05: //�Ա��㷨�л�
				switch (Uart3_Rx_Buf[4])
				{
					case 0x01:
						get_envelope_up (envelope_up,EnvelopeU_len);//p_up��ȡ�ϰ����ߵ�yֵ
						get_envelope_below (envelope_below,EnvelopeB_len); //p_below��ȡ�°����ߵ�ֵ
						break;
					case 0x02:
						get_rectangle_up (rectangleXY,Rctangle_len);//p_up��ȡ�ϰ����ߵ�yֵ
						get_rectangle_below (rectangleXY,Rctangle_len); //p_below��ȡ�°����ߵ�ֵ
						break;
					case 0x03:
						get_interval_up (intervalXY,Interval_len);//p_up��ȡ�ϰ����ߵ�yֵ
						get_interval_below (intervalXY,Interval_len); //p_below��ȡ�°����ߵ�ֵ
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
	//���������ж�
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{		
		Uart3_Rx_Buf[Uart3_Rx_Data_Len] = USART3->DR; 
		Uart3_Rx_Data_Len++;
	}
	//�����ж�
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  
  {
		a = USART3->SR;  
    a = USART3->DR; //���IDLE��־     
		Uart3_Rx_F = 1;
	}
	//�����ж˴���
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
  * @brief  USART1 GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
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
	
	/* ʹ�ܴ����ж�*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //���������ж�
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); //���������ж�
	USART_ClearFlag(USART3,USART_FLAG_TC);
	USART_ITConfig(USART3,USART_IT_TC,ENABLE);//���������ж�
	
	USART_Cmd(USART3, ENABLE);
}




/*********************************************END OF FILE**********************/



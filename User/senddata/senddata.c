
#include "global.h"

void senddata(void)
{
	switch(send_num)
	{
		case 1: //�Ѿ����յ�����
			{
				if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
				{
					USART1->DR = (0x55 & (uint16_t)0x01FF);//��������ͷ
					send_num++;
					AccumulateCheck=0;
				}
				break;
			}
		case 2: //����ͷ�������,�������ݴ���
			{
				if(WorkMode==0x01)
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
					{
						USART1->DR = (0x01 & (uint16_t)0x01FF);
						AccumulateCheck=AccumulateCheck+0x01;
						send_num++;
					}
				}
				else if(WorkMode==0x03)
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
					{
						USART1->DR = (0x02 & (uint16_t)0x01FF);
						AccumulateCheck=AccumulateCheck+0x02;
						send_num++;
					}
				}
				break;
			}
		case 3://�������ݳ��ȣ�2���ֽڣ���DataLen����
			{
				if((DataLen&0XFF)==0x55)  //�Ͱ�λΪ0x55
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
							break;
						}
					}
				else if((DataLen&0XFF)==0xAA)  //�Ͱ�λΪ0xAA
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0xA5 & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0xAA;
							send_num++;
							break;
						}
					}
				else
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (DataLen&0XFF & (uint16_t)0x01FF);//DataLen�ĵͰ�λ
							AccumulateCheck=AccumulateCheck+DataLen&0XFF;
							send_num++;
						}
						break;
					}
			}
		case 4:
			{
				if(((DataLen&0XFF00)>>8)==0x55)  //�߰�λΪ0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = 0xAA & (uint16_t)0x01FF;
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = 0x5A & (uint16_t)0x01FF;
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
							break;
						}
				}
				else if(((DataLen&0XFF00)>>8)==0xAA)  //�߰�λΪ0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0xA5 & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0xAA;
							send_num++;
							break;
						}
				}
				else
				{
				if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
				{
					USART1->DR = (((DataLen&0XFF00)>>8) & (uint16_t)0x01FF);//DataLen�ĸ߰�λ
					AccumulateCheck=AccumulateCheck+((DataLen&0XFF00)>>8);
					send_num++;
				}
				break;
				}
			}
		case 5://�����������,2���ֽ�
			{
				SerialNumber++;  //����������1
				if((SerialNumber&0XFF)==0x55)  //�Ͱ�λΪ0x55
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
							break;
						}
					}
				else if((SerialNumber&0XFF)==0xAA)  //�Ͱ�λΪ0xAA
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0xA5 & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0xAA;
							send_num++;
							break;
						}
					}
				else
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (SerialNumber&0XFF & (uint16_t)0x01FF);//�������еĵͰ�λ
							AccumulateCheck=AccumulateCheck+(SerialNumber&0XFF);
							send_num++;
						}
					}
				break;
			}
		case 6:
			{
				if(((SerialNumber&0XFF00)>>8)==0x55)  //�߰�λΪ0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
							break;
						}
				}
				else if(((SerialNumber&0XFF00)>>8)==0xAA)  //�߰�λΪ0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0xA5 & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0xAA;
							send_num++;
							break;
						}
				}
				else
				{
				if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (((SerialNumber&0XFF00)>>8) & (uint16_t)0x01FF);//�������еĵͰ�λ
							AccumulateCheck=AccumulateCheck+((SerialNumber&0XFF00)>>8);
							send_num++;
						}
				break;
				}
			}
		default :break;
		}
		if(send_num>6 && send_num<(2 * DataLen + 7)) //��������,��save_line[10000]�з���
		{
			if(send_num & 0x01) //send_numΪ�������͵Ͱ�λ
			{
				if((save_line[datasend]&0XFF)==0x55)  //�Ͱ�λΪ0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
						}
				}
				else if((save_line[datasend]&0XFF)==0xAA)  //�Ͱ�λΪ0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & 0xFF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0xA5 & 0xFF);
							AccumulateCheck=AccumulateCheck+0xAA;
							send_num++;
						}
				}
				else
				{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (save_line[datasend]&0XFF & (uint16_t)0x01FF);//�������еĵͰ�λ
							AccumulateCheck=AccumulateCheck+(save_line[datasend]&0XFF);
							send_num++;
						}
				}
			}
			else  //send_numΪż�����͸߰�λ
			{
				if(((save_line[datasend]&0XFF00)>>8)==0x55)  //�߰�λΪ0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
							datasend=datasend+s;//�߰�λ������ϣ���ʼ������һ������
						}
				}
				else if(((save_line[datasend]&0XFF00)>>8)==0xAA)  //�߰�λΪ0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0xA5 & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0xAA;
							send_num++;
							datasend=datasend+s;//�߰�λ������ϣ���ʼ������һ������
						}
				}
				else
				{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (((save_line[datasend]&0XFF00)>>8) & 0xFF);//�߰�λ
							AccumulateCheck=AccumulateCheck+((save_line[datasend]&0XFF00)>>8);
							send_num++;
							datasend=datasend+s;//�߰�λ������ϣ���ʼ������һ������
						}
				}
			}
		}
		if(send_num==2 * DataLen + 7)
		{
			if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = ((WorkMode-1) & (uint16_t)0x01FF);//�������еĵͰ�λ
							AccumulateCheck=AccumulateCheck+WorkMode-1;
							send_num++;
						}
		}
		if(send_num==2 * DataLen + 8) //У��λ
		{
			if(AccumulateCheck==0x55)  //�Ͱ�λΪ0x55
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							send_num++;
						}
					}
				else if(AccumulateCheck==0xAA)  //�Ͱ�λΪ0xAA
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //�ȴ��������
							USART1->DR = (0xA5 & (uint16_t)0x01FF);
							send_num++;
						}
					}
				else
					{
				if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (AccumulateCheck & (uint16_t)0x01FF);//�������еĵͰ�λ
							send_num++;
						}
					}
		}
		if(send_num==2 * DataLen + 9)
		{
			if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0x55 & (uint16_t)0x01FF);//�������еĵͰ�λ
							send_num++;
						}
			if(WorkMode==0x03)  //debugģʽ���Ա���һ�μ�����ֹͣģʽ
				{
						TIM5_Mode_Config(); 
				}
		}
}


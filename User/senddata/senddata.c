
#include "global.h"

void senddata(void)
{
	switch(send_num)
	{
		case 1: //已经接收到数据
			{
				if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
				{
					USART1->DR = (0x55 & (uint16_t)0x01FF);//发送数据头
					send_num++;
					AccumulateCheck=0;
				}
				break;
			}
		case 2: //数据头发送完毕,发送数据代码
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
		case 3://发送数据长度，2个字节，由DataLen给出
			{
				if((DataLen&0XFF)==0x55)  //低八位为0x55
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
							break;
						}
					}
				else if((DataLen&0XFF)==0xAA)  //低八位为0xAA
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
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
							USART1->DR = (DataLen&0XFF & (uint16_t)0x01FF);//DataLen的低八位
							AccumulateCheck=AccumulateCheck+DataLen&0XFF;
							send_num++;
						}
						break;
					}
			}
		case 4:
			{
				if(((DataLen&0XFF00)>>8)==0x55)  //高八位为0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = 0xAA & (uint16_t)0x01FF;
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = 0x5A & (uint16_t)0x01FF;
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
							break;
						}
				}
				else if(((DataLen&0XFF00)>>8)==0xAA)  //高八位为0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
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
					USART1->DR = (((DataLen&0XFF00)>>8) & (uint16_t)0x01FF);//DataLen的高八位
					AccumulateCheck=AccumulateCheck+((DataLen&0XFF00)>>8);
					send_num++;
				}
				break;
				}
			}
		case 5://发送数据序号,2个字节
			{
				SerialNumber++;  //数据序数加1
				if((SerialNumber&0XFF)==0x55)  //低八位为0x55
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
							break;
						}
					}
				else if((SerialNumber&0XFF)==0xAA)  //低八位为0xAA
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
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
							USART1->DR = (SerialNumber&0XFF & (uint16_t)0x01FF);//数据序列的低八位
							AccumulateCheck=AccumulateCheck+(SerialNumber&0XFF);
							send_num++;
						}
					}
				break;
			}
		case 6:
			{
				if(((SerialNumber&0XFF00)>>8)==0x55)  //高八位为0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
							break;
						}
				}
				else if(((SerialNumber&0XFF00)>>8)==0xAA)  //高八位为0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
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
							USART1->DR = (((SerialNumber&0XFF00)>>8) & (uint16_t)0x01FF);//数据序列的低八位
							AccumulateCheck=AccumulateCheck+((SerialNumber&0XFF00)>>8);
							send_num++;
						}
				break;
				}
			}
		default :break;
		}
		if(send_num>6 && send_num<(2 * DataLen + 7)) //发送数据,从save_line[10000]中发送
		{
			if(send_num & 0x01) //send_num为奇数发送低八位
			{
				if((save_line[datasend]&0XFF)==0x55)  //低八位为0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
						}
				}
				else if((save_line[datasend]&0XFF)==0xAA)  //低八位为0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & 0xFF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = (0xA5 & 0xFF);
							AccumulateCheck=AccumulateCheck+0xAA;
							send_num++;
						}
				}
				else
				{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (save_line[datasend]&0XFF & (uint16_t)0x01FF);//数据序列的低八位
							AccumulateCheck=AccumulateCheck+(save_line[datasend]&0XFF);
							send_num++;
						}
				}
			}
			else  //send_num为偶数发送高八位
			{
				if(((save_line[datasend]&0XFF00)>>8)==0x55)  //高八位为0x55
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0x55;
							send_num++;
							datasend=datasend+s;//高八位发送完毕，开始发送下一个数据
						}
				}
				else if(((save_line[datasend]&0XFF00)>>8)==0xAA)  //高八位为0xAA
				{
					if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = (0xA5 & (uint16_t)0x01FF);
							AccumulateCheck=AccumulateCheck+0xAA;
							send_num++;
							datasend=datasend+s;//高八位发送完毕，开始发送下一个数据
						}
				}
				else
				{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (((save_line[datasend]&0XFF00)>>8) & 0xFF);//高八位
							AccumulateCheck=AccumulateCheck+((save_line[datasend]&0XFF00)>>8);
							send_num++;
							datasend=datasend+s;//高八位发送完毕，开始发送下一个数据
						}
				}
			}
		}
		if(send_num==2 * DataLen + 7)
		{
			if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = ((WorkMode-1) & (uint16_t)0x01FF);//数据序列的低八位
							AccumulateCheck=AccumulateCheck+WorkMode-1;
							send_num++;
						}
		}
		if(send_num==2 * DataLen + 8) //校验位
		{
			if(AccumulateCheck==0x55)  //低八位为0x55
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = (0x5A & (uint16_t)0x01FF);
							send_num++;
						}
					}
				else if(AccumulateCheck==0xAA)  //低八位为0xAA
					{
						if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0xAA & (uint16_t)0x01FF);
							while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);  //等待发送完成
							USART1->DR = (0xA5 & (uint16_t)0x01FF);
							send_num++;
						}
					}
				else
					{
				if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (AccumulateCheck & (uint16_t)0x01FF);//数据序列的低八位
							send_num++;
						}
					}
		}
		if(send_num==2 * DataLen + 9)
		{
			if((USART1->SR & USART_FLAG_TC) != (uint16_t)RESET)
						{
							USART1->DR = (0x55 & (uint16_t)0x01FF);//数据序列的低八位
							send_num++;
						}
			if(WorkMode==0x03)  //debug模式，对比完一次即进入停止模式
				{
						TIM5_Mode_Config(); 
				}
		}
}


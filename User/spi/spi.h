#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32������ 
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/28
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//����˵��
//V1.1 20151230
//������SPI2��Ӧ�ô���
////////////////////////////////////////////////////////////////////////////////// 	 

				    
// SPI�����ٶ����� 
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7

#define	ADS_CS 	PGout(12)  		//ADS79XX��Ƭѡ�ź�

void ADS_SPI_Init(void);
 u16 SPI_ADS7946_READ(void);
void SPI1_Init1(void);						  	    													  
void SPI5_Init(void);			 //��ʼ��SPI��
void SPI5_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI5_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
void SPI1_Init(void);
void SPI1_SetSpeed(u8 SpeedSet);
u8 SPI1_ReadWriteByte(u8 TxData);
u16 SPI1_ReadWriteWord(u16 TxData);
#endif

#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板 
//SPI 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/28
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//********************************************************************************
//升级说明
//V1.1 20151230
//新增对SPI2的应用代码
////////////////////////////////////////////////////////////////////////////////// 	 

				    
// SPI总线速度设置 
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7

#define	ADS_CS 	PGout(12)  		//ADS79XX的片选信号

void ADS_SPI_Init(void);
 u16 SPI_ADS7946_READ(void);
void SPI1_Init1(void);						  	    													  
void SPI5_Init(void);			 //初始化SPI口
void SPI5_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI5_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
void SPI1_Init(void);
void SPI1_SetSpeed(u8 SpeedSet);
u8 SPI1_ReadWriteByte(u8 TxData);
u16 SPI1_ReadWriteWord(u16 TxData);
#endif


 #include "./spi/spi.h"
//ALIENTEK STM32������ 

#define ReadAD0 {	\
									ADS_CS=0;	\
									/* ����һ���ֽ� */\
									SPI1->DR=0x55; \
								}

u8 	 i = 0;
//������SPI 1 ��Ӧ�ô���
////////////////////////////////////////////////////////////////////////////////// 	 
//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��


void SPI1_Init1(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ�� SPI ��GPIO ʱ�� */
  RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOG, ENABLE);

  /*!< SPI_FLASH_SPI ʱ��ʹ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//GPIO_DeInit(GPIOB);
	//�������Ÿ���
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); 
	
	/*!< ���� SPI_FLASH_SPI ����: SCK */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*!< ���� SPI_FLASH_SPI ����: MISO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	/*!< ���� SPI_FLASH_SPI ����: MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  

	/*!< ���� SPI_FLASH_SPI ����: CS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
	GPIOG->BSRRL=GPIO_Pin_12;
	
	SPI_I2S_DeInit(SPI1);
  // ADSPIģʽ
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* ʹ�� FLASH_SPI  */
  SPI_Cmd(SPI1, ENABLE);
}

 u16 SPI_ADS7946_READ(void)
 {
   // u8 Buffer[2];
	  u16 Temp=0;
	 i = 0;
	 ADS_CS=0;//ADS_CS  
	 SPI1->DR=0x55;	 	  	    	//����һ��word  ��ȡadͨ��1����ֵ
	 while((SPI1->SR&1)==0){i++;}		//�ȴ�������һ��word  
	 Temp=SPI1->DR;          		//�����յ�������	
	 ADS_CS=1;
	 ///*  ��λ����
	 ADS_CS=1;
	 ADS_CS=1;
	 ADS_CS=1;
	 ADS_CS=0;
	 SPI1->DR=0x55;	 	  	    	//����һ��word  ��ȡadͨ��1����ֵ
	 while((SPI1->SR&1)==0);		//�ȴ�������һ��word  
	 Temp=SPI1->DR;          		//�����յ�������	
	 ADS_CS=1;
	 //*/
	 Temp=Temp>>2;
	 return Temp;
 }

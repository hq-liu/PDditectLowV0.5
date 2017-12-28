/**
  ******************************************************************************
  * @file    encoder.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   ���ļ���ʼ����TIM3��TIM4��TIM5������ʱ��
						 TIM4��TIM5���ж�
						 TIM4ÿ����һ���ж϶�ȡһ��ADֵ��ѹ��ֵ���������������ʵ��������
						 TIM5ͨ���жϿ��Ƶ�ǰ״̬Ϊʱ�䴥������λ�ƴ���
  ******************************************************************************
  * @attention ��

  ******************************************************************************
  */


#include "global.h"


//void TIM3_IRQHandler(void)
//{ 	
//	
//}
void TIM4_IRQHandler(void)
{ 
	if((TIM4->SR & 0x01) && (TIM4->DIER & 0x01))//����ж� 
	{
		if(recive_num<=10000)
		{
				num=SPI_ADS7946_READ();
				//save_line[recive_num]=num;  //��һ���ж�save�������һλ
				save_line[recive_num]=recive_num;
				recive_num++;  //��¼�ɼ�������Ĳ�����һ,recive_num++�൱��xֵ			
		}
  }
	TIM4->SR &= ~0x01;	
}
void TIM5_IRQHandler(void)
{ 
	if((TIM5->SR & 0x01) && (TIM5->DIER & 0x01))//����ж� 
	{
		if(Tim5State==0)  //S1�׶ν���S2�׶�
		{
				//TIM_Cmd(TIM6,DISABLE);//�ر�ʱ�䴥��
				//TIM6->CR1 &= (uint16_t)~TIM_CR1_CEN;
				Tim5State=1;  //�л���ǰ״̬
				GPIOB->BSRRH = GPIO_Pin_1 ; //����λ�ƴ���DS0����
				TIM5->ARR=depth;  //TIM���ֵ����ΪS2
				TIM5->CNT=0;
				TIM4->CR1 |= TIM_CR1_CEN;  //����TIM4,׼��λ�ƴ����ɼ�
				TIM4->CNT=0;
				//��������ͷ������	
				//���ݴ��룬0x01Ϊʵʱģʽ��0x02Ϊ����ģʽ
				HeadCnt=0;
				DataHead[HeadCnt]=0x55;HeadCnt++;
				if(WorkMode==0x01) {DataHead[HeadCnt]=0x01;HeadCnt++;}
				else if(WorkMode==0x03) {DataHead[HeadCnt]=0x02;HeadCnt++;} 
				//���ݳ��ȣ�����DataLen�ó�����Ϊ�߰�λ�͵Ͱ�λ����
				if(((Data_Len&0XFF00)>>8)==0x55) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0x5A;HeadCnt++;}
				else if(((Data_Len&0XFF00)>>8)==0xAA) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0xA5;HeadCnt++;}
				else {DataHead[HeadCnt]=(Data_Len&0XFF00)>>8;HeadCnt++;}
				if((Data_Len&0XFF)==0x55) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0x5A;HeadCnt++;}
				else if((Data_Len&0XFF)==0xAA) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0xA5;HeadCnt++;}
				else {DataHead[HeadCnt]=Data_Len&0XFF;HeadCnt++;}
				
				//������ţ�����SerialNumber�ó�
				SerialNumber++;
				if(((SerialNumber&0XFF00)>>8)==0x55) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0x5A;HeadCnt++;}
				else if(((SerialNumber&0XFF00)>>8)==0xAA) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0xA5;HeadCnt++;}
				else {DataHead[HeadCnt]=(SerialNumber&0XFF00)>>8;HeadCnt++;}
				if((SerialNumber&0XFF)==0x55) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0x5A;HeadCnt++;}
				else if((SerialNumber&0XFF)==0xAA) {DataHead[HeadCnt]=0xAA;HeadCnt++;DataHead[HeadCnt]=0xA5;HeadCnt++;}
				else {DataHead[HeadCnt]=SerialNumber&0XFF;HeadCnt++;}
				
				for(s=0;s<HeadCnt;s++) //��DataHead��������ݷ��ͳ�ȥ
				{
						while((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);
						USART1->DR=DataHead[s];
				}
				send_num=1;
				datasend=0;   //��ʼ����һ�����ݲɼ�
				recive_num=0;  
				compared_num=0;
				DataPointer=0;
				cntt=0;
		}
		else if(Tim5State==1) //S2״̬�л���S3״̬
		{
				Tim5State=2;  //�л���ǰ״̬
				TIM4->CR1 &= (uint16_t)~TIM_CR1_CEN; //�ر�λ�ƴ���
				GPIOB->BSRRL = GPIO_Pin_1 ;  //�ر�LED
				TIM5->CR1 &= (uint16_t)~TIM_CR1_CEN; //�ر�TIM5
				//��������β������
				//�����������ͣ�OK���ݷ���0x00,NG���ݷ���0x01,ALL���ݷ���0x02
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ��TIM4ʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	
	GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//PA0 PA1��������	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
		
	// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn; 	
	// ������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// ���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_DeInit(TIM4);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = Period4-1;  //�趨��������װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3ʱ��Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
                 
	/*----------������ģʽ����---------*/
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ1�������½�������
	TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 15;  //ѡ������Ƚ��˲��� 
	TIM_ICInit(TIM4, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM2
	
	//TIM_ARRPreloadConfig(TIM4, ENABLE);//ʹ��Ԥװ�� 2017.2.8 ������ִ˾�Ӧ��ע�� ԭ��δ��
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM2�ĸ��±�־λ
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//���и����ж�
	//Reset counter
	TIM4->CNT = 0;//
	//TIM_Cmd(TIM4, ENABLE);   //����TIM3��ʱ�� ��Ҫ���ʱ�ſ�����ʱ��

}
void TIM3_Mode_Config(u16 temp)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	//PA0 ch1 ,PA1 ch2 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//ʹ��TIM4ʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	
	GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//PA0 PA1��������	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);   
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);

	TIM_DeInit(TIM3);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = temp;  //�趨��������װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3ʱ��Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              
                 
	/*----------������ģʽ����---------*/
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ1�������½�������
	TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 14;  //ѡ������Ƚ��˲��� 
	TIM_ICInit(TIM3, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM3

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIM2�ĸ��±�־λ
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//���и����ж�
	TIM3->CNT = 0;//
	TIM_Cmd(TIM3, ENABLE);   //����TIM3��ʱ��
}

void TIM5_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//ʹ��TIM5ʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);//ʹ��GPIOAʱ��
	
	GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOH, &GPIO_InitStructure);   
	
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource10,GPIO_AF_TIM5); //TIM5 CH1
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource11,GPIO_AF_TIM5);  //TIM5 CH2 
		
		// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQn; 	
	// ������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	// ���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
 	

	TIM_DeInit(TIM5);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  //�趨��������װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM5ʱ��Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);              
                 
	/*----------������ģʽ����---------*/
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 0;  //ѡ������Ƚ��˲��� 
	TIM_ICInit(TIM5, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM2
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);//���TIM2�ĸ��±�־λ
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);//���и����ж�
	//Reset counter
	TIM5->CNT = 10;//
	TIM_Cmd(TIM5, ENABLE);   //����TIM2��ʱ��

}

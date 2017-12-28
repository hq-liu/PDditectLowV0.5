/**
  ******************************************************************************
  * @file    main.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   �Ƚ��г�ʼ��������֮�������ѭ��
						 ��������Ҫ���Ĳ��ж�
						 1.�ж��Ƿ����µ����������ɣ�����������receive_command()�������ã�û��������
						 2.�ж��Ѿ��Աȵĵ����Ƿ�С�ڽ��յĵ��������������жԱȣ�������������ȴ��µ����ݵ���
						 3.�жϷ�������SendData�е������Ƿ�С���Ѿ��Աȵ����ݣ�����������SendData��������������ȴ��µĶԱ����ݵ���
						 4.�ж�SendData���Ѿ����͵������Ƿ�С���Ѿ��������ݣ��������ͨ������1�������ݣ��������ȴ��µ�����������
  ******************************************************************************
  * @attention
  *	�Ƿ���Ҫ������Ա���Ҫͨ������ָ����ɣ�recive_num����ʼֵΪ-1����һ������+1����
	* compared_num����ʼֵΪ0���Ա���һ������+1����
	* DataPointer����ʼֵΪ0��ÿ�����ӵ�ֵΪGap��
	* cntt����ʼֵΪ0��ÿ������귢����������+2��
	* datasend����ʼֵΪ0��������һ������+1��
  * ĿǰС��ģ�����з��ַ�����Ӧ����û�����⣬����û��ϸ�㣬�ܿ���©��1��2λ����
  ******************************************************************************
  */
#include "global.h"

int main(void)
{
	//���г�ʼ��
	Stm32_Clock_Init(320,25,2,8);
	delay_init(160);
	/* Configure the NVIC Preemption Priority Bits */ 
	//�����ж����ȼ��飬��������������һ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����
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
	TIM5->ARR=position;  //TIM5��ARR�Ĵ��������޸��ж����ֵ	
	//GPIOTest_Config();
	EXTIX_Init();
	BasicIO_Config();
	get_envelope_up (envelope_up,EnvelopeU_len);
	get_envelope_below (envelope_below,EnvelopeB_len); //��ʼ���Ա�����
	//TIM_Cmd(TIM6,ENABLE);
	Gap=saving/sampling;
	Data_Len=10000/Gap;  //��ȡҪ���͵����ݳ��� 
		
  while(1)
	{
		 if(Uart3_Rx_F)  //��������������Ҫ����
		 {
			receive_command();  //�����������
			Uart3_Rx_F = 0;
			Uart3_Rx_Data_Len=0;
		 }
			//��ʼ�Ա�
			if(compared_num < recive_num)//��ʼ�Ա��㷨
			{
				if(( save_line[compared_num] > y_up[compared_num] ) || ( save_line[compared_num] < y_below[compared_num] )) //����ɼ��ź��ڰ�����֮�⣬�����NG�źţ�����ѭ��
				{
					flag_ng = 1; //��¼����recive_numֵ��recive_numֵ��x��ֵ��������������				
				}
				else{
					GPIOB->BSRRH = GPIO_Pin_0 ;  //����NG�ź�
				}
				compared_num++;
			}
			
			if(DataPointer < compared_num)  //�Ա��꿪ʼ�����Ҫ���͵�����
			{
					if((save_line[DataPointer]&0xFF00)>>8 == 0x55) //�Ͱ�λΪ0x55
					{
						SendData[cntt] = 0xAA;cntt++;
						SendData[cntt] = 0x5A;cntt++;
					}
					else if((save_line[DataPointer]&0xFF00)>>8 == 0xAA) //�Ͱ�λΪ0xAA
					{
						SendData[cntt] = 0xAA;cntt++;
						SendData[cntt] = 0xA5;cntt++;
					}
					else
					{
						SendData[cntt] = (save_line[DataPointer]&0xFF00)>>8;cntt++;
					}
					
					if(((save_line[DataPointer]&0xFF)) == 0x55)//�߰�λΪ0x55
					{
						SendData[cntt] = 0xAA;cntt++;
						SendData[cntt] = 0x5A;cntt++;
					}
					else if(((save_line[DataPointer]&0xFF)) == 0xAA)//�߰�λΪ0x55
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
	

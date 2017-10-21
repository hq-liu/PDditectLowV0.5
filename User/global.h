#include "stm32f4xx.h"
#include "./usart/bsp_usart1.h"
#include "./usart/bsp_usart2.h"
#include "./usart/bsp_usart3.h"
#include "./spi/spi.h"
#include "./encoder/encoder.h"
#include "./tim/bsp_basic_tim.h"
#include "./gpiotest/gpiotest.h"   
#include "bsp_basic_io.h"
#include "./Algorithm/Algorithm.h"
#include "./senddata/senddata.h"


#include "delay.h"
#include "./KEY/key.h"
#include "./EXTI/exti.h"
#include "./SDRAM./sdram.h"
#include <stdio.h>

#define Distance 1000  //�������е���ʼ�ɼ���ǰһ�ξ��뿪ʼ�л�������ʽ

extern u8 WorkMode;  //Ϊ1ʱ����ģʽ��Ϊ0ʱֹͣģʽ��Ϊ3ʱdebug����ģʽ
extern u8 flag_ng;  //ok��ng���ź�
extern u8 choosealg;  //�㷨ѡ��0���������ߣ�1�������Σ�2�������䣩
extern u32 depth;   //������ȣ����û����룬��λmm
extern u32 sampling;   //���ok�źż�������û����룬��λmm
extern u32 saving;   //�������ݵļ����Ӧ����sampling��������  ��λ΢��
extern u32 position; //��Ч�źŵ���ʼ��
extern u8 sensor; //����������
extern u8 encoder; //��դ�߲���
extern u8 trigger; //�����ɼ���ʽѡ��
extern u8 Turn;  //��ת�źţ��Ѿ�ȡ��
extern u16 s;	//ѭ�������м����
extern u16 Gap; //ÿ��Gap�����ݷ���һ�Σ�Gap��Զ����saving/sampling
extern u16 envelope_up[];
extern u16 envelope_below[]; //�û����������°�����
extern u16 rectangleXY[]; //�����ȡ���Σ�4ά���飬��ʽΪ��x1,y1,x2,y2...��
extern u16 intervalXY[]; //�����ȡ���� ����ʽͬ����
extern u16 EnvelopeU_len;  //�ϰ��������ݳ���
extern u16 EnvelopeB_len;  //�°��������ݳ���
extern u16 Rctangle_len; 	//�������ݳ���
extern u16 Interval_len;	//�����㷨���ݳ���

extern u16 cntt;		//��¼SendData�ĵ�ǰ���λ��
extern u16 DataPointer;  //��¼SendData��ǰ���λ������ʵ�����еĶ�Ӧλ�ã�ÿ��Gap�����ݱ���һ������
extern u16 DataLen;  //�������ݵĳ���
extern u8 start_singnal; //Ϊ1ʱ˵����ʼ����λ�ƴ�����Ϊ0ʱ����ʱ�䴥��
extern u16 save_line[10000]; //��ʵ�������������
extern u8 SendData[4000]; //��Ҫ���͵�������������飬�Ѿ�����Э�����
extern u8 DataHead[10];	//����ͷ
extern u8 HeadCnt;	//����ͷ�ķ��������ָ��
extern u8 DataTail[3];//����β�ķ�������
extern u8 TailCnt;	//����β�ķ��������е�ָ��
extern int recive_num; //��¼��ǰ���յ������ݸ���
extern u8 Tim5State;//����TIM5��״̬ 
extern u16 num; //��¼��ǰ���յ���ѹ��ֵ��������ʵ����save_line
extern u16 y_up[10001];//x��Ӧ���������������ߵ�yֵ
extern u16 y_below[10001];  //x��Ӧ���������������ߵ�y
extern float line_k[];  //�������ֱ�߶η��̵Ĳ������������������趨20��ֵ
extern float line_b[];  //��������Ϊfloat��
extern u16 compared_num ;  //��¼�Ѿ��ȽϹ��ĵ���

extern u8 RX_data; //��λ������λ��������
extern u16 Num_RX;  //��λ������λ�����յ������ݼ���
extern u8 recive_data[20000];
extern u8 recive_start;  //����0x55Ϊ1���ٴ�����0x55��Ϊ0
extern u8 flag_r; //�������ݵı�־λ
extern u8 recive_finish;  //recive_start��1��Ϊ0��recive_finish��Ϊ1
extern u16 send_num ; //�������Ʒ������ݵ�����
extern u16 datasend;  //��¼�Ѿ����͵ĶԱȽ��
extern u16 SerialNumber; //�������
extern u16 AccumulateCheck;  //�ۼӺ�У��
extern u16 Command_Length; //�����
extern u8 Command_1;  //�������ݵ��ݴ�
extern u8 CompareRec; //�Ա��㷨�����ݽ��ձ�־λ
extern u8 Code; //������룬�ɴ˱������ֵ�ǰ�������ݵ�����

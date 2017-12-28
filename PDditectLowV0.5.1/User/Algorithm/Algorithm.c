
/**
  ******************************************************************************
  * @file    Algorithm.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   ���ļ��������ø����㷨�ĶԱ�����
						 ��������������y_up��y_below���ڴ�Ų�ͬλ���µĶԱ����䣬ÿ�α���㷨�����
						 ��Ҫ���������������Ԫ��
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#include "global.h"
void count_envelope
		(u16* e_up ,u16 e_up_len,u16* e_be ,u16 e_be_len)
{
	u16 i = 0;
	u16 x = 0;  //����ѭ�������м����
	u16 ArrayLen;
	float a=1.0; 
	float lk = 0;
	float lb = 0;
	
	ArrayLen = e_up_len * 2;
	for(i=0;i<10001;i++) 
	{
		y_up[i]=0x4000;
		y_below[i]=0x0;
	}
	if(e_up_len > 0) 
	{
		for(i=0 ; i<e_up_len-1 ; i++)
		{
			//��ȡ�߶�б�ʺ�ƫ��ֵ k=(y2-y1)/(x2-x1)
			//b=y1-k*x1
			lk = (e_up[i*2+3] - e_up[i*2+1]) / (e_up[i*2+2] - e_up[i*2+0]) ;
			lb = e_up[i*2+1] - e_up[i*2+0] * lk ;
			//���ݱ��ε��߶�ֵ��ȷ�����е�yֵ
			for(x=e_up[i*2+0] ; x<e_up[i*2+2] ; x++) y_up[x] = x * lk + lb;
		}
	}
	if(e_be_len > 0) 
	{
		for(i=0 ; i<e_be_len-1 ; i++)
		{
			//��ȡ�߶�б�ʺ�ƫ��ֵ k=(y2-y1)/(x2-x1)
			lk = (e_be[i*2+3] - e_be[i*2+1]) / (e_be[i*2+2] - e_be[i*2+0]) ;
			//b=y1-k*x1
			lb = e_be[i*2+1] - e_be[i*2+0] * lk ;
			//���ݱ��ε��߶�ֵ��ȷ�����е�yֵ
			for(x=e_be[i*2+0] ; x<e_be[i*2+2] ; x++) y_below[x] = x * lk + lb;
		}
	}
}

void get_envelope_up(u16 envelope_upl[], u16 size_up) //�������㷨,�ϰ�����ѡȡ�����ɺ������ݣ��ɼ�1000����
{
	u16 i = 0;
	u16 k = 0;  //����ѭ�������м����
	u16 ArrayLen;
	float a = 1.0;

	ArrayLen = 2 * size_up;
	if (size_up == 0) //����ǿ�����
	{
		for (i = 0; i<10000; i++)
			y_up[i] = 0x4000;
	}
	else
	{
		for (i = 0; i < ArrayLen - 2; i = i + 2) {
			y_up[envelope_upl[i]] = envelope_upl[i + 1];
		}
		for (i = 0; i < ArrayLen - 2; i = i + 2)    //������ֱ�߶η���,ÿ4������Ϊ1��
		{
			if (envelope_upl[i] != envelope_upl[i + 2]) {
				line_k[i / 2] = a* (envelope_upl[i + 1] - envelope_upl[i + 3]) / (envelope_upl[i] - envelope_upl[i + 2]);
				line_b[i / 2] = a* (envelope_upl[i + 3] * envelope_upl[i] - envelope_upl[i + 1] * envelope_upl[i + 2]) / (envelope_upl[i] - envelope_upl[i + 2]);
			}//����x1=x2�����
			else {
				if (envelope_upl[i + 1] <= envelope_upl[i + 3])
					y_up[envelope_upl[i]] = envelope_upl[i + 1];
				else
					y_up[envelope_upl[i]] = envelope_upl[i + 3];
			}
		}

		for (i = 0; i < 10000; i++) //�����ϰ���������yֵ
		{
			if ((i < envelope_upl[0]) || i > envelope_upl[ArrayLen - 2])   //������������������Կ���Ϊû������
				y_up[i] = 0x4000;   //14λAD���ֵ
			else {
				for (k = 0; k < ArrayLen; k = k + 2)
				{
					if ((i < envelope_upl[k + 2]) && (i > envelope_upl[k]))
					{	
							y_up[i] = i * line_k[k / 2] + line_b[k / 2]; //y=kx+b��������ߵ���ɢyֵ
					}
				}
			}
		}
	}
}

void get_envelope_below (u16 envelope_belowl[],u16 size_below) //�������㷨,�°�����ѡȡ�����ɺ������ݣ��ɼ�1000����
{
	u16 i = 0;
	u16 k = 0;  //����ѭ�������м����
	float a=1.0; 
	u16 ArrayLen;
	ArrayLen=2 * size_below;
	if(size_below==0) //����ǿ�����
	{
			for(i=0;i<10001;i++)
				y_below[i]=0x0000;
	}
	else
	{
		for (i = 0; i < ArrayLen - 2; i = i + 2) {
			y_below[envelope_belowl[i]] = envelope_belowl[i + 1];
		}
	for(i = 0;i < ArrayLen-2;i=i+2)    //������ֱ�߶η���,ÿ4������Ϊ1��
	 {
		 if(envelope_belowl[i] != envelope_belowl[i+2]){
			line_k[i/2] =a* (envelope_belowl[i+1] - envelope_belowl[i+3]) / (envelope_belowl[i] - envelope_belowl[i+2]);
			line_b[i/2] =a* (envelope_belowl[i+3] * envelope_belowl[i] - envelope_belowl[i+1] * envelope_belowl[i+2]) / (envelope_belowl[i] - envelope_belowl[i+2]);
		 }
		 else {
				if (envelope_belowl[i + 1] <= envelope_belowl[i + 3])
					y_up[envelope_belowl[i]] = envelope_belowl[i + 3];
				else
					y_up[envelope_belowl[i]] = envelope_belowl[i + 1];
			}
	 }
	 
	for(i = 0;i < 10000;i++) //�����ϰ���������yֵ
	 {
		 if((i< envelope_below[0]) || i > envelope_below[ArrayLen-2])   //������������������Կ���Ϊû������
			 y_below[i] = 0;   //14λAD���ֵ
		 else
			for(k = 0;k < ArrayLen ; k=k+2) 
				{ 
				  if((i< envelope_belowl[k+2])&&(i > envelope_belowl[k]))	
				  {
								y_below[i] = i * line_k[k/2] + line_b[k/2]; //y=kx+b��������ߵ���ɢyֵ
				  }
	      }
	 }
	}
}

/*rectangleXY[]����ʽΪx1,y1,x2,y2...*/
void get_rectangle_up(u16 rectangleXY[] ,u16 Rctangle_len) //��ȡ���ζԱ��㷨���ϱ߽�
{
	u16 i;
	u16 k;
	u16 ArrayLen;
	ArrayLen=2 * Rctangle_len;
	if(Rctangle_len==0) //����ǿ�����
	{
			for(i=0;i<10001;i++)
				y_up[i]=0x4000;
	}
	else
	{
	for(i=0;i<10001;i++)
	{
		for(k=0;k<ArrayLen;k=k+4)
		{
			if(i<rectangleXY[k])  //xi��x1�����,y��ֵΪ���
			{
				y_up[i]=0x4000;
				break;
			}
			else if(i<rectangleXY[k+2]) //xi��x1��x2�м䣬yֵӦΪy2
			{
				y_up[i]=rectangleXY[k+3];
				break;
			}
			else  //xi��x2�ұߣ�������һ��ѭ��,����һ�����ζԱ�
				continue;
		}
	}
	}
}

void get_rectangle_below(u16 rectangleXY[] ,u16 Rctangle_len) //��ȡ���ζԱ��㷨���±߽�
{
	u16 i;
	u16 k;
	u16 ArrayLen;
	ArrayLen=2 * Rctangle_len;
	if(Rctangle_len==0) //����ǿ�����
	{
			for(i=0;i<10001;i++)
				y_below[i]=0x0000;
	}
	else
	{
	for(i=0;i<10001;i++)
	{
		for(k=0;k<ArrayLen;k=k+4)
		{
			if(i<rectangleXY[k])  //xi��x1����ߣ�y��ֵΪ0
			{
				y_below[i]=0;
				break;
			}
			else if(i<rectangleXY[k+2]) //xi��x1��x2�м䣬yֵӦΪy1
			{
				y_below[i]=rectangleXY[k+1];
				break;
			}
			else  //xi��x2�ұߣ�������һ��ѭ��,����һ�����ζԱ�
				continue;
		}
	}
	}
}

/*intervalXY����ʽΪx1,y1,x2,y2...*/
void get_interval_up(u16 intervalXY[] ,u16 Interval_len) //��ȡ��ֵ�㷨���ϱ߽�
{
	u16 i;
	u16 k;
	u16 ArrayLen;
	ArrayLen=2 * Interval_len;
	if(Interval_len==0) //����ǿ�����
	{
			for(i=0;i<10001;i++)
				y_up[i]=0x4000;
	}
	else
	{
	for(i=0;i<10001;i++)
	{
		for(k=0;k<ArrayLen;k=k+4)  //xi��x1��x2���ʱ��yֵӦΪy1,�����ʱΪ���
		{
			if((i==intervalXY[k])||(i==intervalXY[k+2]))
			{
				y_up[i]=intervalXY[k+3];
				break;
			}
			else
				y_up[i]=0x4000;
		}
	}
	}
}

void get_interval_below(u16 intervalXY[] ,u16 Interval_len) //��ȡ��ֵ�㷨���ϱ߽�
{
	u16 i;
	u16 k;
	u16 ArrayLen;
	ArrayLen=2 * Interval_len;
	if(Interval_len==0) //����ǿ�����
	{
			for(i=0;i<10001;i++)
				y_below[i]=0x0000;
	}
	else
	{
	for(i=0;i<10001;i++)
	{
		for(k=0;k<ArrayLen;k=k+4)  //xi��x1��x2���ʱ��yֵӦΪy2,�����ʱΪ0
		{
			if((i==intervalXY[k])||(i==intervalXY[k+2]))
			{
				y_below[i]=intervalXY[k+1];
				break;
			}
			else
			{
				y_below[i]=0;
				break;
			}
		}
	}
	}
}

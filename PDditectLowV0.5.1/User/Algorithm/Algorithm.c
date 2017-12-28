
/**
  ******************************************************************************
  * @file    Algorithm.c
  * @author  lhq
  * @version V1.0
  * @date    2017-3-16
  * @brief   该文件用于配置各个算法的对比区间
						 设置了两个数组y_up和y_below用于存放不同位移下的对比区间，每次变更算法或参数
						 都要更改这两个数组的元素
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
	u16 x = 0;  //用于循环语句的中间变量
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
			//求取线段斜率和偏移值 k=(y2-y1)/(x2-x1)
			//b=y1-k*x1
			lk = (e_up[i*2+3] - e_up[i*2+1]) / (e_up[i*2+2] - e_up[i*2+0]) ;
			lb = e_up[i*2+1] - e_up[i*2+0] * lk ;
			//根据本段的线段值，确定所有的y值
			for(x=e_up[i*2+0] ; x<e_up[i*2+2] ; x++) y_up[x] = x * lk + lb;
		}
	}
	if(e_be_len > 0) 
	{
		for(i=0 ; i<e_be_len-1 ; i++)
		{
			//求取线段斜率和偏移值 k=(y2-y1)/(x2-x1)
			lk = (e_be[i*2+3] - e_be[i*2+1]) / (e_be[i*2+2] - e_be[i*2+0]) ;
			//b=y1-k*x1
			lb = e_be[i*2+1] - e_be[i*2+0] * lk ;
			//根据本段的线段值，确定所有的y值
			for(x=e_be[i*2+0] ; x<e_be[i*2+2] ; x++) y_below[x] = x * lk + lb;
		}
	}
}

void get_envelope_up(u16 envelope_upl[], u16 size_up) //包络线算法,上包络线选取点数由函数传递，采集1000个点
{
	u16 i = 0;
	u16 k = 0;  //用于循环语句的中间变量
	u16 ArrayLen;
	float a = 1.0;

	ArrayLen = 2 * size_up;
	if (size_up == 0) //如果是空数组
	{
		for (i = 0; i<10000; i++)
			y_up[i] = 0x4000;
	}
	else
	{
		for (i = 0; i < ArrayLen - 2; i = i + 2) {
			y_up[envelope_upl[i]] = envelope_upl[i + 1];
		}
		for (i = 0; i < ArrayLen - 2; i = i + 2)    //计算上直线段方程,每4个数据为1组
		{
			if (envelope_upl[i] != envelope_upl[i + 2]) {
				line_k[i / 2] = a* (envelope_upl[i + 1] - envelope_upl[i + 3]) / (envelope_upl[i] - envelope_upl[i + 2]);
				line_b[i / 2] = a* (envelope_upl[i + 3] * envelope_upl[i] - envelope_upl[i + 1] * envelope_upl[i + 2]) / (envelope_upl[i] - envelope_upl[i + 2]);
			}//避免x1=x2的情况
			else {
				if (envelope_upl[i + 1] <= envelope_upl[i + 3])
					y_up[envelope_upl[i]] = envelope_upl[i + 1];
				else
					y_up[envelope_upl[i]] = envelope_upl[i + 3];
			}
		}

		for (i = 0; i < 10000; i++) //计算上包络线所有y值
		{
			if ((i < envelope_upl[0]) || i > envelope_upl[ArrayLen - 2])   //在最左右两点以外可以考虑为没有限制
				y_up[i] = 0x4000;   //14位AD最大值
			else {
				for (k = 0; k < ArrayLen; k = k + 2)
				{
					if ((i < envelope_upl[k + 2]) && (i > envelope_upl[k]))
					{	
							y_up[i] = i * line_k[k / 2] + line_b[k / 2]; //y=kx+b计算包络线的离散y值
					}
				}
			}
		}
	}
}

void get_envelope_below (u16 envelope_belowl[],u16 size_below) //包络线算法,下包络线选取点数由函数传递，采集1000个点
{
	u16 i = 0;
	u16 k = 0;  //用于循环语句的中间变量
	float a=1.0; 
	u16 ArrayLen;
	ArrayLen=2 * size_below;
	if(size_below==0) //如果是空数组
	{
			for(i=0;i<10001;i++)
				y_below[i]=0x0000;
	}
	else
	{
		for (i = 0; i < ArrayLen - 2; i = i + 2) {
			y_below[envelope_belowl[i]] = envelope_belowl[i + 1];
		}
	for(i = 0;i < ArrayLen-2;i=i+2)    //计算上直线段方程,每4个数据为1组
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
	 
	for(i = 0;i < 10000;i++) //计算上包络线所有y值
	 {
		 if((i< envelope_below[0]) || i > envelope_below[ArrayLen-2])   //在最左右两点以外可以考虑为没有限制
			 y_below[i] = 0;   //14位AD最大值
		 else
			for(k = 0;k < ArrayLen ; k=k+2) 
				{ 
				  if((i< envelope_belowl[k+2])&&(i > envelope_belowl[k]))	
				  {
								y_below[i] = i * line_k[k/2] + line_b[k/2]; //y=kx+b计算包络线的离散y值
				  }
	      }
	 }
	}
}

/*rectangleXY[]的形式为x1,y1,x2,y2...*/
void get_rectangle_up(u16 rectangleXY[] ,u16 Rctangle_len) //获取矩形对比算法的上边界
{
	u16 i;
	u16 k;
	u16 ArrayLen;
	ArrayLen=2 * Rctangle_len;
	if(Rctangle_len==0) //如果是空数组
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
			if(i<rectangleXY[k])  //xi在x1的左边,y的值为最大
			{
				y_up[i]=0x4000;
				break;
			}
			else if(i<rectangleXY[k+2]) //xi在x1和x2中间，y值应为y2
			{
				y_up[i]=rectangleXY[k+3];
				break;
			}
			else  //xi在x2右边，进入下一个循环,与下一个矩形对比
				continue;
		}
	}
	}
}

void get_rectangle_below(u16 rectangleXY[] ,u16 Rctangle_len) //获取矩形对比算法的下边界
{
	u16 i;
	u16 k;
	u16 ArrayLen;
	ArrayLen=2 * Rctangle_len;
	if(Rctangle_len==0) //如果是空数组
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
			if(i<rectangleXY[k])  //xi在x1的左边，y的值为0
			{
				y_below[i]=0;
				break;
			}
			else if(i<rectangleXY[k+2]) //xi在x1和x2中间，y值应为y1
			{
				y_below[i]=rectangleXY[k+1];
				break;
			}
			else  //xi在x2右边，进入下一个循环,与下一个矩形对比
				continue;
		}
	}
	}
}

/*intervalXY的形式为x1,y1,x2,y2...*/
void get_interval_up(u16 intervalXY[] ,u16 Interval_len) //获取极值算法的上边界
{
	u16 i;
	u16 k;
	u16 ArrayLen;
	ArrayLen=2 * Interval_len;
	if(Interval_len==0) //如果是空数组
	{
			for(i=0;i<10001;i++)
				y_up[i]=0x4000;
	}
	else
	{
	for(i=0;i<10001;i++)
	{
		for(k=0;k<ArrayLen;k=k+4)  //xi与x1或x2相等时，y值应为y1,不相等时为最大
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

void get_interval_below(u16 intervalXY[] ,u16 Interval_len) //获取极值算法的上边界
{
	u16 i;
	u16 k;
	u16 ArrayLen;
	ArrayLen=2 * Interval_len;
	if(Interval_len==0) //如果是空数组
	{
			for(i=0;i<10001;i++)
				y_below[i]=0x0000;
	}
	else
	{
	for(i=0;i<10001;i++)
	{
		for(k=0;k<ArrayLen;k=k+4)  //xi与x1或x2相等时，y值应为y2,不相等时为0
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

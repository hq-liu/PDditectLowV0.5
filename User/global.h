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

#define Distance 1000  //插针运行到开始采集的前一段距离开始切换触发方式

extern u8 WorkMode;  //为1时工作模式，为0时停止模式，为3时debug单次模式
extern u8 flag_ng;  //ok和ng的信号
extern u8 choosealg;  //算法选择（0——包络线；1——矩形；2——区间）
extern u32 depth;   //插针深度，由用户输入，单位mm
extern u32 sampling;   //输出ok信号间隔，由用户输入，单位mm
extern u32 saving;   //保存数据的间隔，应该是sampling的整数倍  单位微米
extern u32 position; //有效信号的起始点
extern u8 sensor; //传感器参数
extern u8 encoder; //光栅尺参数
extern u8 trigger; //触发采集方式选择
extern u8 Turn;  //回转信号，已经取消
extern u16 s;	//循环语句的中间变量
extern u16 Gap; //每隔Gap个数据发送一次，Gap永远等于saving/sampling
extern u16 envelope_up[];
extern u16 envelope_below[]; //用户划定的上下包络线
extern u16 rectangleXY[]; //两点获取矩形，4维数组，形式为（x1,y1,x2,y2...）
extern u16 intervalXY[]; //两点获取区间 ，形式同矩形
extern u16 EnvelopeU_len;  //上包络线数据长度
extern u16 EnvelopeB_len;  //下包络线数据长度
extern u16 Rctangle_len; 	//矩形数据长度
extern u16 Interval_len;	//区间算法数据长度

extern u16 cntt;		//记录SendData的当前存放位置
extern u16 DataPointer;  //记录SendData当前存放位置在真实数组中的对应位置，每隔Gap个数据保存一个数据
extern u16 DataLen;  //发送数据的长度
extern u8 start_singnal; //为1时说明开始进入位移触发，为0时进入时间触发
extern u16 save_line[10000]; //真实数据所存的数组
extern u8 SendData[4000]; //需要发送的数据所存的数组，已经按照协议算好
extern u8 DataHead[10];	//数据头
extern u8 HeadCnt;	//数据头的发送数组的指针
extern u8 DataTail[3];//数据尾的发送数组
extern u8 TailCnt;	//数据尾的发送数组中的指针
extern int recive_num; //记录当前接收到的数据个数
extern u8 Tim5State;//控制TIM5的状态 
extern u16 num; //记录当前接收到的压力值，放入真实数组save_line
extern u16 y_up[10001];//x对应的上下两条包络线的y值
extern u16 y_below[10001];  //x对应的上下两条包络线的y
extern float line_k[];  //计算各个直线段方程的参数，包络线最大可以设定20个值
extern float line_b[];  //参数必须为float型
extern u16 compared_num ;  //记录已经比较过的点数

extern u8 RX_data; //上位机到下位机的数据
extern u16 Num_RX;  //上位机到下位机接收到的数据计数
extern u8 recive_data[20000];
extern u8 recive_start;  //遇到0x55为1，再次遇到0x55变为0
extern u8 flag_r; //接收数据的标志位
extern u8 recive_finish;  //recive_start由1变为0，recive_finish变为1
extern u16 send_num ; //用来控制发送数据的类型
extern u16 datasend;  //记录已经发送的对比结果
extern u16 SerialNumber; //数据序号
extern u16 AccumulateCheck;  //累加和校验
extern u16 Command_Length; //命令长度
extern u8 Command_1;  //接收数据的暂存
extern u8 CompareRec; //对比算法的数据接收标志位
extern u8 Code; //命令代码，由此变量区分当前接收数据的类型

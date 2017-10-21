/**
  ******************************************************************************
  * @file    Global.c
  * @author  lhq
  * @version V1.1
  * @date    2017-3-16
  * @brief   该文件存储了本程序中通用变量
  ******************************************************************************
  * @attention 无

  ******************************************************************************
  */


#include "global.h"	

u8 		WorkMode=0x01;  	//为1时工作模式，为0时停止模式，为3时debug单次模式
u8 		flag_ng=0;  			//ok和ng的信号，为0时表示ok，默认为0；为1时表示ng，并发出ng信号
u8 		choosealg=0x01;  	//算法选择（0——包络线；1——矩形；2——区间）
u32 	depth=30000;   		//插针深度，由用户输入，单位mm
u32 	sampling=4;   		//输出ok信号间隔，由用户输入，单位mm
u32 	saving=40;   		//保存数据的间隔，应该是sampling的整数倍  单位微米
u32 	position=10000; 	//有效信号的起始点
u8 		sensor=0x01; 			//传感器参数
u8 		encoder=0x01; 		//光栅尺参数
u8 		trigger = 0x01; 	//触发采集方式选择
u8 		Turn=0;   				//回转信号，已经取消
u16 	s;   							//循环语句的中间变量
u16 	Gap=0;  					//每隔Gap个数据发送一次，Gap永远等于saving/sampling
u16 	EnvelopeU_len=0;  //上包络线数据长度
u16 	EnvelopeB_len=0;  //下包络线数据长度
u16 	Rctangle_len=0; 	//矩形数据长度
u16 	Interval_len=0;		//区间算法数据长度
u16 	envelope_up[10000] 		__attribute__((at(0XC0000000)));
u16 	envelope_below[10000] __attribute__((at(0XC0010000))); //用户划定的上下包络线
u16 	rectangleXY[10000] 		__attribute__((at(0XC0020000))); //两点获取矩形，4维数组，形式为（x1,y1,x2,y2...）
u16 	intervalXY[10000] 		__attribute__((at(0XC0030000))); //两点获取区间 ，形式同矩形

u16 	cntt=0;							//记录SendData的当前存放位置
u16 	DataPointer=0;  		//记录SendData当前存放位置在真实数组中的对应位置，每隔Gap个数据保存一个数据
u16 	DataLen;  					//发送数据的长度
u8 		start_singnal=0; 		//为1时说明开始进入位移触发，为0时进入时间触发
u16 	save_line[10000];  	//真实数据所存的数组
u8 		SendData[4000];			//需要发送的数据所存的数组，已经按照协议算好
u8 		DataHead[10]; 			//数据头
u8 		HeadCnt=0;   				//数据头的发送数组的指针
u8 		DataTail[3];				//数据尾的发送数组
u8 		TailCnt=0;					//数据尾的发送数组中的指针
int 	recive_num=0;				//记录当前接收到的数据个数
u16 	num=0;							//记录当前接收到的压力值，参与对比并放入真实数组save_line
u8 		Tim5State=0; 				//控制TIM5的状态 
u16 	y_up[10001];				//x对应的上下两条包络线的y值
u16 	y_below[10001];  		//x对应的上下两条包络线的y
float line_k[10000] 	__attribute__((at(0XC0040000)));  //计算各个直线段方程的参数，包络线最大可以设定20个值
float line_b[10000] 	__attribute__((at(0XC0060000)));  //参数必须为float型
u16 	compared_num = 0;  	//记录已经比较过的点数


/*通信变量*/
u8 RX_data; 					//上位机到下位机的数据
u16 Num_RX;  					//上位机到下位机接收到的数据计数
u8 recive_data[20000] __attribute__((at(0XC0080000)));
u8 recive_start=0;  	//遇到0x55为1，再次遇到0x55变为0
u8 flag_r=0;   				//接收数据的标志位
u8 recive_finish=0;  	//recive_start由1变为0，recive_finish变为1
u16 send_num = 0; 		//用来控制发送数据的类型
u16 datasend=0;  			//记录已经发送的对比结果
u16 SerialNumber=0; 	//数据序号
u16 AccumulateCheck=0x00;  //累加和校验
u16 Command_Length=0; //命令长度
u8 Command_1=0;  			//接收数据的暂存
u8 CompareRec=0; 			//对比算法的数据接收标志位  
u8 Code=0;   					//命令代码，由此变量区分当前接收数据的类型

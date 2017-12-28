#include "stm32f4xx.h"

#define CapValue 							1000    //反馈电容值单位pF
#define PressureSensitivity  	9.6   	//压力传感器灵敏度单位 pC/N
#define ADRef								 	4				//AD参考电压
#define ADResolution					16384		//14位结果，2的14次方

//(ADRef * CapValue) / PressureSensitivity  //416N 取整得400
#define MeasureRange0					400


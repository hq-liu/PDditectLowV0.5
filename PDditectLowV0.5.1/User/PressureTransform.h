#include "stm32f4xx.h"

#define CapValue 							1000    //��������ֵ��λpF
#define PressureSensitivity  	9.6   	//ѹ�������������ȵ�λ pC/N
#define ADRef								 	4				//AD�ο���ѹ
#define ADResolution					16384		//14λ�����2��14�η�

//(ADRef * CapValue) / PressureSensitivity  //416N ȡ����400
#define MeasureRange0					400


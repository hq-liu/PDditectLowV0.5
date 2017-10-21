#include "PressureTransform.h"


 /**
  * @brief  压力值变换为AD数字量
  * @param  压力值
  * @retval 对应的AD数字量
  */
u16 Pre_2_ADValue(float pressure)
{
	float a_Vol;
	u16 adValue;
	a_Vol = ( pressure * PressureSensitivity ) / CapValue;
	adValue = ( a_Vol / ADRef ) * ADResolution;
	return adValue;
}

 /**
  * @brief  AD数字量变换为压力值
  * @param  AD数字量
  * @retval 对应的压力值
  */
float ADValue_2_Pre(u16 ADValue)
{
	float Pressure;
	float A_Vol;
	A_Vol = (ADValue * ADRef) / ADResolution;
	Pressure = (A_Vol * CapValue) / PressureSensitivity;
	return Pressure;
}


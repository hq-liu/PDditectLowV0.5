#include "PressureTransform.h"


 /**
  * @brief  ѹ��ֵ�任ΪAD������
  * @param  ѹ��ֵ
  * @retval ��Ӧ��AD������
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
  * @brief  AD�������任Ϊѹ��ֵ
  * @param  AD������
  * @retval ��Ӧ��ѹ��ֵ
  */
float ADValue_2_Pre(u16 ADValue)
{
	float Pressure;
	float A_Vol;
	A_Vol = (ADValue * ADRef) / ADResolution;
	Pressure = (A_Vol * CapValue) / PressureSensitivity;
	return Pressure;
}


#ifndef __HX711_H
#define __HX711_H

#include "sys.h"

#define HX711_SCK PCout(4)// PB0
#define HX711_DOUT PCin(5)// PB1

//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValueL 1597.2
#define GapValueM1 1680.2
#define GapValueM2 1629.2
#define GapValueM3 1636.2
#define GapValueH 1678.2

extern void HX711Init(void);
extern u32 HX711_Read(void);
extern void Get_Maopi(void);
extern void Get_Weight(void);

extern u8 Flag_Error;
extern u32 HX711_Buffer;
//extern u32 Weight_Maopi;
//extern s32 Weight_Shiwu;
extern float Weight_Maopi;
extern float Weight_Shiwu;
extern float Get_Weight_Average(u8 times);

#endif

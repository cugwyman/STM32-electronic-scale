#ifndef __HX711_H
#define __HX711_H

#include "sys.h"

#define HX711_SCK PCout(4)// PB0
#define HX711_DOUT PCin(5)// PB1


extern void Init_HX711pin(void);
extern u32 HX711_Read(void);
extern void Get_Maopi(void);
extern void Get_Weight(void);

extern u32 HX711_Buffer;
//extern u32 Weight_Maopi;
//extern s32 Weight_Shiwu;
extern float Weight_Maopi;
extern float Weight_Shiwu;
extern u8 Flag_Error;
float Get_Weight_Shiwu_Average(float ch,u8 times);

#endif

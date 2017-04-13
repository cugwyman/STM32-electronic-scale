#ifndef __KEY_H
#define __KEY_H
#include "sys.h"
#define KEY0 PAin(13)   //PA13
#define KEY1 PAin(15)	//PA15 
#define KEY2 PAin(0)	//PA0  WK_UP


#define HPin  GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0
#define LPin  GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4
//#define HRead();  GPIO_ReadInputData(GPIOD);
//#define LRead();  GPIO_ReadInputData(GPIOD);
void KEYInit(void);
u8 KEY_Scan(void);
u8 HL_Scan(void);

void HL_Init_HScan(void);
void HL_Init_LScan(void);

extern u8 key_value;
extern u8 key;

#endif

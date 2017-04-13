#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "exti.h"
#include "lcd.h"
#include "rtc.h"
#include "HX711.h"
#include "ds18b20.h"
#include "timer.h"

void GeneralInit(void);
void DisplayInit(void);
void Weight(void);

extern float RTC_weight; 

#endif


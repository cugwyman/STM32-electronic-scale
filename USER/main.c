#include "System.h"

int main(void)
{
    GeneralInit();
    while(1)	
    {
        RTC_weight=Get_Weight_Average(11);               //≤‚ ‘‘› ±’‚—˘
        delay_ms(10); 
	}	 
}



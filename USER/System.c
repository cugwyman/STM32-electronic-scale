#include "System.h"

float RTC_weight; 

void DisplayInit()
{
    POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,70,"Welcome!");
	LCD_ShowString(60,90,"temperature:   . C");	
    LCD_ShowString(60,110,"CUG@Wyman");	
	//显示提示信息
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(60,130,"value_per_g:");	      
	LCD_ShowString(60,150,"    0.00yuan");	      
	LCD_ShowString(60,170,"weight:");	      
	LCD_ShowString(60,190,"    0.0g");	      
	LCD_ShowString(60,210,"value:");	      
	LCD_ShowString(60,230,"    0.00yuan");	      
	LCD_ShowString(60,250,"sum:");	      
	LCD_ShowString(60,270,"    0.00yuan");	      
}

void Weight()
{
    float temp_gf,temp_vaif,temp_sumf;//display g
    u16 temp_g,temp_v,temp_vf,temp_vai,temp_sum,num1;    //display g
  
    u8 i=0;                              //key
    u32 last_ReadValue;
    float sum,num[100];

    if(Read)
	{
        switch(Read)
        {
            case 1:num1=num1*10+Read;last_ReadValue=Read;break;
            case 2:num1=num1*10+Read;last_ReadValue=Read;break;
            case 3:num1=num1*10+Read;last_ReadValue=Read;break;
            case 4:num1=num1*10+Read;last_ReadValue=Read;break;
            case 5:num1=num1*10+Read;last_ReadValue=Read;break;
            case 6:num1=num1*10+Read;last_ReadValue=Read;break;
            case 7:num1=num1*10+Read;last_ReadValue=Read;break;
            case 8:num1=num1*10+Read;last_ReadValue=Read;break;
            case 9:num1=num1*10+Read;last_ReadValue=Read;break;
            case 10:num1*=10;last_ReadValue=Read;             break;
            case 11:num1=(num1-last_ReadValue)/10;                 break;
            case 12:num1=0;                                        break;               	
            case 13:Get_Maopi();  
                    RTC_weight=Get_Weight_Average(11);             
                    temp_g=RTC_weight;
                    LCD_ShowNum(76,190,temp_g,3,16);//显示重量
                    temp_gf=RTC_weight-temp_g;
                    temp_gf=(int)(temp_gf*10);
                    LCD_ShowNum(108,190,temp_gf,1,16);
                    if(RTC_weight>500)
                    {
                        LCD_ShowString(60,30,"warning!");	      
                    }
                    break; 
            case 14:for(i=0;i<100;i++)	
                    {
                        num[i]=0;
					}
					i=0;sum=0;
					temp_vai=num[i];temp_vaif=num[i];
                    LCD_ShowNum(60,230,temp_vai,5,16);//显示value1
					temp_vaif-=temp_vai;
                    temp_vaif=(int)(temp_vaif*100);
                    LCD_ShowNum(108,230,temp_vaif,2,16);
					temp_sum=sum;temp_sumf=sum;
                    LCD_ShowNum(60,270,temp_sum,5,16);//显示value1
					temp_sumf-=temp_sum;
                    temp_sumf=(int)(temp_sumf*100);
                    LCD_ShowNum(108,270,temp_sumf,2,16);
                    break;
			case 15:num[i]=RTC_weight*num1/100;
	                temp_g=RTC_weight;
	                LCD_ShowNum(76,190,temp_g,3,16);//显示重量//188
	                temp_gf=RTC_weight-temp_g;
	                temp_gf=(int)(temp_gf*10);
	                LCD_ShowNum(108,190,temp_gf,1,16);//236
					temp_vai=num[i];temp_vaif=num[i];
                    LCD_ShowNum(60,230,temp_vai,5,16);//显示value1
					temp_vaif-=temp_vai;
                    temp_vaif=(int)(temp_vaif*100);
                    LCD_ShowNum(108,230,temp_vaif,2,16);
					num1=0;
					i++;
					break;
            case 16:for(i=0;i<100;i++)	
                    {
						sum+=num[i];
					}
					temp_sum=sum;temp_sumf=sum;
                    LCD_ShowNum(60,270,temp_sum,5,16);//显示value1
					temp_sumf-=temp_sum;
                    temp_sumf=(int)(temp_sumf*100);
                    LCD_ShowNum(108,270,temp_sumf,2,16);
					sum=0;
                    break;
			default:break;
		}
		temp_v=num1/100;
		LCD_ShowNum(60,150,temp_v,5,16);//显示value
        temp_vf=(int)(num1%100);
        LCD_ShowNum(108,150,temp_vf,2,16);

	}
}

void GeneralInit()
{
    SystemInit();
    delay_init(72);	     //延时初始化
    HX711Init();
 	uart_init(115200);
	KEYInit();
    LCD_Init();
    DisplayInit();
    delay_ms(1000);
	Get_Maopi();		 //获取毛皮重量
    TIM3_Int_Init(100-1,7200-1);//10ms中断 
}

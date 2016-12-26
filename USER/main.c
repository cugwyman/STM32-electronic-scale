#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "exti.h"
#include "pwm.h"
#include "lcd.h"
#include "rtc.h"
#include "HX711.h"
#include "ds18b20.h"

//extern u32 Read;
//extern u32 ReadValue;


u8 key_value;

float Get_Weight_Average(u8 times);

 int main(void)
 {
   float RTC_weight,temp_gf,temp_vaif,temp_sumf;//display g
	 u16 temp_g,temp_v,temp_vf,temp_vai,temp_sum,num1;    //display g
	  u8 t=0;	//RTC
 	  	short temperature;   	 

	 u8 i=0;                              //key
	 u32 ReadValue,last_ReadValue;
   float sum,num[100];
	 
	 u8 key;
	 
	 
	 num1=0;
	 
	 	Init_HX711pin();

	 SystemInit();
	delay_init(72);	     //延时初始化
	NVIC_Configuration();
 	uart_init(9600);
// 	LED_Init();
	KEY_Init();
	LCD_Init();
	
//	TIM2_Int_Init(100-1,7200-1);//10ms中断 
	
//	RTC_Init();
//  TIM3_Int_Init(100,7199);
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
//	POINT_COLOR=BLUE;//设置字体为蓝色
//	LCD_ShowString(20,130,"value_per_g(yuan):");	      
//	LCD_ShowString(20,150,"   10.54     1.25    5.36");	      
//	LCD_ShowString(20,170,"weight(g):");	      
//	LCD_ShowString(20,190,"  135.78   250.42   18.33");	      
//	LCD_ShowString(20,210,"value(yuan):");	      
//	LCD_ShowString(20,230," 1431.12   313.03   98.25");	      
//	LCD_ShowString(20,250,"sum(yuan):");	      
//	LCD_ShowString(20,270,"                  1842.40");	      

	Get_Maopi();				//称毛皮重量                           临时注释
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();				//重新获取毛皮重量


//		printf("毛皮 = %0.1f g\r\n",Weight_Maopi); //打印 
	//	Get_Weight();
	//Weight_Maopi=Weight_Shiwu;
	//		printf("毛皮 = %0.1f g\r\n",Weight_Shiwu); //打印 



//while(1)
//{
//	key=HL_Scan();
//	if(key!=0)
//	{
//		printf("key=%d\r\n",key);
//		key=0;
////		delay_ms(100);
//	}
//}


	while(1)
	{	
	  RTC_weight=Get_Weight_Average(11);               //测试暂时这样
//TIM_Cmd(TIM2, ENABLE);  //关闭TIM2
			printf("净重量 = %0.1f g\r\n",RTC_weight); //打印 
	temp_g=RTC_weight;

		LCD_ShowNum(76,190,temp_g,3,16);//显示重量//188
		temp_gf=RTC_weight-temp_g;
		temp_gf=(int)(temp_gf*10);
		LCD_ShowNum(108,190,temp_gf,1,16);//236
			if(RTC_weight>500)
			{
				LCD_ShowString(60,30,"warning!");	      

			}

				t=KEY_Scan();//得到键值
	   	if(t)
		{						  
        						printf("key = %d \r\n",t); //打印 

			switch(t)
			{				 
				case 2:
  RTC_weight=Get_Weight_Average(11);               //测试暂时这样
//TIM_Cmd(TIM2, ENABLE);  //关闭TIM2
			printf("净重量 = %0.1f g\r\n",RTC_weight); //打印 
	temp_g=RTC_weight;

		LCD_ShowNum(76,190,temp_g,3,16);//显示重量//188
		temp_gf=RTC_weight-temp_g;
		temp_gf=(int)(temp_gf*10);
		LCD_ShowNum(108,190,temp_gf,1,16);//236
			if(RTC_weight>500)
			{
				LCD_ShowString(60,30,"warning!");	      

			}
					break;
				case 1:
			while(1)	
			{
				key_value=HL_Scan();
						       	printf("%d \n",key_value); //打印 

					if(key_value)
	         {

		    			switch(key_value)
              {
               	case 1:num1=num1*10+key_value;last_ReadValue=key_value;break;
               	case 2:num1=num1*10+key_value;last_ReadValue=key_value;break;
               	case 3:num1=num1*10+key_value;last_ReadValue=key_value;break;
               	case 4:num1=num1*10+key_value;last_ReadValue=key_value;break;
               	case 5:num1=num1*10+key_value;last_ReadValue=key_value;break;
               	case 6:num1=num1*10+key_value;last_ReadValue=key_value;break;
               	case 7:num1=num1*10+key_value;last_ReadValue=key_value;break;
               	case 8:num1=num1*10+key_value;last_ReadValue=key_value;break;
               	case 9:num1=num1*10+key_value;last_ReadValue=key_value;break;
               	case 10:num1*=10;last_ReadValue=key_value;printf("ReadValue=%d",key_value);printf("\n");break;
               	case 11:num1=(num1-last_ReadValue)/10;printf("ReadValue=%d",key_value);printf("\n");break;
               	case 12:num1=0;printf("ReadValue=%d",key_value);printf("\n");break;               	
								case 13:	Get_Maopi();  RTC_weight=Get_Weight_Average(11);               //测试暂时这样
//TIM_Cmd(TIM2, ENABLE);  //关闭TIM2
			printf("净重量 = %0.1f g\r\n",RTC_weight); //打印 
	temp_g=RTC_weight;

		LCD_ShowNum(76,190,temp_g,3,16);//显示重量//188
		temp_gf=RTC_weight-temp_g;
		temp_gf=(int)(temp_gf*10);
		LCD_ShowNum(108,190,temp_gf,1,16);//236
			if(RTC_weight>500)
			{
				LCD_ShowString(60,30,"warning!");	      

			}
printf("ReadValue=%d",key_value);printf("\n");break;         //去皮后的按键重量情况需要修改 RTC_weight1,把重量那边的毛皮变量extern，赋予新值；    	
                case 14:for(i=0;i<100;i++)	
                              {
																num[i]=0;
															//	sure_weight[i]=0;
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
                              printf("ReadValue=%d",key_value);printf("\n");break;               	
                        //sure_weight[i]=RTC_weight;
						  	 case 15: 			  num[i]=RTC_weight*num1/100;
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
															    printf("ReadValue=%d",key_value);
								                  printf("\n");
								                  printf("本物品=%0.2f",num[i]);				
				                      		printf("\n");		
								                  num1=0;
															    i++;
//															    if(ReadValue!=15)
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
															    printf("ReadValue=%d",key_value);
																	printf("\n");	
				                      		printf("总价=%0.2f",sum);				
				                      		printf("\n");		
															    sum=0;
                                  break;
								default:		break;
				        }
//				key_value=0;			
								temp_v=num1/100;
		      LCD_ShowNum(60,150,temp_v,5,16);//显示value
  //    OLED_Refresh_Gram();
      temp_vf=(int)(num1%100);
      
      LCD_ShowNum(108,150,temp_vf,2,16);

		}
					 				t=KEY_Scan();//得到键值
	   	if(t)break;

	}
  

					break;
			}



		}else delay_ms(10); 

	}	 

//温度   
		

		//printf("t1:%d\n",temperature);

//重量   
//if(key%10==0)
//{
////TIM_Cmd(TIM2, DISABLE);  //关闭TIM2
//  RTC_weight=Get_Weight_Average(11);               //测试暂时这样
////TIM_Cmd(TIM2, ENABLE);  //关闭TIM2
//			printf("净重量 = %0.1f g\r\n",RTC_weight); //打印 
//	temp_g=RTC_weight;

//		LCD_ShowNum(76,190,temp_g,3,16);//显示重量//188
//		temp_gf=RTC_weight-temp_g;
//		temp_gf=(int)(temp_gf*10);
//		LCD_ShowNum(108,190,temp_gf,1,16);//236
//delay_ms(200);


//键盘
			





														 
			
//}
//key++;
//		key=HL_Scan();

//	delay_ms(200);

 
 
 }


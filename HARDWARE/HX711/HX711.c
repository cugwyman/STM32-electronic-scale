#include "HX711.h"
#include "delay.h"
#include "usart.h"

u32 HX711_Buffer;
float Weight_Maopi;
float Weight_Shiwu;
u8 Flag_Error = 0;


void HX711Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PF端口时钟

	//HX711_SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
	
	//HX711_DOUT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//输入上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);  
	
	GPIO_SetBits(GPIOC,GPIO_Pin_4);					//初始化设置为0
}


//****************************************************
//读取HX711
//****************************************************
u32 HX711_Read(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_DOUT=1; 
	delay_us(1);
  	HX711_SCK=0; 
  	count=0; 
  	while(HX711_DOUT); 
    for(i=0;i<24;i++)
	{ 
	  	HX711_SCK=1; 
	  	count=count<<1; 
		delay_us(1);
		HX711_SCK=0; 
	  	if(HX711_DOUT)
			count++; 
		delay_us(1);
	} 
 	HX711_SCK=1; 
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	delay_us(1);
	HX711_SCK=0;  
	return(count);
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi(void)
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//称重
//****************************************************
void Get_Weight(void)
{
	HX711_Buffer = HX711_Read();
	if(HX711_Buffer > Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。
	
		if(Weight_Shiwu<=100000)
			Weight_Shiwu = ((float)Weight_Shiwu/GapValueL); 	//计算实物的实际重量
		
		if(Weight_Shiwu>100000&&Weight_Shiwu<300000)
			Weight_Shiwu = ((float)Weight_Shiwu/GapValueM1); 	//计算实物的实际重量
		
		if(Weight_Shiwu>=300000&&Weight_Shiwu<=450000)
			Weight_Shiwu = ((float)Weight_Shiwu/GapValueM2); 	//计算实物的实际重量
	
		if(Weight_Shiwu>450000&&Weight_Shiwu<700000)
			Weight_Shiwu = ((float)Weight_Shiwu/GapValueM3); 	//计算实物的实际重量

		if(Weight_Shiwu>=700000)
			Weight_Shiwu = ((float)Weight_Shiwu/GapValueH); 	//计算实物的实际重量
    }

}

//中位数滤波，times必须为奇数
//BubbleSort冒泡排序
//@param flag	优化算法
float Get_Weight_Average(u8 times)
{
	float temp_val;
	float val_buf[11];
	u8 t,i,j;
    int flag = 1;

	for(t=0; t<times; t++)
	{
		Get_Weight();
		val_buf[t] = Weight_Shiwu;
		delay_ms(5);
	}
	for(j = 0; j < times && flag; j++)       
	{
        flag = 0;
		for(i = 1; i< times- j; i++)
		{
			if(val_buf[i] > val_buf[i+1])
			{
                temp_val=val_buf[i];
				val_buf[i]=val_buf[i+1];
				val_buf[i+1]=temp_val;
                flag = 1;
			}
		} 	 
    }
    return val_buf[(times-1)/2];//取中值
}



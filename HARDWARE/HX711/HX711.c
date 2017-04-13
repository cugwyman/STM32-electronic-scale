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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PF�˿�ʱ��

	//HX711_SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB
	
	//HX711_DOUT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);  
	
	GPIO_SetBits(GPIOC,GPIO_Pin_4);					//��ʼ������Ϊ0
}


//****************************************************
//��ȡHX711
//****************************************************
u32 HX711_Read(void)	//����128
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
    count=count^0x800000;//��25�������½�����ʱ��ת������
	delay_us(1);
	HX711_SCK=0;  
	return(count);
}

//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi(void)
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//����
//****************************************************
void Get_Weight(void)
{
	HX711_Buffer = HX711_Read();
	if(HX711_Buffer > Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//��ȡʵ���AD������ֵ��
	
		if(Weight_Shiwu<=100000)
			Weight_Shiwu = ((float)Weight_Shiwu/GapValueL); 	//����ʵ���ʵ������
		
		if(Weight_Shiwu>100000&&Weight_Shiwu<300000)
			Weight_Shiwu = ((float)Weight_Shiwu/GapValueM1); 	//����ʵ���ʵ������
		
		if(Weight_Shiwu>=300000&&Weight_Shiwu<=450000)
			Weight_Shiwu = ((float)Weight_Shiwu/GapValueM2); 	//����ʵ���ʵ������
	
		if(Weight_Shiwu>450000&&Weight_Shiwu<700000)
			Weight_Shiwu = ((float)Weight_Shiwu/GapValueM3); 	//����ʵ���ʵ������

		if(Weight_Shiwu>=700000)
			Weight_Shiwu = ((float)Weight_Shiwu/GapValueH); 	//����ʵ���ʵ������
    }

}

//��λ���˲���times����Ϊ����
//BubbleSortð������
//@param flag	�Ż��㷨
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
    return val_buf[(times-1)/2];//ȡ��ֵ
}



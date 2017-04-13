#include "key.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

void KEYInit(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化KEY0-->GPIOA.13,KEY1-->GPIOA.15  上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

u8 KEY_Scan(void)
{	 
	static u8 key_up=1;//按键按松开标志	
 
 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	if(key_up&&(KEY0==0||KEY1==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)
		{
			 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
			return 2;
		}
		else if(KEY1==0)
		{ 
	     	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
			return 1;
		}
	}else if(KEY0==1&&KEY1==1)key_up=1; 	    
 
 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	return 0;// 无按键按下
}

//行置为输入 默认为1 列置为输出默认为0	扫描行 
void HL_Init_HScan(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
     GPIO_InitStructure.GPIO_Pin=HPin;
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	 GPIO_Init(GPIOA,&GPIO_InitStructure); 

	 GPIO_InitStructure.GPIO_Pin=LPin;
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA,&GPIO_InitStructure); 
	 GPIO_ResetBits(GPIOA,LPin);
}

//行置为输出 默认为0 列置为输入默认为1	扫描列 
void HL_Init_LScan(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
     GPIO_InitStructure.GPIO_Pin=LPin;
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	 GPIO_Init(GPIOA,&GPIO_InitStructure); 

	 GPIO_InitStructure.GPIO_Pin=HPin;
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA,&GPIO_InitStructure); 
	 GPIO_ResetBits(GPIOA,HPin);
}

u8 HL_Scan(void)
{
	static u8 key_Up=1;//按键按松开标志	

    u16 temp=0;
	u8 key=0;
	HL_Init_HScan();
	temp=GPIO_ReadInputData(GPIOA);
    temp&=0x000f; //得到低四位数据 检测4个2进制数中0的存在
	if(key_Up&&(temp==0x0e||temp==0x0d||temp==0x0b||temp==0x07))
	{
		delay_ms(10);//去抖动 
        key_Up=0;
		if(temp==0x0e)//第一行有按键被按下
            key=1	;
        else if(temp==0x0d)
            key=2;
        else if(temp==0x0b)
            key=3;
        else if(temp==0x07)
            key=4;
    }
	else
	{
		key=0;
		if(temp==0x0f)key_Up=1;
	}
    HL_Init_LScan();
    temp=GPIO_ReadInputData(GPIOA);
    temp&=0x00f0;//得到高四位 即列数据 
    if((temp==0xe0)&&(key!=0))//第一列被按下
        key=(key-1)*4+1;
    else if((temp==0xd0)&&(key!=0))
        key=(key-1)*4+2;
    else if((temp==0xb0)&&(key!=0))
        key=(key-1)*4+3;
    else if((temp==0x70)&&(key!=0))
        key=(key-1)*4+4;
    else key=0;
    
    return key;
}




#include "sys.h"
#include "delay.h"
#include "key.h"

void InitKey(void) //初始化矩阵键盘要使用的GPIO口
{
   GPIO_InitTypeDef GPIO_InitStructure;

   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  //定义PA0到PA3为推挽输出
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
   
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
   GPIO_Init(GPIOA,&GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU; //定义PC4到PC7为上拉输入
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
   GPIO_Init(GPIOA,&GPIO_InitStructure);
}

int key(void)  //实现矩阵键盘。返回值为，各按键的键值，此键值由用户自己定义
{
   u32 KeyVal;  //KeyVal为最后返回的键值
   //u16 WriteVal=0;   //WriteVal为要写给PA口的数据

   GPIO_Write(GPIOA,(GPIOA->ODR&0xfff0|0x0));//先让PA0到PA3全部输出低

   if((GPIOA->IDR&0x00f0)==0x0010)  //如果，PC4到PC7全为1，则，没有键按下。此时，返回值为-1
       return -1;
    delay_ms(10);	//延时5ms去抖动
	if((GPIOA->IDR&0x00f0)==0x0000)  //如果，延时5ms后，PC4到PC7又全为0，则，刚才引脚的电位变化是抖动产生的
	   return -1;
   
   GPIO_Write(GPIOA,(GPIOA->ODR&0xfff0|0xe));//让PA3到PA0输出二进制的0001.
   switch(GPIOA->IDR&0x00f0)     //对PC4到PC7的值进行判断，以输出不同的键值
   {
       case 0x00e0:KeyVal=0;  break;
	   case 0x00d0:KeyVal=1;  break;
	   case	0x00b0:KeyVal=2;   break;
	   case 0x0070:KeyVal=3;   break;
   }
   GPIO_Write(GPIOA,(GPIOA->ODR&0xfff0|0xd));//让PA3到PA0输出二进制的0010
   switch(GPIOA->IDR&0x00f0)				//对PC4到PC7的值进行判断，以输出不同键值
   {
      case 0x00e0:KeyVal=4;  break;
	  case 0x00d0:KeyVal=5;  break;
	  case 0x00b0:KeyVal=6;   break;
	  case 0x0070:KeyVal=7;   break;
   }
   GPIO_Write(GPIOA,(GPIOA->ODR&0xfff0|0xb)); //让PA3到PA0输出二进制的0100
   switch(GPIOA->IDR&0x00f0)				  //对PC4到PC7的值进行判断，以输出不同的键值
   {
      case 0x00e0:KeyVal=8;  break;
	  case 0x00d0:KeyVal=9;   break;
	  case 0x00b0:KeyVal=10;   break;
	  case 0x0070:KeyVal=11;   break;
   } 
   GPIO_Write(GPIOA,(GPIOA->ODR&0xfff0|0x7)); //让PA3到PA0输出二进制的1000
   switch(GPIOA->IDR&0x00f0)                  //对PC4到PC7的值进行判断，以输出不同的键值
   {
      case 0x00e0:KeyVal=12;  break;
	  case 0x00d0:KeyVal=13;   break;
	  case 0x00b0:KeyVal=14;   break;
	  case 0x0070:KeyVal=15;   break;
   }
   return KeyVal;
}

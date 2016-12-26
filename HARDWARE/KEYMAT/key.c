#include "sys.h"
#include "delay.h"
#include "key.h"

void InitKey(void) //��ʼ���������Ҫʹ�õ�GPIO��
{
   GPIO_InitTypeDef GPIO_InitStructure;

   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  //����PA0��PA3Ϊ�������
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
   
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
   GPIO_Init(GPIOA,&GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU; //����PC4��PC7Ϊ��������
   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
   GPIO_Init(GPIOA,&GPIO_InitStructure);
}

int key(void)  //ʵ�־�����̡�����ֵΪ���������ļ�ֵ���˼�ֵ���û��Լ�����
{
   u32 KeyVal;  //KeyValΪ��󷵻صļ�ֵ
   //u16 WriteVal=0;   //WriteValΪҪд��PA�ڵ�����

   GPIO_Write(GPIOA,(GPIOA->ODR&0xfff0|0x0));//����PA0��PA3ȫ�������

   if((GPIOA->IDR&0x00f0)==0x0010)  //�����PC4��PC7ȫΪ1����û�м����¡���ʱ������ֵΪ-1
       return -1;
    delay_ms(10);	//��ʱ5msȥ����
	if((GPIOA->IDR&0x00f0)==0x0000)  //�������ʱ5ms��PC4��PC7��ȫΪ0���򣬸ղ����ŵĵ�λ�仯�Ƕ���������
	   return -1;
   
   GPIO_Write(GPIOA,(GPIOA->ODR&0xfff0|0xe));//��PA3��PA0��������Ƶ�0001.
   switch(GPIOA->IDR&0x00f0)     //��PC4��PC7��ֵ�����жϣ��������ͬ�ļ�ֵ
   {
       case 0x00e0:KeyVal=0;  break;
	   case 0x00d0:KeyVal=1;  break;
	   case	0x00b0:KeyVal=2;   break;
	   case 0x0070:KeyVal=3;   break;
   }
   GPIO_Write(GPIOA,(GPIOA->ODR&0xfff0|0xd));//��PA3��PA0��������Ƶ�0010
   switch(GPIOA->IDR&0x00f0)				//��PC4��PC7��ֵ�����жϣ��������ͬ��ֵ
   {
      case 0x00e0:KeyVal=4;  break;
	  case 0x00d0:KeyVal=5;  break;
	  case 0x00b0:KeyVal=6;   break;
	  case 0x0070:KeyVal=7;   break;
   }
   GPIO_Write(GPIOA,(GPIOA->ODR&0xfff0|0xb)); //��PA3��PA0��������Ƶ�0100
   switch(GPIOA->IDR&0x00f0)				  //��PC4��PC7��ֵ�����жϣ��������ͬ�ļ�ֵ
   {
      case 0x00e0:KeyVal=8;  break;
	  case 0x00d0:KeyVal=9;   break;
	  case 0x00b0:KeyVal=10;   break;
	  case 0x0070:KeyVal=11;   break;
   } 
   GPIO_Write(GPIOA,(GPIOA->ODR&0xfff0|0x7)); //��PA3��PA0��������Ƶ�1000
   switch(GPIOA->IDR&0x00f0)                  //��PC4��PC7��ֵ�����жϣ��������ͬ�ļ�ֵ
   {
      case 0x00e0:KeyVal=12;  break;
	  case 0x00d0:KeyVal=13;   break;
	  case 0x00b0:KeyVal=14;   break;
	  case 0x0070:KeyVal=15;   break;
   }
   return KeyVal;
}

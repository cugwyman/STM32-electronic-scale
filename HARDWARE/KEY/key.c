#include "key.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

extern u8 key_value;

void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ��KEY0-->GPIOA.13,KEY1-->GPIOA.15  ��������
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

		 

}
u8 KEY_Scan(void)
{	 
	static u8 key_up=1;//�������ɿ���־	
 
 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	if(key_up&&(KEY0==0||KEY1==0))
	{
		delay_ms(10);//ȥ���� 
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
	return 0;// �ް�������
}
//����Ϊ���� Ĭ��Ϊ1 ����Ϊ���Ĭ��Ϊ0	ɨ���� 
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
//����Ϊ��� Ĭ��Ϊ0 ����Ϊ����Ĭ��Ϊ1	ɨ���� 
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
		static u8 key_Up=1;//�������ɿ���־	

    u16 temp=0;
	u8 key=0;
	HL_Init_HScan();
	temp=GPIO_ReadInputData(GPIOA);
    temp&=0x000f; //�õ�����λ���� ���4��2��������0�Ĵ���
	if(key_Up&&(temp==0x0e||temp==0x0d||temp==0x0b||temp==0x07))
	{
				delay_ms(10);//ȥ���� 
        key_Up=0;
		if(temp==0x0e)//��һ���а���������
	{
	  key=1	;
	}
	else if(temp==0x0d)
	{
	  key=2;
	}
	else if(temp==0x0b)
	{
	  key=3;
	}
	else if(temp==0x07)
	{
	  key=4;
	}
}
	else
	{
		key=0;
		if(temp==0x0f)key_Up=1;
//		printf("temp=%x\r\n",temp);
//		return key;
	}
//printf("key!\r\n");
   HL_Init_LScan();
   temp=GPIO_ReadInputData(GPIOA);
   temp&=0x00f0;//�õ�����λ �������� 
   if((temp==0xe0)&&(key!=0))//��һ�б�����
   {
      key=(key-1)*4+1;
   }else
   if((temp==0xd0)&&(key!=0))
   {
      key=(key-1)*4+2;
   }else
   if((temp==0xb0)&&(key!=0))
   {
     key=(key-1)*4+3;
   }else
   if((temp==0x70)&&(key!=0))
   {
     key=(key-1)*4+4;
   }
   else key=0;
  
   return key;
}



//ͨ�ö�ʱ��2�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		  
//void TIM2_Int_Init(u16 arr,u16 psc)
//{	
//		NVIC_InitTypeDef NVIC_InitStructure;
//		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM2ʱ��ʹ��    
//		//��ʱ��TIM4��ʼ��
//		TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
//		TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
//		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�
//		TIM_Cmd(TIM2,ENABLE);//������ʱ��2
//		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
//}



////��ʱ��2�жϷ������		    
//void TIM2_IRQHandler(void)//�ж�485���������Ƿ������ʱ
//{ 	
//		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//�Ǹ����ж�
//		{	 			   
//				TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIM7�����жϱ�־  
////			printf("TIM2\r\n");	
//			
//				key_value=HL_Scan();
//				if(key_value!=0)
//				{
//					TIM_Cmd(TIM2, DISABLE);  //�ر�TIM2
//					
//				}	
//		}				
//}



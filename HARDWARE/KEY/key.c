#include "key.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

extern u8 key_value;

void KEY_Init(void) //IO初始化
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
   temp&=0x00f0;//得到高四位 即列数据 
   if((temp==0xe0)&&(key!=0))//第一列被按下
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



//通用定时器2中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz 
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		  
//void TIM2_Int_Init(u16 arr,u16 psc)
//{	
//		NVIC_InitTypeDef NVIC_InitStructure;
//		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM2时钟使能    
//		//定时器TIM4初始化
//		TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
//		TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
//		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
//		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断
//		TIM_Cmd(TIM2,ENABLE);//开启定时器2
//		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//}



////定时器2中断服务程序		    
//void TIM2_IRQHandler(void)//判定485总线数据是否堵塞超时
//{ 	
//		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//是更新中断
//		{	 			   
//				TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIM7更新中断标志  
////			printf("TIM2\r\n");	
//			
//				key_value=HL_Scan();
//				if(key_value!=0)
//				{
//					TIM_Cmd(TIM2, DISABLE);  //关闭TIM2
//					
//				}	
//		}				
//}



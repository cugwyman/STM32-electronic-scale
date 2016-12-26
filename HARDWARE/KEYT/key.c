#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟
}

//stm32矩阵键盘翻转扫描法（PB口的低8位）
//将此矩阵键盘扫描程序放在定时器中断服务程序里
//每10ms进入一次中断服务程序
s8 scan_MatrixKey(void)
{
    #define PORT GPIOA->IDR
    u8 column;//列
    u8 row;//行
    u8 tmp;//临时变量
		static u8 key_count = 0;//按键被中断扫描的次数
    s8 MatrixKey_value = 20;//初始值不能为0~15 
    
    ///////////IO口的配置/////////////
    //低8位为推挽输出
    GPIOA->CRL &= 0X00000000;
    GPIOA->CRL |= 0X33333333;
    //初值:低4位为低,次低4位为高
    GPIOA->ODR &= 0XFF00;
    GPIOA->ODR |= 0X00F0;
    //次低4位为上拉输入
    GPIOA->CRL &= 0X0000FFFF;
    GPIOA->CRL |= 0X88880000;
  

    tmp = PORT;//必须要
    if (tmp != 0XF0)//如果有键按下
    {   
        //防止长按时,持续自增导致变量溢出
        if (key_count <= 2)
        {
            key_count++;
        }
    }
    //若产生抖动按键被抬起则计数清0 
    else
    {
        key_count = 0;
    }
    //若按键连续2次扫描均处于按下状态
    //则认为按键确实被按下了
    if (key_count == 2)
    {          
        column = tmp & 0X00F0;//获取列号 
        
        ///////////IO口的配置/////////////
        //低8位为推挽输出
        GPIOA->CRL &= 0X00000000;
        GPIOA->CRL |= 0X33333333;
        //翻转:低4位为高,次低4位为低
        GPIOA->ODR &= 0XFF00;
        GPIOA->ODR |= 0X000F;//低4位为高,次低4位为低
        //低4位为上拉输入
        GPIOA->CRL &= 0XFFFF0000;
        GPIOA->CRL |= 0X00008888; 
                            
        row = PORT & 0X000F;//获取行号                                         
        switch (column | row)//column|row为按键被按下对应端口的编码
        {   
            //按键对应的码表（可以根据需求调整欲返回的键值） 
            case 0XEE: MatrixKey_value = 3; break;
            case 0XDE: MatrixKey_value = 2; break;
            case 0XBE: MatrixKey_value = 1; break;                
            case 0X7E: MatrixKey_value = 0; break;                                              
            case 0XED: MatrixKey_value = 7; break;
            case 0XDD: MatrixKey_value = 6; break;
            case 0XBD: MatrixKey_value = 5; break;
            case 0X7D: MatrixKey_value = 4; break;                                  
            case 0XEB: MatrixKey_value = 11; break;
            case 0XDB: MatrixKey_value = 10; break;
            case 0XBB: MatrixKey_value = 9; break;
            case 0X7B: MatrixKey_value = 8; break;                                    
            case 0XE7: MatrixKey_value = 15; break;
            case 0XD7: MatrixKey_value = 14; break;
            case 0XB7: MatrixKey_value = 13; break;
            case 0X77: MatrixKey_value = 12; break;
            default:   break;     
        }
			printf("键值：%d\r\n",MatrixKey_value);
//			USART_SendData(USART1, MatrixKey_value);//向串口1发送数据
//			printf("\r\n");
    } 
    //若没有按键被按下(已松手)则扫描次数清0
    //方便下次按下扫描计数
    if ((PORT & 0X00FF) == 0x00F0)
    {
        key_count = 0;  
    }

    return MatrixKey_value;
}

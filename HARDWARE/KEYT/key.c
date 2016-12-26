#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��
}

//stm32������̷�תɨ�跨��PB�ڵĵ�8λ��
//���˾������ɨ�������ڶ�ʱ���жϷ��������
//ÿ10ms����һ���жϷ������
s8 scan_MatrixKey(void)
{
    #define PORT GPIOA->IDR
    u8 column;//��
    u8 row;//��
    u8 tmp;//��ʱ����
		static u8 key_count = 0;//�������ж�ɨ��Ĵ���
    s8 MatrixKey_value = 20;//��ʼֵ����Ϊ0~15 
    
    ///////////IO�ڵ�����/////////////
    //��8λΪ�������
    GPIOA->CRL &= 0X00000000;
    GPIOA->CRL |= 0X33333333;
    //��ֵ:��4λΪ��,�ε�4λΪ��
    GPIOA->ODR &= 0XFF00;
    GPIOA->ODR |= 0X00F0;
    //�ε�4λΪ��������
    GPIOA->CRL &= 0X0000FFFF;
    GPIOA->CRL |= 0X88880000;
  

    tmp = PORT;//����Ҫ
    if (tmp != 0XF0)//����м�����
    {   
        //��ֹ����ʱ,�����������±������
        if (key_count <= 2)
        {
            key_count++;
        }
    }
    //����������������̧���������0 
    else
    {
        key_count = 0;
    }
    //����������2��ɨ������ڰ���״̬
    //����Ϊ����ȷʵ��������
    if (key_count == 2)
    {          
        column = tmp & 0X00F0;//��ȡ�к� 
        
        ///////////IO�ڵ�����/////////////
        //��8λΪ�������
        GPIOA->CRL &= 0X00000000;
        GPIOA->CRL |= 0X33333333;
        //��ת:��4λΪ��,�ε�4λΪ��
        GPIOA->ODR &= 0XFF00;
        GPIOA->ODR |= 0X000F;//��4λΪ��,�ε�4λΪ��
        //��4λΪ��������
        GPIOA->CRL &= 0XFFFF0000;
        GPIOA->CRL |= 0X00008888; 
                            
        row = PORT & 0X000F;//��ȡ�к�                                         
        switch (column | row)//column|rowΪ���������¶�Ӧ�˿ڵı���
        {   
            //������Ӧ��������Ը���������������صļ�ֵ�� 
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
			printf("��ֵ��%d\r\n",MatrixKey_value);
//			USART_SendData(USART1, MatrixKey_value);//�򴮿�1��������
//			printf("\r\n");
    } 
    //��û�а���������(������)��ɨ�������0
    //�����´ΰ���ɨ�����
    if ((PORT & 0X00FF) == 0x00F0)
    {
        key_count = 0;  
    }

    return MatrixKey_value;
}

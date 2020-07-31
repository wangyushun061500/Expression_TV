#include "config.h"
#include "15w.h"
#include "uart.h"
#include "mpu6050.h"
#include "oled.h"

// ���������ļ��ı���

extern bit busy;   

//�������ļ�����

float value = 0;        /* mpu6050��������洢�� */
// ��ǰ��������

void Delay5us();
void Delay1000ms();

void main(void)
{
    uart_init();
    Delay1000ms();
    SendString("STC15F2K60S2\r\nUart ON !\r\n");
    mpu6050_init();
	while (1)
    {
        value = MPU6050_Get_Data(1);					//��ȡ��x��ļнǣ��Ƕȱ��Ŵ�10��
		SendString("x��ļнǣ�");
		Delay5us();
		serial_one_send_float(value / 10,1);			//�Ƕȳ���10�����Ӵ��ڷ���,�ڶ�������Ϊ����һλС��
		Delay5us();
		SendString("\r\n");								//����
		
		value = MPU6050_Get_Data(2);					//��ȡ��y��ļнǣ��Ƕȱ��Ŵ�10��
		SendString("y��ļнǣ�");
		Delay5us();
		serial_one_send_float(value / 10,1);			//�Ƕȳ���10�����Ӵ��ڷ���
		Delay5us();
		SendString("\r\n");								//����
		
		value = MPU6050_Get_Data(3);					//��ȡ��z��ļнǣ��Ƕȱ��Ŵ�10��
		SendString("z��ļнǣ�");
		Delay5us();
		serial_one_send_float(value / 10,1);			//�Ƕȳ���10�����Ӵ��ڷ���
		Delay5us();
		SendString("\r\n");								//����
		
		value = MPU6050_Get_Data(4);					//��ȡ��x����ٶȣ���ֵ���Ŵ�100��
		SendString("x����ٶȣ�");
		Delay5us();
		serial_one_send_float(value/100,1);				//�Ƕȳ���100�����Ӵ��ڷ������ڶ�������Ϊ����һλС��
		Delay5us();
		SendString("\r\n");								//����
		
		value = MPU6050_Get_Data(5);					//��ȡ��y����ٶȣ���ֵ���Ŵ�100��
		SendString("y����ٶȣ�");
		Delay5us();
		serial_one_send_float(value / 100,1);			//�Ƕȳ���100�����Ӵ��ڷ���
		Delay5us();
		SendString("\r\n");								//����
		
		value = MPU6050_Get_Data(6);					//��ȡ��z����ٶȣ���ֵ���Ŵ�100��
		SendString("z����ٶȣ�");
		Delay5us();
		serial_one_send_float(value / 100,1);			//�Ƕȳ���100�����Ӵ��ڷ���
		Delay5us();
		SendString("\r\n");								//����

		value = MPU6050_Get_Data(7);					//��ȡ���¶�
		SendString("��ǰ�¶�Ϊ��");
		Delay5us();
		serial_one_send_float(value / 100,1);			//�Ƕȳ���100�����Ӵ��ڷ���
		Delay5us();
		SendString("\r\n");								//����
		SendString("\r\n");								//����
		Delay1000ms();
    }
    
}

/*��ʱ5us����*/
void Delay5us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 27;
	while (--i);
}

/*��ʱ1s����*/
void Delay1000ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 92;
	j = 50;
	k = 238;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

/*uart�жϷ������*/
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //���RIλ
        P0 = SBUF;              //P0��ʾ��������
        P54 = RB8;              //P2.2��ʾУ��λ
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
        busy = 0;               //��æ��־
    }
}
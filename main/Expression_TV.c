#include "config.h"
#include "15w.h"
#include "uart.h"
#include "mpu6050.h"
#include "oled.h"

// 引用其他文件的变量

extern bit busy;   

//声明本文件变量

float value = 0;        /* mpu6050数据输出存储器 */
// 提前声明函数

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
        value = MPU6050_Get_Data(1);					//获取与x轴的夹角，角度被放大10倍
		SendString("x轴的夹角：");
		Delay5us();
		serial_one_send_float(value / 10,1);			//角度除以10，并从串口发出,第二个参数为保留一位小数
		Delay5us();
		SendString("\r\n");								//换行
		
		value = MPU6050_Get_Data(2);					//获取与y轴的夹角，角度被放大10倍
		SendString("y轴的夹角：");
		Delay5us();
		serial_one_send_float(value / 10,1);			//角度除以10，并从串口发出
		Delay5us();
		SendString("\r\n");								//换行
		
		value = MPU6050_Get_Data(3);					//获取与z轴的夹角，角度被放大10倍
		SendString("z轴的夹角：");
		Delay5us();
		serial_one_send_float(value / 10,1);			//角度除以10，并从串口发出
		Delay5us();
		SendString("\r\n");								//换行
		
		value = MPU6050_Get_Data(4);					//获取与x轴加速度，数值被放大100倍
		SendString("x轴加速度：");
		Delay5us();
		serial_one_send_float(value/100,1);				//角度除以100，并从串口发出，第二个参数为保留一位小数
		Delay5us();
		SendString("\r\n");								//换行
		
		value = MPU6050_Get_Data(5);					//获取与y轴加速度，数值被放大100倍
		SendString("y轴加速度：");
		Delay5us();
		serial_one_send_float(value / 100,1);			//角度除以100，并从串口发出
		Delay5us();
		SendString("\r\n");								//换行
		
		value = MPU6050_Get_Data(6);					//获取与z轴加速度，数值被放大100倍
		SendString("z轴加速度：");
		Delay5us();
		serial_one_send_float(value / 100,1);			//角度除以100，并从串口发出
		Delay5us();
		SendString("\r\n");								//换行

		value = MPU6050_Get_Data(7);					//获取与温度
		SendString("当前温度为：");
		Delay5us();
		serial_one_send_float(value / 100,1);			//角度除以100，并从串口发出
		Delay5us();
		SendString("\r\n");								//换行
		SendString("\r\n");								//换行
		Delay1000ms();
    }
    
}

/*延时5us函数*/
void Delay5us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 27;
	while (--i);
}

/*延时1s函数*/
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

/*uart中断服务程序*/
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //清除RI位
        P0 = SBUF;              //P0显示串口数据
        P54 = RB8;              //P2.2显示校验位
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy = 0;               //清忙标志
    }
}
#ifndef __CONFIG_H
#define __CONFIG_H

#include "15w.h"
#include "INTRINS.H"
#include <MATH.H>

/* 声明配置 */
// OLED声明引脚

#define OLED_Vcc P11        //OLED供电正电源
#define OLED_Gnd P12        //OLED供电接地
#define OLED_CS P04         /* 片选引脚端 */
#define OLED_RST P06        /* 复位引脚端 */
#define OLED_DC P05         /* DC端 */
#define OLED_SDIN P07       //D1
#define OLED_SCL P10        //时钟 D0
// 声明元器件管脚

#define Vcc P20		//电源正极
#define Gnd P21		//电源负极
#define Scl P22		//I2C时钟线
#define Sda P23		//I2C数据线
#define Xda P24		//外接I2C数据线
#define Xcl P25		//外接I2C时钟线
#define Ad0 P26		//I2C地址最低位
#define Int P27		//中断信号输出

// 声明串口传输设置

#define FOSC 24000000L		//系统频率
#define BAUD 115200 		//波特率
#define NONE_PARITY     0       //无校验
#define ODD_PARITY      1       //奇校验
#define EVEN_PARITY     2       //偶校验
#define MARK_PARITY     3       //标记校验
#define SPACE_PARITY    4       //空白校验
#define PARITYBIT NONE_PARITY   //定义校验位
#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

// 声明mpu6050内部地址

#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B	//加速度X轴高8位
#define	ACCEL_XOUT_L	0x3C    //加速度X轴低8位
#define	ACCEL_YOUT_H	0x3D    //加速度Y轴高8位
#define	ACCEL_YOUT_L	0x3E    //加速度Y轴低8位
#define	ACCEL_ZOUT_H	0x3F    //加速度Z轴高8位
#define	ACCEL_ZOUT_L	0x40    //加速度Z轴低8位
#define	TEMP_OUT_H		0x41    //温度传感器高8位
#define	TEMP_OUT_L		0x42    //温度传感器低8位
#define	GYRO_XOUT_H		0x43    //角度X轴高8位
#define	GYRO_XOUT_L		0x44	//角度X轴低8位
#define	GYRO_YOUT_H		0x45    //角度Y轴高8位
#define	GYRO_YOUT_L		0x46    //角度Y轴低8位
#define	GYRO_ZOUT_H		0x47    //角度Z轴高8位
#define	GYRO_ZOUT_L		0x48    //角度Z轴低8位
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xd0	//IIC写入时的地址字节数据，+1为读取

#endif
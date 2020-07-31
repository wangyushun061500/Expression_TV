#include "mpu6050.h"
#include "config.h"
#include "uart.h"
#include "delay.h"

/*mpu6050初始化函数*/
void mpu6050_init ()
{
	// 配置IO输出模式
	P2M0 = 0x01;
	P2M1 = 0x00;
	//配置电源电平
	MPU_6050_Vcc = 1;
	MPU_6050_Gnd = 0;
	//配置mpu6050地址，ad0为设置最低位地址设置
	Int = 0;
	//初始化寄存器
	Delay1000ms();
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
	SendString("Init Done!\r\n");
}

/*I2C开始信号*/
void I2C_Start()
{
    Sda = 1;                    //拉高数据线
    Scl = 1;                    //拉高时钟线
    Delay5us();                 //延时
    Sda = 0;                    //产生下降沿
    Delay5us();                 //延时
    Scl = 0;                    //拉低时钟线
}

/*I2C停止信号*/
void I2C_Stop()
{
    Sda = 0;                    //拉低数据线
    Scl = 1;                    //拉高时钟线
    Delay5us();                 //延时
    Sda = 1;                    //产生上升沿
    Delay5us();                 //延时
}

/*I2C发送应答信号*/
void I2C_SendACK(bit ack)
{
    Sda = ack;                  //写应答信号
    Scl = 1;                    //拉高时钟线
    Delay5us();                 //延时
    Scl = 0;                    //拉低时钟线
    Delay5us();                 //延时
}

/*I2C接收应答信号*/
bit I2C_RecvACK()
{
	bit ack;
    Scl = 1;                    //拉高时钟线
    Delay5us();                 //延时
    ack = Sda;                   //读应答信号
    Scl = 0;                    //拉低时钟线
    Delay5us();                 //延时
    return ack;
}

/*I2C发送一个字节数据*/
void I2C_SendByte(unsigned char dat)
{
    unsigned char i;
    for (i=0; i<8; i++)         //8位计数器
    {
        dat = dat << 1;         //移出数据的最高位
        Sda = CY;               //送数据口
        Scl = 1;                //拉高时钟线
        Delay5us();             //延时
        Scl = 0;                //拉低时钟线
        Delay5us();             //延时
    }
     while(I2C_RecvACK());
}

/*I2C接收一个字节数据*/
unsigned char I2C_RecvByte()
{
    unsigned char i;
    unsigned char dat = 0;
    Sda = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat = dat << 1;
        Scl = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= Sda;             //读数据               
        Scl = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}

/*I2C写入一个字节数据*/
void Single_WriteI2C(unsigned char REG_Address,unsigned char REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
}

/*从I2C设备读取一个字节数据*/
unsigned char Single_ReadI2C(unsigned char REG_Address)
{
	unsigned char REG_data;
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据
	I2C_SendACK(1);                //接收应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}

/*数据融合算法*/
int GetData(unsigned char REG_Address)
{
	unsigned char H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //合成数据
}

/*获取角速度或者加速度*/
int Get_Gyro_Data(unsigned char gyro_id)
{
	switch(gyro_id)
	{
		case 1:	return GetData(ACCEL_XOUT_H);	break;
		case 2:	return GetData(ACCEL_YOUT_H);	break;
		case 3:	return GetData(ACCEL_ZOUT_H);	break;
		case 4:	return GetData(GYRO_XOUT_H) ;	break;
		case 5:	return GetData(GYRO_YOUT_H) ;	break;
		case 6:	return GetData(GYRO_ZOUT_H) ;	break;
	}
	return 0;
}

/*转换夹角*/
int MPU6050_Get_Angle(int x,int y,int z,unsigned char dir)
{
	float xdata temp;
	float xdata res = 0;
	switch(dir)
	{
		case 0://与z轴的夹角
				temp = sqrt(((float)x*(float)x+(float)y*(float)y))/(float)z;
				res  = atan(temp);
		break;
		case 1://与x轴的夹角
				temp = (float)x/sqrt(((float)y*(float)y+(float)z*(float)z));
				res  = atan(temp);
		break;
		case 2://与y轴的夹角
				temp = (float)y/sqrt(((float)x*(float)x+(float)z*(float)z));
				res  = atan(temp);
		break;
	}
	return (int)(res*1800/3.1416);//弧度转换为角度,扩大10倍
}

/*数值处理*/
int MPU6050_Get_Data(unsigned angle_id)
{
	switch(angle_id)
	{
		case 1:return MPU6050_Get_Angle( Get_Gyro_Data(1), Get_Gyro_Data(2), Get_Gyro_Data(3), 1);break;
		case 2:return MPU6050_Get_Angle( Get_Gyro_Data(1), Get_Gyro_Data(2), Get_Gyro_Data(3), 2);break;
		case 3:return MPU6050_Get_Angle( Get_Gyro_Data(1), Get_Gyro_Data(2), Get_Gyro_Data(3), 0);break;
		case 4:return (int)((float)((float)Get_Gyro_Data(4)/16384)*9.8*100);
		case 5:return (int)((float)((float)Get_Gyro_Data(5)/16384)*9.8*100);
		case 6:return (int)((float)((float)Get_Gyro_Data(6)/16384)*9.8*100);
		case 7:return (int)((float)((float)((float)GetData(TEMP_OUT_H)/340)+36.53)*100);
	}
	return 0;
}

#include "15w.h"
#include "INTRINS.H"
#include <MATH.H>

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
#define	ACCEL_XOUT_H	0x3B	
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xd0	//IIC写入时的地址字节数据，+1为读取

// 变量声明

bit busy;
float value = 0;

// 提前声明函数

void mpu6050_init();
void uart_init();
void Delay5us();
void Delay1000ms();
void SendData(unsigned char dat);
void SendString(char *s);
void  I2C_Start();
void  I2C_Stop();
void  I2C_SendACK(bit ack);
bit   I2C_RecvACK();
void  I2C_SendByte(unsigned char dat);
unsigned char I2C_RecvByte();
unsigned char Single_ReadI2C(unsigned char REG_Address);						//读取I2C数据
void  Single_WriteI2C(unsigned char REG_Address,unsigned char REG_data);		//向I2C写入数据
int GetData(unsigned char REG_Address);
int Get_Gyro_Data(unsigned char gyro_id);
int MPU6050_Get_Angle(int x,int y,int z,unsigned char dir);
int MPU6050_Get_Data(unsigned angle_id);
void serial_one_send_number(long num);
void serial_one_send_float(double float_val, char bit_val);


void main(void)
{
	uart_init();
	T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
    ES = 1;                     //使能串口1中断
    EA = 1;
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
		
		value = MPU6050_Get_Data(4);					//获取与y轴加速度，数值被放大100倍
		SendString("y轴加速度：");
		Delay5us();
		serial_one_send_float(value / 100,1);			//角度除以100，并从串口发出
		Delay5us();
		SendString("\r\n");								//换行
		
		value = MPU6050_Get_Data(4);					//获取与z轴加速度，数值被放大100倍
		SendString("z轴加速度：");
		Delay5us();
		serial_one_send_float(value / 100,1);			//角度除以100，并从串口发出
		Delay5us();
		SendString("\r\n");								//换行
		SendString("\r\n");								//换行
		Delay1000ms();
	}
	

}

/*mpu6050初始化函数*/
void mpu6050_init ()
{
	// 配置IO输出模式
	P2M0 = 0x01;
	P2M1 = 0x00;
	//配置电源电平
	Vcc = 1;
	Gnd = 0;
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

/*uart初始化函数*/
void uart_init ()
{
	ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
	#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;                //8位可变波特率
	#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9位可变波特率,校验位初始为1
	#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9位可变波特率,校验位初始为0
	#endif
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

/*串口数据发送*/
void SendData(unsigned char dat)
{
    while (busy);               //等待前面的数据发送完成
    ACC = dat;                  //获取校验位P (PSW.0)
    if (P)                      //根据P来设置校验位
    {
		#if (PARITYBIT == ODD_PARITY)
			TB8 = 0;                //设置校验位为0
		#elif (PARITYBIT == EVEN_PARITY)
			TB8 = 1;                //设置校验位为1
		#endif
    }
    else
    {
		#if (PARITYBIT == ODD_PARITY)
    	    TB8 = 1;                //设置校验位为1
		#elif (PARITYBIT == EVEN_PARITY)
    	    TB8 = 0;                //设置校验位为0
		#endif
    }
    busy = 1;
    SBUF = ACC;                 //写数据到UART数据寄存器
}

/*发送字符串*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
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
	}
	return 0;
}

/*uart发送long数值*/
void serial_one_send_number(long num)
{
	long dat = 0;
	unsigned char  length = 0;
	if(num < 0)										//当数值为负数时
	{
		SendData('-');	//输出负号
		num = -num;									//将数值取相反数
	}
	
	if(num == 0)									//当数值为0时
		SendData('0');	//输出字符0
	else											//当数值不为0时
	{
		while(num)									//将数值倒过来
		{
			dat = dat * 10;
			dat = dat + num % 10;
			num = num / 10;
			length++;
		}
		
		while(length--)							//从第一位开始输出倒过来的数值
		{
			SendData(dat % 10 + '0');
			dat = dat / 10;
		}
	}
}


void serial_one_send_float(double float_val, char bit_val)
{
	long xdata value_int = 0;
	long xdata value_flt = 0;
	
	if(float_val < 0)
	{
		SendData('-');
		float_val = -float_val;
	}
	
	value_int = (long)float_val;
	
	float_val = float_val - (double)value_int;
	
	for(;bit_val;bit_val--)
		float_val = float_val * 10;
	
	serial_one_send_number(value_int);
	SendData('.');
	serial_one_send_number((long)float_val);
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
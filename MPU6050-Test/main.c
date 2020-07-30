#include "15w.h"
#include "INTRINS.H"
#include <MATH.H>

// ����Ԫ�����ܽ�

#define Vcc P20		//��Դ����
#define Gnd P21		//��Դ����
#define Scl P22		//I2Cʱ����
#define Sda P23		//I2C������
#define Xda P24		//���I2C������
#define Xcl P25		//���I2Cʱ����
#define Ad0 P26		//I2C��ַ���λ
#define Int P27		//�ж��ź����

// �������ڴ�������

#define FOSC 24000000L		//ϵͳƵ��
#define BAUD 115200 		//������
#define NONE_PARITY     0       //��У��
#define ODD_PARITY      1       //��У��
#define EVEN_PARITY     2       //żУ��
#define MARK_PARITY     3       //���У��
#define SPACE_PARITY    4       //�հ�У��
#define PARITYBIT NONE_PARITY   //����У��λ
#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

// ����mpu6050�ڲ���ַ
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
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
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	0xd0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

// ��������

bit busy;
float value = 0;

// ��ǰ��������

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
unsigned char Single_ReadI2C(unsigned char REG_Address);						//��ȡI2C����
void  Single_WriteI2C(unsigned char REG_Address,unsigned char REG_data);		//��I2Cд������
int GetData(unsigned char REG_Address);
int Get_Gyro_Data(unsigned char gyro_id);
int MPU6050_Get_Angle(int x,int y,int z,unsigned char dir);
int MPU6050_Get_Data(unsigned angle_id);
void serial_one_send_number(long num);
void serial_one_send_float(double float_val, char bit_val);


void main(void)
{
	uart_init();
	T2L = (65536 - (FOSC/4/BAUD));   //���ò�������װֵ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
    ES = 1;                     //ʹ�ܴ���1�ж�
    EA = 1;
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
		
		value = MPU6050_Get_Data(4);					//��ȡ��y����ٶȣ���ֵ���Ŵ�100��
		SendString("y����ٶȣ�");
		Delay5us();
		serial_one_send_float(value / 100,1);			//�Ƕȳ���100�����Ӵ��ڷ���
		Delay5us();
		SendString("\r\n");								//����
		
		value = MPU6050_Get_Data(4);					//��ȡ��z����ٶȣ���ֵ���Ŵ�100��
		SendString("z����ٶȣ�");
		Delay5us();
		serial_one_send_float(value / 100,1);			//�Ƕȳ���100�����Ӵ��ڷ���
		Delay5us();
		SendString("\r\n");								//����
		SendString("\r\n");								//����
		Delay1000ms();
	}
	

}

/*mpu6050��ʼ������*/
void mpu6050_init ()
{
	// ����IO���ģʽ
	P2M0 = 0x01;
	P2M1 = 0x00;
	//���õ�Դ��ƽ
	Vcc = 1;
	Gnd = 0;
	//����mpu6050��ַ��ad0Ϊ�������λ��ַ����
	Int = 0;
	//��ʼ���Ĵ���
	Delay1000ms();
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//�������״̬
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
	SendString("Init Done!\r\n");
}

/*uart��ʼ������*/
void uart_init ()
{
	ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
	#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;                //8λ�ɱ䲨����
	#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9λ�ɱ䲨����,У��λ��ʼΪ1
	#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9λ�ɱ䲨����,У��λ��ʼΪ0
	#endif
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

/*�������ݷ���*/
void SendData(unsigned char dat)
{
    while (busy);               //�ȴ�ǰ������ݷ������
    ACC = dat;                  //��ȡУ��λP (PSW.0)
    if (P)                      //����P������У��λ
    {
		#if (PARITYBIT == ODD_PARITY)
			TB8 = 0;                //����У��λΪ0
		#elif (PARITYBIT == EVEN_PARITY)
			TB8 = 1;                //����У��λΪ1
		#endif
    }
    else
    {
		#if (PARITYBIT == ODD_PARITY)
    	    TB8 = 1;                //����У��λΪ1
		#elif (PARITYBIT == EVEN_PARITY)
    	    TB8 = 0;                //����У��λΪ0
		#endif
    }
    busy = 1;
    SBUF = ACC;                 //д���ݵ�UART���ݼĴ���
}

/*�����ַ���*/
void SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
}

/*I2C��ʼ�ź�*/
void I2C_Start()
{
    Sda = 1;                    //����������
    Scl = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    Sda = 0;                    //�����½���
    Delay5us();                 //��ʱ
    Scl = 0;                    //����ʱ����
}

/*I2Cֹͣ�ź�*/
void I2C_Stop()
{
    Sda = 0;                    //����������
    Scl = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    Sda = 1;                    //����������
    Delay5us();                 //��ʱ
}

/*I2C����Ӧ���ź�*/
void I2C_SendACK(bit ack)
{
    Sda = ack;                  //дӦ���ź�
    Scl = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    Scl = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
}

/*I2C����Ӧ���ź�*/
bit I2C_RecvACK()
{
	bit ack;
    Scl = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    ack = Sda;                   //��Ӧ���ź�
    Scl = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
    return ack;
}

/*I2C����һ���ֽ�����*/
void I2C_SendByte(unsigned char dat)
{
    unsigned char i;
    for (i=0; i<8; i++)         //8λ������
    {
        dat = dat << 1;         //�Ƴ����ݵ����λ
        Sda = CY;               //�����ݿ�
        Scl = 1;                //����ʱ����
        Delay5us();             //��ʱ
        Scl = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
     while(I2C_RecvACK());
}

/*I2C����һ���ֽ�����*/
unsigned char I2C_RecvByte()
{
    unsigned char i;
    unsigned char dat = 0;
    Sda = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat = dat << 1;
        Scl = 1;                //����ʱ����
        Delay5us();             //��ʱ
        dat |= Sda;             //������               
        Scl = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    return dat;
}

/*I2Cд��һ���ֽ�����*/
void Single_WriteI2C(unsigned char REG_Address,unsigned char REG_data)
{
    I2C_Start();                  //��ʼ�ź�
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
    I2C_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
    I2C_Stop();                   //����ֹͣ�ź�
}

/*��I2C�豸��ȡһ���ֽ�����*/
unsigned char Single_ReadI2C(unsigned char REG_Address)
{
	unsigned char REG_data;
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ	
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
	REG_data=I2C_RecvByte();       //�����Ĵ�������
	I2C_SendACK(1);                //����Ӧ���ź�
	I2C_Stop();                    //ֹͣ�ź�
	return REG_data;
}

/*�����ں��㷨*/
int GetData(unsigned char REG_Address)
{
	unsigned char H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //�ϳ�����
}

/*��ȡ���ٶȻ��߼��ٶ�*/
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

/*ת���н�*/
int MPU6050_Get_Angle(int x,int y,int z,unsigned char dir)
{
	float xdata temp;
	float xdata res = 0;
	switch(dir)
	{
		case 0://��z��ļн�
				temp = sqrt(((float)x*(float)x+(float)y*(float)y))/(float)z;
				res  = atan(temp);
		break;
		case 1://��x��ļн�
				temp = (float)x/sqrt(((float)y*(float)y+(float)z*(float)z));
				res  = atan(temp);
		break;
		case 2://��y��ļн�
				temp = (float)y/sqrt(((float)x*(float)x+(float)z*(float)z));
				res  = atan(temp);
		break;
	}
	return (int)(res*1800/3.1416);//����ת��Ϊ�Ƕ�,����10��
}

/*��ֵ����*/
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

/*uart����long��ֵ*/
void serial_one_send_number(long num)
{
	long dat = 0;
	unsigned char  length = 0;
	if(num < 0)										//����ֵΪ����ʱ
	{
		SendData('-');	//�������
		num = -num;									//����ֵȡ�෴��
	}
	
	if(num == 0)									//����ֵΪ0ʱ
		SendData('0');	//����ַ�0
	else											//����ֵ��Ϊ0ʱ
	{
		while(num)									//����ֵ������
		{
			dat = dat * 10;
			dat = dat + num % 10;
			num = num / 10;
			length++;
		}
		
		while(length--)							//�ӵ�һλ��ʼ�������������ֵ
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
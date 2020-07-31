#include "uart.h"
#include "config.h"

/* 校检位变量声明 */
bit busy;

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

    /* uart相关寄存器设置 */
    T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
    ES = 1;                     //使能串口1中断
    EA = 1;
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

/* uart发送float数值 */
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

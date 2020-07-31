#include "config.h"
#include "15w.h"
#include "delay.h"
#include "uart.h"
#include "mpu6050.h"
#include "oled.h"
#include "bmp.h"

// 引用其他文件的变量

extern bit busy;   

//声明本文件变量
unsigned char count;
data unsigned char ss = 0,mm = 14,      //初始化分钟和秒
hh = 13,dd = 20,                        //初始化小时和天
nn = 5,yy = 20;                         //初始化年份和月份
float value = 0;                        /* mpu6050数据输出存储器 */
// 提前声明函数
void Timer0Init(void);

void main(void)
{
    unsigned char i;
    uart_init();
    Delay1000ms();
    SendString("STC15F2K60S2\r\nUart ON !\r\n");
    mpu6050_init();
    Timer0Init();

    OLED_Init();
    SendString("OLED-Init Yes !\r\n");
    OLED_Clear(); 
    SendString("OLED-Clear Yes !\r\n");
    for(i=6; i>0; i--)                              //开启动画，循环六次
    {
		OLED_DrawBMP(0,0,128,8,BMP1);
		delay_ms(200);
		OLED_DrawBMP(0,0,128,8,BMP2);
		delay_ms(200);
	}
	OLED_Clear();                                   //初始化清屏一次
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
    count = 1;
	while (1)
    {
        if(count == 0)
        {
            OLED_ShowCHinese(0,0,0);                //王
            OLED_ShowCHinese(18,0,1);               //昱
            OLED_ShowCHinese(36,0,2);               //顺
            OLED_ShowCHinese(54,0,3);               //的
            OLED_ShowCHinese(72,0,4);               //工
            OLED_ShowCHinese(90,0,5);               //作
            OLED_ShowCHinese(108,0,6);              //室
            OLED_ShowString(10,2,"Hello world!");
            OLED_ShowCHinese(0,4,7);                //日
            OLED_ShowCHinese(16,4,8);               //期
            OLED_ShowString(32,4,"-");
            OLED_ShowNum(0+32+8,4,20,2,16);         //年份	
            OLED_ShowNum(16+32+8,4,yy,2,16);        //年份
            OLED_ShowString(32+32+8,4,"/");
            OLED_ShowNum(40+32+8,4,nn,2,16);        //月份
            OLED_ShowString(56+32+8,4,"/");
            OLED_ShowNum(64+32+8,4,dd,2,16);        //天数
            OLED_ShowCHinese(0,6,9);                //时
            OLED_ShowCHinese(16,6,10);              //间
            OLED_ShowString(32,6,"-");
            OLED_ShowNum(0+32+8,6,hh,2,16);         //小时
            OLED_ShowString(16+32+8,6,":");
            OLED_ShowNum(24+32+8,6,mm,2,16);        //分钟
            OLED_ShowString(40+32+8,6,":");
            OLED_ShowNum(48+32+8,6,ss,2,16);        //秒
		}
        if(count == 1)
        {
            OLED_DrawBMP(0,0,128,8,lan0);
			delay_ms(100);
			OLED_DrawBMP(0,0,128,8,lan1);
			delay_ms(150);
			OLED_DrawBMP(0,0,128,8,lan2);
			delay_ms(200);
			OLED_DrawBMP(0,0,128,8,lan1);
			delay_ms(100);
        }
        if(count == 2)
        {
            for(i=4; i>0; i--)                              //开启动画，循环六次
            {
                OLED_DrawBMP(0,0,128,8,yun0);
                delay_ms(100);
                OLED_DrawBMP(0,0,128,8,yun1);
                delay_ms(100);
                OLED_DrawBMP(0,0,128,8,yun2);
                delay_ms(100);
                OLED_DrawBMP(0,0,128,8,yun3);
                delay_ms(100);
                OLED_DrawBMP(0,0,128,8,yun4);
                delay_ms(100);
                OLED_DrawBMP(0,0,128,8,yun5);
                delay_ms(100); 
            }
		}
        value = MPU6050_Get_Data(4);					//获取与x轴加速度，数值被放大100倍
        value = value/100;
        if(value >= 0.6){
            count = 2;
        }
        else
        {
            count = 1;   
        }
        
    }
    
}

/* t0初始化程序 */
void Timer0Init(void)		//20毫秒@24.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xC0;		    //设置定时初值
	TH0 = 0x63;		    //设置定时初值
	TF0 = 0;		    //清除TF0标志
	TR0 = 1;		    //定时器0开始计时
    ET0 = 1;            //定时器0允许中断

}

/* t0中断服务程序 */
void timer0_isr (void) interrupt 1 {
	unsigned char mss;
	mss++;
	if(mss == 50){
		mss = 0;
		ss++;
		if(ss == 60){
			mm++;
			ss = 0;
			if(mm == 60){
				hh++;
				mm = 0;
				if(hh == 24){
					dd++;
					hh = 0;
					if(dd > 28){
						if(nn == 2){
							nn++;
							dd = 1;
						}
					}
					if(dd > 30){
						if(nn==4 || nn==6 || nn==9 || nn==11){
							nn++;
							dd = 1;
						}else{
							if(dd > 31){
								nn++;
								dd = 1;
							}
						}
					}
					if(nn == 12){
						nn = 1;
						yy++;
					}
				}
			}
		}
	}     
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
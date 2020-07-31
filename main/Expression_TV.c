#include "config.h"
#include "15w.h"
#include "delay.h"
#include "uart.h"
#include "mpu6050.h"
#include "oled.h"
#include "bmp.h"

// ���������ļ��ı���

extern bit busy;   

//�������ļ�����
unsigned char count;
data unsigned char ss = 0,mm = 14,      //��ʼ�����Ӻ���
hh = 13,dd = 20,                        //��ʼ��Сʱ����
nn = 5,yy = 20;                         //��ʼ����ݺ��·�
float value = 0;                        /* mpu6050��������洢�� */
// ��ǰ��������
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
    for(i=6; i>0; i--)                              //����������ѭ������
    {
		OLED_DrawBMP(0,0,128,8,BMP1);
		delay_ms(200);
		OLED_DrawBMP(0,0,128,8,BMP2);
		delay_ms(200);
	}
	OLED_Clear();                                   //��ʼ������һ��
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
    count = 1;
	while (1)
    {
        if(count == 0)
        {
            OLED_ShowCHinese(0,0,0);                //��
            OLED_ShowCHinese(18,0,1);               //��
            OLED_ShowCHinese(36,0,2);               //˳
            OLED_ShowCHinese(54,0,3);               //��
            OLED_ShowCHinese(72,0,4);               //��
            OLED_ShowCHinese(90,0,5);               //��
            OLED_ShowCHinese(108,0,6);              //��
            OLED_ShowString(10,2,"Hello world!");
            OLED_ShowCHinese(0,4,7);                //��
            OLED_ShowCHinese(16,4,8);               //��
            OLED_ShowString(32,4,"-");
            OLED_ShowNum(0+32+8,4,20,2,16);         //���	
            OLED_ShowNum(16+32+8,4,yy,2,16);        //���
            OLED_ShowString(32+32+8,4,"/");
            OLED_ShowNum(40+32+8,4,nn,2,16);        //�·�
            OLED_ShowString(56+32+8,4,"/");
            OLED_ShowNum(64+32+8,4,dd,2,16);        //����
            OLED_ShowCHinese(0,6,9);                //ʱ
            OLED_ShowCHinese(16,6,10);              //��
            OLED_ShowString(32,6,"-");
            OLED_ShowNum(0+32+8,6,hh,2,16);         //Сʱ
            OLED_ShowString(16+32+8,6,":");
            OLED_ShowNum(24+32+8,6,mm,2,16);        //����
            OLED_ShowString(40+32+8,6,":");
            OLED_ShowNum(48+32+8,6,ss,2,16);        //��
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
            for(i=4; i>0; i--)                              //����������ѭ������
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
        value = MPU6050_Get_Data(4);					//��ȡ��x����ٶȣ���ֵ���Ŵ�100��
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

/* t0��ʼ������ */
void Timer0Init(void)		//20����@24.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xC0;		    //���ö�ʱ��ֵ
	TH0 = 0x63;		    //���ö�ʱ��ֵ
	TF0 = 0;		    //���TF0��־
	TR0 = 1;		    //��ʱ��0��ʼ��ʱ
    ET0 = 1;            //��ʱ��0�����ж�

}

/* t0�жϷ������ */
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
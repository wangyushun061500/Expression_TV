#ifndef __UART_H
#define __UART_H

/* 声明函数 */

void uart_init();
void SendData(unsigned char dat);
void SendString(char *s);
void serial_one_send_number(long num);
void serial_one_send_float(double float_val, char bit_val);

#endif
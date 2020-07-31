#ifndef __MPU6050_H
#define __MPU6050_H

/* 函数声明 */

void mpu6050_init();
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

#endif
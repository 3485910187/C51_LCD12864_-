#ifndef __MOTOR_H
#define __MOTOR_H

#include"reg51.h"
#include"delay.h"

#define uchar unsigned char 

sbit Px0 = P3^4;
sbit Px1 = P3^5;
sbit Px2 = P3^6;
sbit Px3 = P3^7; //步进电机控制引脚

void motor_data(uchar da);
void motor_angle(int angle);
void motor_direction(uchar D,int S);

#endif

#include"motor.h"

uchar st[]={0x06,0x02,0x03,0x01,0x09,0x08,0x0c,0x04};

void motor_data(unsigned char da)
{
	Px0 = (bit)(da & 0x01);
	Px1 = (bit)(da & 0x02);
	Px2 = (bit)(da & 0x04);
	Px3 = (bit)(da & 0x08);	
}

void motor_angle(int angle)
{
	motor_data(st[(angle/45)%8]);
}

void motor_direction(uchar D,int S)
{
	uchar i;
	if(D == 1)
	{
		for(i=0;i<8;i++)
		{
			motor_data(st[i]);
			DelayMs(5000-S);
		}
	}
	else
	{
		for(i=7;i>=0;i--)
		{
			motor_data(st[i]);
			DelayMs(5000-S);
		}
	}
}
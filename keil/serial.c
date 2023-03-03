#include"serial.h"


void Serialinit()
{
	SCON = 0x50;
	TMOD = 0x21; //定时器0和2开启；
	TH1 = 256-3;
	TL1 = 256-3;
//	ES = 1; //开启后会和定时器0中断冲突
	EA = 1;
	TR1 = 1;
}

void Sendchar( unsigned char da)
{
	SBUF = da;
	while(!TI);
	TI = 0;
}

void Sendstring(int x,char *str)
{
	char *p = str;
	int i;
	for(i=0;i<x;i++)
	{
		p = str;
		while(*p != '\0')
		{
			Sendchar(*p++);
		}
	}
}


//接受中断
/*
void Seral_interrput() interrupt 4
{
	serialdata = SBUF;
	RI = 0;
}
*/

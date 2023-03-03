#include"reg51.h"
#include"12864.h"
#include"keyboard.h"
#include"sht11.h"
#include"motor.h"
#include"serial.h"
#include"delay.h"

sbit LED = P3^2;
sbit BUZZ = P3^3;

uchar display_password[6]={10,10,10,10,10,10};
uchar password[]={1,2,3,4,5,6};

char* prt_password=display_password;
uchar mode = 0,error = 0,count = 0,buzzmod = 0,serialdata = 0,serial_flag = 0;
int Temperature = 0,Humidity = 0;

void Timeinit()
{
	TMOD = 0x21; //定时器0和1开启；
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;	
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

void scan_key()
{
	uchar keydata = 0;
	uint i = 0,flag = 0;
	if(get_key() != 0xff)
	{
		keydata = get_key();
		while(get_key() != 0xff);
	}
	if(keydata > '0' && keydata < '9' && display_password[5] == 10) //密码
	{
		*prt_password++=keydata-'0';
	}
	else if(keydata == '*') //确定
	{
		for(i=0;i<6;i++)
		{
			if(display_password[i] != password[i])flag = 1;
		}
		if(flag == 1)
		{
			error++; //密码错误
			BUZZ = 0;
			DelayMs(500);
			BUZZ = 1;
			Sendstring(1,"error");
			Sendchar(' ');
			Sendchar('0'+error);
			Sendstring(1,"\r\n");
		}
		else 
		{
			mode = 1; //密码正确
			error = 0;
			motor_angle(180);
			BUZZ = 0;
			DelayMs(500);
			BUZZ = 1;
			ClearScreen(0);
			Sendstring(1,"correct");
			Sendstring(1,"\r\n");
		}
		if(error == 3)
		{
			mode = 2; //锁定
			LED = 1;
			ClearScreen(0);
			Sendstring(5,"error 3");
		}
		for(i=0;i<6;i++)
		{
			display_password[i] = 10;
		}
		prt_password=display_password;
	}
	else if(keydata == '#') //取消
	{
		for(i=0;i<6;i++)
		{
			display_password[i] = 10;
		}
		prt_password=display_password;
	}
	else if(keydata == 'A' && error < 3) //关闭密码箱
	{
		mode = 0;
		error = 0;
		motor_angle(0);
		ClearScreen(0);
	}
	else if(keydata == 'D')
	{
		mode = 0;
		error = 0;
		motor_angle(0);
		ClearScreen(0);		
		LED = 0;
	}
}


void main()
{
	InitLCD();
	Timeinit();
	Serialinit();
	motor_angle(0);
	LED = 0;
	BUZZ = 1;
	while(1)
	{
		//第一行
		Display_ZM(1,0,8*0,19);
		Display_FH(1,0,8*1,4);
		Display_SZ(1,0,8*2,Temperature/100);
		Display_SZ(1,0,8*3,(Temperature/10)%10);
		Display_FH(1,0,8*4,11);
		Display_SZ(1,0,8*5,Temperature%10);		
		Display_HZ(1,0,8*6,16);	
		
		Display_ZM(2,0,8*0,7);
		Display_FH(2,0,8*1,4);
		Display_SZ(2,0,8*2,Humidity/100);
		Display_SZ(2,0,8*3,(Humidity/10)%10);		
		Display_FH(2,0,8*4,11);
		Display_SZ(2,0,8*5,Humidity%10);	
		Display_FH(2,0,8*6,7);
		
		//第二行
		if(mode == 0 || mode == 1)
		{
			Display_HZ(1,2,16*0,0);
			Display_HZ(1,2,16*1,1);
			Display_HZ(1,2,16*2,2);
			Display_FH(1,2,16*3,4);		
			Display_SZ(1,2,16*3+8,0);
			
			Display_SZ(2,2,8*0,0);
			Display_SZ(2,2,8,1);		
		}
		else if(mode == 2)
		{
			Display_HZ(1,2,16*2,12);
			Display_HZ(1,2,16*3,13);	
			
			Display_HZ(2,2,16*0,14);		
			Display_HZ(2,2,16*1,15);				
		}
		
		//第三行
		if(mode == 0)
		{
			Display_SZ(1,4,8*0,display_password[0]);
			Display_SZ(1,4,8*1,display_password[1]);
			Display_SZ(1,4,8*2,display_password[2]);
			Display_SZ(1,4,8*3,display_password[3]);
			Display_SZ(1,4,8*4,display_password[4]);
			Display_SZ(1,4,8*5,display_password[5]);		
			
			Display_SZ(2,4,16*3,error);
		}
		else if(mode == 1)
		{
			Display_HZ(1,4,16*0,0);
			Display_HZ(1,4,16*1,1);
			Display_HZ(1,4,16*2,2);		
			Display_HZ(1,4,16*3,3);		
			
			Display_HZ(2,4,16*0,4);		
			Display_HZ(2,4,16*1,5);					
		}
		else if(mode == 2)
		{
			Display_HZ(1,4,16*1,0);
			Display_HZ(1,4,16*2,1);
			Display_HZ(1,4,16*3,2);		
			
			Display_HZ(2,4,16*0,3);		
			Display_HZ(2,4,16*1,6);		
			Display_HZ(2,4,16*2,7);				
		}	
		
		//第四行
		if(mode == 0)
		{
			Display_HZ(1,6,16*0,8);
			Display_HZ(1,6,16*1,9);	

			Display_HZ(2,6,16*2,10);
			Display_HZ(2,6,16*3,11);	
		}
		
		TH_output(&Temperature,&Humidity);
		scan_key();
		if(Temperature / 10 > 40 || Humidity / 10 > 80)
		{
			buzzmod = 20;
			if(serial_flag == 0)Sendstring(5,"abnormalTH\r\n"),serial_flag = 1;
		}
		else buzzmod = 0,BUZZ = 1,serial_flag = 0;
	}
}

void Time_interrupt() interrupt 1
{
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;	
	if(buzzmod != 0)
	{	
		count++;
		if(count >= buzzmod)
		{
			BUZZ = !BUZZ;
			count = 0;
		}
	}
}


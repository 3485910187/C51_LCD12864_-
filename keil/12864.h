#ifndef __12864_H__
#define __12864_H__

#include <reg51.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char
	
#define DATA P0

sbit EN = P2^0;
sbit RW = P2^1;
sbit RS = P2^2;
sbit RES = P2^3;
sbit cs1 = P2^4;
sbit cs2 = P2^5;

//×ÖÄ£Êý×é
uchar code Hzk[];
uchar code Szk[];
uchar code Fhk[];
uchar code Zmk[];

void CheckState();
void SendCommandToLCD(uchar com);
void SetLine(uchar page);
void SetStartLine(uchar startline);
void SetColumn(uchar column);
void SetOnOff(uchar onoff);
void WriteByte(uchar dat)	;
void SelectScreen(uchar screen);
void ClearScreen(uchar screen);	
void InitLCD() ;
void Display_HZ(uchar screen,uchar page,uchar column,uchar number);
void Display_SZ(uchar screen,uchar page,uchar column,uchar number);
void Display_ZM(uchar screen,uchar page,uchar column,uchar number);
void Display_FH(uchar screen,uchar page,uchar column,uchar number);

#endif
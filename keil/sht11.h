#ifndef __SHT11_H
#define __SHT11_H

#include <reg51.h>
#include <intrins.h>

#define noACK 0				//继续传输数据，用于判断是否结束通讯
#define ACK   1             //结束数据传输；
							//地址  命令  读/写
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define RESET        0x1e   //000   1111    0

sbit Sht11DATA = P2^7;
sbit SCK = P2^6;

enum {TEMP,HUMI};

typedef union		   //定义共同类型
{ unsigned int i;	   //i表示测量得到的温湿度数据（int 形式保存的数据） 
 float f;			   //f表示测量得到的温湿度数据（float 形式保存的数据）
} value;

/********     SHT11函数声明      ********/
void s_connectionreset(void);
char s_measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode);
void calc_sth10(float *p_humidity ,float *p_temperature);
//float calc_dewpoint(float h,float t);

void TH_output(int *T,int *H);

#endif
#ifndef __SERIAL_H
#define __SERIAL_H

#include"reg51.h"

void Serialinit();
void Sendchar( unsigned char da);
void Sendstring(int x,char *str);


#endif
#ifndef VIRTUALLCDH
#define VIRTUALLCDH

#include "m68k.h"
#include "memmap.h"

void lcd_init(int w,int h);
extern addrbank lcd_bank;

#endif
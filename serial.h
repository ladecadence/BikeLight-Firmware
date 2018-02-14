/*
   Copyright (c) 2014 - David Pello

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

*/

#include "msp430.h"
#include <inttypes.h>

#ifndef __MSP430_USART_H
#define __MSP430_USART_H

/* Baudrates for X Clock */
#define     BR0_BD_115200     	138
#define     BR1_BD_115200    	0x00

#define     BR0_BD_115200_8MHZ  0x45
#define     BR1_BD_115200_8MHZ  0x00
#define     MOD_BD_115200_8MHZ  0xAA

#define     BR0_BD_115200_12MHZ 0x68 
#define     BR1_BD_115200_12MHZ 0x00    
#define     MOD_BD_115200_12MHZ 0x08

uint8_t serial_available;

void serial_init();
void serial_putch(uint8_t c);
void serial_puts(const uint8_t *ptr);
uint8_t serial_getc();

#endif

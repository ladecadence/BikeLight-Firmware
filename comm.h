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

#ifndef __COMM_H
#define __COMM_H

#include <inttypes.h>
#include <msp430.h>

/* Paquetes */
#define PKT_COMMAND		0x11
#define PKT_DATA		0x22
#define PKT_STAT		0x33
#define PKT_INFO		0x44

/* Comandos */
#define CMD_ID			0x11
#define CMD_PRG_FLASH		0x22
#define CMD_ERASE_FLASH		0x33
#define CMD_END	        	0xFF

typedef struct
{
	char type;
	char data;
} packet_t;

uint8_t comm_receive_byte() ;
void comm_send_packet(packet_t* packet) ;
void comm_receive_packet(packet_t* packet) ;

#endif

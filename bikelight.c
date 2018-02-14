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

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <msp430g2553.h>

#include "bikelight.h"
#include "matrix.h"
#include "flash.h"
#include "comm.h"
#include "serial.h"
#include "adc.h"

const char CH[] = {
	3, 8, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, // space
	1, 8, 0b1011111, 0b0000000, 0b0000000, 0b0000000, 0b0000000, // !
	3, 8, 0b0000011, 0b0000000, 0b0000011, 0b0000000, 0b0000000, // "
	5, 8, 0b0010100, 0b0111110, 0b0010100, 0b0111110, 0b0010100, // #
	4, 8, 0b0100100, 0b1101010, 0b0101011, 0b0010010, 0b0000000, // $
	5, 8, 0b1100011, 0b0010011, 0b0001000, 0b1100100, 0b1100011, // %
	5, 8, 0b0110110, 0b1001001, 0b1010110, 0b0100000, 0b1010000, // &
	1, 8, 0b0000011, 0b0000000, 0b0000000, 0b0000000, 0b0000000, // '
	3, 8, 0b0011100, 0b0100010, 0b1000001, 0b0000000, 0b0000000, // (
	3, 8, 0b1000001, 0b0100010, 0b0011100, 0b0000000, 0b0000000, // )
	5, 8, 0b0101000, 0b0011000, 0b0001110, 0b0011000, 0b0101000, // *
	5, 8, 0b0001000, 0b0001000, 0b0111110, 0b0001000, 0b0001000, // +
	2, 8, 0b10110000, 0b1110000, 0b0000000, 0b0000000, 0b0000000, // ,
	4, 8, 0b0001000, 0b0001000, 0b0001000, 0b0001000, 0b0000000, // -
	2, 8, 0b1100000, 0b1100000, 0b0000000, 0b0000000, 0b0000000, // .
	4, 8, 0b1100000, 0b0011000, 0b0000110, 0b0000001, 0b0000000, // /
	4, 8, 0b0111110, 0b1000001, 0b1000001, 0b0111110, 0b0000000, // 0
	3, 8, 0b1000010, 0b1111111, 0b1000000, 0b0000000, 0b0000000, // 1
	4, 8, 0b1100010, 0b1010001, 0b1001001, 0b1000110, 0b0000000, // 2
	4, 8, 0b0100010, 0b1000001, 0b1001001, 0b0110110, 0b0000000, // 3
	4, 8, 0b0011000, 0b0010100, 0b0010010, 0b1111111, 0b0000000, // 4
	4, 8, 0b0100111, 0b1000101, 0b1000101, 0b0111001, 0b0000000, // 5
	4, 8, 0b0111110, 0b1001001, 0b1001001, 0b0110000, 0b0000000, // 6
	4, 8, 0b1100001, 0b0010001, 0b0001001, 0b0000111, 0b0000000, // 7
	4, 8, 0b0110110, 0b1001001, 0b1001001, 0b0110110, 0b0000000, // 8
	4, 8, 0b0000110, 0b1001001, 0b1001001, 0b0111110, 0b0000000, // 9
	2, 8, 0b01010000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, // :
	2, 8, 0b10000000, 0b01010000, 0b0000000, 0b0000000, 0b0000000, // ;
	3, 8, 0b0010000, 0b0101000, 0b1000100, 0b0000000, 0b0000000, // <
	3, 8, 0b0010100, 0b0010100, 0b0010100, 0b0000000, 0b0000000, // =
	3, 8, 0b1000100, 0b0101000, 0b0010000, 0b0000000, 0b0000000, // >
	4, 8, 0b0000010, 0b1011001, 0b0001001, 0b0000110, 0b0000000, // ?
	5, 8, 0b0111110, 0b1001001, 0b1010101, 0b1011101, 0b0001110, // @
	4, 8, 0b1111110, 0b0010001, 0b0010001, 0b1111110, 0b0000000, // A
	4, 8, 0b1111111, 0b1001001, 0b1001001, 0b0110110, 0b0000000, // B
	4, 8, 0b0111110, 0b1000001, 0b1000001, 0b0100010, 0b0000000, // C
	4, 8, 0b1111111, 0b1000001, 0b1000001, 0b0111110, 0b0000000, // D
	4, 8, 0b1111111, 0b1001001, 0b1001001, 0b1000001, 0b0000000, // E
	4, 8, 0b1111111, 0b0001001, 0b0001001, 0b0000001, 0b0000000, // F
	4, 8, 0b0111110, 0b1000001, 0b1001001, 0b1111010, 0b0000000, // G
	4, 8, 0b1111111, 0b0001000, 0b0001000, 0b1111111, 0b0000000, // H
	3, 8, 0b1000001, 0b1111111, 0b1000001, 0b0000000, 0b0000000, // I
	4, 8, 0b0110000, 0b1000000, 0b1000001, 0b0111111, 0b0000000, // J
	4, 8, 0b1111111, 0b0001000, 0b0010100, 0b1100011, 0b0000000, // K
	4, 8, 0b1111111, 0b1000000, 0b1000000, 0b1000000, 0b0000000, // L
	5, 8, 0b1111111, 0b0000010, 0b0001100, 0b0000010, 0b1111111, // M
	5, 8, 0b1111111, 0b0000100, 0b0001000, 0b0010000, 0b1111111, // N
	4, 8, 0b0111110, 0b1000001, 0b1000001, 0b0111110, 0b0000000, // O
	4, 8, 0b1111111, 0b0001001, 0b0001001, 0b0000110, 0b0000000, // P
	4, 8, 0b0111110, 0b1000001, 0b1000001, 0b10111110, 0b0000000, // Q
	4, 8, 0b1111111, 0b0001001, 0b0001001, 0b1110110, 0b0000000, // R
	4, 8, 0b1000110, 0b1001001, 0b1001001, 0b0110010, 0b0000000, // S
	5, 8, 0b0000001, 0b0000001, 0b1111111, 0b0000001, 0b0000001, // T
	4, 8, 0b0111111, 0b1000000, 0b1000000, 0b0111111, 0b0000000, // U
	5, 8, 0b0001111, 0b0110000, 0b1000000, 0b0110000, 0b0001111, // V
	5, 8, 0b0111111, 0b1000000, 0b0111000, 0b1000000, 0b0111111, // W
	5, 8, 0b1100011, 0b0010100, 0b0001000, 0b0010100, 0b1100011, // X
	5, 8, 0b0000111, 0b0001000, 0b1110000, 0b0001000, 0b0000111, // Y
	4, 8, 0b1100001, 0b1010001, 0b1001001, 0b1000111, 0b0000000, // Z
	2, 8, 0b1111111, 0b1000001, 0b0000000, 0b0000000, 0b0000000, // [
	4, 8, 0b0000001, 0b0000110, 0b0011000, 0b1100000, 0b0000000, // backslash
	2, 8, 0b1000001, 0b1111111, 0b0000000, 0b0000000, 0b0000000, // ]
	3, 8, 0b0000010, 0b0000001, 0b0000010, 0b0000000, 0b0000000, // hat
	4, 8, 0b1000000, 0b1000000, 0b1000000, 0b1000000, 0b0000000, // _
	2, 8, 0b0000001, 0b0000010, 0b0000000, 0b0000000, 0b0000000, // `
	4, 8, 0b0100000, 0b1010100, 0b1010100, 0b1111000, 0b0000000, // a
	4, 8, 0b1111111, 0b1000100, 0b1000100, 0b0111000, 0b0000000, // b
	4, 8, 0b0111000, 0b1000100, 0b1000100, 0b0101000, 0b0000000, // c
	4, 8, 0b0111000, 0b1000100, 0b1000100, 0b1111111, 0b0000000, // d
	4, 8, 0b0111000, 0b1010100, 0b1010100, 0b0011000, 0b0000000, // e
	3, 8, 0b0000100, 0b1111110, 0b0000101, 0b0000000, 0b0000000, // f
	4, 8, 0b10011000, 0b10100100, 0b10100100, 0b01111000, 0b0000000, // g
	4, 8, 0b1111111, 0b0000100, 0b0000100, 0b1111000, 0b0000000, // h
	3, 8, 0b1000100, 0b1111101, 0b1000000, 0b0000000, 0b0000000, // i
	4, 8, 0b1000000, 0b10000000, 0b10000100, 0b1111101, 0b0000000, // j
	4, 8, 0b1111111, 0b0010000, 0b0101000, 0b1000100, 0b0000000, // k
	3, 8, 0b1000001, 0b1111111, 0b1000000, 0b0000000, 0b0000000, // l
	5, 8, 0b1111100, 0b0000100, 0b1111100, 0b0000100, 0b1111000, // m
	4, 8, 0b1111100, 0b0000100, 0b0000100, 0b1111000, 0b0000000, // n
	4, 8, 0b0111000, 0b1000100, 0b1000100, 0b0111000, 0b0000000, // o
	4, 8, 0b11111100, 0b0100100, 0b0100100, 0b0011000, 0b0000000, // p
	4, 8, 0b0011000, 0b0100100, 0b0100100, 0b11111100, 0b0000000, // q
	4, 8, 0b1111100, 0b0001000, 0b0000100, 0b0000100, 0b0000000, // r
	4, 8, 0b1001000, 0b1010100, 0b1010100, 0b0100100, 0b0000000, // s
	3, 8, 0b0000100, 0b0111111, 0b1000100, 0b0000000, 0b0000000, // t
	4, 8, 0b0111100, 0b1000000, 0b1000000, 0b1111100, 0b0000000, // u
	5, 8, 0b0011100, 0b0100000, 0b1000000, 0b0100000, 0b0011100, // v
	5, 8, 0b0111100, 0b1000000, 0b0111100, 0b1000000, 0b0111100, // w
	5, 8, 0b1000100, 0b0101000, 0b0010000, 0b0101000, 0b1000100, // x
	4, 8, 0b10011100, 0b10100000, 0b10100000, 0b1111100, 0b0000000, // y
	3, 8, 0b1100100, 0b1010100, 0b1001100, 0b0000000, 0b0000000, // z
	3, 8, 0b0001000, 0b0110110, 0b1000001, 0b0000000, 0b0000000, // {
	1, 8, 0b1111111, 0b0000000, 0b0000000, 0b0000000, 0b0000000, // |
	3, 8, 0b1000001, 0b0110110, 0b0001000, 0b0000000, 0b0000000, // }
      4, 8, 0b0001000, 0b0000100, 0b0001000, 0b0000100, 0b0000000, // ~
};

const uint8_t anim[] = {
	2, 150, 5,
	8, 8, 0b00000000,0b01000000,0b01000100,0b01000000,0b01000000,0b01000100,0b01000000,0b00000000,
	8, 8, 0b00000000,0b00100000,0b01000100,0b01000000,0b01000000,0b01000100,0b00100000,0b00000000,
	3, 75, 10,
	8, 8, 0b00000000, 0b00000000, 0b11111110, 0b10000011, 0b10000011, 0b11111110, 0b00000000, 0b00000000, 
	8, 8, 0b00000000, 0b00000000, 0b11111110, 0b11110011, 0b11110011, 0b11111110, 0b00000000, 0b00000000, 
	8, 8, 0b00000000, 0b00000000, 0b11111110, 0b11111111, 0b11111111, 0b11111110, 0b00000000, 0b00000000,
	0,
};

const uint8_t icons[] = {
	8, 8, 0b00000000, 0b00011100, 0b00111111, 0b11111100, 0b11111100, 0b00111111, 0b00011100, 0b00000000, // connected
	8, 8, 0b11000000, 0b10001000, 0b10011000, 0b10111111, 0b10111111, 0b10011000, 0b10001000, 0b11000000, // empty memory
	8, 8, 0b00000000, 0b00000000, 0b11111110, 0b10000011, 0b10000011, 0b11111110, 0b00000000, 0b00000000, // battery low
	8, 8, 0b00000000, 0b00000000, 0b11111110, 0b11110011, 0b11110011, 0b11111110, 0b00000000, 0b00000000, // battery medium
	8, 8, 0b00000000, 0b00000000, 0b11111110, 0b11111111, 0b11111111, 0b11111110, 0b00000000, 0b00000000, // battery full
};

/* variables */

/* buffer to store flash data */
uint8_t data_buffer[BUFFER_SIZE];

/* pins */

uint8_t buffer[10];
int i,j;
uint8_t *data_addr;    /* Address of a flash memory segment*/
packet_t packet;
uint8_t flash_ok;
uint32_t voltage;
uint8_t num_strings, current_string;
uint8_t num_animations, current_animation;
uint8_t *anim_addr;    /* animation frames address */

void delay_ms(uint16_t ms)
{
	while (ms--)
	{
		__delay_cycles(CPU_CLOCK/1000); /* 1000 us = 1000 cycles at 1MHz */
	}
}

void bl_debug_serial(uint8_t data)
{
	packet_t p;
	p.type=PKT_DATA; p.data = data;
	comm_send_packet(&p);
}

uint16_t bl_check_battery()
{
	uint32_t val;

	/* enable battery divisor */
	P2DIR |= BIT3; /* output */
	P2OUT &= ~(BIT3); /* ground */

	val = adc_read(A0);

	val = val * 4200;
	val = val / V_CONV;

	/* disable battery divisor */
	P2DIR &= ~(BIT3);

	return val;
}

void bl_print_char_with_shift(char c, int shift_speed)
{
	if (c < 32) return;
	c -= 32;
	memcpy(buffer, CH + 7*c, 7);
	matrix_write_sprite(8, 0, buffer);
	matrix_set_column(8 + buffer[0], 0);

	int i;
	for (i=0; i<buffer[0]+1; i++) 
	{
		delay_ms(shift_speed);
		matrix_shift_left(0, 0);
	}
}

void bl_print_string_with_shift(char* s, int shift_speed)
{
	while (*s != 0)
	{
		bl_print_char_with_shift(*s, shift_speed);
		s++;
	}
}

void bl_step_animations()
{
	uint8_t t, a;
	uint8_t sd, ad;   /* delays */
	uint8_t nc;       /* num of chars */
	uint8_t nf;       /* num of frames */
	uint8_t nr;       /* number of repeats */
	uint16_t curr_addr; /* address of current data */

	if (num_strings>0)
	{
		/* get current string address */
		t = current_string-1;
		curr_addr = 0;
		while(t)
		{
			/* number of chars */
			nc = *(uint8_t*)(DATA_START+curr_addr);
			curr_addr = curr_addr + nc + 5;

			t--;
		}
		/* get current string  delay_ms */
		sd = *(uint8_t*)(DATA_START+curr_addr+1);
		/* print current string */
		bl_print_string_with_shift((char*)(DATA_START+curr_addr+2), sd);

		/* next */
		current_string++;
		if (current_string > num_strings)
			current_string = 1;
	}

	if (num_animations>0)
	{
		/* get current animation address */
		t = current_animation-1;
		curr_addr = 0;
		while (t)
		{
			/* number of frames */
			nf = *(uint8_t*)(anim_addr+curr_addr);
			/* add memory used by the animation */
			curr_addr = curr_addr + 3 + (10 * nf);

			t--;
		}

		//bl_debug_serial(curr_addr);

		/* get number of frames */
		nf = *(uint8_t*)(anim_addr+curr_addr);
		/* get current animation delay_ms */
		ad = *(uint8_t*)(anim_addr+curr_addr+1);
		/* get number of repeats */
		nr = *(uint8_t*)(anim_addr+curr_addr+2);
		//bl_debug_serial(nf);

		/* show frames */
		for (t=0; t<nr; t++)
		{
			for (a=0; a<nf; a++)
			{
				matrix_write_sprite(0,0, (uint8_t*)((anim_addr+curr_addr+3)+(10*a)));
				delay_ms(ad);
			}
		}

		/* next */
		current_animation++ ;
		if (current_animation > num_animations)
			current_animation = 1;
	}

}

void bl_system_status()
{
	packet.type = PKT_DATA; 
	packet.data = ID;
	comm_send_packet(&packet);

	packet.type = PKT_DATA; 
	packet.data = VER_MAYOR;
	comm_send_packet(&packet);

	packet.type = PKT_DATA; 
	packet.data = VER_MINOR;
	comm_send_packet(&packet);
}

void bl_info()
{
	/* send voltage */
	packet.type = PKT_DATA; 
	packet.data = (uint8_t) voltage & 0x00ff;
	comm_send_packet(&packet);

	packet.type = PKT_DATA; 
	packet.data = (uint8_t) (voltage & 0xff00) >> 8;
	comm_send_packet(&packet);

}

void bl_check_flash()
{

	uint8_t nc, nf, t;
	uint16_t curr_addr=0;

	/* check flash */
	if (*(uint8_t*)FLASH_START == FLASH_MAGIC)
	{
		/* init variables from flash */
		num_strings =  *(uint8_t*)NUM_STRINGS;
		num_animations = *(uint8_t*)NUM_ANIMS;


		/* get animation addresses */
		if (num_strings>0)
		{
			/* get current string address */
			t = num_strings;
			while(t)
			{
				/* number of chars */
				nc = *(uint8_t*)(DATA_START+curr_addr);
				curr_addr = curr_addr + nc + 5;

				t--;
			}
		}
		/* ok, we are at the end of the strings, begginning of the animations. */
		anim_addr = (uint8_t*)DATA_START+curr_addr;

		if (num_animations>0)
		{
			/* get current animation address */
			t = num_animations;
			while (t)
			{
				/* number of frames */
				nf = *(uint8_t*)(DATA_START+curr_addr);
				/* add memory used by the animation */
				curr_addr = curr_addr + 3 + (10 * nf);

				t--;
			}
		}

		/* ok, end of animations, check for the magic number */
		if (*(uint8_t*)(DATA_START+curr_addr) == FLASH_MAGIC)
			flash_ok = 1;
		else
			flash_ok = 0;

	}
	else
		flash_ok = 0;

	current_string = 1;
	current_animation = 1;
}



void bl_reset()
{
	/* generate a hardware reset writing to the watchdog without password */
	WDTCTL = 0;
}

uint8_t bl_write_data() 
{
	packet_t p, q;
	uint8_t i, d;
	uint8_t *addr;
	uint8_t check;

	/* erase flash , 0x0E000 - 0x0FE00*/
	for (i=0; i<15; i++)
	{
		addr = (uint8_t *)FLASH_START+(i*512);
		flash_erase(addr);
	}

	/* flash erased, not longer ok */
	flash_ok = 0;

	/* send ack, ready to write */
	p.type = PKT_STAT; 
	p.data = STAT_OK;
	comm_send_packet(&p);


	/* write loop */
	p.data = CMD_PRG_FLASH;
	addr = (uint8_t*)FLASH_START;

	while (p.data == CMD_PRG_FLASH)
	{
		check = 0;

		/* receive a complete buffer */
		for (i=0; i<BUFFER_SIZE; i++)
		{
			d = comm_receive_byte();
			data_buffer[i] = d;
			/* acumulate checksum */
			check = check + 1 + d;
		}

		/* program data */
		flash_write(addr, data_buffer);
		addr += BUFFER_SIZE;

		/* send checksum */
		q.type = PKT_DATA; q.data = check;
		comm_send_packet(&q);

		/* wait for an answer */
		comm_receive_packet(&p);
	}

	/* ok, we've written stuff... */
	bl_check_flash();

	return STAT_OK;
}


int main()
{

	WDTCTL = WDTPW + WDTHOLD;	/* Stop WDT */

	/* setup clocks */
	/* by default DCOCLKDIV runs MCLK and SMCLK at 16MHz */

	BCSCTL1 = CALBC1_12MHZ;          /* Set range */
	DCOCTL = CALDCO_12MHZ;
	BCSCTL2 |= DIVS_0;           /* SMCLK = DCO / 1 = 12MHz */


	WRITE_SR(GIE);	//Enable global interrupts


	/* test */
	/*
	   data_addr = (uint8_t*)FLASH_START;
	   data_buffer[0] = 0xA5; data_buffer[1] = 0x02; data_buffer[2] = 0x02; data_buffer[3] = 50; data_buffer[4] = 75;
	   flash_erase(data_addr);
	   flash_write(data_addr, data_buffer);  


	   strcpy((char*)data_buffer, "precausiooon amigo conductoooor  \0"); 
	   data_addr = (uint8_t*)STRING1_ADDR;
	   flash_erase(data_addr);
	   flash_write(data_addr, data_buffer);
	   strcpy((char*)data_buffer, "bici hardcore!    \0"); 
	   data_addr = (uint8_t*)STRING2_ADDR;
	   flash_write(data_addr, data_buffer);

	   data_addr = (uint8_t*)anim_addr;
	   flash_erase(data_addr);
	   flash_write(data_addr, (uint8_t*) anim);
	   */

	matrix_init();
	matrix_set_intensity(15);

	serial_init();

	bl_print_string_with_shift("ladecadence.net   ", 50);

	/* Batt pin, input */
	ADC10AE0 |= BIT0;
	/* Batt divisor enable pin, input (high-z) */ 
	P2DIR &= ~(BIT3);
	/* check voltage */
	voltage = bl_check_battery();

	/*
	   char volt[10];
	   ltoa(voltage, volt, 10);
	   strcat(volt, "  ");
	   bl_print_string_with_shift(volt, 100);
	   */

	/* select icon */
	if (voltage > 4100)
		j=40;
	else if (voltage > 3700)
		j=30;
	else
		j=20;

	/* show battery */ 
	matrix_set_intensity(0);
	matrix_write_sprite(0,0, icons+j);
	for (i=0; i<=15; i++)
	{
		matrix_set_intensity(i);
		delay_ms(100);
	}
	delay_ms(1000);
	for (i=15; i>=0; i--)
	{
		matrix_set_intensity(i);
		delay_ms(100);
	}

	matrix_clear();
	delay_ms(500);
	matrix_set_intensity(15);

	/* check flash */
	bl_check_flash();

	/* start */
	current_string = 1;
	current_animation = 1;


	for(;;)
	{
		/* check voltage */
		voltage = bl_check_battery();

		if (voltage < 3500)
		{
			matrix_write_sprite(0,0, icons+20);
			delay_ms(100);
			matrix_clear();
			delay_ms(100);
		}
		else
			if (flash_ok)
				bl_step_animations();
			else
			{
				if (voltage >= 3500)
					matrix_write_sprite(0,0, icons+10);
			}
	}

	return 0;

}

void parse_serial_data()
{
	//__dint();
	IE2 &= ~UCA0RXIE; 
	matrix_write_sprite(0,0, icons);

	comm_receive_packet(&packet);
	switch (packet.type) {
		case PKT_COMMAND:
			switch (packet.data) {
				case CMD_ID:
					bl_info();
					break;
				case CMD_PRG_FLASH:
					bl_write_data();
					break;
			}
			break;
		case PKT_INFO:
			bl_system_status();
			break;
		default:
			break;
	}
	//delay_ms(1000);
	matrix_clear();
	//__eint();
	IE2 |= UCA0RXIE; 
}

/* 
 * Serial receive interrupt
 *
 */

__attribute__ ((interrupt(USCIAB0RX_VECTOR))) void usart0_rx(void)
{
	serial_available = 1;
	parse_serial_data();
	UC0IFG &= ~UCA0RXIFG;
}



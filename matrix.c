/*
   Copyright (c) 2014 - David Pello

   This file is part of BikeLight.

   BikeLight is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   BikeLight is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with BileLight.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "matrix.h"

uint8_t matrix_buffer[8];


void matrix_init()
{
	/* set pins */
	MATRIX_DIR |= MATRIX_DATA_PIN | MATRIX_CLOCK_PIN | MATRIX_LOAD_PIN;

	/* high */
	MATRIX_OUT |= MATRIX_CLOCK_PIN;

	matrix_set_command(max7219_reg_scanLimit, 0x07);      
	matrix_set_command(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
	matrix_set_command(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	matrix_set_command(max7219_reg_displayTest, 0x00); // no display test

	/* empty registers, turn all LEDs off */
	matrix_clear();
	matrix_set_intensity(0x0f);    // the first 0x0f is the value you can set
}

void matrix_shift_out(uint8_t data)
{
	uint8_t i;

	for (i=8; i > 0; i--)
	{
		MATRIX_OUT &= ~(MATRIX_CLOCK_PIN);
		uint8_t mask = 1 << (i-1);
		if (data & mask)
			MATRIX_OUT |= MATRIX_DATA_PIN;
		else
			MATRIX_OUT &= ~(MATRIX_DATA_PIN);

		MATRIX_OUT |= MATRIX_CLOCK_PIN;
	}
}

void matrix_set_intensity(uint8_t intensity)
{
	matrix_set_command(max7219_reg_intensity, intensity);
}

void matrix_clear()
{
	uint8_t i;

	for (i=0; i<8; i++) 
		matrix_set_column(i,0);

	for (i=0; i<80; i++)
		matrix_buffer[i] = 0;
}

void matrix_set_command(uint8_t command, uint8_t value)
{
	MATRIX_OUT |= (MATRIX_LOAD_PIN);

	matrix_shift_out(command);
	matrix_shift_out(value);

	MATRIX_OUT &= ~(MATRIX_LOAD_PIN);
	MATRIX_OUT |= MATRIX_LOAD_PIN;
}

void matrix_set_column(uint8_t col, uint8_t value)
{
	uint8_t n = col / 8;
	uint8_t c = col % 8;

	MATRIX_OUT |= (MATRIX_LOAD_PIN);

	if (n==0)
	{
		matrix_shift_out(c + 1);
		matrix_shift_out(value);
	}
	else
	{
		matrix_shift_out(0);
		matrix_shift_out(0);
	}
	MATRIX_OUT &= ~(MATRIX_LOAD_PIN);
	MATRIX_OUT |= MATRIX_LOAD_PIN;

	matrix_buffer[col] = value;
}

void matrix_set_dot(uint8_t col, uint8_t row, uint8_t value)
{
	bit_write(matrix_buffer[col], row, value);

	uint8_t n = col / 8;
	uint8_t c = col % 8;
	MATRIX_OUT = (MATRIX_LOAD_PIN);
	if (n==0)
	{
		matrix_shift_out(c + 1);
		matrix_shift_out(matrix_buffer[col]);
	}
	else
	{
		matrix_shift_out(0);
		matrix_shift_out(0);
	}
	MATRIX_OUT &= ~(MATRIX_LOAD_PIN);
	MATRIX_OUT |= MATRIX_LOAD_PIN;
}

void matrix_write_sprite(int x, int y, const uint8_t* sprite)
{
	uint8_t i, j;
	uint8_t w = sprite[0];
	uint8_t h = sprite[1];

	if (h == 8 && y == 0)
		for (i=0; i<w; i++)
		{
			int c = x + i;
			if (c>=0 && c<80)
				matrix_set_column(c, sprite[i+2]);
		}
	else
		for (i=0; i<w; i++)
			for (j=0; j<h; j++)
			{
				int c = x + i;
				int r = y + j;
				if (c>=0 && c<80 && r>=0 && r<8)
					matrix_set_dot(c, r, bit_read(sprite[i+2], j));
			}
}

void matrix_reload()
{
	uint8_t i;
	for (i=0; i<8; i++)
	{
		uint8_t col = i;
		MATRIX_OUT = (MATRIX_LOAD_PIN);

		matrix_shift_out(i + 1);
		matrix_shift_out(matrix_buffer[col]);
		col += 8;
		MATRIX_OUT &= ~(MATRIX_LOAD_PIN);
		MATRIX_OUT |= MATRIX_LOAD_PIN;
	}
}

void matrix_shift_left(uint8_t rotate, uint8_t fill_zero)
{
	uint8_t old = matrix_buffer[0];
	uint8_t i;
	for (i=0; i<80; i++)
		matrix_buffer[i] = matrix_buffer[i+1];
	if (rotate) matrix_buffer[8-1] = old;
	else if (fill_zero) matrix_buffer[8-1] = 0;

	matrix_reload();
}

void matrix_shift_right(uint8_t rotate, uint8_t fill_zero)
{
	uint8_t last = 8-1;
	uint8_t old = matrix_buffer[last];
	uint8_t i;
	for (i=79; i>0; i--)
		matrix_buffer[i] = matrix_buffer[i-1];
	if (rotate) matrix_buffer[0] = old;
	else if (fill_zero) matrix_buffer[0] = 0;

	matrix_reload();
}

void matrix_shift_up(uint8_t rotate)
{
	uint8_t i;
	for (i=0; i<8; i++)
	{
		uint8_t b = matrix_buffer[i] & 1;
		matrix_buffer[i] >>= 1;
		if (rotate) bit_write(matrix_buffer[i], 7, b);
	}
	matrix_reload();
}

void matrix_shift_down(uint8_t rotate)
{
	uint8_t i;
	for (i=0; i<8; i++)
	{
		uint8_t b = matrix_buffer[i] & 128;
		matrix_buffer[i] <<= 1;
		if (rotate) bit_write(matrix_buffer[i], 0, b);
	}
	matrix_reload();
}




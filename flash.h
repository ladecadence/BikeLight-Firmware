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

#ifndef __FLASH_H
#define __FLASH_H

#include <msp430.h>
#include <inttypes.h>
//#include <signal.h>

#define FLASH_START     0x0E000
#define NUM_STRINGS     0x0E001
#define NUM_ANIMS       0x0E002
#define DATA_START      0x0E003

#define FLASH_MAGIC     0xA5  // magic number to check if flash has been flashed

void flash_erase(uint8_t *addr);
void flash_write_byte(uint8_t *addr, uint8_t data);
void flash_write(uint8_t *addr, uint8_t* buffer);


#endif

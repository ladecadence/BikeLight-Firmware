#
#   Copyright (c) 2014 - David Pello
#
#   Permission is hereby granted, free of charge, to any person obtaining a copy
#   of this software and associated documentation files (the "Software"), to deal
#   in the Software without restriction, including without limitation the rights
#   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#   copies of the Software, and to permit persons to whom the Software is
#   furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included in
#   all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#   THE SOFTWARE.
#
#

SOURCES=bikelight.c comm.c flash.c matrix.c serial.c adc.c 
BINARY=bikelight

MCU=msp430g2553
CC=msp430-gcc
OBJC=msp430-objcopy
OBJD=msp430-objdump
SIZE=msp430-size
CFLAGS=-Os -Wall -g -mmcu=$(MCU) -I/home/zako/Apps/msp430/msp430/include

DEBUG=mspdebug
DRIVER=rf2500
VOLT=3300

CPPFILES=$(filter %.cpp, $(SOURCES))
CCFILES=$(filter %.cc, $(SOURCES))
BIGCFILES=$(filter %.C, $(SOURCES))
CFILES=$(filter %.c, $(SOURCES))
ASMFILES=$(filter %.S, $(SOURCES))


OBJS=	$(CFILES:.c=.o)		\
	$(CPPFILES:.cpp=.o)	\
	$(BIGCFILES:.C=.o) 	\
	$(CCFILES:.cc=.o)  	\
	$(ASMFILES:.S=.o)

all: $(BINARY).hex $(BINARY).elf

$(BINARY).hex: $(BINARY).elf
	$(OBJC) -O ihex $(BINARY).elf $(BINARY).hex
	$(SIZE) $(BINARY).elf

$(BINARY).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $(BINARY).elf $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -fr *.hex *.elf *~ $(OBJS)

flash: $(BINARY).hex
	$(DEBUG) $(DRIVER) -v $(VOLT) "prog $(BINARY).hex"

info: $(BINARY).elf
	$(SIZE) $(BINARY).elf

debug: 
	$(DEBUG) $(DRIVER) "gdb"

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

#include "serial.h"

void serial_init()
{
	serial_available = 0;

	P1SEL = BIT1 + BIT2;            // Set P1.1 to RXD and P1.2 to TXD
	P1SEL2 = BIT1 + BIT2;

	UCA0CTL1 |= UCSSEL_2;           // Have USCI use SMCLK AKA main CLK
	UCA0BR0 = BR0_BD_115200_12MHZ;                 // 115200 baud
	UCA0BR1 = BR1_BD_115200_12MHZ;
	UCA0MCTL = MOD_BD_115200_12MHZ;                // Modulation
	UCA0CTL1 &= ~UCSWRST;           // Start USCI
	IE2 |= UCA0RXIE;                // Enable USCI0 RX interrupt
}

void serial_putch(uint8_t c) {
	while ( !(IFG2 & UCA0TXIFG) ) ;     // Wait if there is a transmission ongoing
	UCA0TXBUF = c;                      // Put the uint8_t into the UART TX buffer
}

void serial_puts(const uint8_t *ptr) {
	while(*ptr) {                       // Go until we are at the NULL
		serial_putch(*(ptr++));         // put out the single uint8_t
	}
}

uint8_t serial_getc()
{
	if (!serial_available)
		while (!(IFG2&UCA0RXIFG)); // USCI_A0 RX buffer ready?
	serial_available = 0;
	return UCA0RXBUF;
}



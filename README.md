# BikeLight

The BikeLight is a rear light for bicycles, with an 8x8 LED matrix capable of displaying animations and text scrolls.

The hardware is based on a MSP430g2553 microcontroller and a MAX7119 LED driver.

## Compiling

GCC and related tools for MSP430 is needed, and mspdebug for programming the target microcontroller. A Makefile is included. Just run make, make flash.

## Use

There are two python examples showing how to check if the hardware is working, and how to send some strings and animations to the device.

There is also an html + javascript program for helping in creating the animations.





#!/usr/bin/env python

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



import sys, os, serial, time

PORT =      '/dev/ttyUSB0'
BAUDRATE =  115200 
RTS =       None
DTR =       None

# protocol

PKT_COMMAND =       0x11
PKT_DATA =          0x22
PKT_STAT =          0x33
PKT_INFO =          0x44

CMD_ID =            0x11
CMD_PRG_FLASH =     0x22
CMD_ERASE_FLASH =   0x33
CMD_END =           0xFF

STAT_OK =           0x14
STAT_ERROR =        0xEE

def send_packet(type, data):
    ser.write(bytearray([type]))
    time.sleep(0.001)
    ser.write(bytearray([data]))

# open serial port
ser = serial.Serial(PORT, baudrate=BAUDRATE, timeout=2)

send_packet(PKT_INFO, 0x00)
pkt = ser.read(size=6)
if pkt[0] != PKT_DATA or pkt[2] != PKT_DATA or pkt[4] != PKT_DATA:
    print ("Problem with communication");
else:
    print ("Firmware version: " + chr(pkt[3]) + "." + chr(pkt[5]))





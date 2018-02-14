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

# data
# strings = len + delay + " " + str + " " + \0
string1 = chr(19) + chr(75) + " " + "Respeta al ciclista" + " " + "\0"
string2 = chr(10) + chr(50) + " " + "Free Gear!" + " " + "\0"
string3 = chr(16) + chr(75) + " " + "Bici Hardcore!!!" + " " + "\0"
string4 = chr(15) + chr(50) + " " + "ladecadence.net" + " " + "\0"

anim1 = bytearray([ \
        3, 150, 5,
                8,8,0b00000000,0b00000000,0b00000000,0b00011000,0b00011000,0b00000000,0b00000000, 0b00000000, \
                8,8,0b00000000,0b00000000,0b00011000,0b00100100,0b00100100,0b00011000,0b00000000, 0b00000000, \
                8,8,0b00000000,0b00011000,0b00100100,0b01000010,0b01000010,0b00100100,0b00011000, 0b00000000 \
                ])

anim2 = bytearray([ \
        8, 150, 2,
                8,8,0b10000000,0b01000000,0b00100000,0b00010000,0b00001000,0b00000100,0b00000010,0b00000001, \
                8,8,0b00010000,0b00010000,0b00010000,0b00010000,0b00010000,0b00010000,0b00010000,0b00010000, \
                8,8,0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,0b01000000,0b10000000, \
                8,8,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,0b00000000,0b00000000,0b00000000, \
                8,8,0b10000000,0b01000000,0b00100000,0b00010000,0b00001000,0b00000100,0b00000010,0b00000001, \
                8,8,0b00010000,0b00010000,0b00010000,0b00010000,0b00010000,0b00010000,0b00010000,0b00010000, \
                8,8,0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,0b01000000,0b10000000, \
                8,8,0b00000000,0b00000000,0b00000000,0b00000000,0b11111111,0b00000000,0b00000000,0b00000000 \
                ])

anim3 = bytearray([ \
        2, 125, 5,
                8,8,0b00000000,0b00110000,0b01010110,0b01010000,0b01010000,0b01010110,0b00110000,0b00000000, \
                8,8,0b00000000,0b00110000,0b01100110,0b01100000,0b01100000,0b01100110,0b00110000,0b00000000 \
                ])


# prepare buffer
buffer = bytearray()

# fill it with data
buffer.append(0xA5)     # magic number
buffer.append(4)        # 4 strings
buffer.append(3)        # 3 animations

# fill strings
for i in string1:
    buffer.append(ord(i))

for i in string2:
    buffer.append(ord(i))

for i in string3:
    buffer.append(ord(i))

for i in string4:
    buffer.append(ord(i))

#for i in string5:
#    buffer.append(ord(i))

for i in range (len(anim1)):
    buffer.append(anim1[i])

for i in range(len(anim2)):
    buffer.append(anim2[i])

for i in range(len(anim3)):
    buffer.append(anim3[i])

# magic
buffer.append(0xA5)

print ("buffer len: " + str(len(buffer)))

# fill buffer to 128 blocks
if len(buffer)%128 != 0:
    for i in range (128-(len(buffer)%128)):
        buffer.append(0)

print ("buffer len: " + str(len(buffer)))

# print array
#for i in range(256):
#    print (format(i*8, '04x') + ": " , end="")
#    for j in range(8):
#        print (format(buffer[j+(8*i)], '02x') + " ", end="")
#    print()

# write array
file = open ("data.bin", mode='wb')
for i in range(len(buffer)):
    file.write (bytearray([buffer[i]]))
file.close()

def send_packet(type, data):
    ser.write(bytearray([type]))
    time.sleep(0.001)
    ser.write(bytearray([data]))


# open serial port
ser = serial.Serial(PORT, BAUDRATE, timeout=2)

# start flashing
send_packet(PKT_COMMAND, CMD_PRG_FLASH)
# read answer, erased?
pkt = ser.read(size=2)
if pkt[0] == PKT_STAT and pkt[1] == STAT_OK:
    print ("ok, erased, writing...")
    #time.sleep(0.01)
    # n blocks * 128 buffers = len
    for i in range(len(buffer)//128):
        check = 0;
        for j in range(128):
            ser.write(bytearray([buffer[j+(i*128)]]))
            # acumulate checksum
            check = (check + buffer[j+(i*128)] + 1) & 0xFF # we need the & 0xff to limit the result to 8 bit
        # check
        pkt = ser.read(size=2)
        if (pkt[0] != PKT_DATA or pkt[1] != check):
            print ("check error in packet " + str(i))
            break
        else:
            # ok, continue flashing
            if i<(len(buffer)//128)-1:
                send_packet(PKT_COMMAND, CMD_PRG_FLASH)
    # finished
    send_packet(PKT_COMMAND, CMD_END)
    print("written")
else:
    print ("Problem erasing");



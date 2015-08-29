# TWI-RAM
TWI / I2C shared RAM on the AVR for use with the Raspberry Pi

This is a convenience library to help write small programs on the AVR that need to share information with the Pi. It's native AVR, doesn't need any Arduino library support, and once included is effectively fire-and-forget.

It exposes a chunk of RAM on the AVR that can be read and written to via the I2C interface from a Pi master.

http://blog.oscarliang.net/raspberry-pi-arduino-connected-i2c/ provides a good initial tutorial on wiring the Pi and AVR, as well as some general principles. If you see data corruption, note that the Pi does have bugs with its I2C implementation if the AVR is unable to respond quickly enough. These can be mitigated by avoiding long running timer interrupts and by reducing the baud rate of the I2C interface.

Depending on your AVR you will need to change settings in the TWI_Slave_for_RPi.h file. This especially includes the PORT/DDR information which is setup for an ATmega1284P.

Having everything connected correctly and running the main.c program on the AVR will allow you to see the results on the Pi as below.

```
$ i2cdump -y 0 8 b

     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef
00: 48 69 21 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d    Hi!-------------
10: 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d    ----------------
20: 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d    ----------------
30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................

$ i2cset -y 0 8 0x05 65

$ i2cdump -y 0 8 b

     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef
00: 48 69 21 2d 2d 41 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d    Hi!--A----------
10: 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d    ----------------
20: 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d 2d    ----------------
30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
```

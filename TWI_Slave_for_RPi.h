/*
 * TWI_Slave_for_RPi.h
 *
 * Copyright (c) 2015 Bryn Thomas
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */ 

#ifndef TWI_SLAVE_FOR_RPI_H_
#define TWI_SLAVE_FOR_RPI_H_

// Setup the amount of shared RAM that is available between the AVR and accessible via TWI

#define TWI_RAM_MAX 48

// Initialise the shared RAM with this character.

#define TWI_RAM_DEFAULT '-'

// Initialise the shared RAM with this character.

#define TWI_ADDRESS 0x08

// Depending on the AVR, the SCL and SDA pins may be found on different ports.
// We want to be able to control the ports to ensure we disable the pull up registers
// as required when linking to the RPi.

#define TWI_PORT PORTC
#define TWI_DDR DDRC
#define TWI_SCL PC0
#define TWI_SDA PC1

// Our shared memory
extern volatile unsigned char TWI_RAM[TWI_RAM_MAX];

void TWI_init( void );

#endif /* TWI_SLAVE_FOR_RPI_H_ */

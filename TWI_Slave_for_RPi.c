/*
 * TWI_Slave_for_RPi.c
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include "TWI_Slave_for_RPi.h"


// The general bit pattern that we throw into the TWCR to effectively tell the TWI
// and TWI Master to keep going.
// The bits set are as follows:
// [7] TWINT = Tells the TWI hardware that it can stop stretching the clock
//     and begin sending a reply.
// [6] TWEA = Makes the TWI hardware generate an acknowledgment signal to
//     the master.
// [2] TWEN = The overall power switch for the entire TWI system.
// [0] TWIE = Enables the TWI interrupt handler.

#define TWCR_ALL_GOOD_KEEP_GOING 0b11000101

// We distinguish between receiving address bytes and data bytes over the TWI.
// Since there isn't any clear protocol separation between the two, we track state.
// First byte we receive in a transaction is the address, after that it's data.

typedef enum {
	TWI_RECEIVE_ADDR = 0,
	TWI_RECEIVE_DATA = 1,
} TWIReceive;

// These are the main status codes we'll receive from the TWI hardware during
// successful read/write transactions.

typedef enum {
	TWI_BEGIN = 0x60,
	TWI_DATA_RECEIVED = 0x80,

	TWI_SEND_FIRST_BYTE = 0xA8,
	TWI_SEND_NEXT_BYTE = 0xB8,

	TWI_READ_DONE = 0xC8,
	TWI_WRITE_DONE = 0xA0,
	TWI_READ_NOT_INTERESTED = 0xC0
} TWICommands;

// Our shared memory
volatile unsigned char TWI_RAM[TWI_RAM_MAX];

// The index of our next read/write action
volatile unsigned char TWI_RAM_cur_addr;

// State-tracking for action on next byte received.
volatile unsigned char TWI_next_receive_action;

ISR(TWI_vect){
	unsigned char TWSR_cache = TWSR;
	unsigned char TWDR_cache;
	
	switch (TWSR_cache){
		// Opens our transaction and tells us that the next byte we'll receive should be treated
		// as an address.
		case TWI_BEGIN:
		TWCR = TWCR_ALL_GOOD_KEEP_GOING;
		TWI_next_receive_action = TWI_RECEIVE_ADDR;
		break;
		
		case TWI_DATA_RECEIVED:
		TWDR_cache = TWDR;
		TWCR = TWCR_ALL_GOOD_KEEP_GOING;
		if (TWI_next_receive_action == TWI_RECEIVE_DATA){
			// We're expecting to receive data, store the byte we've received into the
			// address index and advance it one place forward.
			if (TWI_RAM_cur_addr < TWI_RAM_MAX) TWI_RAM[TWI_RAM_cur_addr] = TWDR_cache;
			TWI_RAM_cur_addr++;
			} else {
			// This is the first byte, so store it as our address index and then prepare
			// for data.
			TWI_RAM_cur_addr=TWDR_cache;
			TWI_next_receive_action = TWI_RECEIVE_DATA;
		}
		break;
		
		case TWI_SEND_FIRST_BYTE:
		case TWI_SEND_NEXT_BYTE:
		// If we receive a request for data, send what we have at the current address index.
		if (TWI_RAM_cur_addr < TWI_RAM_MAX)
		TWDR = TWI_RAM[TWI_RAM_cur_addr];
		else
		TWDR = 0;
		TWCR = TWCR_ALL_GOOD_KEEP_GOING;
		TWI_RAM_cur_addr++;
		break;
		
		case TWI_READ_DONE:
		case TWI_WRITE_DONE:
		case TWI_READ_NOT_INTERESTED:
		TWCR = TWCR_ALL_GOOD_KEEP_GOING;
		break;
	}
}

void TWI_init( void ){
	for (int i = 0; i < TWI_RAM_MAX; i++)
	TWI_RAM[i] = TWI_RAM_DEFAULT;

	TWI_RAM_cur_addr = 0;
	TWI_next_receive_action = TWI_RECEIVE_ADDR;
	
	TWI_DDR &= (0xFF ^ (1<<TWI_SCL) ^ (1<<TWI_SDA));
	TWI_PORT &= (0xFF ^ (1<<TWI_SCL) ^ (1<<TWI_SDA));

	TWAR = (TWI_ADDRESS << 1);
	
	TWCR = TWCR_ALL_GOOD_KEEP_GOING;

	sei();
}

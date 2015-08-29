#include "TWI_Slave_for_RPi.h"

// Demonstration program that sets up shared memory, writes some data to it, and is accessible
// over TWI/I2C. 

int main(void){
	TWI_init();
	TWI_RAM[0]='H';
	TWI_RAM[1]='i';
	TWI_RAM[2]='!';
	while (1){
	}
}

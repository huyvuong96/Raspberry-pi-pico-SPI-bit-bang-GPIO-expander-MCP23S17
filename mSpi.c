#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <string.h>
#include "mSpi.h"
#include "mGpio.h"
#include "stdio.h"

void    mSpiInit(void){

	gpio_init(CS_PIN);
    gpio_set_dir(CS_PIN, GPIO_OUT);

    gpio_init(CLK_PIN);
    gpio_set_dir(CLK_PIN, GPIO_OUT);

    gpio_init(MOSI_PIN);
    gpio_set_dir(MOSI_PIN, GPIO_OUT);

    gpio_init(MISO_PIN);
    gpio_set_dir(MISO_PIN, GPIO_IN);


    gpio_put(CS_PIN, HIGH);
    
    mGpioWriteByte(0x00,IODIRA); // set portA to output
    mGpioWriteByte(0x01,IODIRB); // portB gpio 0 to input
    mGpioWriteByte(0x01,GPPUB);  // pull up pin
    mGpioWriteByte(0x01,GPINTENB); //set ioc on pin
    mGpioWriteByte(0x01,DEFVALB); //defval value for pin 0b is 1
    mGpioWriteByte(0x01,INTCONB); // set trigger irs based on defval   
}

uint8_t mSpiTransfer( uint8_t outData){
	uint8_t count, in = 0;
	
	gpio_put(CLK_PIN,LOW);

	for(count = 0; count <8; count++){
		in <<= 1;		
		int pull = (outData & 0x80);
		gpio_put(MOSI_PIN, pull);

		gpio_put(CLK_PIN, HIGH);
		
		in += gpio_get(MISO_PIN);

		gpio_put(CLK_PIN, LOW);
		
		outData <<= 1;
	}
	gpio_put(MOSI_PIN, LOW);

	return in;
}

void    mSpiStart(void){
	gpio_put(CS_PIN,LOW);
}
void    mSpiComplete(void){
	gpio_put(CS_PIN, HIGH);
}
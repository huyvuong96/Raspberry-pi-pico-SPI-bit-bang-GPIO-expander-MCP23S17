#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "mSpi.h"
#include "mGpio.h"

uint8_t mGpioReadByte(uint8_t address){
	uint8_t value, preRead = 0x41;

	gpio_put(CS_PIN, LOW);

	mSpiTransfer(preRead);
	mSpiTransfer(address);
	value = mSpiTransfer(0);

	gpio_put(CS_PIN, HIGH);

	return value;
}

void    mGpioInit(void){
	
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(SW1_PIN);
    gpio_pull_up(SW1_PIN);
    gpio_set_dir(SW1_PIN, GPIO_IN);
    

    gpio_init(SW2_PIN);
    gpio_pull_up(SW2_PIN);
    gpio_set_dir(SW2_PIN, GPIO_IN);
}

void    mGpioWriteByte(uint8_t byte, uint8_t address){
	uint8_t preWrite = 0x40;

	gpio_put(CS_PIN,LOW);

	mSpiTransfer(preWrite);
	mSpiTransfer(address);
	mSpiTransfer(byte);

	gpio_put(CS_PIN, HIGH);
}
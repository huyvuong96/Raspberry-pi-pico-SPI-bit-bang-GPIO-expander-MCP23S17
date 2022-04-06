/**
 * @brief CS466 Lab1 Blink proigram based on pico blink example
 * 
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "myAssert.h"
#include <FreeRTOS.h>
#include <task.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <string.h>
#include <mSpi.h>
#include <mGpio.h>

#include <stdio.h>
#define __ASM __asm
#define __INLINE inline
#define __STATIC_INLINE static inline 

__attribute__((always_inline)) __STATIC_INLINE uint32_t __get_IPSR(void){
    uint32_t result;
    __ASM volatile ("MRS %0, ipsr" : "=r" (result));
    return result;
}

int isIntContext(void){
    uint32_t ipsr = __get_IPSR();
    if(ipsr == 0){
        return 0;
    }

    else{return 1;}
}


uint32_t heartbeatDelay = 500;  // ms


void gpio_int(uint gpio, uint32_t unused){

    
    if(gpio == IRS_PIN){
        printf("ISR !!!\n");
    }

    // else if(gpio == SW1_PIN){
    //     printf("SW1 pressed !!!\n");
    //     int test = isIntContext();
    //     printf("Test in isr: %d\n", test);
    // }
}

void hardware_init(void)
{
    //const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(SW1_PIN);
    gpio_pull_up(SW1_PIN);
    gpio_set_dir(SW1_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(SW1_PIN, GPIO_IRQ_EDGE_FALL,true, &gpio_int);
    
    gpio_init(IRS_PIN);
    gpio_pull_up(IRS_PIN);
    gpio_set_dir(IRS_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(IRS_PIN, GPIO_IRQ_EDGE_FALL,true, &gpio_int);

    gpio_init(SW2_PIN);
    gpio_pull_up(SW2_PIN);
    gpio_set_dir(SW2_PIN, GPIO_IN);
    
}

void gpioReadWrite(void* notUsed){
    const uint32_t queryDelay = 100;
    uint8_t regValue;
    uint8_t count = 0;

    while(true){
        mGpioWriteByte(count++, IODIRB);
        regValue = mGpioReadByte(IODIRB);
        printf("IODIRB: %d, ", regValue);

        regValue = mGpioReadByte(IODIRA);
        printf("IODIRA: %d, ", regValue);

        regValue = mGpioReadByte(IPOLA);
        printf("IPOLA: %d\n", regValue);

        vTaskDelay(queryDelay);
    }
}

void LED(uint8_t LED_G, bool ledOn){
    if(ledOn){
        mGpioWriteByte(0x80 ,LED_G); //80 because led was connected to GPIOA7
    }
    else{
        mGpioWriteByte(0,LED_G);
    }

}

void heartbeat(void * notUsed)
{   
    while (true) {
        if(mGpioReadByte(GPIOB) == 0x00){
            LED(GPIOA, 1);
            //printf("ISR: %d", gpio_get(IRS_PIN));
        }

        else{
           printf("hb-tick: %d\n", heartbeatDelay);
           //printf("GPIOB: 0x%02x \n", mGpioReadByte(GPIOB)); 
           int test = isIntContext();
            printf("Test in heartbeat: %d\n", test);       
            LED(GPIOA, 1);            
            gpio_put(LED_PIN, 1);
            vTaskDelay(heartbeatDelay);
            LED(GPIOA, 0);
            gpio_put(LED_PIN, 0);
            vTaskDelay(heartbeatDelay); 
        }
    }
}

int main()
{
    stdio_init_all();
    
    hardware_init();

    mSpiInit();
  
    xTaskCreate(heartbeat, "LED_Task", 256, NULL, 0, NULL);
    //xTaskCreate(gpioReadWrite, "Test_read_write_Task", 256, NULL, 1, NULL);
   
    vTaskStartScheduler();

    while(1){};
    
}



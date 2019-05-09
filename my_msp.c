/*
 * my_msp.c
 * 
 * Danica Fujiwara & Spencer Shaw
 * 
 * CPE 329-17/18 Spring 2019
 */

#include <stdint.h>

#include "msp.h"

#include "my_msp.h"
#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "keypad.h"
#include "spi.h"
#include "uart.h"


inline void init(const unsigned int freq){
    DISABLE_WATCHDOG

    init_dco();
	set_dco(freq);

	dac_init();
	uart_init();

	led_init();
	rgb_init();

	// timer_init();
	//lcd_init();
	//keypad_init();
}

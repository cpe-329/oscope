/*
 * button.c
 *
 * Danica Fujiwara & Spencer Shaw
 *
 * CPE 329-17/18 Spring 2019
 *
 */

#include "button.h"
#include <stdint.h>
#include "msp.h"
#include "my_msp.h"
#include "led.h"

static uint8_t button_val = 0;

inline void button_init() {
    // P1->SEL0 &= ~BUTTON_PIN;
    // P1->SEL1 &= ~BUTTON_PIN;

    // P1->DIR &= ~BUTTON_PIN;
    // P1->REN |= BUTTON_PIN;
    // P1->OUT &= ~BUTTON_PIN;

    // button_val = P1->IN & BUTTON_PIN;


    P1->SEL1 &= ~MANUAL_PIN;

    P1->DIR &= ~MANUAL_PIN;
    P1->REN |= MANUAL_PIN;
    P1->OUT &= ~MANUAL_PIN;

    button_val = P1->IN & MANUAL_PIN;
}

inline uint8_t button_get() {
    uint8_t released;
    uint8_t new_val = P1->IN & MANUAL_PIN;
    released = (button_val == 0) && (new_val > 0);
    button_val = new_val;
    if (new_val > 0) {
        led_on();
    }
    else{
        led_off();
    }
    // return released;
    return new_val;
}

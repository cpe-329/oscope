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
    P1->SEL0 &= ~BUTTON_PIN;
    P1->SEL1 &= ~BUTTON_PIN;

    P1->DIR &= ~BUTTON_PIN;
    P1->REN |= BUTTON_PIN;

    button_val = P1->IN & BUTTON_PIN;
}

inline uint8_t button_get() {
    uint8_t result;
    uint8_t new_val = P1->IN & BUTTON_PIN;
    result = (~button_val) && new_val;
    button_val = new_val;
    led_val(new_val);
    return result;
}
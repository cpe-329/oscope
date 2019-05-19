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
#include "led.h"
#include "msp.h"
#include "my_msp.h"

volatile static uint8_t button_val = 0;

inline void button_init() {
    // P1->SEL0 &= ~BUTTON_PIN;
    // P1->SEL1 &= ~BUTTON_PIN;

    // P1->DIR &= ~BUTTON_PIN;
    // P1->REN |= BUTTON_PIN;
    // P1->OUT &= ~BUTTON_PIN;

    // button_val = P1->IN & BUTTON_PIN;

    P1->SEL0 &= ~TRIGGER_PIN;
    P1->SEL1 &= ~TRIGGER_PIN;

    P1->DIR &= ~TRIGGER_PIN;
    P1->REN |= TRIGGER_PIN;
    P1->OUT |= TRIGGER_PIN;

    button_val = P1->IN & TRIGGER_PIN;
}

inline uint8_t button_get() {
    uint8_t released;
    uint8_t new_val = (P1->IN & TRIGGER_PIN) != 0;
    released = (button_val == 0) && (new_val != 0);
    button_val = new_val;
    return released;
}

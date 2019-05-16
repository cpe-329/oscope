/*
 * main.c
 *
 * Danica Fujiwara & Spencer Shaw
 *
 * CPE 329-17/18 Spring 2019
 *
 */

#include <stdint.h>
#include "msp.h"

#include "adc.h"
#include "delay.h"
#include "led.h"
#include "my_msp.h"
#include "scope_data.h"
#include "scope_term.h"
#include "spi.h"
#include "timers.h"
#include "uart.h"

#define FREQ FREQ_48_MHZ

// Data received from UART
volatile unsigned char char_data;
volatile uint8_t got_fresh_char;

// Whether to refresh terminal view
uint8_t refresh_term = FALSE;
// uint8_t refresh_count = 0;
uint8_t repaint_term = TRUE;

int main(void) {
    //    volatile uint32_t i, j;
    init(FREQ);

    adc_set_calibration(0, 16365);
    paint_terminal();
    while (1) {
        scope_read_data();

        led_on();
        if (repaint_term) {  //| (refresh_count > 120)) {
            paint_terminal();
            repaint_term = FALSE;
            // refresh_count = 0;
        }
        if (refresh_term) {
            scope_refresh_term();
            // refresh_count += 1;
            refresh_term = FALSE;
            scope_reset_num_samples();
        }
        led_off();
    }
}

// Timer A0_0 interrupt service routine
void TA0_0_IRQHandler(void) {
    rgb_set(RGB_RED);
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // Clear the CCR0 interrupt
    reset_refresh_delay();
    repaint_term = TRUE;
    rgb_set(RGB_OFF);
}

// Timer A0_N interrupt service routine for CCR1 - CCR4
void TA0_N_IRQHandler(void) {
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG)  // check for CCR1 interrupt
    {
        rgb_set(RGB_GREEN);
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;  // clear CCR1 interrupt
        increment_refresh_delay();
        // Action for ccr1 intr
        refresh_term = TRUE;
        rgb_set(RGB_OFF);
    }
}

// ADC14 interrupt service routine
void ADC14_IRQHandler(void) {
    rgb_set(RGB_BLUE);
    adc_store_reading(ADC14->MEM[0]);
    rgb_set(RGB_OFF);
}

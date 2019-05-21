/*
 * main.c
 *
 * Danica Fujiwara & Spencer Shaw
 *
 * CPE 329-17/18 Spring 2019
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "msp.h"

#include "adc.h"
#include "button.h"
#include "delay.h"
#include "led.h"
#include "my_msp.h"
#include "scope_data.h"
#include "scope_term.h"
#include "spi.h"
#include "timers.h"
#include "uart.h"

#define FREQ FREQ_48_MHZ

#define SECONDS_COUNT_MAX (4)

volatile bool adc_data_fresh = false;
volatile bool refresh_term = false;
volatile bool repaint_term = true;

volatile bool one_sec_interval = false;
volatile uint8_t seconds_counter = 0;

int main(void) {
    init(FREQ);

    term_clear_screen();
    paint_terminal();
    scope_switch_mode();
    
    while (1) {
        // Check button to switch mode
        if (button_get()) {
            scope_switch_mode();
            repaint_term = true;
            // delay_ms(100, FREQ);
        }

        // Schedule repaint of entire term
        if (seconds_counter >= SECONDS_COUNT_MAX) {
            repaint_term = true;
            seconds_counter = 0;
        }

        // Repaint entire term only if needed
        if (repaint_term) {
            // Repaint UART VT100 terminal
            led_on();
            paint_terminal();
            led_off();

            // Reset number of sample since last refresh
            repaint_term = false;
        }
        if (refresh_term || ((scope_get_mode() == SCOPE_MODE_AC) && one_sec_interval)) {
            // Refresh data displayed in term
            scope_refresh_data();

            led_on();
            // Refresh UART VT100 terminal
            scope_refresh_term();
            led_off();

            // Reset number of sample since last refresh
            scope_reset_num_samples();
            refresh_term = false;
        }

        // Reset statistics once a second
        if (one_sec_interval) {
            scope_store_peak_data();
            scope_reset_min_max();
            scope_reset_num_peaks();
            one_sec_interval = false;
        }

        // Read data from scope
        if (adc_data_fresh) {
            scope_read_data();

            adc_data_fresh = false;
            adc_start_conversion();
        }
    }
}

// Timer A0_0 interrupt service routine
// Every 1 second
void TA0_0_IRQHandler(void) {
    // rgb_set(RGB_RED);
    rgb_toggle(RGB_RED);
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // Clear the CCR0 interrupt
    seconds_counter += 1;
    reset_refresh_delay();
    one_sec_interval = true;
    // rgb_clear(RGB_RED);
}

// Timer A0_N interrupt service routine for CCR1 - CCR4
void TA0_N_IRQHandler(void) {
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG)  // check for CCR1 interrupt
    {
        rgb_set(RGB_GREEN);
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;  // clear CCR1 interrupt
        increment_refresh_delay();
        // Action for ccr1 intr
        refresh_term = true;
        rgb_clear(RGB_GREEN);
    }
}

// ADC14 interrupt service routine
void ADC14_IRQHandler(void) {
    rgb_set(RGB_BLUE);
    adc_store_reading(ADC14->MEM[0]);
    adc_data_fresh = true;
    rgb_clear(RGB_BLUE);
}

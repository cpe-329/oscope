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

#define REPAINT_COUNT_MAX (5)

// Data received from UART
// volatile unsigned char char_data;
// volatile uint8_t got_fresh_char;

// Whether to refresh terminal view
volatile bool refresh_term = false;
volatile bool repaint_term = true;
volatile bool data_fresh = false;
volatile bool one_sec_interval = false;
volatile uint8_t repaint_counter = 0;

int main(void) {
    init(FREQ);

    adc_set_calibration(ADC_CALI_MIN, ADC_CALI_MAX);
    term_clear_screen();
    paint_terminal();

    while (1) {
        // Read data from scope
        // if (data_fresh) {
        scope_read_data();
        data_fresh = false;
        // }
        // Check button to switch mode
        if (button_get() != 0) {
            scope_switch_mode();
            repaint_term = true;
            // delay_ms(100, FREQ);
        }

        if (repaint_counter >= REPAINT_COUNT_MAX) {
            repaint_term = true;
        }
        // Repaint entire term only if needed
        if (repaint_term) {
            scope_refresh_data();

            // Repaint UART VT100 terminal
            rgb_set(RGB_RED);
            paint_terminal();
            rgb_clear(RGB_RED);

            // Reset number of sample since last refresh
            scope_reset_locks();
            repaint_term = false;
            refresh_term = false;
            repaint_counter = 0;
        } else if (refresh_term) {
            // Refresh data displayed in term
            scope_refresh_data();

            rgb_set(RGB_GREEN);
            // Refresh UART VT100 terminal
            scope_refresh_term();
            rgb_clear(RGB_GREEN);

            // Reset number of sample since last refresh
            scope_reset_num_samples();
            refresh_term = false;
        }

        if (one_sec_interval) {
            scope_store_peak_data();
            scope_reset_min_max();
            scope_reset_num_peaks();
            one_sec_interval = false;
        }
    }
}

// Timer A0_0 interrupt service routine
void TA0_0_IRQHandler(void) {
    // rgb_set(RGB_RED);
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // Clear the CCR0 interrupt
    repaint_counter += 1;
    reset_refresh_delay();
    one_sec_interval = true;
    refresh_term = true;
    // rgb_set(RGB_OFF);
}

// Timer A0_N interrupt service routine for CCR1 - CCR4
void TA0_N_IRQHandler(void) {
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG)  // check for CCR1 interrupt
    {
        // rgb_set(RGB_RED);
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;  // clear CCR1 interrupt
        increment_refresh_delay();
        // Action for ccr1 intr
        refresh_term = true;
        // rgb_clear(RGB_RED);
    }
}

// ADC14 interrupt service routine
void ADC14_IRQHandler(void) {
    rgb_set(RGB_BLUE);
    data_fresh = true;
    adc_store_reading(ADC14->MEM[0]);
    rgb_clear(RGB_BLUE);
}

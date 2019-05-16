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
#include "uart.h"

#define FREQ FREQ_48_MHZ

// Data received from UART
volatile unsigned char char_data;
volatile uint8_t got_fresh_char;

// Whether to refresh terminal view
uint8_t refresh_term = FALSE;

int main(void) {
    //    volatile uint32_t i, j;
    init(FREQ);

    adc_set_calibration(0, 16365);

    while (1) {
        //        for (j = 20; j > 0; j--){
        //            for (i = 20000; i > 0; i--);  // Delay
        //            adc_record();
        //        }
        //        led_on();
        //        adc_report_avg();
        //        // adc_report_range();
        //        led_off();
        scope_read_data();
        // if (refresh_term) {
        scope_refresh_term();
        refresh_term = FALSE;
        led_off();
        // delay_ms(500, FREQ);
        led_on();
        // }
    }
}

// Timer A0_0 interrupt service routine
void TA0_0_IRQHandler(void) {
    rgb_set(RGB_GREEN);
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // Clear the CCR0 interrupt
    refresh_term = TRUE;
    rgb_set(RGB_OFF);
}

// Timer A0_N interrupt service routine for CCR1 - CCR4
void TA0_N_IRQHandler(void) {
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG)  // check for CCR1 interrupt
    {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;  // clear CCR1 interrupt
                                                    // Action for ccr1 intr
    }
}

// ADC14 interrupt service routine
void ADC14_IRQHandler(void) {
    rgb_set(RGB_BLUE);
    adc_store_reading(ADC14->MEM[0]);
    rgb_set(RGB_OFF);
}

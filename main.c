///*
// * main.c
// *
// * Danica Fujiwara & Spencer Shaw
// *
// * CPE 329-17/18 Spring 2019
// *
// */

//******************************************************************************
//  MSP432P401 Demo - ADC14, Sample A1, AVcc Ref, Set P1.0 if A1 > 0.5*AVcc
//
//   A single sample is made on A1 with reference to AVcc. Software sets
//   ADC14_CTL0_SC to start sample and conversion - ADC14_CTL0_SC automatically
//   cleared at EOC. Using MODCLK = 25 MHz for sample and conversion timing
//   Pulse mode with sample timing of 16 cycles. ADC14 interrupts on conversion
//   completion. If A1 > 0.5*AVcc, P1.0 set, else reset.
//
//
//                MSP432P401R
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//        >---|P5.4/A1      P1.0|-->LED
//
//******************************************************************************

#include <stdint.h>
#include "msp.h"

#include "my_msp.h"
#include "delay.h"
#include "led.h"
#include "spi.h"
#include "uart.h"
#include "adc.h"

#define FREQ FREQ_48_MHZ

volatile unsigned char char_data;
volatile uint8_t got_fresh_char;

int main(void)
{
    volatile uint32_t i, j;

    init(FREQ);

    // GPIO Setup
    P5->SEL1 |= BIT4;                 // Configure P5.4 for ADC
    P5->SEL0 |= BIT4;

    // Sampling time, S&H=16, ADC14 on
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON;
    ADC14->CTL1 = ADC14_CTL1_RES_3;     // Use sampling timer, 14-bit conversion

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_1; // A1 ADC input select; Vref=AVCC
    ADC14->IER0 |= ADC14_IER0_IE0;        // Enable ADC conv complete interrupt
    ADC14->CTL0 |= ADC14_CTL0_ENC;        // Enable conversions

    // Enable ADC interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);


     // Configure UART pins
    P1->SEL0 |= UART_PINS;                // set 2-UART pin as secondary function

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
                      EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK

    // Baud Rate calculation
    // 3000000/(115200) = 26.041667
    // Fractional portion = 0.041667
    // User's Guide Table 21-4: UCBRSx = 0x00
    // UCBRx = int (26.041667 / 16) = 1
    // UCBRFx = int (((26.041667/16)-1)*16) = 10

    EUSCI_A0->BRW = 1;                      // Using baud rate calculator
    EUSCI_A0->MCTLW = (10 << EUSCI_A_MCTLW_BRF_OFS) |
                      EUSCI_A_MCTLW_OS16;
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt
    
    // Enable global interrupt
    __enable_irq();

    // Enable eUSCIA0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);
    // Enable global interrupt
    __enable_irq();
    

    adc_set_calibration(0, 16365);

    while (1)
    {
        for (j = 20; j > 0; j--){
            for (i = 20000; i > 0; i--);  // Delay
            adc_record();
        }
        led_on();
        adc_report_avg();
        // adc_report_range();
        led_off();
    }
}

// ADC14 interrupt service routine
void ADC14_IRQHandler(void) {
    adc_store_reading(ADC14->MEM[0]);
    // if (ADC14->MEM[0] >= 0x2000)      // ADC12MEM0 = A1 > 0.5AVcc?
    //   P1->OUT |= BIT0;                // P1.0 = 1
    // else
    //   P1->OUT &= ~BIT0;               // P1.0 = 0
}

/*
* adc.c
* 
* Danica Fujiwara & Spencer Shaw
*
* CPE 329-17/18 Spring 2019
*
*/

#include "adc.h"
#include "uart.h"

static unsigned int adc_value = 0;
static unsigned int calibration_shift = 0;
static unsigned int calibration_scale = 5;
static unsigned int adc_history[AVG_LENGTH] = {0, 0, 0, 0};
static unsigned int max_value = 0;
static unsigned int min_value = 16000;

void adc_record(){
    unsigned int avg_val = 0;
    unsigned int avg_val_mv = 0;
    
    adc_log_reading();
    avg_val = adc_get_avg();

    if (avg_val > max_value){
        max_value = avg_val;
    }
    if(avg_val < min_value){
        min_value = avg_val;
    }

    adc_start_conversion();
}

void adc_report_avg(){

    unsigned int avg_val  = adc_get_avg();
    unsigned int avg_val_mv = adc_map_val(avg_val);

    uart_write_volts(avg_val_mv);
}

void adc_report_range(){
    uart_write('m');
    uart_write('i');
    uart_write('n');
    uart_write(':');
    uart_write(' ');
    uart_write_int(min_value);
    uart_write_nl();
    uart_write('m');
    uart_write('a');
    uart_write('x');
    uart_write(':');
    uart_write(' ');
    uart_write_int(max_value);
    uart_write_nl();
}

// Start sampling/conversion
inline void adc_start_conversion(){
    ADC14->CTL0 |= ADC14_CTL0_SC;
}

unsigned int adc_map_val(const unsigned int val){
    unsigned int val_mv = (val - calibration_shift) / calibration_scale;
    return val_mv;
}

void adc_set_calibration(unsigned int min, unsigned int max){
    calibration_shift = min;
    calibration_scale = (max - min) / MAX_READING_MV;
    min_value = 16000;
    max_value - 0;
}

void adc_calibrate_on_range(){
    adc_set_calibration(min_value, max_value);
}

inline void adc_store_reading(unsigned int val){
    adc_value = val;
}

unsigned int adc_log_reading(){
    int i;
    for(i = 0; i < AVG_LENGTH - 1; i++){
        adc_history[i + 1] = adc_history[i];
    }
    adc_history[0] = adc_value;
}

unsigned int adc_get_avg(){
    int i;
    unsigned int sum = 0;
    for(i = 0; i < AVG_LENGTH; i++){
        sum += adc_history[i];
    }
    return sum / AVG_LENGTH;
}

void uart_write_volts(unsigned int val_mv){
    uart_write_int(val_mv / 1000);
    uart_write('.');
    uart_write_int(val_mv % 1000 / 100);
    uart_write_int(val_mv % 100 / 10);
    uart_write_int(val_mv % 10);
    uart_write('V');
    uart_write_nl();
}

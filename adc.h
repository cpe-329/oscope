/*
* adc.h
* 
* Danica Fujiwara & Spencer Shaw
*
* CPE 329-17/18 Spring 2019
*
*/

#ifndef ADC_H_
#define ADC_H_

#define AVG_LENGTH (4)
#define MAX_READING_MV (3000)


void adc_record();
void adc_report_avg();
void adc_report_range();
inline void adc_start_conversion();
unsigned int adc_map_val(const unsigned int val);
void adc_set_calibration(unsigned int min, unsigned int max);
void adc_calibrate_on_range();
unsigned int adc_log_reading();
unsigned int adc_get_avg();
void uart_write_volts(unsigned int val_mv);

#endif /* ADH_H_ */

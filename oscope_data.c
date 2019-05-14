/*
 * oscope_data.c
 *
 *  Created on: May 9, 2019
 *      Author: sfshaw
 */
#include <stdint.h>

#include "oscope_data.h"

// Mode selction
uint8_t oscope_mode = OSCOPE_MODE_AC;

// DC Mode data
unsigned int dc_value = 0;
// mV from 0 to 3000

// AC Mode data
unsigned int ac_dc_offset = 0;
// mV from 0 to 3000

unsigned int ac_pkpk = 0;
// mV from 0 to 3000

unsigned int ac_freq = 0;
// Hz from 1 to 1000

unsigned int ac_period = 0;
// ms from 1 to 1000

unsigned int histogram[HISTOGRAM_SIZE] = {};
// mV from 0 to 3000

unsigned int histogram_div = 0;
// ms or us from 1 to 999
// assert(histogram_div * 9 <= 1000)

uint8_t histogram_units = 0;
// 0 for ms, 1 for us

unsigned int num_samples = 0;
// Number of samples taken since last term refresh

void read_oscope_data() {
  unsigned int avg_val = 0;
  unsigned int avg_val_mv = 0;

  adc_log_reading();
  avg_val = adc_get_avg();

  if (avg_val > adc_get_max_value()) {
    adc_set_max_value(avg_val);
  }
  if (avg_val < adc_get_min_value()) {
    adc_set_min_value(avg_val);
  }
  switch (oscope_mode) {
    case OSCOPE_MODE_DC:

      break;
    case OSCOPE_MODE_AC:

      break;
    default:
  }
  adc_start_conversion();
}
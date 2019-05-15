/*
 * scope_data.c
 *
 * Danica Fujiwara & Spencer Shaw
 *
 * CPE 329-17/18 Spring 2019
 *
 */

#include <stdint.h>

#include "scope_data.h"

static uint8_t scope_mode = SCOPE_MODE_DC;
static unsigned int dc_value = 0;
static unsigned int ac_dc_offset = 0;
static unsigned int ac_pkpk = 0;
static unsigned int ac_freq = 0;
static unsigned int ac_period = 0;
static unsigned int histogram[HISTOGRAM_SIZE] = {};
static unsigned int histogram_div = 0;
uint8_t histogram_units = 0;
unsigned int num_samples = 0;

// Mode selction
inline uint8_t scope_get_mode() { return scope_mode; }

// DC Mode data
inline unsigned int scope_get_dc_value() {
    // mV from 0 to 3000
    return dc_value;
}

// AC Mode data
inline unsigned int scope_ac_dc_offset() {
    // mV from 0 to 3000
    return ac_dc_offset;
}

inline unsigned int scope_get_ac_pkpk() {
    // mV from 0 to 3000
    return ac_pkpk;
}

inline unsigned int scope_get_ac_freq() {
    // Hz from 1 to 1000
    return ac_freq;
}

inline unsigned int scope_get_ac_period() {
    // ms from 1 to 1000
    return ac_period;
}

inline unsigned int scope_get_histogram(uint8_t i) {
    // mV from 0 to 3000
    return histogram[i];
}

inline unsigned int scope_get_histogram_div() {
    // ms or us from 1 to 999
    // assert(histogram_div * 9 <= 1000)
    return histogram_div;
}

inline uint8_t scope_get_histogram_units() {
    // 0 for ms, 1 for us
    return histogram_units;
}

inline unsigned int scope_get_num_samples() { return num_samples; }
// Number of samples taken since last term refresh
void read_scope_data() {
    unsigned int avg_val = 0;
    adc_log_reading();
    avg_val = adc_get_avg();

    if (avg_val > adc_get_max_value()) {
        adc_set_max_value(avg_val);
    }
    if (avg_val < adc_get_min_value()) {
        adc_set_min_value(avg_val);
    }
    switch (scope_mode) {
    case SCOPE_MODE_DC:
        dc_value = avg_val;
        break;
    case SCOPE_MODE_AC:

        break;
    default:
    }
    adc_start_conversion();
}
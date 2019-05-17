/*
 * scope_data.c
 *
 * Danica Fujiwara & Spencer Shaw
 *
 * CPE 329-17/18 Spring 2019
 *
 */

#include <stdint.h>

#include "adc.h"
#include "delay.h"
#include "led.h"
#include "scope_data.h"
#include "scope_term.h"

static uint8_t scope_mode = SCOPE_MODE_DC;
static unsigned int dc_value = 0;
static unsigned int ac_dc_offset = 0;
static unsigned int ac_pkpk = 0;
static unsigned int ac_freq = 0;
static unsigned int ac_period = 0;
static unsigned int histogram[HISTOGRAM_SIZE] = {};
// static unsigned int histogram_div = 0;
// uint8_t histogram_units = 0;
static unsigned int num_samples = 0;
static unsigned int num_peaks = 0;
static unsigned int max_val = 0;
static unsigned int min_val = 16000;
static uint8_t finding_peak = FALSE;

// Mode selction
inline uint8_t scope_get_mode() {
    return scope_mode;
}


// DC Mode data
inline unsigned int scope_get_dc_value() {
    // mV from 0 to 300
    return dc_value;
}

// AC Mode data
inline unsigned int scope_get_ac_dc_offset() {
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
    return 13;  // histogram_div;
}

inline uint8_t scope_get_histogram_units() {
    // 0 for ms, 1 for us
    return 1;  // histogram_units;
}

// Number of samples taken since last term refresh
inline unsigned int scope_get_num_samples() {
    return num_samples;
}

inline void scope_reset_num_samples() {
    num_samples = 0;
}

inline void scope_reset_num_peaks() {
    num_peaks = 0;
}
inline void scope_reset_min_max() {
    min_val = 16000;
    max_val = 0;
}

inline void count_peaks(unsigned int val) {
    if (finding_peak) {
        // Finding peak

    } else {  // Finding trough
    }
}

inline void scope_update_histogram() {
    int i;
    for (i = 0; i < HISTOGRAM_SIZE - 1; i++) {
        histogram[i + 1] = histogram[i];
    }
    histogram[0] = dc_value;
}

inline void scope_switch_mode() {
    switch (scope_mode) {
        case SCOPE_MODE_DC:
            scope_mode = SCOPE_MODE_AC;
            break;
        default:
            scope_mode = SCOPE_MODE_DC;
    }
}

void scope_read_data() {
    unsigned int avg_val = 0;
    // Read in new data
    adc_log_reading();
    num_samples += 1;
    // Get updated average
    avg_val = adc_get_avg();

    // Record now min or max values
    if (avg_val > max_val) {
        max_val = avg_val;
    } else if (avg_val < min_val) {
        min_val = avg_val;
    }
    // DC MODE
    dc_value = adc_map_val(avg_val);
    // For either scope mode
    if(scope_mode == SCOPE_MODE_AC){
            // AC Mode
            ac_pkpk = adc_map_val(max_val - min_val);
            ac_dc_offset = ac_pkpk >> 1;

            count_peaks(avg_val);
            // TODO:
            ac_freq = num_peaks;  // / SAMPLE_TIME;
            ac_period = 0;
    }
    adc_start_conversion();
}

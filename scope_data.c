/*
 * scope_data.c
 *
 * Danica Fujiwara & Spencer Shaw
 *
 * CPE 329-17/18 Spring 2019
 *
 */

#include <stdbool.h>
#include <stdint.h>

#include "adc.h"
#include "delay.h"
#include "led.h"
#include "scope_data.h"
#include "scope_term.h"
#include "timers.h"

volatile static uint8_t scope_mode = SCOPE_MODE_AC;
volatile static unsigned int dc_value = 0;
volatile static unsigned int ac_dc_offset = 0;
volatile static unsigned int ac_pkpk = 0;
volatile static unsigned int ac_freq = 0;
volatile static unsigned int ac_period = 0;
volatile static unsigned int histogram[HISTOGRAM_SIZE] = {};
static unsigned int histogram_div = SCOPE_DC_HIST_DIV;
uint8_t histogram_units = 0;
volatile static unsigned int num_samples = 0;

volatile static unsigned int num_peaks = 0;

volatile static unsigned int max_val = 0;
volatile static unsigned int min_val = 16000;

volatile static unsigned int max_prev = 0;
volatile static unsigned int min_prev = 16000;

volatile static unsigned int peak_delta = 0;
volatile static unsigned int fast_ac_pkpk = 0;
volatile static unsigned int min_max_valid = false;
volatile static bool dc_offset_valid = false;

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
    return histogram_div;
}

inline uint8_t scope_get_histogram_units() {
    // 0 for ms, 1 for s
    return histogram_units;
}

// Number of samples taken since last term refresh
inline unsigned int scope_get_num_samples() {
    return num_samples;
}

void scope_store_peak_data() {
    ac_pkpk = fast_ac_pkpk;
    ac_freq = num_peaks;  // REPAINT_PERIOD;
}

inline void scope_reset_num_samples() {
    num_samples = 0;
}

inline void scope_reset_num_peaks() {
    num_peaks = 0;
}

inline void scope_reset_min_max() {
    min_prev = min_val;
    max_prev = max_val;
    min_val = 16000;
    max_val = 0;
}

inline void scope_reset_locks() {
    min_max_valid = false;
    dc_offset_valid = false;
}

inline void count_peaks(unsigned int val) {
    static bool finding_peak = true;
    if (finding_peak) {
        // Finding peak
        if (val > max_prev - peak_delta) {
            finding_peak = false;
        }
    } else {
        // Finding trough
        if (val < min_prev + peak_delta) {
            num_peaks += 1;
            finding_peak = true;
        }
    }
}

inline void scope_update_histogram() {
    int i;
    for (i = HISTOGRAM_SIZE - 1; i > 0; i -= 1) {
        histogram[i] = histogram[i - 1];
    }
    histogram[0] = dc_value;
}

inline void scope_switch_mode() {
    switch (scope_mode) {
        case SCOPE_MODE_DC:
            scope_mode = SCOPE_MODE_AC;
            histogram_div = SCOPE_AC_HIST_DIV;
            histogram_units = SCOPE_HIST_UNITS_S;
            timers_resume_fast();
            return;
        default:
            scope_mode = SCOPE_MODE_DC;
            histogram_div = SCOPE_DC_HIST_DIV;
            histogram_units = SCOPE_HIST_UNITS_MS;
            timers_stop_fast();
            return;
    }
}

// Process latest value from ADC 
void scope_read_data() {
    unsigned int avg_val = 0;
    // Read in new data
    adc_log_reading();
    num_samples += 1;

    // Get updated average
    avg_val = adc_get_avg();

    // Record new min or max values
    if (avg_val > max_val) {
        max_val = avg_val;
    } else if (avg_val < min_val) {
        min_val = avg_val;
    }
    count_peaks(avg_val);
}

// Prep for screen refresh
void scope_refresh_data() {
    unsigned int avg_val = adc_get_avg();

    // Update histogram data
    scope_update_histogram();

    // AC/DC MODE
    dc_value = adc_map_val(avg_val);

    // AC Mode
    if (scope_mode == SCOPE_MODE_AC) {
        // AC Mode
        fast_ac_pkpk = adc_map_val(max_val - min_val);

        // if (min_max_valid) {
        ac_dc_offset = adc_map_val((ac_pkpk >> 1) + min_val);
        //     dc_offset_valid =
        //         (min_val < ac_dc_offset) && (max_val > ac_dc_offset);
        // }

        ac_period = 1000 / ac_freq;
    }
}

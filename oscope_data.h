/*
 * oscope_data.h
 *
 *  Created on: May 9, 2019
 *      Author: sfshaw
 */

#ifndef OSCOPE_DATA_H_
#define OSCOPE_DATA_H_

#include <stdint.h>
#define OSCOPE_MODE_AC (0)
#define OSCOPE_MODE_DC (1)

#define HISTOGRAM_SIZE (9)

uint8_t oscope_mode;

// DC Mode data

unsigned int dc_value;
// mV from 0 to 3000


// AC Mode data

unsigned int ac_dc_offset;
// mV from 0 to 3000

unsigned int ac_pkpk;
// mV from 0 to 3000

unsigned int ac_freq;
// Hz from 1 to 1000

unsigned int ac_period;
// ms from 1 to 1000

unsigned int histogram[HISTOGRAM_SIZE];
// mV from 0 to 3000

unsigned int histogram_div;
// ms or us from 1 to 999
// assert(histogram_div * 9 <= 1000)

uint8_t histogram_units;
// 0 for ms, 1 for us

unsigned int num_samples;
// Number of samples taken since last term refresh


#endif /* OSCOPE_DATA_H_ */

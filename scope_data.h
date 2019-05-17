/*
 * scope_data.h
 *
 * Danica Fujiwara & Spencer Shaw
 *
 * CPE 329-17/18 Spring 2019
 *
 */

#ifndef SCOPE_DATA_H_
#define SCOPE_DATA_H_

#include <stdint.h>
#define SCOPE_MODE_AC (0)
#define SCOPE_MODE_DC (1)

#define HISTOGRAM_SIZE (9)

inline uint8_t scope_get_mode();
inline unsigned int scope_get_dc_value();
inline unsigned int scope_get_ac_dc_offset();
inline unsigned int scope_get_ac_pkpk();
inline unsigned int scope_get_ac_freq();
inline unsigned int scope_get_ac_period();
inline unsigned int scope_get_histogram(uint8_t i);
inline unsigned int scope_get_histogram_div();
inline uint8_t scope_get_histogram_units();
inline unsigned int scope_get_num_samples();
inline void scope_reset_num_samples();
inline void scope_update_histogram();

// Main functions for scope
inline void scope_switch_mode();
void scope_read_data();
inline void scope_refresh();

#endif /* SCOPE_DATA_H_ */

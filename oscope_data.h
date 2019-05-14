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

void read_oscope_data();

#endif /* OSCOPE_DATA_H_ */

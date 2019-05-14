/*
 * oscope_term.h
 *
 *  Created on: May 9, 2019
 *      Author: sfshaw
 */

#ifndef OSCOPE_TERM_H_
#define OSCOPE_TERM_H_


#define ESC 0x1B
#define WIDTH 24
#define LENGTH 80
/*home is (x = 0, y = 0)*/
#define INFO_X_CORD 6
#define INFO_Y_CORD 4
#define HIST_TITLE_X 5
#define HIST_TITLE_Y 4

void refresh_terminal();


#endif /* OSCOPE_TERM_H_ */

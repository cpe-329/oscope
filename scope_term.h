/*
 * oscope_term.h
 *
 * Danica Fujiwara & Spencer Shaw
 *
 * CPE 329-17/18 Spring 2019
 *
 */

#ifndef OSCOPE_TERM_H_
#define OSCOPE_TERM_H_

#define ESC 0x1B
#define WIDTH 24
#define LENGTH 80
/*home is (x = 0, y = 0)*/
#define INFO_X_CORD 6
#define INFO_Y_CORD 2
#define HIST_TITLE_X 55
#define HIST_TITLE_Y 1
#define VOLT_DIVISION 157
#define GRAPH_LENGTH 33
#define GRAPH_BOTTOM 20
#define GRAPH_LEFT 42

void scope_refresh_term();
void draw_vertical(unsigned int length, unsigned int x, unsigned int y, char c);

#endif /* OSCOPE_TERM_H_ */

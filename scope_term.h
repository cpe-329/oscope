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
#define INFO_Y_CORD 1
#define HIST_TITLE_X 48
#define HIST_TITLE_Y 1
#define GRAPH_HEIGHT 19
#define BAR 1
#define DIVIDE_GRAPH 30
#define VOLT_DIVISION 167
#define GRAPH_LENGTH 40
#define GRAPH_BOTTOM 20
#define GRAPH_LEFT 32
#define TIME_X_START 75
#define TIME_Y 22
void scope_refresh_term();
void paint_terminal();
void term_clear_screen();


#endif /* OSCOPE_TERM_H_ */

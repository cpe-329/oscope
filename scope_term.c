/*
 * oscope_term.c
 *
 * Danica Fujiwara & Spencer Shaw
 *
 * CPE 329-17/18 Spring 2019
 *
 */

#include "scope_term.h"
#include <stdint.h>
#include "scope_data.h"
#include "delay.h"
#include "uart.h"

void move_down(unsigned int val) {
    unsigned char command[2] = {ESC, '['};
    uart_write_string(command, 2);
    uart_write_int(val);
    uart_write('B');
}

void move_up(unsigned int val) {
    unsigned char command[2] = {ESC, '['};
    uart_write_string(command, 2);
    uart_write_int(val);
    uart_write('A');
}

void move_left(unsigned int val) {
    unsigned char command[2] = {ESC, '['};
    uart_write_string(command, 2);
    uart_write_int(val);
    uart_write('D');
}

void move_right(unsigned int val) {
    unsigned char command[2] = {ESC, '['};
    uart_write_string(command, 2);
    uart_write_int(val);
    uart_write('C');
}

void move_home() {
    unsigned char command[3] = {ESC, '[', 'H'};
    uart_write_string(command, 3);
}

void hide_cursor(){
    unsigned char command[4] = {ESC, '[', '8','m'};
    uart_write_string(command,4);
}

void clear_screen() {
    unsigned char command[4] = {ESC, '[', '2', 'J'};
    uart_write_string(command, 4);
}

void move_cursor(unsigned int x, unsigned int y) {
    move_home();
    move_down(y);
    move_right(x);
}

void draw_horizontal(unsigned int length, char c) {
    int i;
    for (i = 0; i < length; i++) {
        uart_write(c);
    }
}

void draw_vertical(unsigned int length,
                   unsigned int x,
                   unsigned int y,
                   char c) {
    int i;

    move_cursor(x,y);
    for (i = 0; i < length; i++) {
        uart_write(c);
        y++;
        move_cursor(x,y);
    }
}
void print_border() {
    move_home();
    draw_horizontal(LENGTH, '=');
    draw_vertical(WIDTH-2, LENGTH-1, 1, '|');
    draw_vertical(WIDTH-2, 0, 1, '|');
    draw_horizontal(LENGTH, '=');
    draw_vertical(WIDTH - 2, 30, 1, '|');
}

void print_info() {
    int y = INFO_Y_CORD;
    move_cursor(INFO_X_CORD, y);
    if (scope_get_mode() == SCOPE_MODE_AC) {
        uart_write_string("AC MODE", 7);
        y += 2;
        move_cursor(INFO_X_CORD, y);
        uart_write_string("AC PKPK: ", 9);
        uart_write_int(scope_get_ac_pkpk());
        y += 2;
        move_cursor(INFO_X_CORD, y);
        uart_write_string("AC FREQ: ", 9);
        uart_write_int(scope_get_ac_freq());
        y += 2;
        move_cursor(INFO_X_CORD, y);
        uart_write_string("AC PERIOD: ", 11);
        uart_write_int(scope_get_ac_period());

    } else {
        uart_write_string("DC MODE", 7);
    }
    y += 2;
    move_cursor(INFO_X_CORD, y);
    uart_write_string("DC OFFSET: ", 11);
    uart_write_int(scope_get_ac_dc_offset());
    y += 2;
    move_cursor(INFO_X_CORD, y);
    uart_write_string("NUM SAMPLES: ", 13);
    uart_write_int(scope_get_num_samples());
}

void print_graph_title() {
    move_cursor(HIST_TITLE_X, HIST_TITLE_Y);
    uart_write_string("HISTOGRAM", 9);
}

void print_time_divisions() {}

void print_volt_divisions() {
    int volt_mes_y = HIST_TITLE_Y + 1;
    int i;
    int volts = 3000;
    move_cursor(75, volt_mes_y);
    for (i = 0; i < 7; i++) {
        uart_write_int(volts/1000);
        if((volts/500) %2){
            uart_write_string(".5", 2);
        }
        uart_write('V');
        volt_mes_y+= 3;
        move_cursor(75, volt_mes_y);
        volts -= 500;
    }
}

void print_bar(unsigned int val, unsigned int x, unsigned int y) {
    int mes = 0, count = 0;
    while (mes < val) {
        mes += VOLT_DIVISION;
        count++;
    }
    draw_vertical(count, x, y, '|');
}

void print_graph_border() {
    print_graph_title();
    print_volt_divisions();
}
void print_DC_Graph() {
    int height = 0, volts = 0;
    if (scope_get_mode() == SCOPE_MODE_DC) {
        print_time_divisions();
        while (scope_get_dc_value() < volts) {
            volts += VOLT_DIVISION;
            height++;
        }
        move_cursor(GRAPH_LEFT, GRAPH_BOTTOM + height);
        draw_horizontal(GRAPH_LENGTH, '-');
    }
}
void print_AC_Graph() {}

void scope_refresh_term() {
    hide_cursor();
    print_AC_Graph();
    print_DC_Graph();
    print_info();
}

void paint_terminal() {
    clear_screen();
    hide_cursor();
    print_border();
    print_info();
    print_graph_title();
    print_volt_divisions();
    move_home();
}

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
#include "delay.h"
#include "scope_data.h"
#include "uart.h"

static unsigned char com[2] = {ESC, '['};

void move_down(unsigned int val) {
    uart_write_string(com, 2);
    uart_write_int(val);
    uart_write('B');
}

void move_up(unsigned int val) {
    uart_write_string(com, 2);
    uart_write_int(val);
    uart_write('A');
}

void move_left(unsigned int val) {
    uart_write_string(com, 2);
    uart_write_int(val);
    uart_write('D');
}

void move_right(unsigned int val) {
    uart_write_string(com, 2);
    uart_write_int(val);
    uart_write('C');
}

void move_home() {
    unsigned char command[3] = {ESC, '[', 'H'};
    uart_write_string(command, 3);
}

void hide_cursor() {
    unsigned char command[4] = {ESC, '[', '8', 'm'};
    uart_write_string(command, 4);
}

void term_clear_screen() {
    unsigned char command[4] = {ESC, '[', '2', 'J'};
    uart_write_string(command, 4);
}

void move_cursor(unsigned int x, unsigned int y) {
    uart_write_string(com, 2);
    uart_write_int(y);
    uart_write(';');
    uart_write_int(x);
    uart_write('H');
}

void draw_horizontal(unsigned int length,
                     unsigned int x,
                     unsigned int y,
                     char c) {
    int i;
    move_cursor(x, y);
    for (i = 0; i < length; i++) {
        uart_write(c);
    }
}

void draw_vertical(unsigned int length,
                   unsigned int x,
                   unsigned int y,
                   char c,
                   int bar) {
    int i;

    move_cursor(x, y);
    for (i = 0; i < length; i++) {
        if (i == 0 || i == length - 1) {
            if (bar) {
                uart_write(c);
            } else {
                uart_write('+');
            }
        } else {
            uart_write(c);
        }
        y--;
        move_cursor(x, y);
    }
}

void print_border() {
    draw_horizontal(LENGTH - 2, 2, 1, '=');
    draw_horizontal(LENGTH - 2, 2, WIDTH, '=');
    draw_vertical(WIDTH, LENGTH, WIDTH, '|', 0);
    draw_vertical(WIDTH, 1, WIDTH, '|', 0);
    draw_vertical(WIDTH, DIVIDE_GRAPH, WIDTH, '|', 0);
}

void print_info_text() {
    int i, y = INFO_Y_CORD + 2;
    move_cursor(INFO_X_CORD, y);
    if (scope_get_mode() == SCOPE_MODE_AC) {
        uart_write_string("  AC MODE", 9);
        y += 2;
        move_cursor(INFO_X_CORD, y);
        uart_write_string("AC PKPK: ", 9);
        y += 2;
        move_cursor(INFO_X_CORD, y);
        uart_write_string("DC OFFSET: ", 11);
        y += 2;
        move_cursor(INFO_X_CORD, y);
        uart_write_string("AC FREQ: ", 9);
        y += 2;
        move_cursor(INFO_X_CORD, y);
        uart_write_string("AC PERIOD: ", 11);

    } else {
        uart_write_string("  DC MODE", 9);
        y += 2;
        move_cursor(INFO_X_CORD, y);
        uart_write_string("DC OFFSET: ", 11);
    }
    y += 2;
    move_cursor(INFO_X_CORD, y);
    uart_write_string("NUM SAMPLES: ", 13);
    if (scope_get_mode() == SCOPE_MODE_DC) {
        for (i = 0; i < 3; i++) {
            y += 2;
            move_cursor(INFO_X_CORD, y);
            uart_write_string("                  ", 18);
        }
    }
}

void print_info_values() {
    int y = INFO_Y_CORD + 2;
    move_cursor(INFO_X_CORD, y);
    if (scope_get_mode() == SCOPE_MODE_AC) {
        y += 2;
        move_cursor(INFO_X_CORD + 9, y);
        uart_write_volts(scope_get_ac_pkpk());
        uart_write_string("  ", 2);
        y += 2;
        move_cursor(INFO_X_CORD + 11, y);
        uart_write_volts(scope_get_ac_dc_offset());
        uart_write_string("      ", 6);

        y += 2;
        move_cursor(INFO_X_CORD + 9, y);
        uart_write_int(scope_get_ac_freq());
        uart_write_string ("Hz      ",8);
        y += 2;
        move_cursor(INFO_X_CORD + 11, y);
        uart_write_int(scope_get_ac_period());
        uart_write_string("ms   ", 5);

    } else {
        y += 2;
        move_cursor(INFO_X_CORD + 11, y);
        uart_write_volts(scope_get_dc_value());
    }
    y += 2;
    move_cursor(INFO_X_CORD + 13, y);
    uart_write_int(scope_get_num_samples());
    uart_write_string("    ", 4);
}

void print_graph_title() {
    move_cursor(HIST_TITLE_X, HIST_TITLE_Y);
    uart_write_string("HISTOGRAM", 9);
    move_cursor(INFO_X_CORD, INFO_Y_CORD);
    uart_write_string("OSCILLOSCOPE", 12);
}

void print_time_divisions() {
    int times, time = 0, x = TIME_X_START;
    move_cursor(x, TIME_Y);
    if (scope_get_histogram_units() == 1) {
        uart_write_string(" s", 2);
    } else {
        uart_write_string("ms", 2);
    }
    x -= 5;
    move_cursor(x, TIME_Y);
    for (times = 0; times < HISTOGRAM_SIZE; times++) {
        uart_write_int(time);
        time += scope_get_histogram_div();
        x -= 5;
        move_cursor(x, TIME_Y);
    }
}

void print_volt_divisions() {
    int volt_mes_y = HIST_TITLE_Y + 1;
    int i;
    int volts = 330;
    move_cursor(74, volt_mes_y);
    for (i = 0; i < 7; i++) {
        uart_write_volts(volts);
        volt_mes_y += 3;
        move_cursor(74, volt_mes_y);
        volts -= 55;
    }
}

void print_bar(unsigned int val, unsigned int x, unsigned int y) {
    int mes = 0, height = 0;
    while (val > mes) {
        mes += VOLT_DIVISION;
        height++;
    }
    if (val < VOLT_DIVISION) {
        draw_vertical(1, x, y, '-', BAR);
        draw_vertical(GRAPH_HEIGHT - 1, x, y - 1, ' ', BAR);
    } else {
        draw_vertical(height, x, y, '|', BAR);
        draw_vertical(GRAPH_HEIGHT - height, x, y - height, ' ', BAR);
    }
}

void print_graph_border() {
    print_graph_title();
    print_volt_divisions();
}

void print_Graph() {
    int i;
    for (i = 0; i < HISTOGRAM_SIZE; i++) {
        print_bar(scope_get_histogram(i), GRAPH_LEFT + (HISTOGRAM_SIZE - i) * 5,
                  GRAPH_BOTTOM);
    }
}

void scope_refresh_term() {
    hide_cursor();
    print_Graph();
    print_info_values();
}

void paint_terminal() {
    // term_clear_screen();
    hide_cursor();
    print_border();
    print_info_text();
    print_graph_title();
    print_volt_divisions();
    print_time_divisions();
    scope_refresh_term();
    move_home();
}

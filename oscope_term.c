/*
 * oscope_term.c
 *
 * Danica Fujiwara & Spencer Shaw
 *
 * CPE 329-17/18 Spring 2019
 *
 */

#include "oscope_term.h"
#include "oscope_data.h"
#include "uart.h"
#include <stdint.h>


void move_down(unsigned int val) {
    unsigned char command[] = {ESC, '[', val, 'B'};
    uart_write_string(command, 4);
}

void move_up(unsigned int val) {
    unsigned char command[] = {ESC, '[', val, 'A'};
    uart_write_string(command, 4);
}

void move_left(unsigned int val) {
    unsigned char command[] = {ESC, '[', val, 'D'};
    uart_write_string(command, 4);
}

void move_right(unsigned int val) {
    unsigned char command[] = {ESC, '[', val, 'C'};
    uart_write_string(command, 4);
}

void move_home() {
    unsigned char command[] = {ESC, '[', 'H'};
    uart_write_string(command, 3);
}

void clear_screen() {
    unsigned char command[] = {ESC, '[', '2', 'J'};
    uart_write_string(command, 4);
}

void move_cursor(unsigned int x, unsigned int y) {
    unsigned char command[] = {ESC, '[', x, ';', y, 'H'};
    uart_write_string(command, 4);
}

void draw_horizontal(unsigned int length, char c) {
    int i;
    for (i = 0; i < length; i++) {
        uart_write(c);
    }
}

void draw_vertical(unsigned int length, unsigned int x, unsigned int y,
                   char c) {
    int i;
    move_cursor(x, y);
    for (i = 0; i < length; i++) {
        uart_write(c);
        y++;
        move_cursor(x, y);
    }
}
void print_border() {
    move_home();
    draw_horizontal(LENGTH, 'X');
    draw_vertical(WIDTH, WIDTH, 0, 'X');
    draw_vertical(WIDTH, 0, 0, 'X');
    draw_horizontal(LENGTH, 'X');
    draw_vertical(WIDTH - 2, 40, 1, 'X');
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

void print_time_divisions() {

}

void print_volt_divisions() {
    int volt_mes_y = HIST_TITLE_Y +1;
    int i,volts = 3000;
    move_cursor(77, volt_mes_y);
    for(i=0; i <20; i+= 4){
        uart_write_int(volts - i*(VOLT_DIVISION*4));
        uart_write('V');
        volt_mes_y++;
        move_cursor(77, volt_mes_y);
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

void print_graph_border(){
    print_graph_title();
    print_volt_divisions();
}
void print_DC_Graph(){
    int height = 0, volts = 0;
    if(scope_get_mode()== SCOPE_MODE_DC){
        print_time_divisions();
        while (get_dc_offset() < volts){
            volts += VOLT_DIVISION;
            height ++;
        }
        move_cursor(GRAPH_LEFT, GRAPH_BOTTOM+height);
        draw_horizontal(GRAPH_LENGTH, '-');
    }
}
void print_AC_Graph(){

}

void scope_refresh_terminal() {
  clear_screen();
  print_border();
  print_info();
  print_graph_border();
  print_DC_Graph();
  print_AC_Graph();
}

void update_terminal() {
    print_info();
    print_DC_Graph();
    print_AC_Graph();
}

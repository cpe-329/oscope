/*
 * oscope_term.c
 *
 *  Created on: May 9, 2019
 *      Author: sfshaw
 */

#include "oscope_term.h"
#include "oscope_data.h"
#include <stdint.h>
#include "uart.h"



void update_terminal(){

}

void move_down(unsigned int val){
    unsigned char command[] = {ESC, '[', val, 'B'};
    uart_write_string(command, 4);
}

void move_up(unsigned int val){
    unsigned char command[] = {ESC, '[', val, 'A'};
    uart_write_string(command, 4);
}

void move_left(unsigned int val){
    unsigned char command[] = {ESC, '[', val, 'D'};
    uart_write_string(command, 4);
}

void move_right(unsigned int val){
    unsigned char command[] = {ESC, '[', val, 'C'};
    uart_write_string(command, 4);
}


void move_home(){
    unsigned char command[] = {ESC, '[', 'H'};
    uart_write_string(command, 3);
}

void clear_screen(){
    unsigned char command[] = {ESC, '[', '2','J'};
    uart_write_string(command, 4);
}

void move_cursor(unsigned int x, unsigned int y){
    unsigned char command[] = {ESC, '[', x, ';', y,'H'};
    uart_write_string(command, 4);
}

void draw_horizontal(unsigned int length, char c){
    int i;
    for (i=0; i < length; i++){
        uart_write(c);
    }
}

void draw_vertical(unsigned int length, unsigned int x, unsigned int y, char c){
    int i;
    move_cursor(x,  y);
    for (i=0; i < length; i++){
        uart_write(c);
        y++;
        move_cursor(x, y);
    }
}
void print_border(){
    move_home();
    draw_horizontal(LENGTH, 'X');
    draw_vertical(WIDTH, WIDTH, 0, 'X');
    draw_vertical(WIDTH, 0, 0, 'X');
    draw_horizontal(LENGTH, 'X');
    draw_vertical(WIDTH-2, 40, 1,'X');
}

void print_info(){
    int y = INFO_Y_CORD;
    move_cursor(INFO_X_CORD,y);
    if (oscope_mode == OSCOPE_MODE_AC){
        uart_write_string("AC MODE",7);
        y+=2;
        move_cursor(INFO_X_CORD, y);
        uart_write_string("AC PKPK: ",9);
        uart_write_int(ac_pkpk);
        y+=2;
        move_cursor(INFO_X_CORD, y);
        uart_write_string("AC FREQ: ",9);
        uart_write_int(ac_freq);
        y+=2;
        move_cursor(INFO_X_CORD, y);
        uart_write_string("AC PERIOD: ", 11);
        uart_write_int(ac_period);

    }
    else{
        uart_write_string("DC MODE", 7);
    }
    y+=2;
    move_cursor(INFO_X_CORD, y);
    uart_write_string("DC OFFSET: ", 11);
    uart_write_int(ac_dc_offset);
    y+=2;
    move_cursor(INFO_X_CORD, y);
    uart_write_string("NUM SAMPLES: ", 13);
    uart_write_int(num_samples);


}

void print_graph_title(){
    move_cursor(HIST_TITLE_X, HIST_TITLE_Y);
    uart_write_string("HISTOGRAM", 9);
}

void refresh_terminal(){
    clear_screen();
    print_border();
    print_info();
}


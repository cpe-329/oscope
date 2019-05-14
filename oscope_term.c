/*
 * oscope_term.c
 *
 *  Created on: May 9, 2019
 *      Author: sfshaw
 */

#include "oscope_term.h"

void refresh_terminal(){

}

void update_terminal(){

}

void move_down(unsigned int val){
    printf("%c[%dB",ESC, val);
}

void move_up(unsigned int val){
    printf("%c[%dA",ESC, val);
}

void move_left(unsigned int val){
    printf("%c[%dD",ESC, val);
}

void move_right(unsigned int val){
    printf("%c[%dC",ESC, val);
}


void move_home(){
    printf("%c[H",ESC);
}

void clear_screen(){
    printf("%c[%2J",ESC);
}

void move_cursor(unsigned int x, unsigned int y){
    move_home();
    move_right(x);
    move_down(y);
}

void draw_horizontal(unsigned int length, char x){
    int i;
    for (i=0; i < length; i++){
        printf("%c", x);
    }
}

void draw_vertical(unsigned int length, char x){
    int i;
    for (i=0; i < length; i++){
        printf("%c", x);
        move_down();

    }
}
void print_border(){
    move_home();
    draw_horizontal(LENGTH, 'X');
    draw_vertical(WIDTH, 'X');
    move_home();
    draw_vertical(WIDTH, 'X');
    draw_horizontal(LENGTH, 'X');
    move_cursor(40, 0);
    draw_vertical(WIDTH-2, 'X')
}

void print_info(){
    int y = INFO_Y_CORD;
    move_cursor(INFO_X_CORD,y);
    if (oscope_mode = OSCOPE_MODE_AC){
        printf("AC MODE");
        y+=2;
        move_cursor(INFO_X_CORD, y);
        printf("AC PKPK: %d"; ac_pkpk);
        y+=2;
        move_cursor(INFO_X_CORD, y);
        printf("AC FREQ: %d"; ac_freq);
        y+=2;
        move_cursor(INFO_X_CORD, y);
        printf("AC PERIOD: %d"; ac_period);

    }
    else{
        printf("DC MODE");
    }
    y+=2;
    move_cursor(INFO_X_CORD, y);
    printf("DC OFFSET: %d"; dc_offset);
    y+=2;
    move_cursor(INFO_X_CORD, y);
    printf("NUM SAMPLES: %d"; num_samples);


}

void init_terminal(){
    clear_screen();
    print_border();
    print_info();
}

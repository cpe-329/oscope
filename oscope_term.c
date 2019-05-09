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


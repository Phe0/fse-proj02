#ifndef _MENU_H_
#define _MENU_H_

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "alarm.h"
#include "tcp_server.h"

void set_temperature(float value);
void set_humidity(float value);
void person_enter();
void person_leaves();
void init_menu();
void close_menu();
int get_option(WINDOW * menuwin, char * choices[], int size);
void set_title(WINDOW* menuwin, char* title);
WINDOW* get_menu(int position, int ammount);
void data_info();
void menu();
void set_box(WINDOW* menu, char* title);

#endif
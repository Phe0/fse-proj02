#ifndef _PEOPLE_COUNTER_H_
#define _PEOPLE_COUNTER_H_

#include "gpio.h"

void* check_people_in(void* arg);
void* check_people_out(void* arg);
int get_current_people_in();

#endif
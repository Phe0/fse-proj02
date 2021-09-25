#ifndef _GPIO_H_
#define _GPIO_H_

#include <bcm2835.h>
#include <sched.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define ON  1
#define OFF 0

int init_bcm();
int convert_pin(int gpio_pin);
void close_bcm();
int write_gpio(int pin, int status);
int read_gpio(int pin);
void listen_event(int pin);
void close_event(int pin);
void close_all_events();
void* check_people_in(void* arg);
void* check_people_out(void* arg);
int get_current_people_in();
void set_max_priority();
void set_default_priority();
void set_as_input(int gpio_pin);
void set_as_output(int gpio_pin);

#endif
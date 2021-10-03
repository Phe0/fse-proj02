#ifndef _GPIO_H_
#define _GPIO_H_

#include <wiringPi.h>
#include <sched.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#include "json_parser.h"

#define ON  1
#define OFF 0

#define PERSON_IN_PIN   13
#define PERSON_OUT_PIN  19

#define IGNORE_CHANGE_BELOW_USEC 200000

void handle_presence();
void handle_smoke();
void handle_person_in();
void handle_person_out();
int convert_pin(int gpio_pin);
void set_max_priority();
void set_as_input(int gpio_pin, char* type);
void set_as_output(int gpio_pin);
int init_gpio(struct configuration config);
int write_gpio(int pin, int status);
int read_gpio(int pin);
int check_input(int pin);
int check_output(int pin);

#endif
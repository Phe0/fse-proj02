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

#include "json_parser.h"

#define ON  1
#define OFF 0

#define PERSON_IN_PIN   13
#define PERSON_OUT_PIN  19

void set_outputs(struct device* outputs_array, int outputs_ammount);
void set_inputs(struct device* inputs_array, int inputs_ammount);
int init_bcm();
int convert_pin(int gpio_pin);
void close_bcm();
int write_gpio(int pin, int status);
int invert_gpio(int pin);
int read_gpio(int pin);
void listen_event(int pin);
int check_input(int pin);
int check_output(int pin);
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
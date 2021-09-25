#include "people_counter.h"

int people_in = 0;
int people_out = 0;

void* check_people_in(void* arg) {
    int pin = *((int *)arg);

    while(1) {
        bcm2835_gpio_set_eds(convert_pin(pin));
        sleep(0.2);

        if (bcm2835_gpio_eds(convert_pin(pin))) {
            people_in++;
        }
    }
}

void* check_people_out(void* arg) {
    int pin = *((int *)arg);

    while(1) {
        bcm2835_gpio_set_eds(convert_pin(pin));
        sleep(0.2);

        if (bcm2835_gpio_eds(convert_pin(pin))) {
            people_out++;
        }
    }
}

int get_current_people_in() {
    return people_in - people_out;
}
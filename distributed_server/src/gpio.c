#include "gpio.h"

int pin_map[] = {
    -1,
    -1,
    -1,
    -1,
    7, // 4
    21, // 5
    22, // 6
    11, // 7
    10, // 8
    13, // 9
    12, // 10
    14, // 11
    26, // 12
    23, // 13
    -1,
    -1,
    27, // 16
    11, // 17
    1, // 18
    24, // 19
    28, // 20
    29, // 21
    3, // 22
    4, // 23
    5, // 24
    2, // 25
    25, // 26
    2  // 27
};

int* inputs;
int inputs_length;

int* outputs;
int outputs_length;

int open_events[28];
int event_counter = 0;

struct timeval last_change;

void handle_presence() {
    struct timeval now;
    unsigned long diff;
    gettimeofday(&now, NULL);
    diff = (now.tv_sec * 1000000 + now.tv_usec) - (last_change.tv_sec * 1000000 + last_change.tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        printf("alarme de presenca\n");
    }
    last_change = now;
}

void handle_smoke() {
    struct timeval now;
    unsigned long diff;
    gettimeofday(&now, NULL);
    diff = (now.tv_sec * 1000000 + now.tv_usec) - (last_change.tv_sec * 1000000 + last_change.tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        printf("alarme de fumaca\n");
    }
    last_change = now;
}

void handle_person_in() {
    struct timeval now;
    unsigned long diff;
    gettimeofday(&now, NULL);
    diff = (now.tv_sec * 1000000 + now.tv_usec) - (last_change.tv_sec * 1000000 + last_change.tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        printf("pessoa entrou\n");
    }
    last_change = now;
}

void handle_person_out() {
    struct timeval now;
    unsigned long diff;
    gettimeofday(&now, NULL);
    diff = (now.tv_sec * 1000000 + now.tv_usec) - (last_change.tv_sec * 1000000 + last_change.tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        printf("pessoa saiu\n");
    }
    last_change = now;
}

int convert_pin(int gpio_pin) {
    return pin_map[gpio_pin];
}

void set_max_priority() {
    struct sched_param sched;
    memset(&sched, 0, sizeof(sched));

    sched.sched_priority = sched_get_priority_max(SCHED_FIFO);
    sched_setscheduler(0, SCHED_FIFO, &sched);
}

void set_as_input(int gpio_pin, char* type) {
    pinMode(convert_pin(gpio_pin), INPUT);
    if (strlen(type)) {
        if (strcmp(type, "presenca") == 0 || strcmp(type, "janela") == 0 || strcmp(type, "porta") == 0) {
            wiringPiISR(convert_pin(gpio_pin), INT_EDGE_RISING, &handle_presence);
        }
        if (strcmp(type, "fumaca") == 0) {
            wiringPiISR(convert_pin(gpio_pin), INT_EDGE_RISING, &handle_smoke);
        }
        if (strcmp(type, "contagem") == 0) {
            if (gpio_pin == PERSON_IN_PIN) {
                wiringPiISR(convert_pin(gpio_pin), INT_EDGE_RISING, &handle_person_in);
            }
            if (gpio_pin == PERSON_OUT_PIN) {
                wiringPiISR(convert_pin(gpio_pin), INT_EDGE_RISING, &handle_person_out);
            }
        }
    }
}

void set_as_output(int gpio_pin) {
    pinMode(convert_pin(gpio_pin), OUTPUT);
}

int init_gpio(struct configuration config) {

    set_max_priority();

    mlockall(MCL_CURRENT | MCL_FUTURE);

    wiringPiSetup();

    gettimeofday(&last_change, NULL);

    for (int i = 0; i < config.inputs_length; i++) {
        set_as_input(config.inputs[i].gpio, config.inputs[i].type);
    }

    for (int i = 0; i < config.outputs_length; i++) {
        set_as_output(config.outputs[i].gpio);
    }

    return 1;
}

int write_gpio(int pin, int status) {

    int isListed = 0;
    for(int i = 0; i < outputs_length; i++) {
        if (outputs[i] == pin) {
            isListed = 1;
            break;
        }
    }

    if (!isListed)
        return -1;

    digitalWrite(convert_pin(pin), status);

    int current_level = digitalRead(convert_pin(pin));
    if (status == current_level)
        return 1;
    return 0;
}

int read_gpio(int pin) {
    int result = digitalRead(convert_pin(pin));
    return result;
}

int check_input(int pin) {
    for (int i = 0; i < inputs_length; i++) {
        if (inputs[i] == pin)
            return 1;
    }
    return 0;
}

int check_output(int pin) {
    for (int i = 0; i < outputs_length; i++) {
        if (outputs[i] == pin)
            return 1;
    }
    return 0;
}

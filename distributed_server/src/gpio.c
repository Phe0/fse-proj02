#include "gpio.h"

int pin_map[] = {
    -1,
    -1,
    -1,
    -1,
    RPI_V2_GPIO_P1_07, // 4
    RPI_V2_GPIO_P1_29, // 5
    RPI_V2_GPIO_P1_31, // 6
    RPI_V2_GPIO_P1_26, // 7
    RPI_V2_GPIO_P1_24, // 8
    RPI_V2_GPIO_P1_21, // 9
    RPI_V2_GPIO_P1_19, // 10
    RPI_V2_GPIO_P1_23, // 11
    RPI_V2_GPIO_P1_32, // 12
    RPI_V2_GPIO_P1_33, // 13
    -1,
    -1,
    RPI_V2_GPIO_P1_36, // 16
    RPI_V2_GPIO_P1_11, // 17
    RPI_V2_GPIO_P1_12, // 18
    RPI_V2_GPIO_P1_35, // 19
    RPI_V2_GPIO_P1_38, // 20
    RPI_V2_GPIO_P1_40, // 21
    RPI_V2_GPIO_P1_15, // 22
    RPI_V2_GPIO_P1_16, // 23
    RPI_V2_GPIO_P1_18, // 24
    RPI_V2_GPIO_P1_22, // 25
    RPI_V2_GPIO_P1_37, // 26
    RPI_V2_GPIO_P1_13  // 27
};

int* inputs;
int inputs_length;

int* outputs;
int outputs_length;

int open_events[28];
int event_counter = 0;

void set_outputs(struct device* outputs_array, int outputs_ammount) {
    outputs = malloc(outputs_ammount * sizeof(int));
    outputs_length = outputs_ammount;
    for (int i = 0; i < outputs_ammount; i++) {
        outputs[i] = outputs_array[i].gpio;
    }
}

void set_inputs(struct device* inputs_array, int inputs_ammount) {
    inputs = malloc(inputs_ammount * sizeof(int));
    inputs_length = inputs_ammount;
    for (int i = 0; i < inputs_ammount; i++) {
        inputs[i] = inputs_array[i].gpio;
    }
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

void set_default_priority() {
    struct sched_param sched;
    memset(&sched, 0, sizeof(sched));

    sched.sched_priority = 0;
    sched_setscheduler(0, SCHED_OTHER, &sched);
}

void set_as_input(int gpio_pin) {
    bcm2835_gpio_fsel(convert_pin(gpio_pin), BCM2835_GPIO_FSEL_INPT);
}

void set_as_output(int gpio_pin) {
    bcm2835_gpio_fsel(convert_pin(gpio_pin), BCM2835_GPIO_FSEL_OUTP);
}

int init_bcm() {

    set_max_priority();

    mlockall(MCL_CURRENT | MCL_FUTURE);

    if(!bcm2835_init())
        return 0;

    for (int i = 0; i < inputs_length; i++) {
        set_as_input(inputs[i]);
    }

    for (int i = 0; i < outputs_length; i++) {
        set_as_output(outputs[i]);
    }

    return 1;
}

void close_bcm() {
    bcm2835_close();
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

    bcm2835_gpio_write(convert_pin(pin), status);

    int current_level = bcm2835_gpio_lev(convert_pin(pin));
    if (status == current_level)
        return 1;
    return 0;
}

int invert_gpio(int pin) {
    int current_status = read_gpio(pin);
    if (current_status == HIGH)
        return write_gpio(pin, LOW);
    else
        return write_gpio(pin, HIGH);
}

int read_gpio(int pin) {
    int result = bcm2835_gpio_lev(convert_pin(pin));
    return result;
}

void listen_event(int pin) {
    bcm2835_gpio_ren(convert_pin(pin));
    open_events[event_counter] = pin;
    event_counter++;
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

void close_event(int pin) {
    bcm2835_gpio_clr_ren(convert_pin(pin));
}

void close_all_events() {
    for (int i = 0; i < event_counter; i++) {
        close_event(open_events[i]);
    }
}
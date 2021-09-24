#include "gpio.h"

int pin_map[] = {
    -1,
    -1,
    -1,
    -1,
    RPI_V2_GPIO_P1_07,
    RPI_V2_GPIO_P1_29,
    RPI_V2_GPIO_P1_31,
    RPI_V2_GPIO_P1_26,
    RPI_V2_GPIO_P1_24,
    RPI_V2_GPIO_P1_21,
    RPI_V2_GPIO_P1_19,
    RPI_V2_GPIO_P1_23,
    RPI_V2_GPIO_P1_32,
    RPI_V2_GPIO_P1_33,
    -1,
    -1,
    RPI_V2_GPIO_P1_36,
    RPI_V2_GPIO_P1_11,
    RPI_V2_GPIO_P1_12,
    RPI_V2_GPIO_P1_35,
    -1,
    -1,
    RPI_V2_GPIO_P1_15,
    RPI_V2_GPIO_P1_16,
    RPI_V2_GPIO_P1_18,
    RPI_V2_GPIO_P1_22,
    RPI_V2_GPIO_P1_37,
    RPI_V2_GPIO_P1_13
};

int inputs[] = {26, 23, 9, 11, 10, 13, 19};

int outputs[] = {4, 17, 27, 7, 16, 22, 25, 8, 12, 18, 24, 5, 6};

int init_bcm(int pin) {
    if(!bcm2835_init())
        return 0;

    bcm2835_gpio_fsel(pin_map[7], BCM2835_GPIO_FSEL_OUTP);

    /*
    for (int i = 0; i < sizeof(inputs)/sizeof(inputs[0]); i++) {
        bcm2835_gpio_fsel(inputs[i], BCM2835_GPIO_FSEL_INPT);
    }

    for (int i = 0; i < sizeof(outputs)/sizeof(outputs[0]); i++) {
        bcm2835_gpio_fsel(outputs[i], BCM2835_GPIO_FSEL_OUTP);
    }
    */

    return 1;
}

void write_gpio(int pin, int status) {
    bcm2835_gpio_write(pin_map[7], status);
}

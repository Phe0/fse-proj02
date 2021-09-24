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
    -1,
    -1,
    RPI_V2_GPIO_P1_15, // 20
    RPI_V2_GPIO_P1_16, // 21
    RPI_V2_GPIO_P1_18, // 22
    RPI_V2_GPIO_P1_22, // 23
    RPI_V2_GPIO_P1_37, // 24
    RPI_V2_GPIO_P1_13  // 25
};

int inputs[] = {26, 23, 9, 11, 10, 13, 19};

int outputs[] = {4, 17, 27, 7, 16, 22, 25, 8, 12, 18, 24, 5, 6};

int init_bcm() {
    if(!bcm2835_init())
        return 0;

    for (int i = 0; i < sizeof(inputs)/sizeof(inputs[0]); i++) {
        bcm2835_gpio_fsel(pin_map[inputs[i]], BCM2835_GPIO_FSEL_INPT);
    }

    for (int i = 0; i < sizeof(outputs)/sizeof(outputs[0]); i++) {
        bcm2835_gpio_fsel(pin_map[outputs[i]], BCM2835_GPIO_FSEL_OUTP);
    }

    return 1;
}

int write_gpio(int pin, int status) {
    bcm2835_gpio_write(pin_map[pin], status);

    int current_level = bcm2835_gpio_lev(pin_map[pin]);
    if (status == current_level)
        return 1;
    return 0;
}

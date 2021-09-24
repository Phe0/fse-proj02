#ifndef _GPIO_H_
#define _GPIO_H_

#include <bcm2835.h>

#define ON  1
#define OFF 0

int init_bcm();

int write_gpio(int pin, int status);

#endif
#ifndef _DHT22_H_
#define _DHT22_H_

#include "gpio.h"

#define DHT_MAXCOUNT 32000
#define DHT_PULSES   41

#define DHT_PIN      20
#define MAXTIMINGS   100

float get_temperature();
float get_humidity();
int read_dht_data();
void* set_dht_values(void* arg);

#endif
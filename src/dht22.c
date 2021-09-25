#include "dht22.h"

float temperature;
float humidity;

float get_temperature() {
    return temperature;
}

float get_humidity() {
    return humidity;
}

int read_dht_data() {
    int pulseCounts[DHT_PULSES*2] = {0};

    set_as_output(DHT_PIN);
    write_gpio(DHT_PIN, HIGH);
    usleep(500000);

    write_gpio(DHT_PIN, LOW);
    usleep(20000);

    set_as_input(DHT_PIN);

    while(read_gpio(DHT_PIN)) {
        usleep(1);
    }

    uint32_t count = 0;
    while (read_gpio(DHT_PIN)) {
        if (++count >= DHT_MAXCOUNT) {
            set_default_priority();
            return -1;
        }
    }

    for (int i=0; i < DHT_PULSES*2; i+=2) {
        while(!read_gpio(DHT_PIN)) {
            if (++pulseCounts[i] >= DHT_MAXCOUNT) {
                set_default_priority();
                return -1;
            }
        }

        while(read_gpio(DHT_PIN)) {
            if (++pulseCounts[i+1] >= DHT_MAXCOUNT) {
                set_default_priority();
                return -1;
            }
        }
    }

    set_default_priority();

    uint32_t threshold = 0;
    for (int i=2; i < DHT_PULSES*2; i+=2) {
        threshold += pulseCounts[i];
    }
    threshold /= DHT_PULSES-1;

    uint32_t data[5] = {0};
    for (int i=3; i < DHT_PULSES*2; i+=2) {
        int index = (i-3) / 16;
        data[index] <<= 1;
        if (pulseCounts[i] >= threshold) {
            data[index] |= 1;
        }
    }

    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        humidity = (data[0] * 256 + data[1]) / 10.0f;
        temperature = ((data[2] & 0x7F) * 256 + data[3]) / 10.0f;
        if (data[2] & 0x80) {
            temperature *= -1.0f;
        }
        return 1;
    }
    return -1;
}

void* set_dht_values(void *arg) {
    while(1) {
        read_dht_data();
        sleep(1);
    }
}
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
    int data[5] = { 0, 0, 0, 0, 0 };
    int counter = 0;
    int laststate = HIGH;
    int j = 0;

    set_as_output(DHT_PIN);

    write_gpio(DHT_PIN, LOW);
    delay(18);

    set_as_input(DHT_PIN);

    while(read_gpio(DHT_PIN) == HIGH) {
        usleep(1);
    }

    for (int i = 0; i < MAXTIMINGS; i++) {
        counter = 0;
        while(read_gpio(DHT_PIN) == laststate) {
            counter++;
            if(counter == 1000)
                break;
        }
        laststate = read_gpio(DHT_PIN);
        if (counter == 1000) break;

        if (i>3 && i%2 == 0) {
            data[j/8] <<= 1;
            if (counter > 200)
                data[j/8] |= 1;
            j++;
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
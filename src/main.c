#include "main.h"

int main(int argc, char* argv[]) {
    int pin = atoi(argv[1]);

    if (!init_bcm(pin)) {
        perror("Error initing bcm2835");
        exit(1);
    }

    write_gpio(pin, ON);

    sleep(5);

    write_gpio(pin, OFF);

    return 0;
}
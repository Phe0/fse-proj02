#include "main.h"

int main(int argc, char* argv[]) {

    //int pin = atoi(argv[1]);
    init_client("192.168.0.52", 10123);

    while(1) {
        int pin;
        scanf("%d", &pin);
        if (pin < 0)
            break;
        int result = make_order(pin);
        printf("%d\n", result);
    }

    close_client();

    return 0;
}
#include "main.h"

pthread_t thread_in, thread_out, thread_dht;

void handle_interuption(int signal) {
    close_all_events();
    pthread_cancel(thread_in);
    pthread_cancel(thread_out);
    pthread_cancel(thread_dht);
    pthread_join(thread_in, NULL);
    pthread_join(thread_out, NULL);
    pthread_join(thread_dht, NULL);
    close_bcm();
    exit(0);
}

int main(int argc, char* argv[]) {
    int pin_in = atoi(argv[1]);
    int pin_out = atoi(argv[2]);

    if (!init_bcm()) {
        perror("Error initing bcm2835");
        exit(1);
    }

    listen_event(pin_in);
    listen_event(pin_out);

    signal(SIGINT, handle_interuption);

    pthread_create(&thread_in, NULL, &check_people_in, (void *)&pin_in);
    pthread_create(&thread_out, NULL, &check_people_out, (void *)&pin_out);
    pthread_create(&thread_dht, NULL, &set_dht_values, NULL);

    while(1) {
        printf("Temperatura: %.1f\n", get_temperature());
        printf("Umidade: %.1f\n", get_humidity());
        printf("Pessoas: %d\n\n", get_current_people_in());
        sleep(1);
    }

    pthread_join(thread_in, NULL);
    pthread_join(thread_out, NULL);
    pthread_join(thread_dht, NULL);

    return 0;
}
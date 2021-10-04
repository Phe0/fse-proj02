#include "main.h"

pthread_t thread_client;

int has_person_in_counter;
int has_person_out_counter;


void handle_interuption(int signal) {
    pthread_cancel(thread_client);
    pthread_join(thread_client, NULL);
    close_client();
    close_json();
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        perror("Arquivo de configuração deve ser fornecido");
        exit(1);
    }
    read_file(argv[1]);
    struct configuration config = parse_json();

    if (!init_gpio(config)) {
        perror("Error initing bcm2835");
        exit(1);
    }

    init_client(config.ip, config.porta_central);
    
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        error("Erro reading config file");
    }
    send_file(fp);
    fclose(fp);

    pthread_create(&thread_client, NULL, &receive_orders, NULL);

    while(1) {
        read_dht_data();
        float temp = get_temperature();
        float humidity = get_humidity();

        printf("Enviando temperatura e umidade\n");
        send_float(1, temp);
        send_float(2, humidity);
        
        sleep(1);
    }

    pthread_cancel(thread_client);
    pthread_join(thread_client, NULL);

    close_json();

    close_client();

    return 0;
}
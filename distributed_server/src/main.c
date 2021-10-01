#include "main.h"

pthread_t thread_in, thread_out, thread_dht, thread_server;

int has_person_in_counter;
int has_person_out_counter;

void handle_interuption(int signal) {
    close_all_events();
    if (has_person_in_counter) {
        pthread_cancel(thread_in);
        pthread_join(thread_in, NULL);
    }
    if (has_person_out_counter) {
        pthread_cancel(thread_out);
        pthread_join(thread_out, NULL);
    }
    pthread_cancel(thread_dht);
    pthread_join(thread_dht, NULL);

    pthread_cancel(thread_server);
    pthread_join(thread_server, NULL);
    close_bcm();
    close_sockets();
    close_json();
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        perror("Arquivo de configuração deve ser fornecido");
        exit(1);
    }

    signal(SIGINT, handle_interuption);

    read_file(argv[1]);
    struct configuration config = parse_json();

    if (!init_bcm()) {
        perror("Error initing bcm2835");
        exit(1);
    }

    set_outputs(config.outputs, config.outputs_length);
    set_inputs(config.inputs, config.inputs_length);

    has_person_in_counter = check_input(PERSON_IN_PIN);
    has_person_out_counter = check_input(PERSON_OUT_PIN);


    if (has_person_in_counter) {
        int pin_in = PERSON_IN_PIN;
        listen_event(PERSON_IN_PIN);
        pthread_create(&thread_in, NULL, &check_people_in, (void *)&pin_in);
    }

    if (has_person_out_counter) {
        int pin_out = PERSON_OUT_PIN;
        listen_event(PERSON_OUT_PIN);
        pthread_create(&thread_out, NULL, &check_people_out, (void *)&pin_out);
    }
    pthread_create(&thread_dht, NULL, &set_dht_values, NULL);
    
    // pthread_create(&thread_server, NULL, &server, (void*)&config.porta);

    init_client(config.ip, 10023);

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        error("Erro reading config file");
    }

    send_file(fp);

    //while(1) {
        /*
        printf("Temperatura: %.1f\n", get_temperature());
        printf("Umidade: %.1f\n", get_humidity());
        printf("Pessoas: %d\n", get_current_people_in());
        
        //enviar temperatura, umidade e pessoas via tcp aqui
        sleep(1);
        */
    //}

    pthread_join(thread_in, NULL);
    pthread_join(thread_out, NULL);
    pthread_join(thread_dht, NULL);
    // pthread_join(thread_server, NULL);

    close_sockets();
    close_bcm();
    close_json();

    return 0;
}
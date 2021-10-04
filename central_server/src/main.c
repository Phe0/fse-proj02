#include "main.h"

pthread_t thread_server;

void handle_interuption() {
    pthread_cancel(thread_server);
    pthread_join(thread_server, NULL);
    close_all_sockets();
    exit(0);
}

int main(int argc, char* argv[]) {

    signal(SIGINT, handle_interuption);

    int server_port = 10023;

    pthread_create(&thread_server, NULL, &server, (void*)&server_port);

    while(1) {
        int option;
        printf("O que deseja fazer?\n");
        printf("1) Acionar dispositivo\n");
        if (get_presece_alarm()) {
            printf("2) Desligar alarme de presença\n");
        } else {
            printf("2) Ligar alarme de presença\n");
        }
        if (get_smoke_alarm()) {
            printf("3) Desligar alarme de fumaça\n");
        } else {
            printf("3) Ligar alarme de fumaça\n");
        }

        scanf("%d", &option);

        printf("\n\n");
        if (option == 1) {
            struct connection* conn_list = get_conn_list();
            int conn_list_size = get_conn_list_size();

            printf("Qual dipositivo deseja acionar?\n");

            int counter = 1;
            for (int i = 0; i < conn_list_size; i++) {
                for (int j = 0; j < conn_list[i].config.outputs_length; j++) {
                    int state;
                    if (conn_list[i].config.outputs[j].state >= 0) state = conn_list[i].config.outputs[j].state;
                    else state = request_state(conn_list[i].config.outputs[j].gpio, conn_list[i].socket);
                    printf("%d ) ", counter);
                    if (state) {
                        printf("Desligar ");
                    } else {
                        printf("Ligar ");
                    }
                    printf("%s\n", conn_list[i].config.outputs[j].tag);
                    counter++;
                }
            }

            int device_index;

            scanf("%d", &device_index);

            int second_counter = 1;
            int has_ordered = 0;

            for (int i = 0; i < conn_list_size; i++) {
                for (int j = 0; j < conn_list[i].config.outputs_length; j++) {
                    if (second_counter == device_index) {
                        make_order(conn_list[i].config.outputs[j].gpio);
                        has_ordered = 1;
                        break;
                    }
                    second_counter++;
                }
                if (has_ordered) break;
            }

            if (!has_ordered) printf("Opção inválida\n\n");

        } else if (option == 2) {
            invert_presece_alarm();
        } else if (option == 3) {
            invert_smoke_alarm();
        } else {
            printf("Opção inválida\n\n");
        }
    }

    return 0;
}
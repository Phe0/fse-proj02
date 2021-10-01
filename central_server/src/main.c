#include "main.h"

pthread_t thread_server;

void handle_interuption() {
    pthread_cancel(thread_server);
    pthread_join(thread_server, NULL);
    close_sockets();
    close_all_clients();
    exit(0);
}

int main(int argc, char* argv[]) {

    // init server 
    // connect
    // receive config file
    // init client

    signal(SIGINT, handle_interuption);

    int server_port = 10023;

    pthread_create(&thread_server, NULL, &server, (void*)&server_port);

    while(1) {
        int size = get_conn_list_size();
        if (!size) {
            // printf("Zero conexões estabelecidas\n");
        } else {
            printf("%d conexões\n\n", size);
            struct connection* conn_list = get_conn_list();

            for (int i = 0; i < size; i++) {
                if (conn_list[i].is_valid) {
                    print_config(conn_list[i].config);
                } else {
                    printf("Conexão %d inválida\n", i);
                }
            }
        }
        sleep(1);
    }
    pthread_join(thread_server, NULL);

    return 0;
}
#include "main.h"

pthread_t thread_server;

void handle_interuption() {
    close_all_sockets();
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
        int gpio;
        scanf("%d", &gpio);

        if (gpio < 0) {
            close_all_sockets();
            pthread_cancel(thread_server);
            pthread_join(thread_server, NULL);
            exit(0);
        }

        make_order(gpio);
    }

    return 0;
}
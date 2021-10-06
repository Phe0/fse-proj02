#include "main.h"

pthread_t thread_server;

void handle_interuption() {
    log_file("Closing program");
    close_file();
    close_menu();
    pthread_cancel(thread_server);
    pthread_join(thread_server, NULL);
    close_all_sockets();
    exit(0);
}

int main(int argc, char* argv[]) {

    signal(SIGINT, handle_interuption);

    create_file();

    int server_port = 10023;

    pthread_create(&thread_server, NULL, &server, (void*)&server_port);

    init_menu();

    data_info();

    menu();

    handle_interuption();

    return 0;
}
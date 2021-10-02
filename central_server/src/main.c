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

    server((void*)&server_port);

    return 0;
}
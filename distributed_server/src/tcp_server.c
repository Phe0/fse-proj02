#include "tcp_server.h"

int server_socket, client_socket;
struct sockaddr_in servaddr, cliaddr;
socklen_t cli_len;

int receive_order() {
    int pin;
    int r = recv(client_socket, (void*)&pin, sizeof(int), 0);
    if (r == 0) {
        printf("Conexão perdida\n");
        return 0;
    }
    printf("Recebeu ordem para pino %d\n", pin);
    int result = invert_gpio(pin);
    if (send(client_socket, &result, sizeof(int), 0) < 0)
        printf("Erro no send");
    return 1;
}

void* order_handler() {
    while(1) {
        receive_order();
    }
}

void server(void* arg) {
    int port = *((int *)arg);
    printf("Tentando conectar com a porta %d\n", port);
    int is_running = init_server(port);
    printf("Conexão estabelecida\n");
    while(1) {
        if (!is_running) {
            printf("Tentando reconectar com servidor central\n");
            client_socket = accept(server_socket, (struct sockaddr*)&cliaddr, &cli_len);
            if (client_socket >= 0) {
                printf("Conexão estabelecida\n");
                is_running = 1;
            }
        } else {
            is_running = receive_order();
        }
    }
}

int init_server(int port) {
    
    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    check(server_socket, "Falha ao ligar socket do servidor");

    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        return 0;
    }

    if (listen(server_socket, 5) < 0) {
        return 0;
    }

    cli_len = sizeof((struct sockaddr *) &cliaddr);
    client_socket = accept(server_socket, (struct sockaddr*)&cliaddr, &cli_len);

    if (client_socket < 0) {
        return 0;
    }
    return 1;
}

void close_sockets() {
    close(server_socket);
    close(client_socket);
}
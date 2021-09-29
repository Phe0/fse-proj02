#include "tcp_client.h"

int client_socket;

void init_client(char* server_ip, int port) {
    struct sockaddr_in servaddr;

    client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(client_socket < 0)
        error("Erro opening socket");


    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server_ip);
    servaddr.sin_port = htons(port);

    if (connect(client_socket, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        error("Connection failed");
}

void close_client() {
    close(client_socket);
}

int make_order(int gpio) {
    if (send(client_socket, &gpio, sizeof(int), 0) < 0)
        printf("erro no send\n");
    int result;
    if (recv(client_socket, &result, sizeof(int), 0) < 0)
        printf("erro no recv\n");
    return result;
}
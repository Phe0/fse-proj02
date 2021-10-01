#include "tcp_client.h"

int client_socket;

void send_file(FILE* fp) {
    char buffer[SIZE] = {0};
    int n;

    while((n = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        send(client_socket, buffer, n, 0);
        printf("%s", buffer);
        memset((char *)&buffer, 0, sizeof(buffer));
    }

    char end[3];
    end[0] = '#';
    end[1] = '@';
    end[2] = 0;

    send(client_socket, end, 3, 0);
}

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
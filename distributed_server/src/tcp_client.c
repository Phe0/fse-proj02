#include "tcp_client.h"

int client_socket;

void send_file(FILE* fp) {
    char buffer[SIZE] = {0};
    int n;

    while((n = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        send(client_socket, buffer, n, 0);
        memset((char *)&buffer, 0, sizeof(buffer));
    }

    char end[3];
    end[0] = '#';
    end[1] = '@';
    end[2] = 0;

    send(client_socket, end, 3, 0);
}

void send_float(int type, float value) {
    send(client_socket, &type, sizeof(type), 0);
    send(client_socket, &value, sizeof(value), 0);
}

void send_int(int type, int value) {
    send(client_socket, &type, sizeof(type), 0);
    send(client_socket, &value, sizeof(value), 0);
}

void send_event(int type) {
    send(client_socket, &type, sizeof(type), 0);
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

void* receive_orders() {
    printf("iniciando\n");
    while(1) {
        int type;
        recv(client_socket, &type, sizeof(type), 0);
        printf("type %d\n", type);
        if (type == 1) {
            int gpio;
            recv(client_socket, &gpio, sizeof(gpio), 0);
            printf("gpio %d\n", gpio);
            int state = read_gpio(gpio);
            printf("state %d\n", state);
            send(client_socket, &state, sizeof(state), 0);
        } else if (type == 2) {
            int gpio;
            recv(client_socket, &gpio, sizeof(gpio), 0);
            printf("gpio %d\n", gpio);
            int result = invert_gpio(gpio);
            printf("result %d\n", result);
            int id = 7;
            send(client_socket, &id, sizeof(id), 0);
            send(client_socket, &result, sizeof(result), 0);
        }
    }
}

void close_client() {
    close(client_socket);
}
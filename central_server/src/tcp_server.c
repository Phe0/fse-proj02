#include "tcp_server.h"

int server_socket;
struct sockaddr_in servaddr;
struct connection conn_list[1000];
int conn_list_size = 0;

struct connection* get_conn_list() {
    return conn_list;
}

int get_conn_list_size() {
    return conn_list_size;
}

FILE* receive_file(int socket) {
    FILE* fp;
    char *filename = "config.json";
    char buffer[SIZE] = {};
    int n;

    fp = fopen(filename, "w+");
    int should_exit = 0;
    while((n = recv(socket, buffer, sizeof(buffer), 0)) > 0 || !should_exit) {
        if (buffer[n-3] == '#' && buffer[n-2] == '@') {
            should_exit = 1;
            buffer[n-3] = 0;
        }
        fputs(buffer, fp);
        memset((char *)&buffer, 0, sizeof(buffer));
    }
    return fp;
}

void* server(void* arg) {
    int port = *((int*)arg);
    printf("Tentando abrir server na porta %d\n", port);
    check(init_server(port), "Server could not be opened");
    printf("Servidor aberto\n");
    while(1) {
        struct sockaddr_in cliaddr;
        socklen_t cli_len = sizeof((struct sockaddr *) &cliaddr);
        int client_socket = accept(server_socket, (struct sockaddr*)&cliaddr, &cli_len);
        if (client_socket >= 0) {
            printf("Conexão estabelecida\n");
            FILE* fp = receive_file(client_socket);
            struct configuration config = parse_json(fp); 
            struct connection conn;
            conn.is_valid = 1;
            conn.socket = client_socket;
            conn.config = config;
            conn_list[conn_list_size] = conn;
            conn_list_size++;
            printf("3\n");
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

    if (listen(server_socket, 10) < 0) {
        return 0;
    }
    return 1;
}

void close_sockets() {
    close(server_socket);
}

void close_all_clients() {
    for (int i = 0; i < conn_list_size; i++) {
        close(conn_list[i].socket);
    }
}
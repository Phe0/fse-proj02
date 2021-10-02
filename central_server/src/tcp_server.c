#include "tcp_server.h"

int server_socket;
struct sockaddr_in servaddr;
struct connection conn_list[MAX_CONNECTIONS];
pthread_t threads_in[MAX_CONNECTIONS];
int conn_list_size = 0;

struct connection* get_conn_list() {
    return conn_list;
}

int get_conn_list_size() {
    return conn_list_size;
}

void remove_connection(int index) {
    close(conn_list[index].socket);

    for (int i = index; i< conn_list_size; i++) {
        conn_list[i] = conn_list[i+1];
    }
    conn_list_size--;
}

void remove_thread(int index) {
    pthread_cancel(threads_in[index]);
    pthread_join(threads_in[index], NULL);

    for (int i = index; i < conn_list_size+1; i++) {
        threads_in[i] = threads_in[i+1];
    }
    conn_list_size--;
}

FILE* receive_file(int socket) {
    FILE* fp;
    char *filename = "config.json";
    char buffer[SIZE] = {};
    int n;

    fp = fopen(filename, "w+");
    int should_exit = 0;
    while((n = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
        if (buffer[n-3] == '#' && buffer[n-2] == '@') {
            should_exit = 1;
            buffer[n-3] = 0;
        }
        fputs(buffer, fp);
        memset((char *)&buffer, 0, sizeof(buffer));
        if (should_exit) break;
    }
    return fp;
}

int compare_connection(struct connection a, struct connection b) {
    int same_socket = a.socket == b.socket;
    int same_ip = strcmp(inet_ntoa(a.address.sin_addr), inet_ntoa(b.address.sin_addr));
    int same_port = ntohs(a.address.sin_port) == ntohs(b.address.sin_port);
    return same_socket && (same_ip == 0) && same_port;
}

void* listen_dist_server(struct connection* conn) {
    while(1) {
        int type;
        int n;
        n = recv(conn->socket, &type, sizeof(type), 0);
        if (type == 1) {
            float value;
            n = recv(conn->socket, &value, sizeof(value), 0);
            printf("Temperatura: %f\n", value);
        } else if (type == 2) {
            float value;
            n = recv(conn->socket, &value, sizeof(value), 0);
            printf("Umidade: %f\n", value);
        } else if (type == 3) {
            int value;
            n = recv(conn->socket, &value, sizeof(value), 0);
            printf("Pessoas: %d\n", value);
        }
        if (n == 0) {
            int index;
            for (int i = 0; i < conn_list_size; i++) {
                int is_equal = compare_connection(*conn, conn_list[i]);
                if (is_equal) {
                    index = i;
                    break;
                }
            }
            remove_connection(index);
            remove_thread(index);
        }
    }
}

void server(void* arg) {
    int port = *((int*)arg);
    printf("Tentando abrir server na porta %d\n", port);
    check(init_server(port), "Server could not be opened");
    printf("Servidor aberto\n");
    while(1) {
        struct sockaddr_in cliaddr;
        socklen_t cli_len = sizeof((struct sockaddr *) &cliaddr);
        int client_socket = accept(server_socket, (struct sockaddr*)&cliaddr, &cli_len);
        
        if (client_socket < 0)
            continue;

        printf("Conexão estabelecida\n");
        FILE* fp = receive_file(client_socket);
        struct configuration config = parse_json(fp); 
        struct connection conn;
        print_config(config);
        conn.address = cliaddr;
        conn.socket = client_socket;
        conn.config = config;

        pthread_create(&threads_in[conn_list_size], NULL, (void*)listen_dist_server, (void*)&conn);

        conn_list[conn_list_size++] = conn;

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

void close_all_sockets() {
    for (int i = 0; i < conn_list_size; i++) {
        pthread_cancel(threads_in[i]);
        pthread_join(threads_in[i], NULL);
        close(conn_list[i].socket);
    }
    close(server_socket);
}
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

int check_type(char* type) {
    for (int i = 0; i < conn_list_size; i++) {
        for (int j = 0; j < conn_list[i].config.outputs_length; j++) {
            if (strcmp(conn_list[i].config.outputs[j].type, type) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

void turn_all_devices_by_type(char* type, int desired_state) {
    for (int i = 0; i < conn_list_size; i++) {
        for (int j = 0; j < conn_list[i].config.outputs_length; j++) {
            if (strcmp(conn_list[i].config.outputs[j].type, type) == 0) {
                int state = request_state(conn_list[i].config.outputs[j].gpio, conn_list[i].socket);

                if (state != desired_state) {
                    make_order(conn_list[i].config.outputs[j].gpio);
                }
            }
        }
    }
}

void remove_connection(int index) {
    log_file("Removing connection from connections list");
    close(conn_list[index].socket);

    for (int i = index; i< conn_list_size; i++) {
        conn_list[i] = conn_list[i+1];
    }
    conn_list_size--;
}

void remove_thread(int index) {
    log_file("Closing thread");
    pthread_cancel(threads_in[index]);
    pthread_join(threads_in[index], NULL);

    for (int i = index; i < conn_list_size+1; i++) {
        threads_in[i] = threads_in[i+1];
    }
    conn_list_size--;
}

FILE* receive_file(int socket) {
    log_file("Receiving config file from distributed server");
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
    log_file("Comparing connections");
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
            log_file("Received temperature from distributed server");
            set_temperature(value);
        } else if (type == 2) {
            float value;
            n = recv(conn->socket, &value, sizeof(value), 0);
            log_file("Received humidity from distributed server");
            set_humidity(value);
        } else if (type == 3) {
            int value;
            n = recv(conn->socket, &value, sizeof(value), 0);
            log_file("Presence detected");
            if (get_presece_alarm()) {
                log_file("Turning alarm on");
                play_alarm();
            }
        } else if (type == 4) {
            int value;
            n = recv(conn->socket, &value, sizeof(value), 0);
            if (get_smoke_alarm()) {
                log_file("Turning alarm on");
                play_alarm();
                int aspersor = find_aspersor();
                if (aspersor) {
                    make_order(aspersor);
                }
            }
        } else if (type == 5) {
            int value;
            n = recv(conn->socket, &value, sizeof(value), 0);
            log_file("Person entered");
            person_enter();
        } else if (type == 6) {
            int value;
            n = recv(conn->socket, &value, sizeof(value), 0);
            log_file("Person leaved");
            person_leaves();
        }
        if (n == 0) {
            log_file("Connection has been closed");
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

int find_aspersor() {
    for (int i = 0; i < conn_list_size; i++) {
        for (int j = 0; j < conn_list[i].config.outputs_length; j++) {
            if (strcmp(conn_list[i].config.outputs[j].type, "aspersor") == 0) {
                return conn_list[i].config.outputs[j].gpio;
            }
        }
    }
    return 0;
}

int request_state(int gpio, int socket) {
    log_file("Requesting state from distributed server");
    int request = 1;
    send(socket, &request, sizeof(request), 0);
    send(socket, &gpio, sizeof(gpio), 0);
    int state;
    recv(socket, &state, sizeof(state), 0);
    return state;
}

int find_connection_by_output(int gpio) {
    for (int i = 0; i < conn_list_size; i++) {
        for (int j = 0; i < conn_list[i].config.outputs_length; j++) {
            if (conn_list[i].config.outputs[j].gpio == gpio) {
                return i;
            }
        }
    }
    return -1;
}

int find_output_by_gpio(struct configuration config, int gpio) {
    for (int i = 0; i < config.outputs_length; i++) {
        if (config.outputs[i].gpio == gpio)
            return i;
    }
    return -1;
}

void make_order(int gpio) {
    log_file("Ordering state change from distributed server");
    int conn_index = find_connection_by_output(gpio);
    if (conn_index < 0) {
        //log_file_to_win("Erro ao realizar ordem\n");
        return;
    }

    int output_index = find_output_by_gpio(conn_list[conn_index].config, gpio);

    int request = 2;
    send(conn_list[conn_index].socket, &request, sizeof(request), 0);
    send(conn_list[conn_index].socket, &gpio, sizeof(gpio), 0);
    int type, value;
    recv(conn_list[conn_index].socket, &type, sizeof(type), 0);
    recv(conn_list[conn_index].socket, &value, sizeof(value), 0);

    if (value) {
        conn_list[conn_index].config.outputs[output_index].state = !conn_list[conn_index].config.outputs[output_index].state;
        log_file("Success on state change");
    } else {
        log_file("Fail on state change");
    }
}

void* server(void* arg) {
    int port = *((int*)arg);
    log_file("Trying to open server");
    check(init_server(port), "Server could not be opened");
    log_file("Server opened");

    while(1) {
        struct sockaddr_in cliaddr;
        socklen_t cli_len = sizeof((struct sockaddr *) &cliaddr);
        int client_socket = accept(server_socket, (struct sockaddr*)&cliaddr, &cli_len);
        
        if (client_socket < 0)
            continue;

        // printf("Conexão estabelecida\n");
        FILE* fp = receive_file(client_socket);
        struct configuration config = parse_json(fp); 
        struct connection conn;

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
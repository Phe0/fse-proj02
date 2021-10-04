#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "error.h"
#include "json_parser.h"

#define SIZE 256
#define MAX_CONNECTIONS 100

struct connection {
    int socket;
    struct sockaddr_in address;
    struct configuration config;
};

struct content {
    int type;
    float value;
};

struct connection* get_conn_list();
int get_conn_list_size();
FILE* receive_file(int socket);
int receive_order();
void* order_handler();
int init_server(int port);
void close_all_sockets();
void* server(void* arg);
void close_all_clients();
void get_output_states(struct configuration config, int socket);
int request_state(int gpio, int socket);
void make_order(int gpio);

#endif
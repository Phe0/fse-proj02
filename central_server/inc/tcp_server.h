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

#include "error.h"
#include "json_parser.h"

#define SIZE 256

struct connection {
    int is_valid;
    int socket;
    struct configuration config;
};

struct connection* get_conn_list();
int get_conn_list_size();
void receive_file();
int receive_order();
void* order_handler();
int init_server(int port);
void close_sockets();
void* server(void* arg);
void close_all_clients();

#endif
#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "error.h"
#include "gpio.h"

struct order {
    int gpio;
    int status;
};

int receive_order();
void order_handler();
int init_server(int port);
void close_sockets();
void* server(void* arg);

#endif
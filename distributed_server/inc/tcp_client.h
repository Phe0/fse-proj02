#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "error.h"
#include "gpio.h"

#define SIZE 256

struct content {
    int type;
    float value;
};

void send_file(FILE* fp);
void init_client(char* server_ip, int port);
void close_client();
void send_float(int type, float value);
void send_int(int type, int value);
void send_event(int type);
void* receive_orders();

#endif
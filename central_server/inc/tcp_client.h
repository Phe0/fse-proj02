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

void init_client(char* server_ip, int port);
void close_client();
int make_order(int gpio);

#endif
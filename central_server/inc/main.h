#ifndef _MAIN_H_
#define _MAIN_H_

#include <pthread.h>
#include <signal.h>

#include "tcp_client.h"
#include "tcp_server.h"

void handle_interuption();
int main(int argc, char* argv[]);

#endif
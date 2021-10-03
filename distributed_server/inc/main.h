#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "gpio.h"
#include "error.h"
#include "dht22.h"
#include "json_parser.h"
#include "tcp_client.h"

void handle_interuption(int signal);
int main(int argc, char* argv[]);

#endif
#ifndef _MAIN_H_
#define _MAIN_H_

#include <pthread.h>
#include <signal.h>

#include "tcp_server.h"
#include "alarm.h"
#include "menu.h"
#include "logger.h"

void handle_interuption();
int main(int argc, char* argv[]);

#endif
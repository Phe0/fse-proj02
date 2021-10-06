#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <time.h>
#include <stdio.h>

void create_file();
void log_file(char* text);
void close_file();

#endif
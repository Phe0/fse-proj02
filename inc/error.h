#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdlib.h>
#include <stdio.h>

void error(char* message);
void check(int result, char* message);

#endif
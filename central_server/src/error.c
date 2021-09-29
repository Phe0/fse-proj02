#include "error.h"

void error(char* message) {
    perror(message);
    exit(1);
}

void check(int result, char* message) {
    if (result <= 0)
        error(message);
}
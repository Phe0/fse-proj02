#include "logger.h"

FILE* log_fileger;

void create_file() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char filename[28];

    snprintf(filename, 28, "log_files/%d-%d-%d_%d:%d:%d.csv", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    log_fileger = fopen(filename, "w+");

    fprintf(log_fileger, "data;hora;log_file\n");
}

void log_file(char* text) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(log_fileger, "%d/%d/%d;%d:%d:%d;%s\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, text);
}

void close_file() {
    fclose(log_fileger);
}
#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_

#include <stdio.h>
#include <stdlib.h>

#include "cJSON.h"
#include "error.h"

struct device {
    char* type;
    char* tag;
    int gpio;
    int state;
};

struct configuration {
    char* ip;
    int porta_central;
    int porta_distribuida;
    char* nome;
    unsigned int outputs_length;
    struct device* outputs;
    unsigned int inputs_length;
    struct device* inputs;
};

void print_config(struct configuration config);
void read_file(char* filepath);
char* get_string(cJSON *json, char* attribute);
int get_int(cJSON *json, char* attribute);
struct device* get_device_array(cJSON* json, char* attribute, unsigned int* size);
struct configuration parse_json();
void close_json();

#endif
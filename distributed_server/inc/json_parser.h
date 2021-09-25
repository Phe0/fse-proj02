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
};

struct configuration {
    char* ip;
    int porta;
    char* nome;
    struct device* outputs;
    struct device* inputs;
};

unsigned int get_outputs_length();
unsigned int get_inputs_length();
void read_file(char* filepath);
char* get_string(cJSON *json, char* attribute);
int get_int(cJSON *json, char* attribute);
struct device* get_device_array(cJSON* json, char* attribute, unsigned int* size);
struct configuration parse_json();

#endif
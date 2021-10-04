#include "json_parser.h"

void print_config(struct configuration config) {
    printf("IP %s\n", config.ip);
    printf("Nome %s\n", config.nome);
    printf("OUTPUTS\n");

    for (int i = 0; i < config.outputs_length; i++) {
        printf("Type %s\n", config.outputs[i].type);
        printf("Tag %s\n", config.outputs[i].tag);
        printf("GPIO %d\n", config.outputs[i].gpio);
    }

    for (int i = 0; i < config.inputs_length; i++) {
        printf("Type %s\n", config.inputs[i].type);
        printf("Tag %s\n", config.inputs[i].tag);
        printf("GPIO %d\n\n", config.inputs[i].gpio);
    }
    printf("---------------------------\n");
}

char* get_string(cJSON *json, char* attribute) {
    cJSON* content = NULL;
    content = cJSON_GetObjectItemCaseSensitive(json, attribute);
    if (cJSON_IsString(content) && (content->valuestring != NULL)) {
        return content->valuestring;
    }
    error("Erro ao ler string");
    return "";
}

int get_int(cJSON *json, char* attribute) {
    cJSON* content = NULL;
    content = cJSON_GetObjectItemCaseSensitive(json, attribute);
    if (cJSON_IsNumber(content)) {
        return content->valueint;
    }
    error("Erro ao ler inteiro");
    return -1;
}

struct device* get_device_array(cJSON* json, char* attribute, unsigned int* size) {
    cJSON* devices = NULL;
    cJSON* device = NULL;
    int counter = 0;

    struct device* device_array;

    devices = cJSON_GetObjectItemCaseSensitive(json, attribute);

    if(cJSON_IsArray(devices)) {
        *size = cJSON_GetArraySize(devices);
        device_array = malloc(*size * sizeof(struct device));

        cJSON_ArrayForEach(device, devices) {
            struct device dev;

            dev.type = get_string(device, "type");
            dev.tag = get_string(device, "tag");
            dev.gpio = get_int(device, "gpio");
            
            device_array[counter] = dev;
            counter++;
        }
    }  else {
        cJSON_Delete(json);
        error("outputs não é um array");
    }

    return device_array;
}

struct configuration parse_json(FILE* file) {
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(file_size + 1);
    fread(buffer, 1, file_size, file);
    fclose(file);

    buffer[file_size] = 0;

    struct configuration config;
    cJSON* config_json = cJSON_ParseWithLength(buffer, file_size);

    if (config_json == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Erro no JSON: %s\n", error_ptr);
        }
        cJSON_Delete(config_json);
        error("Erro ao ler JSON");
    }

    config.ip = get_string(config_json, "ip");
    config.porta_central = get_int(config_json, "porta_central");
    config.porta_distribuida = get_int(config_json, "porta_distribuida");
    config.nome = get_string(config_json, "nome");

    config.outputs = get_device_array(config_json, "outputs", &config.outputs_length);
    config.inputs = get_device_array(config_json, "inputs", &config.inputs_length);

    return config;
}
#include "json_parser.h"

long file_size;
char* buffer;

void read_file(char* filepath) {
    FILE* file = fopen(filepath, "r");

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = malloc(file_size + 1);
    fread(buffer, 1, file_size, file);
    fclose(file);

    buffer[file_size] = 0;
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

struct configuration parse_json() {
    struct configuration config;
    cJSON *json = cJSON_ParseWithLength(buffer, file_size);

    if (json == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Erro no JSON: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        error("Erro ao ler JSON");
    }

    config.ip = get_string(json, "ip");
    config.porta = get_int(json, "porta");
    config.nome = get_string(json, "nome");

    config.outputs = get_device_array(json, "outputs", &config.outputs_length);
    config.inputs = get_device_array(json, "inputs", &config.inputs_length);

    cJSON_Delete(json);

    return config;
}
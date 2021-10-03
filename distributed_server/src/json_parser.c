#include "json_parser.h"

long file_size;
char* buffer;
cJSON* config_json;

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

int* find_type(struct device* devices, int devices_length, char* type, int* ammount) {
    int* found = malloc(devices_length * sizeof(int));
    int counter = 0;
    for (int i = 0; i < devices_length; i++) {
        if (strcmp(devices[i].type, type) == 0) {
            found[counter] = i;
            counter++;
        }
    }
    int* found_indexes = malloc(counter * sizeof(int));
    *ammount = counter;
    for (int i = 0; i < counter; i++) {
        found_indexes[i] = found[i];
    }
    free(found);
    return found_indexes;
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
    config_json = cJSON_ParseWithLength(buffer, file_size);

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

void close_json() {
    cJSON_Delete(config_json);
}
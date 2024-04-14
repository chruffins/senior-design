#include "../include/utils.h"

char* convert_static_string_to_dynamic(const char* s_string) {
    char* buffer = malloc(sizeof(char)*(strlen(s_string)+1));
    strcpy(buffer, s_string);

    return buffer;
}
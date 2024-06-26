#pragma once

#include<stdlib.h>
#include<string.h>

#ifdef DEBUG
#define DEBUG_PRINTF(fmt, ...) \
            do { fprintf(stderr, fmt, __VA_ARGS__); } while (0)
#else
#define DEBUG_PRINTF(fmt, ...)
#endif

enum ERROR_CODES {
    ERROR_FAILED_TO_INIT_LIB=-1,
};

char* convert_static_string_to_dynamic(const char*);
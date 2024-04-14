#include "../include/script.h"

chrus_script *chrus_script_create(const char *source) {
    chrus_script *new_script = malloc(sizeof(chrus_script));

    new_script->source_name = source;
    new_script->enabled = true;

    return new_script;
}

const char* chrus_script_get_source(chrus_script* restrict this) {
    return this->source_name;
}

void chrus_script_destroy(chrus_script* restrict this) {
    free(this->source_name);
    free(this);
}
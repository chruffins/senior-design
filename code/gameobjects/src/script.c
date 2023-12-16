#include "../include/script.h"

chrus_script *chrus_script_create(const char *source) {
    chrus_script *new_script = malloc(sizeof(chrus_script));

    new_script->source_name = source;
    new_script->enabled = true;

    return new_script;
}
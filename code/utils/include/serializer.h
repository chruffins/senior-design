/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * Scenes are saved and loaded from JSON files. This is an attempt
 * at saving them.
*/
#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#include "../../gameobjects/include/scene.h"
#include "pdjson.h"
#include "vector.h"

enum chrus_serializing_codes {
    CHRUS_PARSER_ERROR=1,
    CHRUS_PARSER_OBJECT_ENDED=2,
    
};

void chrus_serializer_save_scene(chrus_scene*, const char*);

chrus_scene* chrus_serializer_deserialize_scene(const char* filename);
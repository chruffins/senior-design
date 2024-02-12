/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * Scenes are saved and loaded from JSON files. This deals with loading them.
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
    CHRUS_PARSER_WRONG_FIELDNAME=3,
};

chrus_scene* chrus_serializer_deserialize_scene(const char* filename);
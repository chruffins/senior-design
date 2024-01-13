/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * Object that denotes "scripts". Parenting an "enabled" script to 
 * a scene will cause the scene to execute the script at the path stored 
 * in source_name.
*/
#pragma once

#include<stdbool.h>
#include<stdlib.h>

#include<luajit-2.1/lua.h>
#include<luajit-2.1/lauxlib.h>
#include<luajit-2.1/lualib.h>

typedef struct chrus_script_t chrus_script;

// as a security consideration we probably shouldn't give script objects a pointer to their lua vm
// scripts just store where their source is located
struct chrus_script_t {
    const char *source_name;
    bool enabled;
};

chrus_script *chrus_script_create(const char *source);
//void chrus_script_run(chrus_script *this);
//void chrus_script_set_luavm(chrus_script *this, lua_State *vm);

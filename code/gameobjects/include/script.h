#pragma once

#include<stdbool.h>
#include<stdlib.h>

#include<luajit/lua.h>
#include<luajit/lauxlib.h>
#include<luajit/lualib.h>

// as a security consideration we probably shouldn't give script objects a pointer to their lua vm
// scripts just store where their source is located
struct chrus_script_t {
    const char *source_name;
    bool enabled;
} typedef chrus_script;

chrus_script *chrus_script_create(const char *source);
//void chrus_script_run(chrus_script *this);
//void chrus_script_set_luavm(chrus_script *this, lua_State *vm);

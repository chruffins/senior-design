/*
#include<lua5.1/lua.h>

*/
#include "../../luajit/src/lua.h"
#include "../../luajit/src/lauxlib.h"
#include "../../luajit/src/lualib.h"

#include<allegro5/allegro.h>

#include<stdio.h>

int main(int argc, char **argv) {
    // init allegro
    al_init();

    // init lua state
    lua_State *L = luaL_newstate();

    // load lua libs
    luaL_openlibs(L);

    if (luaL_loadfile(L, "allegrotest.lua") || lua_pcall(L, 0, 0, 0)) {
        printf("This thing failed to run: %s\n", lua_tostring(L, -1));
    };

    lua_close(L);

    return 0; 
}
/*
#include<lua5.1/lua.h>

*/
#include<lua5.1/lualib.h>
#include<lua5.1/lauxlib.h>
#include "../../luajit/src/luajit.h"
#include<stdio.h>

int main(int argc, char **argv) {
    // init lua state
    lua_State *L = luaL_newstate();

    // load lua libs
    luaL_openlibs(L);

    if (luaL_loadfile(L, "helloworld.lua") || lua_pcall(L, 0, 0, 0)) {
        printf("This thing failed to run: %s\n", lua_tostring(L, -1));
    };

    lua_close(L);

    return 0; 
}
#include <cstdio>
#include <dlfcn.h>
#include <stdlib.h>

#include "loadfunctions.h"

DECL_FUNCTION(T_loadstring, luaL_loadstring);
#if LUA_VERSION_NUM == 501
DECL_FUNCTION(T_loadfile, luaL_loadfile);
#else
DECL_FUNCTION(T_loadfile, luaL_loadfilex);
#endif

void load_from(const char *name) {
    void *libhandle = dlopen(name, RTLD_LAZY);
    if (!libhandle) {
        fprintf(stderr, "cannot open library %s: %s\n", name, dlerror());
        exit(1);
    }

    SET_FUNCTION(T_loadstring, dlsym(libhandle, "luaL_loadstring"));
#if LUA_VERSION_NUM == 501
    SET_FUNCTION(T_loadfile, dlsym(libhandle, "luaL_loadfile"));
#else
    SET_FUNCTION(T_loadfile, dlsym(libhandle, "luaL_loadfilex"));
#endif
}
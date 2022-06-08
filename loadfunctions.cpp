#include <cstdio>
#include <dlfcn.h>
#include <stdlib.h>

#include "loadfunctions.h"

DECL_FUNCTION(luaL_loadstring);
#if LUA_VERSION_NUM == 501
DECL_FUNCTION(luaL_loadfile);
#else
DECL_FUNCTION(luaL_loadfilex);
#endif

void load_from(const char *name) {
    void *libhandle = dlopen(name, RTLD_LAZY);
    if (!libhandle) {
        fprintf(stderr, "cannot open library %s: %s\n", name, dlerror());
        exit(1);
    }
    LOAD_FUNCTION(luaL_loadstring);
#if LUA_VERSION_NUM == 501
    LOAD_FUNCTION(luaL_loadfile);
#else
    LOAD_FUNCTION(luaL_loadfilex);
#endif
}
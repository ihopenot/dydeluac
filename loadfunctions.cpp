#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>

#include "loadfunctions.h"

DECL_FUNCTION(T_loadstring, luaL_loadstring);
#if LUA_VERSION_NUM == 501
DECL_FUNCTION(T_loadfile, luaL_loadfile);
#else
DECL_FUNCTION(T_loadfile, luaL_loadfilex);
#endif

void *get_module_base(const char *module) {
    char filename[128];
    char tmp[1024];
    void *ret = 0;
    sprintf(filename, "/proc/self/maps");
    FILE *f = fopen(filename, "r");
    while (fgets(tmp, 1000, f)) {
        if (strstr(tmp, module)) {
            ret = (void *)strtoul(tmp, NULL, 16);
            break;
        }
    }
    fclose(f);
    return ret;
}

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

    /* get functions manually

    // get base address of libhandle
    const char *mname = name + strlen(name);
    for (int i = strlen(name); i; i--) {
        if (*mname == '/') {
            mname++;
            break;
        }
        mname--;
    }
    void *base = get_module_base(mname);

    printf("%p\n", base);

    SET_FUNCTION(T_loadstring, (uint8_t *)base + 0x1491C);
    printf("%p\n", T_loadstring);
    SET_FUNCTION(T_loadfile, (uint8_t *)base + 0x14458);

    */
}
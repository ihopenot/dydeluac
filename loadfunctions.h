extern "C" {
#include <dlfcn.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

extern void *libhandle;
#define DECL_FUNCTION(name) typeof(name) *_##name
#define LOAD_FUNCTION(name) _##name = (typeof(name) *)dlsym(libhandle, #name)

extern DECL_FUNCTION(luaL_loadstring);
#if LUA_VERSION_NUM == 501
extern DECL_FUNCTION(luaL_loadfile);
#else
extern DECL_FUNCTION(luaL_loadfilex);
#endif

void load_from(const char *name);
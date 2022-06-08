extern "C" {
#include <dlfcn.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

extern void *libhandle;
#define DECL_FUNCTION(name, func) decltype(func) *name
#define SET_FUNCTION(name, value) name = (decltype(name))(value)

extern DECL_FUNCTION(T_loadstring, luaL_loadstring);
#if LUA_VERSION_NUM == 501
extern DECL_FUNCTION(T_loadfile, luaL_loadfile);
#else
extern DECL_FUNCTION(T_loadfile, luaL_loadfilex);
#endif

void load_from(const char *name);
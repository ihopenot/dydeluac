#include "loadfunctions.h"

extern "C" {
#include <cerrno>
#include <lauxlib.h>
#include <ldebug.h>
#include <lobject.h>
#include <lopcodes.h>
#include <lua.h>
#include <lualib.h>
#if LUA_VERSION_NUM == 504
#include "lopnames.h"
#endif
#include <lstate.h>
#include <lundump.h>
}

#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// hack
#ifdef __ANDROID
typeof(__errno) *__errno_location = __errno;
#endif

#if LUA_VERSION_NUM >= 504
#define toproto(L, i) getproto(s2v(L->top + (i)))
#elif LUA_VERSION_NUM >= 502
#define toproto(L, i) getproto(L->top + (i))
#else
#define toproto(L, i) (clvalue(L->top + (i))->l.p)
#define LUA_OK 0
#endif

// FIXME: fix test
static const char *progname = "dydeluac";

static void fatal(const char *message) {
    fprintf(stderr, "%s: %s\n", progname, message);
    exit(1);
}

static void cannot(const char *what) {
    fprintf(stderr, "%s: cannot %s %s\n", progname, what, strerror(errno));
    exit(1);
}

static int writer(lua_State *L, const void *p, size_t size, void *u) {
    UNUSED(L);
    return (fwrite(p, size, 1, (FILE *)u) != 1) && (size != 0);
}

int opmap[NUM_OPCODES + 1];
#if LUA_VERSION_NUM >= 504
#define OPTABLE opnames
#else
#define OPTABLE luaP_opnames
#endif

void compare_proto(const Proto *f, const Proto *_f, int *map) {
    assert(f->sizep == _f->sizep);
    for (int i = 0; i < f->sizep; i++) {
        compare_proto(f->p[i], _f->p[i], map);
    }

    assert(f->sizecode == _f->sizecode);
    for (int op, _op, i = 0; i < f->sizecode; i++) {
        op = GET_OPCODE(f->code[i]);
        _op = GET_OPCODE(_f->code[i]);
        if (map[_op] != -1)
            assert(map[_op] == op);
        else
            map[_op] = op;
    }
}

#include "randop.h"

int get_opmap() {
    lua_State *L = luaL_newstate();
    if (luaL_loadstring(L, allops) != LUA_OK) {
        printf("%s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }
    const Proto *f = toproto(L, -1);

    lua_State *_L = luaL_newstate();
    if (T_loadstring(_L, allops) != LUA_OK) {
        printf("%s\n", lua_tostring(_L, -1));
        lua_close(_L);
        return 1;
    }
    const Proto *_f = toproto(_L, -1);

    memset(opmap, -1, sizeof(opmap));
    compare_proto(f, _f, opmap);

    int recheck[NUM_OPCODES], unk_cnt = 0, unk_idx = -1;
    memset(recheck, 0, sizeof(recheck));
    for (int i = 0; i < NUM_OPCODES; i++) {
        if (opmap[i] != -1) {
            assert(recheck[opmap[i]] == 0);
            recheck[opmap[i]] = 1;
        } else {
            unk_cnt += 1;
            unk_idx = i;
        }
    }
    if (unk_cnt == 1) {
        for (int i = 0; i < NUM_OPCODES; i++) {
            if (recheck[i] == 0) {
                opmap[unk_idx] = i;
                break;
            }
        }
    }

#ifdef DEBUG
    printf("-----------------opmap-----------------\n");
    for (int i = 0; i < NUM_OPCODES; i++) {
        printf("%d: %d(%s <- %s)\n", i, opmap[i],
               opmap[i] == -1 ? "UNKNOWN" : OPTABLE[opmap[i]], OPTABLE[i]);
    }
    printf("----------------------------------------\n");
#endif
    return 0;
}

void repair_code(const Proto *f, int *map) {
    for (int i = 0; i < f->sizep; i++) {
        repair_code(f->p[i], map);
    }

    for (int i = 0; i < f->sizecode; i++) {
        int op = GET_OPCODE(f->code[i]);
        if (map[op] != -1)
            SET_OPCODE(f->code[i], map[op]);
        else {
            fatal("unknown opcode");
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("usage: %s <input.luac> <output.luac> <liblua>\n", argv[0]);
        return 0;
    }

    progname = argv[0];
    const char *filename = argv[1];
    const char *output = argv[2];
    const char *libname = argv[3];

    load_from(libname);

    lua_State *L = luaL_newstate();
#if LUA_VERSION_NUM >= 502
    if (T_loadfile(L, filename, NULL) != LUA_OK)
#else
    if (T_loadfile(L, filename) != LUA_OK)
#endif
    {
        printf("%s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    const Proto *f = toproto(L, -1);

    get_opmap();

    repair_code(f, opmap);

    FILE *D = fopen(output, "wb");
    if (D == NULL)
        cannot("open");

    lua_lock(L);
#if LUA_VERSION_NUM >= 503
    lua_dump(L, writer, D, 0);
#else
    lua_dump(L, writer, D);
#endif
    lua_unlock(L);

    if (ferror(D))
        cannot("write");
    if (fclose(D))
        cannot("close");

    return 0;
}

const char allops[] =
    "local a, b\n"

    "a = b -- OP_MOVE\n"
    "a = 1 -- OP_LOADI\n"
    "a = 1.0 -- OP_LOADF\n"
    "a = 123456 -- OP_LOADK\n"

    "-- TODO: OP_LOADKX\n"

    "a = false -- OP_LOADFALSE\n"
    "a = 1 > 2 -- OP_LFALSESKIP OP_LTI OP_JMP\n"
    "a = true --OP_LOADTRUE\n"
    "a = nil -- OP_LOADNIL\n"

    "function Upv() a = a end -- OP_GETUPVAL OP_SETUPVAL OP_CLOSURE\n"

    "local c = _G -- OP_GETTABUP OP_SETTABUP\n"

    "-- OP_EXTRAARG # only for 5.4\n"
    "c = {0} -- OP_NEWTABLE OP_SETLIST\n"

    "b = c[a] -- OP_GETTABLE\n"
    "b = c[1] -- OP_GETI\n"
    "b = c[\"\"] -- OP_GETFIELD\n"
    "c[a] = b -- OP_SETTABLE\n"
    "c[1] = b -- OP_SETI\n"
    "c[\"\"] = b -- OP_SETFIELD\n"

    "b:Upv() -- OP_SELF OP_CALL\n"

    "b = a + 1 -- OP_ADDI OP_MMBINI\n"
    "b = a + 123456 -- OP_ADDK OP_MMBINK\n"
    "b = b + b -- OP_ADD OP_MMBIN\n"

    "b = b - 123456 -- OP_SUBK\n"
    "b = b - b -- OP_SUB\n"

    "b = b * 1 -- OP_MULK\n"
    "b = b % 1 -- OP_MODK\n"
    "b = b ^ 1 -- OP_POWK\n"
    "b = b / 1 -- OP_DIVK\n"
    "b = b * b -- OP_MUL\n"
    "b = b % b -- OP_MOD\n"
    "b = b ^ b -- OP_POW\n"
    "b = b / b -- OP_DIV\n"
#if LUA_VERSION_NUM >= 503
    "b = b // 1 -- OP_IDIVK\n"
    "b = b & 1 -- OP_BANDK\n"
    "b = b | 1 -- OP_BORK\n"
    "b = b ~ 1 -- OP_BXORK\n"
    "b = b >> 1 -- OP_SHRI\n"
    "b = 1 << b -- OP_SHLI\n"
    "b = b // b -- OP_IDIV\n"
    "b = b & b -- OP_BAND\n"
    "b = b | b -- OP_BOR\n"
    "b = b ~ b -- OP_BXOR\n"
    "b = b >> b -- OP_SHR\n"
    "b = b << b -- OP_SHL\n"
    "b = ~b -- OP_BNOT\n"
#endif
    "b = -b -- OP_UNM\n"
    "b = not b -- OP_NOT\n"
    "b = #b -- OP_LEN\n"
    "b = b .. b -- OP_CONCAT\n"

    "for i, v in {1, 2, 3} do break end -- OP_TFORPREP OP_TFORCALL OP_TFORLOOP "
    "OP_CLOSE\n"

    "for i = 1, 10 do end -- OP_FORLOOP OP_FORPREP\n"

#if LUA_VERSION_NUM >= 504
    "local d <close> -- OP_TBC\n"
#endif
    "b = a == b -- OP_EQ\n"
    "b = a < b -- OP_LT\n"
    "b = a <= b -- OP_LE\n"

    "b = a == 1 -- OP_EQI\n"
    "b = a == 123456 -- OP_EQK\n"
    "b = a <= 1 -- OP_LEI\n"
    "b = a > 1 -- OP_GTI\n"
    "b = a >= 1 -- OP_GEI\n"
    "b = a and b -- OP_TESTSET\n"
    "b = a and b or c -- OP_TEST\n"

    "function T() return T() end -- OP_TAILCALL\n"

    "function RV(...) return ... end -- OP_VARARG OP_RETURN OP_VARARGREP\n"

    "function R0() return 1 end -- OP_RETURN0 OP_RETURN1\n";

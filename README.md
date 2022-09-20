# Dydeluc

通过调用魔改的lua.so解密加密后luac为标准luac。

目前支持恢复字节码顺序打乱，load前加密等。

## How to compile

使用 [xmake](https://github.com/xmake-io/xmake) 方便在不同架构系统上编译。  

首先确认魔改后的lua二进制文件使用了那个版本，假设是5.3.6

```
LUAVER=5.3.6 xmake f -c
LUAVER=5.3.6 xmake b
```

编译产物在build文件夹下对应架构文件夹内。

## How to use
```
usage: ./dydeluac <input.luac> <output.luac> <liblua>

input.luac: 需要解密的luac

output.luac: 解密后luac存放位置

liblua: 魔改的lua二进制文件
```
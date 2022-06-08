set_defaultarchs("linux|x64")

lua_version=os.getenv("LUAVER")

add_requires("lua " .. lua_version)

target("main")
    set_kind("binary")
    add_rules("mode.debug")
    -- set_toolchains("ndk", {plat = "android", arch = "arm64", ndk = "/home/ihopenot/android-ndk-r23b", sdkver="24"})
    add_files("loadfunctions.cpp", "main.cpp")
    add_packages("lua")
target_end()

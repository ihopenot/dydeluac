-- set_defaultarchs("linux|x64")

lua_version=os.getenv("LUAVER")

add_requires("lua " .. lua_version)

-- target("linux-x64")
--     set_kind("binary")
--     add_rules("mode.debug")
--     add_files("loadfunctions.cpp", "main.cpp")
--     add_packages("lua")
-- target_end()


target("dydeluac")
    set_kind("binary")
    add_rules("mode.debug")
    set_arch("linux")
    -- set_arch("armeabi-v7a")
    -- set_toolchains("ndk", {plat = "android", arch = "armeabi-v7a", ndk = "/home/ihopenot/android-ndk-r23b", sdkver="24"})
    add_files("loadfunctions.cpp", "main.cpp")
    add_packages("lua")
target_end()
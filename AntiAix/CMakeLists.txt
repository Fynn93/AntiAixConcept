cmake_minimum_required(VERSION 3.30)
project(AntiAix)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories(.)

add_executable(AntiAix
        src/main.cpp
        src/ProcessUtils.cpp
        src/ProcessUtils.hpp
        external/minhook/buffer.c
        external/minhook/buffer.h
        external/minhook/hook.c
        external/minhook/trampoline.c
        external/minhook/trampoline.h
        external/minhook/hde/hde32.c
        external/minhook/hde/hde32.h
        external/minhook/hde/hde64.c
        external/minhook/hde/hde64.h
        external/minhook/hde/pstdint.h
        external/minhook/hde/table32.h
        external/minhook/hde/table64.h
)

target_link_libraries(AntiAix
        comctl32
)
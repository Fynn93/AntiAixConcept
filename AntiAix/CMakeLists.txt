cmake_minimum_required(VERSION 3.30)
project(AntiAix)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/vcpkg/installed/x64-windows-static/debug;C:/vcpkg/installed/x64-windows-static")

include_directories(.)

find_package(httplib CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES
        CONFIGURE_DEPENDS
        src/*.cpp
        src/*.c
        src/*.hpp
        src/*.h,
        external/*.c
        external/*.h
)

add_executable(AntiAix
        ${SRC_FILES}
)

target_link_libraries(AntiAix PRIVATE httplib::httplib ntdll.dll)
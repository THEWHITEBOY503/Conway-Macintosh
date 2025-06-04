# How to build using CMake as a standalone project:
# mkdir build
# cd build
# cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/Retro68-build/toolchain/m68k-apple-macos/cmake/retro68.toolchain.cmake
# make

##### General Stuff #####
cmake_minimum_required(VERSION 3.22)
project(Conway)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

##### Build Options #####

add_compile_options(
        # Debugging w/ no optimizations
        #        "-g"
        #        "-gdwarf-4"
        #        "-Og"
        # General warnings
        "-Wall"
        "-Wextra"
        # Specific warnings
        "-Wcast-align"
        "-Wcast-qual"
        "-Wswitch-enum"
        "-Wnull-dereference"
        "-Wshadow"
        "-Wpointer-arith"
        # static analyzer
        "-fanalyzer"
        # Helpful options
        "-fno-common"
)

# Optimizations :)
add_compile_options("-Ofast")
add_compile_options("-fimplicit-constexpr")
add_compile_options("-Wdisabled-optimization")

# Enable -ffunction-sections and -gc-sections to make the app as small as possible
add_compile_options("-ffunction-sections")
add_link_options("-Wl,-gc-sections")
if (CMAKE_SYSTEM_NAME MATCHES Retro68)
    # On 68K, also enable --mac-single to build it as a single-segment app (so that this code path doesn't rot)
    add_link_options("-Wl,--mac-single")
endif ()

##### Code #####

include_directories(src)

add_subdirectory(src/Button)
add_subdirectory(src/Field)
add_subdirectory(src/FieldUpdater)
add_subdirectory(src/GridDraw)

add_application(Conway
        src/conway.cpp
        src/conway.h
)

target_link_libraries(Conway
        Button
        FieldUpdater
)

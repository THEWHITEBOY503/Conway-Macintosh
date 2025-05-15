#!/bin/sh

SRC_PATH="${1:?}"
BUILD_PATH="${2:?}"

RETRO68_CMAKE_TOOLCHAIN_PATH="/Retro68-build/toolchain/m68k-apple-macos/cmake/retro68.toolchain.cmake"

cd "$BUILD_PATH" || exit
rm -rf -- ./.*
rm -rf -- ./*

cmake "$SRC_PATH/../" -DCMAKE_TOOLCHAIN_FILE="${RETRO68_CMAKE_TOOLCHAIN_PATH}" &&
	make

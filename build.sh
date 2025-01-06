#!/usr/bin/env sh

mkdir -p build
cmake . -B build -G "Unix Makefiles" -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build

./build/qt-demo

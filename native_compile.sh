#!/bin/bash

touch src/hello.cpp
#cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" -H. -BbuildNative
cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -H. -BbuildNative
cd buildNative
make -j4 all
#make all
cd ..
mkdir -p buildNative/bin/shaders
mkdir -p buildNative/bin/textures
cp shaders/* buildNative/bin/shaders
cp textures/* buildNative/bin/textures

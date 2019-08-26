#!/bin/bash

cp shaders/* buildNative/bin/shaders
cp textures/* buildNative/bin/textures
cd buildNative/bin
./hello
cd ../..

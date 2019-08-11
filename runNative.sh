#!/bin/bash

cp shaders/* buildNative/bin/shaders
cd buildNative/bin
./hello
cd ../..

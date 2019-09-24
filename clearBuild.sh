#!/bin/sh
# make clean, make操作

mkdir -p Debug
cd Debug
rm -rf *
cd ..
./build.sh

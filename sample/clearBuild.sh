#!/bin/sh
# make clean, make操作

mkdir -p Debug
cd Debug
rm -rf *
cmake ..
make -j8
cd -
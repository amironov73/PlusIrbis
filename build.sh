#!/usr/bin/env bash

mkdir build     2> /dev/null
mkdir artifacts 2> /dev/null
cd build || exit

cmake -D CMAKE_BUILD_TYPE=Release .. || exit
cmake --build . || exit
cmake --build . --target install

cd ..

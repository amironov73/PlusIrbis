#! /bin/sh

mkdir build     2> /dev/null
mkdir artifacts 2> /dev/null
cd build || exit

cmake -D CMAKE_BUILD_TYPE=Release ..
cmake --build .
cmake --install .

cd ..

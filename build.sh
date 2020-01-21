#! /bin/sh

cmake -B ./build -D CMAKE_BUILD_TYPE=Release .
cmake --build ./build --config Release 
cmake --install ./build --config Release 


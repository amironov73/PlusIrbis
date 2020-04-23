@echo off

mkdir build     > nul 2> nul
mkdir artifacts > nul 2> nul

SET CMAKE="C:\Program Files\JetBrains\CLion\bin\cmake\win\bin\cmake.exe"

%CMAKE% -G "Visual Studio 16 2019" -T "Intel C++ Compiler 19.1" -B ./build -D CMAKE_BUILD_TYPE=Release .
%CMAKE% --build ./build --config Release -j 4
%CMAKE% --install ./build --config Release 

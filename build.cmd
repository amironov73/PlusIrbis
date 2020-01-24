@echo off

mkdir build     > nul 2> nul
mkdir artifacts > nul 2> nul

SET CMAKE="C:\Program Files\JetBrains\CLion\bin\cmake\win\bin\cmake.exe"

%CMAKE% -B ./build -D CMAKE_BUILD_TYPE=Release .
%CMAKE% --build ./build --config Release -j 4
%CMAKE% --install ./build --config Release 

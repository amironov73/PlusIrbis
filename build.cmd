@echo off

SET CMAKE="C:\Program Files\JetBrains\CLion\bin\cmake\win\bin\cmake.exe"

%CMAKE% -B ./build -D CMAKE_BUILD_TYPE=Release .
%CMAKE% --build ./build --config Release 
%CMAKE% --install ./build --config Release 

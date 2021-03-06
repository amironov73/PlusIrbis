@echo off

mkdir build     > nul 2> nul
mkdir artifacts > nul 2> nul

rem SET MINGW_PATH=C:/Qt/Tools/mingw492_32/bin
SET MINGW_PATH=C:/Qt/Tools/mingw530_32/bin
SET MINGW_PATH2=C:\Qt\Tools\mingw530_32\bin

SET MINGW_C=%MINGW_PATH%/gcc.exe
SET MINGW_CPP=%MINGW_PATH%/g++.exe
SET MINGW_MAKE=%MINGW_PATH%/mingw32-make.exe
SET CMAKE="C:\Program Files\JetBrains\CLion\bin\cmake\win\bin\cmake.exe"
SET PATH=%MINGW_PATH%;%PATH%

%CMAKE% -B ./build -G "MinGW Makefiles" -D CMAKE_C_COMPILER=%MINGW_C% -D CMAKE_CXX_COMPILER=%MINGW_CPP% -D CMAKE_MAKE_PROGRAM=%MINGW_MAKE% -D CMAKE_BUILD_TYPE=Release .
%CMAKE% --build ./build --config Release -j 2
%CMAKE% --install ./build --config Release 

copy "%MINGW_PATH2%\libgcc_s_dw2-1.dll"  artifacts
copy "%MINGW_PATH2%\libstdc++-6.dll"     artifacts
copy "%MINGW_PATH2%\libwinpthread-1.dll" artifacts

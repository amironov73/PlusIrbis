@echo off

mkdir build     > nul 2> nul
mkdir artifacts > nul 2> nul

SET CLANG_PATH=C:/Clang/bin
SET MINGW_PATH=C:/Qt/Tools/mingw530_32/bin

SET CLANG_C=%CLANG_PATH%/clang.exe
SET CLANG_CPP=%CLANG_PATH%/clang++.exe
SET MINGW_MAKE=%MINGW_PATH%/mingw32-make.exe
SET CMAKE="C:\Program Files\JetBrains\CLion\bin\cmake\win\bin\cmake.exe"
SET PATH=%CLANG_PATH%;%PATH%

SET LIB=C:/Qt/Tools/mingw530_32/i686-w64-mingw32/lib

%CMAKE% -B ./build -G "MinGW Makefiles" -D CMAKE_C_COMPILER=%CLANG_C% -D CMAKE_CXX_COMPILER=%CLANG_CPP% -D CMAKE_MAKE_PROGRAM=%MINGW_MAKE% -D CMAKE_LIBRARY_PATH=C:/Qt/Tools/mingw530_32/i686-w64-mingw32/lib -D CMAKE_BUILD_TYPE=Release .
%CMAKE% --build ./build --config Release -j 2
%CMAKE% --install ./build --config Release 

copy "%MINGW_PATH2%\libgcc_s_dw2-1.dll"  artifacts
copy "%MINGW_PATH2%\libstdc++-6.dll"     artifacts
copy "%MINGW_PATH2%\libwinpthread-1.dll" artifacts

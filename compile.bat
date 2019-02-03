@echo off

call :BUILD 64 x64
rem call :BUILD 32 x86

goto :END

:BUILD

echo ======================================================
echo BUILD %1 %2
echo ======================================================

SetLocal

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars%1.bat"

cd Source
msbuild /t:clean /t:build PlusIrbis.sln
cd ..

EndLocal

:END


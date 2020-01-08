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
msbuild /t:clean /t:build /p:Configuration=Debug      PlusIrbis.2017.sln
msbuild /t:clean /t:build /p:Configuration=Release    PlusIrbis.2017.sln
msbuild /t:clean /t:build /p:Configuration=DebugDll   PlusIrbis.2017.sln
msbuild /t:clean /t:build /p:Configuration=ReleaseDll PlusIrbis.2017.sln
cd ..

EndLocal

:END


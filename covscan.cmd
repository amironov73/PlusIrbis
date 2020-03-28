@echo off

rem call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set PATH=D:\Coverity\bin;%PATH%

cov-build --dir cov-int msbuild.exe PlusIrbis.2017.sln /property:Configuration=Release /property:Platform=x64


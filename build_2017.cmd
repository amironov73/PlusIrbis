@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

msbuild PlusIrbis.2017.sln /property:Configuration=Release /property:Platform=x64


@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

msbuild PlusIrbis.2019.sln /property:Configuration=Release /property:Platform=x64


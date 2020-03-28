@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat"

msbuild PlusIrbis.2015.sln /property:Configuration=Release /property:Platform=x64


@echo off

"C:\Program Files\JetBrains\CLion\bin\cmake\win\bin\cmake.exe" -B ./build -D CMAKE_BUILD_TYPE=Release .
"C:\Program Files\JetBrains\CLion\bin\cmake\win\bin\cmake.exe" --build ./build --config Release 
"C:\Program Files\JetBrains\CLion\bin\cmake\win\bin\cmake.exe" --install ./build --config Release 

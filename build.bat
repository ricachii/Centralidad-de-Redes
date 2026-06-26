@echo off
echo Building CentralidadRedes...
if not exist build mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make -j4
cd ..
echo.
echo Build done. Corriendo experimentos...
echo.
build\centralidad.exe

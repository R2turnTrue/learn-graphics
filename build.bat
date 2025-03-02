@echo off
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build ./
cd ..
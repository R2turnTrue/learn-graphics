@echo off
mkdir build
cd build
cmake .. -DCMAKE_C_COMPILER:FILEPATH=gcc.exe -DCMAKE_CXX_COMPILER:FILEPATH=g++.exe -G "Ninja"
ninja
cd ..
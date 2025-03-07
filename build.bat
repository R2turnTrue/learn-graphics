@echo off
mkdir build
cd build
cmake .. -DCMAKE_C_COMPILER:FILEPATH=C:\clang\bin\clang.exe -DCMAKE_CXX_COMPILER:FILEPATH=C:\clang\bin\clang++.exe -G "Ninja"
ninja
cd ..
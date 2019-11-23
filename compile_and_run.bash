#!/bin/bash

function log_and_exit () 
{
    printf "Build failed.\n"
    exit 1
}

mkdir -p build
cd build
cmake .. || log_and_exit
cmake --build . || log_and_exit
echo ""
if [ -e "OKGen" ]; then
    ./OKGen
elif [ -e "OKGen.exe" ]; then
    ./OKGen.exe
fi
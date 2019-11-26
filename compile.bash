#!/bin/bash

function log_and_exit () 
{
    printf "Build failed.\n"
    exit 1
}

mkdir -p build
cd build
cmake .. || log_and_exit
if [ "$1" == "--Release" ] || [ "$2" == "--Release" ]; then
    cmake --build . --config Release || log_and_exit
else
    cmake --build . --config Release || log_and_exit
fi

echo ""
if [ "$1" == "--run" ] || [ "$2" == "--run" ]; then
    if [ -e "OKGen" ]; then
        ./OKGen
    elif [ -e "OKGen.exe" ]; then
        ./OKGen.exe
    fi
fi
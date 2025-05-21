#!/bin/bash

if [ $# -ne 1 ]; then
    echo "c , relase-compile"
    echo "dc , debug-compile"
    echo "r , run"

elif [ "$1" == "release-compile" ] || [ "$1" == "c" ]; then
    clear
    rm -rf build
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make

elif [ "$1" == "debug-compile" ] || [ "$1" == "dc" ]; then 
    clear
    rm -rf build
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make

elif [ "$1" == "run" ] || [ "$1" == "r" ]; then 
    date
    ./build/OCR_PROGRAM

fi
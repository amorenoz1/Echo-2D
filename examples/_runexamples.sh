#!/bin/bash

if [ "$#" -lt 2 ]; then
   echo "usage:  ./_runexamples.sh <compile | run> <example directory name>"
fi

path_to_lib="../../target/lib"
path_to_include="../../target/include"

cd ./$2
ls

if [ "$1" == "compile" ]; then
   cc="clang++ $2.cpp -I/usr/local/opt/freetype2/include/freetype2 -std=c++11 -fsanitize=address -g -Wall -Wextra -L$path_to_lib -lEcho2D -I$path_to_include -o $2"
   eval "$cc"

elif [ "$1" == "run" ]; then
   export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:"$path_to_lib"
   run_cmd="./$2" 
   eval "$run_cmd"
fi





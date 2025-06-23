#!/bin/bash

# Compile the C++ source file
g++ -o src/see_map src/see_map.cpp -Wall -g

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo -e "Compilation finished successfully.\n"
    ./src/see_map
else
    echo -e "Compilation failed.\n"
fi

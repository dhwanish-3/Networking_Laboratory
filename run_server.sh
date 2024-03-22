#!/bin/bash

# Find the file that was modified last
file=$(find . -name "*.c" -type f -printf '%T@ %p\n' | sort -n | tail -1 | cut -f2- -d" ")

# Get the directory and filename without extension
dir=$(dirname "$file")
filename=$(basename "$file" | cut -f 1 -d '.')

# change directory
echo "changing directory to $dir"
cd "$dir"

# compiling the files using 
make

# run the server
echo "running server $filename"

portnumber=$1

# if portnumber is given as argument then run with portnumber
if [ $# -eq 1 ]; then
    "./$filename" "$portnumber"
else
    "./$filename"
fi

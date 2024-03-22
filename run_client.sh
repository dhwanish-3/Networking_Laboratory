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

# getting the ip address
ipaddress=$(hostname -I | cut -f1 -d' ')

# run the client
echo "running client $filename"

portnumber=$1

# if the number of arguments is 1 then 
# run the client with the ip address and port number
if [ $# -eq 1 ]; then
    "./$filename" "$ipaddress" "$portnumber"
else
    "./$filename" "$ipaddress"
fi

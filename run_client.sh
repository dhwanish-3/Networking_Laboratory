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

# removing the characters after _ in filename
base=$(echo $filename | cut -f 1 -d '_')

# appending _client to the base name
client=$base"_client"

# getting the ip address
ipaddress=$(hostname -I | cut -f1 -d' ')

# run the client
echo "running client $client"

"./$client" "$ipaddress"
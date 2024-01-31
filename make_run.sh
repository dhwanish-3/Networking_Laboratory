#!/bin/bash

#* use this script if a makefile is present in the directory

# Find the file that was modified last
file=$(find . -name "*.c" -type f -printf '%T@ %p\n' | sort -n | tail -1 | cut -f2- -d" ")

# Get the directory and filename without extension
dir=$(dirname "$file")
filename=$(basename "$file" | cut -f 1 -d '.')

# compiling the files using 
make

# getting server object name
# removing the characters after _ in filename
base=$(echo $filename | cut -f 1 -d '_')

# appending _server and _client to the base name
server=$base"_server"
client=$base"_client"

# change directory
cd "$dir"
# run the server
echo "running server $server in the background"
./$server &

# getting the ip address
ipaddress=$(hostname -I | cut -f1 -d' ')

# run the client
echo "running client $client"

./$client $ipaddress

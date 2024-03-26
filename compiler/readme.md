## Compile and Run C Program Application
Compiler application which upon recieving a c file from client, compiles it and executes it. The output of the program is sent back to client

**NOTE** : 
- there is a delay of 5 seconds(recv timeout) between recieving the file and executing it
- after client is connected to server the filename has to be entered within 5 seconds or else connection is lost

### How to run
1. Run the server
```bash
cd server
gcc server.c -o server
./server <port_number>
```
2. Run the client
```bash
cd client
gcc client.c -o client
./client <server_ip_address> <port_number>
```

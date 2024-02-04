## TCP Daytime Client Server

- compile both server and client files using
```
gcc -o udp_server udp_server.c -pthread
gcc -o udp_client udp_client.c
```
or use the Makefile
```
make
```

- **run server using**
```
./udp_server
```

- **run client using**
```
./udp_client <IP_ADDRESS>
``` 

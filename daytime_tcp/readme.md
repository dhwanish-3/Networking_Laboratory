## TCP Daytime Client Server

- compile both server and client files using
```
gcc -o daytime_server daytime_server.c
gcc -o daytime_client daytime_client.c
```
or use the Makefile
```
make
```

- **run server using**
```
./daytime_server <PORT_NUMBER>
```

- **run client using**
```
./daytime_client <IP_ADDRESS> <PORT_NUMBER>
``` 

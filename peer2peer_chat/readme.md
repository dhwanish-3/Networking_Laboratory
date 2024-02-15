### Peer to Peer Chat Application

This is a simple peer to peer chat application using TCP. The chat application is implemented using threads. The chat application is implemented using two peers. The peers can send and receive messages from each other.

#### How to run
1. Compile the file
```bash
gcc -o peer1 peer.c -pthread
gcc -o peer2 peer.c -pthread
```

2. Run the peers
```bash
./peer1 <IP Address> <Port Number1> <Port Number2>
./peer2 <IP Address> <Port Number2> <Port Number1>
```

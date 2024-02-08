## FTP concurrent using TCP
This is a simple FTP server and client using TCP

**NOTE** : If the filename entered by the client is found, the server sends the file to the client. If the file is not found, the server sends an error message to the client.

### How to run
1. Run the server
```bash
cd server
./ftp_server
```
2. Run the client
```bash
cd client
./ftp_client <server_ip>
```

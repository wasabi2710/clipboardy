### data transfer over socket
1. establish socket
2. connect to server
3. listen for connection
4. data streaming

### secure data transmission: rsa key pair
Client Key Generation: Each client generates a public-private key pair upon connection.
Public Key Transmission: The client sends its public key to the server.
Server Encryption: The server uses the client's public key to encrypt the data.
Data Transmission: The server sends the encrypted data back to the client.
Client Decryption: The client uses its private key to decrypt the received data.
Real-Time Key Management: The server handles multiple clients by associating each client's public key with their specific socket descriptor, ensuring no key mixing. Public keys are managed in real-time without persistent storage.
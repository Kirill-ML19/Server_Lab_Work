# Cybernetic Implant Server

This is a simple server for managing the states of cybernetic implants. 
The server supports basic commands for updating and retrieving the state of implants by a unique `agent_id`.

## Technologies

1. C++17
2. POSIX sockets
3. Multithreading using std::thread
4. Makefile for building the project

## Project structure

```
├── build/ 
├── include/ 
│ ├── client_session.h
│ ├── implant_manager.h
│ ├── protocol.h
│ ├── server.h
│ └── utils.h
├── src/ 
│ ├── client_session.cpp
│ ├── implant_manager.cpp
│ ├── main.cpp
│ ├── protocol.cpp
│ ├── server.cpp
│ └── utils.cpp
├── Makefile 
└── README.md 
```

## Build and launch

```bash 
1. Clone the repository

git clone https://github.com/Kirill-ML19/Server_Lab_Work
cd Server_Lab_Work

2. Building the project

make

3. Starting the server

make run 
./server

4. Client connection

nc localhost 5555
telnet localhost 5555
```
## Commands for working with the server via the client

1. ```GET_STATE agent_id=``` - obtaining a state.

2. ```UPDATE_STATE agent_id= active= battery=``` - status update.
# Student Management System with TCP Server-Client

## Overview
This project implements a student management system with a client-server architecture using TCP sockets. It allows users to add, modify, and delete student records and their associated courses through a networked interface.

## Features
- **Student Management**: Add, modify, and delete student records.
- **Course Management**: Add, modify, and delete course records linked to students.
- **TCP Client-Server Communication**: The system enables remote access via a TCP client communicating with a TCP server.
- **Data Persistence**: The system writes updated student and course data to a file.

## File Structure
### Source Files:
- `student.c`: Implements functions to manage student records.
- `student.h`: Defines the `Student` structure and associated functions.
- `course.c`: Implements functions to manage course records.
- `course.h`: Defines the `Course` structure and associated functions.
- `parser.c`: Handles parsing client requests and writing data to a file.
- `parser.h`: Defines parsing-related functions and enums.
- `TCP_client.c`: Implements a client program that sends requests to the server.
- `TCP_server.c`: Implements a server that processes client requests and updates student data accordingly.

### Compilation and Execution
#### Compiling the Server and Client:
```bash
gcc -o server TCP_server.c student.c course.c parser.c -lpthread
gcc -o client TCP_client.c parser.c
```

#### Running the Server:
```bash
./server
```

#### Running the Client:
```bash
./client <input_file>
```

## How It Works
1. The **server** waits for client connections.
2. The **client** reads commands from an input file and sends them to the server.
3. The server processes the request and updates student or course records.
4. The updated records are logged and written to a file (`23_09.out`).
5. The server responds with a success message.

## Supported Commands
The client input file should contain structured commands such as:
```
# add student
1001, John Doe, 3.5, 2

# add course
1001, 101, 85
1001, 102, 90

# modify student
1001, 3.8

# delete course
1001, 101

# delete student
1001
```

## Notes
- The server supports **one client connection at a time**.
- Data is stored in a **doubly linked list**.
- `pthread_mutex` ensures thread safety when handling client requests.
- The server logs requests and responses to `output.txt`.

## Author
Shahryar Rza

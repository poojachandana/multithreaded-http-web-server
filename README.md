Multithreaded HTTP Web Server in C++

A multithreaded HTTP web server built from scratch using C++, Socket Programming, POSIX Threads (Pthreads), and Semaphores. The server handles multiple client connections concurrently and serves static web content over HTTP.

Features

* Multithreaded client handling using POSIX Threads
* HTTP/1.1 request parsing
* Static file serving (HTML, CSS, Images)
* MIME type detection
* Thread synchronization using Semaphores
* TCP/IP Socket Programming
* Concurrent client request processing

Tech Stack

* C++
* Socket Programming
* POSIX Threads (Pthreads)
* Semaphores
* TCP/IP
* HTTP Protocol

Project Structure

multithreaded-server/
├── server.cpp
├── server.h
└── public/
    ├── index.html
    └── favicon.ico

Build and Run

Compile

clang++ -std=c++17 -Wall -o server server.cpp -lpthread

Run

./server

Server starts at:

http://localhost:8080

Testing Concurrent Requests

Open another terminal and run:

for i in {1..10}; do curl http://localhost:8080/ & done
wait

This generates multiple simultaneous requests and demonstrates multithreaded processing.

Key Concepts Implemented

* Socket Creation (socket)
* Port Binding (bind)
* Connection Listening (listen)
* Client Acceptance (accept)
* Thread Creation (pthread_create)
* Synchronization (sem_wait, sem_post)
* File Handling (open, read, write)
* HTTP Request Processing

Learning Outcomes

* Operating Systems Concepts
* Computer Networks
* Concurrent Programming
* Thread Synchronization
* Low-Level System Programming
* HTTP Protocol Fundamentals

Author

Pooja Chandana

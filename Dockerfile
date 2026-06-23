FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y g++

WORKDIR /app

COPY . .

RUN g++ -std=c++17 -pthread server.cpp -o server

EXPOSE 8080

CMD ["./server"]

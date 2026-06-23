// server.cpp — macOS compatible
#include <arpa/inet.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "server.h"

using namespace std;

#define BUFFER_SIZE 4096
#define REQUEST_SIZE 1024
int PORT = 8080;
#define MAX_THREADS 20

sem_t semaphore_mutex;
int thread_count = 0;

// Read string from 'src' up to character 'end'
string readUntil(const string& src, char end) {
    string result;
    for (char c : src) {
        if (c == end) break;
        result += c;
    }
    return result;
}

// Find MIME Content-Type for a file extension
string findContentType(const string& ext) {
    int count = sizeof(fileExtension) / sizeof(fileExtension[0]);

    for (int i = 0; i < count; i++) {
        if (fileExtension[i] == ext)
            return ContentType[i];
    }

    printf("Serving .%s as text/html\n", ext.c_str());
    return "Content-Type: text/html\r\n\r\n";
}

// Send file to client
void send_file(int client_fd, const string& path, const string& content_type) {

    string header =
    "HTTP/1.1 200 OK\r\n"
    + content_type +
    "Connection: close\r\n\r\n";

write(client_fd, header.c_str(), header.length());

    string full_path = "public" + path;

    int file_fd = open(full_path.c_str(), O_RDONLY);

    if (file_fd < 0) {
        printf("Cannot open: %s\n", full_path.c_str());

        write(
            client_fd,
            Messages[NOT_FOUND].c_str(),
            Messages[NOT_FOUND].length()
        );

        return;
    }

    char buf[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(file_fd, buf, sizeof(buf))) > 0) {
        write(client_fd, buf, bytes_read);
    }

    close(file_fd);

    printf("Sent: %s\n", full_path.c_str());
}

// Thread handler
void* connection_handler(void* arg) {

    int client_fd = *((int*)arg);
    delete (int*)arg;

    sem_wait(&semaphore_mutex);

    thread_count++;

    printf("Active threads: %d\n", thread_count);

    if (thread_count > MAX_THREADS) {

        write(
            client_fd,
            Messages[BAD_REQUEST].c_str(),
            Messages[BAD_REQUEST].length()
        );

        thread_count--;

        close(client_fd);

        sem_post(&semaphore_mutex);

        pthread_exit(NULL);
    }

    sem_post(&semaphore_mutex);

    char request_buf[REQUEST_SIZE] = {0};

    int bytes = read(client_fd, request_buf, REQUEST_SIZE - 1);

printf("Bytes received: %d\n", bytes);

if (bytes > 0) {
    request_buf[bytes] = '\0';
    printf("REQUEST:\n%s\n", request_buf);
}

    if (bytes <= 0) {

        printf("Client disconnected or read error.\n");

    } else {

        string request(request_buf);

        string method = readUntil(request, ' ');

        string rest = request.substr(method.length() + 1);

        string path = readUntil(rest, ' ');

        string path_copy = path;

        size_t dot_pos = path_copy.find('.');

        string ext =
            (dot_pos != string::npos)
            ? path_copy.substr(dot_pos + 1)
            : "";

        size_t q = ext.find('?');

        if (q != string::npos)
            ext = ext.substr(0, q);

        if (path.length() <= 1)
            path = "/index.html";

        if (method == "HEAD")
{
    string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 0\r\n"
        "\r\n";

    write(client_fd, response.c_str(), response.size());
}
else if (method == "GET" || method == "POST")
{
    sem_wait(&semaphore_mutex);

    send_file(
        client_fd,
        path,
        findContentType(ext)
    );

    sem_post(&semaphore_mutex);
}
    }

    printf("Closing connection.\n");

    close(client_fd);

    sem_wait(&semaphore_mutex);

    thread_count--;

    sem_post(&semaphore_mutex);

    pthread_exit(NULL);
}


int main() {

    char* env_port = getenv("PORT");
    if (env_port) {
        PORT = atoi(env_port);
    }

    sem_init(&semaphore_mutex, 0, 1);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd <= 0) {
        perror("socket failed");
        exit(1);
    }

    int opt = 1;

    setsockopt(
        server_fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt)
    );

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd,
             (struct sockaddr*)&addr,
             sizeof(addr)) < 0) {

        perror("bind failed");
        exit(1);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        exit(1);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {

        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);

        int client_fd = accept(
            server_fd,
            (struct sockaddr*)&client_addr,
            &len
        );

        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        char ip[INET_ADDRSTRLEN];

        inet_ntop(
            AF_INET,
            &client_addr.sin_addr,
            ip,
            sizeof(ip)
        );

        printf("Client connected: %s\n", ip);

        pthread_t thread;
        int* client_ptr = new int(client_fd);

        if (pthread_create(
                &thread,
                NULL,
                connection_handler,
                client_ptr) != 0) {

            perror("pthread_create failed");
            close(client_fd);
            delete client_ptr;
        }

        pthread_detach(thread);
    }

    sem_destroy(&semaphore_mutex);
    close(server_fd);

    return 0;
}

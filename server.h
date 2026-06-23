// server.h
#pragma once
#include <string>
using namespace std;

typedef enum {
    HTTP_HEADER,
    BAD_REQUEST,
    NOT_FOUND
} messageType;

// Supported file extensions
string fileExtension[] = {
    "aac", "avi", "bmp", "css", "gif", "ico", "js",
    "json", "mp3", "mp4", "otf", "png", "php", "rtf",
    "svg", "txt", "webm", "webp", "woff", "woff2", "zip",
    "html", "htm", "jpeg", "jpg",
};

// Corresponding Content-Type headers
string ContentType[] = {
    "Content-Type: audio/aac\r\n\r\n",
    "Content-Type: video/x-msvideo\r\n\r\n",
    "Content-Type: image/bmp\r\n\r\n",
    "Content-Type: text/css\r\n\r\n",
    "Content-Type: image/gif\r\n\r\n",
    "Content-Type: image/vnd.microsoft.icon\r\n\r\n",
    "Content-Type: text/javascript\r\n\r\n",
    "Content-Type: application/json\r\n\r\n",
    "Content-Type: audio/mpeg\r\n\r\n",
    "Content-Type: video/mp4\r\n\r\n",
    "Content-Type: font/otf\r\n\r\n",
    "Content-Type: image/png\r\n\r\n",
    "Content-Type: application/x-httpd-php\r\n\r\n",
    "Content-Type: application/rtf\r\n\r\n",
    "Content-Type: image/svg+xml\r\n\r\n",
    "Content-Type: text/plain\r\n\r\n",
    "Content-Type: video/webm\r\n\r\n",
    "Content-Type: image/webp\r\n\r\n",
    "Content-Type: font/woff\r\n\r\n",
    "Content-Type: font/woff2\r\n\r\n",
    "Content-Type: application/zip\r\n\r\n",
    "Content-Type: text/html\r\n\r\n",
    "Content-Type: text/html\r\n\r\n",
    "Content-Type: image/jpeg\r\n\r\n",
    "Content-Type: image/jpeg\r\n\r\n",
};

// HTTP response templates
string Messages[] = {
    "HTTP/1.1 200 OK\r\n",
    "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n"
    "<!doctype html><html><body>Server busy.</body></html>",
    "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"
    "<!doctype html><html><body>File not found.</body></html>"
};

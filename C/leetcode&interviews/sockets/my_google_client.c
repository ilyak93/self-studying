#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#define MAX_BUFFER_SIZE 100000
#define PORT 80

int main() {
    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    // Resolve Google's IP address
    struct hostent* server = gethostbyname("www.google.com");
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        return -1;
    }

    // Set up the server address structure
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    // Prepare the HTTP GET request
    char *request = "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";

    // Send the request
    send(sock, request, strlen(request), 0);
    printf("HTTP request sent\n");

    // Receive the response
    char buffer[MAX_BUFFER_SIZE] = {0};
    int total_bytes = 0;
    int bytes_received;
    while ((bytes_received = read(sock, buffer + total_bytes, MAX_BUFFER_SIZE - total_bytes - 1)) > 0) {
        total_bytes += bytes_received;
        if (total_bytes > MAX_BUFFER_SIZE - 1) {
            break;
        }
    }

    // Null-terminate the received data
    buffer[total_bytes] = '\0';

    // Print the response
    printf("Response from Google:\n%s\n", buffer);
    
    // Close the socket
    close(sock);
    return 0;
}
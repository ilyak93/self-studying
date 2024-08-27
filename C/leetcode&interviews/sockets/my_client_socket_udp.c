// UDP Client
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    // Creating socket file descriptor
    int client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    struct sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT)
    };

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Send message to server
    char* hello = "Hello from client";
    sendto(client_fd, hello, strlen(hello), 0,
           (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("Hello message sent to server\n");

    // Prepare to receive response from server
    char buffer[BUFFER_SIZE] = { 0 };
    socklen_t serv_len = sizeof(serv_addr);

    // Receive response from server
    int valread = recvfrom(client_fd, buffer, BUFFER_SIZE, 0,
                           (struct sockaddr*)&serv_addr, &serv_len);
    printf("Received from server: %s\n", buffer);

    close(client_fd);
    return 0;
}
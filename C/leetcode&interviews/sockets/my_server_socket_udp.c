// UDP Server
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <assert.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    // Creating socket file descriptor
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP server is listening on port %d\n", PORT);

    // Prepare to receive message from client
    char buffer[BUFFER_SIZE] = { 0 };
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Receive message from client
    int valread = recvfrom(server_fd, buffer, BUFFER_SIZE, 0,
                           (struct sockaddr*)&client_addr, &client_len);
    assert(valread > 0);
    printf("Received from client: %s\n", buffer);

    // Send response to client
    char* hello = "Hello from server";
    sendto(server_fd, hello, strlen(hello), 0,
           (struct sockaddr*)&client_addr, client_len);
    printf("Hello message sent to client\n");

    close(server_fd);
    return 0;
}
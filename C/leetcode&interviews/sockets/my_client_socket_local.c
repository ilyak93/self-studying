// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/test_socket"
#define BUFFER_SIZE 1024

int main() {
    // Create socket
    int client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    struct sockaddr_un server_addr = {0};
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Connect to server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    // Send data
    const char* message = "Hello from client!";
    send(client_fd, message, strlen(message), 0);

    // Receive response
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("recv error");
    } else {
        printf("Received: %s\n", buffer);
    }

    // Clean up
    close(client_fd);

    return 0;
}
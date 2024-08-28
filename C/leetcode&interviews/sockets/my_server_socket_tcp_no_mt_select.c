#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>

#define MAX_CLIENTS 50
#define BUFFER_SIZE 1024


int main() {
  int clients_num = 0;
    // Create server socket
    int server_socket = -1;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server socket to allow multiple connections
    /*
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    */
    // Setup server address structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5001);

    // Bind the socket to localhost port 5000
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 50) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 5000\n");

    fd_set read_fds;

    // Add child sockets to set
    int client_sockets[MAX_CLIENTS] = {0};

    while (1) {
        // Clear the socket set
        FD_ZERO(&read_fds);

        // Add server socket to set
        FD_SET(server_socket, &read_fds);
        int max_sd = server_socket;


        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_sockets[i];
            if (sd > 0) {
                FD_SET(sd, &read_fds);
                if (sd > max_sd) max_sd = sd;
            }
        }

        // Wait for an activity on one of the sockets
        int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            printf("select error");
        }

        // If something happened on the server socket, then it's an incoming connection
        if (FD_ISSET(server_socket, &read_fds)) {
            int new_socket;
            struct sockaddr_in client_addr;
            int addrlen = sizeof(client_addr);

            if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            } else {
                clients_num++;
                printf("%d clients num \n", clients_num);
            }

            printf("New connection, socket fd is %d, ip is : %s, port : %d\n",
                   new_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // Send welcome message
            char *welcome_message = "Welcome to the server!";
            if (send(new_socket, welcome_message, strlen(welcome_message), 0) != strlen(welcome_message)) {
                perror("send failed");
            } else {
              printf("Message sent\n");
            }

            // Add new socket to array of sockets
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    printf("Adding %d socket to list of sockets as %d\n", new_socket, i);
                    break;
                }
            }
        }

        // Check for I/O operation on other sockets
        /*
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_sockets[i];

            if (FD_ISSET(sd, &read_fds)) {
                char buffer[BUFFER_SIZE];
                int valread = read(sd, buffer, BUFFER_SIZE);

                if (valread == 0) {
                    // Someone disconnected, get their details and print
                    struct sockaddr_in client_addr;
                    int addrlen = sizeof(client_addr);
                    getpeername(sd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);
                    printf("Host disconnected, ip %s, port %d\n",
                           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                    // Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_sockets[i] = 0;
                } else {
                    // Echo back the message that came in
                    buffer[valread] = '\0';
                    printf("Received from client: %s\n", buffer);
                    send(sd, buffer, strlen(buffer), 0);
                }
            }
        }
        */
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define NUM_CLIENTS 45
#define BUFFER_SIZE 1024

void *client_thread(void *arg) {
    int client_id = *(int*)arg;
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5001);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return NULL;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return NULL;
    } else {
        printf("\nConnection Successful \n");
    }

    // Receive greeting message from server
    char buffer[BUFFER_SIZE] = {0};
    sleep(0.000000001);
    int valread = read(sock, buffer, BUFFER_SIZE);
    printf("Client %d received: %s\n", client_id, buffer);

    // Send message to server
    /*
    char message[BUFFER_SIZE];
    snprintf(message, BUFFER_SIZE, "Hello from client %d", client_id);
    send(sock, message, strlen(message), 0);
    printf("Client %d sent: %s\n", client_id, message);
    */
    // Keep connection open for a while
    sleep(5);

    close(sock);
    return NULL;
}

int main() {
    pthread_t threads[NUM_CLIENTS];
    int client_ids[NUM_CLIENTS];

    for (int i = 0; i < NUM_CLIENTS; i++) {
        client_ids[i] = i;
        if (pthread_create(&threads[i], NULL, client_thread, &client_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
        //sleep(0.000000001);
    }

    for (int i = 0; i < NUM_CLIENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

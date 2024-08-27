#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Global variables (minimized)
sem_t x, y;
int readercount = 0;

#define PORT_NUM 8989

void* reader(void* param) {
    sem_wait(&x);
    readercount++;

    if (readercount == 1)
        sem_wait(&y);

    sem_post(&x);

    printf("\n%d reader is inside", readercount);

    sleep(1.5);

    sem_wait(&x);
    readercount--;

    if (readercount == 0) {
        sem_post(&y);
    }

    sem_post(&x);

    printf("\n%d Reader is leaving", readercount + 1);
    pthread_exit(NULL);
}

void* writer(void* param)
{
    printf("\nWriter is trying to enter");

    sem_wait(&y);

    printf("\nWriter has entered");

    sleep(3);

    sem_post(&y);

    printf("\nWriter is leaving");
    pthread_exit(NULL);
}

int main()
{
    // Initialize semaphores
    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);

    // Socket setup
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT_NUM)
    };

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen on the socket
    if (listen(serverSocket, 50) == 0)
        printf("Listening\n");
    else {
        printf("Listen Error\n");
        exit(EXIT_FAILURE);
    }

    // Thread handling
    pthread_t writerthreads[100], readerthreads[100];
    int readers_index = 0;
    int writers_index = 0;

    while (1) {
        struct sockaddr_storage serverStorage;
        socklen_t addr_size = sizeof(serverStorage);

        // Accept new connection
        int newSocket = accept(serverSocket, (struct sockaddr*)&serverStorage, &addr_size);
        if (newSocket < 0) {
            perror("Accept failed");
            continue;
        }

        int choice = 0;
        recv(newSocket, &choice, sizeof(choice), 0);

        if (choice == 1) {
            if (pthread_create(&readerthreads[readers_index], NULL, reader, &newSocket) != 0)
                printf("Failed to create reader thread\n");
            readers_index++;
        }
        else if (choice == 2) {
            if (pthread_create(&writerthreads[writers_index], NULL, writer, &newSocket) != 0)
                printf("Failed to create writer thread\n");
            writers_index++;
        }

        if (readers_index >= 20 || writers_index >= 20) {
          break;
        }
    }

    for (int i = 0; i < readers_index; i++) {
        pthread_join(readerthreads[i], NULL);
    }
    for (int i = 0; i < writers_index; i++) {
        pthread_join(writerthreads[i], NULL);
    }

    return 0;
}
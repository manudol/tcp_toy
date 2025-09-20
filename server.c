#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h> // for closing socket

#include <pthread.h>

void *handle_client(void *arg)
{
    int client_fd = (int)(intptr_t)arg;
    while(1) {
        if (client_fd > 0) {
            char buffer[1024];
            int bytes_recv = recv(client_fd, buffer, sizeof(buffer), 0);
            if (bytes_recv > 0) {
                buffer[bytes_recv] = '\0';
                printf("Client says: %s", buffer);
                char msg[1024] = "Hello, Client!\n\0";
                
                int num_bytes_sent = send(client_fd, msg, strlen(msg), 0);

                if (num_bytes_sent == (int)strlen(msg)) printf("Same length\n");

                if (strncmp(buffer, "bye", 3) == 0) {
                    printf("Client diconnecting with 'bye'\n");
                    break;
                }

            } else if (bytes_recv == 0) {
                printf("Client disconnected: Exiting gracefully ;)\n");
                break;
            } else {
                perror("recv error");
                break;
            }
        } else {
            perror("error accepting with 'accept()'\n");
            break;
        }
    }
    close(client_fd);
    return NULL;
}

int main()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("error creating socket\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_port = htons(8080);

   if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
        perror("Error binding 'bind()' socket\n");
        close(sock_fd);
        exit(EXIT_FAILURE);   
    }

    if (listen(sock_fd, SOMAXCONN) == -1) {
        perror("listen failed\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        int client_fd = accept(sock_fd, NULL, NULL);
        printf("client connects\n");
        // pethread...
        pthread_t tid;
        pthread_create(&tid, NULL, (void *)handle_client, (void *)(intptr_t)client_fd);
        pthread_detach(tid);
    }

    close(sock_fd);

    return 0;
}


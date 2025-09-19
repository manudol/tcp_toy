#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h> // for closing socket

int main()
{
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);    
    server_addr.sin_port = htons(8080);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket connection failed\n");
        return -1;
    }
    while(1) {
        char client_msg[256];
        char server_response[1024];
        
        printf(">>> ");
        scanf("%s", client_msg);

        if (strlen(client_msg) > 0) {
            int msg_sent = send(client_socket, client_msg, strlen(client_msg), 0);
            if (msg_sent > 0) {
                printf("✔️\n");
                // Receive response from server
                int bytes_received = recv(client_socket, server_response, sizeof(server_response) - 1, 0);
                if (bytes_received > 0) {
                    server_response[bytes_received] = '\0'; // Null terminate
                    printf("Server response: %s\n", server_response);
                } else if (bytes_received == 0) {
                    printf("Server closed connection\n");
                    break;
                } else {
                    perror("recv failed");
                    break;
                }
            } else {
                printf("❌\n");
                break;
            } 
        }
    }
    close(client_socket);
    return 0;
}

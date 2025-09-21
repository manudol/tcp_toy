#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h> // for closing socket

#include "utils.h"

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
        char client_msg[1024];
 
        printf(">>> ");
        fgets(client_msg, sizeof(client_msg), stdin);
    
        if (strlen(client_msg) > 1) {

            int msg_sent = send(client_socket, client_msg, strlen(client_msg), 0); 

            if (msg_sent > 0) {
                // Receive response from server
                char temp_bytes;
                ssize_t available = recv(client_socket, &temp_bytes, 1, MSG_PEEK);
                
                if (available > 0) {
                    int available_bytes = 0;
                    ioctl(client_socket, FIONREAD, &available_bytes);
                    
                    char* server_response = malloc(available_bytes + 1); 
                    int bytes_received = recv(client_socket, server_response, available_bytes, 0);

                    if (bytes_received > 0) {
                        server_response[bytes_received] = '\0';
                        printf("%s", server_response);

                        free(server_response);

                    } else if (bytes_received == 0) {
                        printf("Server closed connection\n");
                        break;
                    } else {
                        perror("recv failed");
                        break;
                    }
                } else { break; }
            } else {
                printf("❌\n");
                break;
            } 
        }
    }
    close(client_socket);
    return 0;
}

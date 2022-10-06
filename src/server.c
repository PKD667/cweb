#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "errno.h"

#include "../include/main.h"

int strtsckt()
{
    char* dout = NULL;

    int socket_desc, client_sock;
    unsigned int  client_size;
    
    struct sockaddr_in server_addr, client_addr;

    char* response = (char*)calloc(256,sizeof(char));
    char* message = (char*)calloc(256,sizeof(char));

    

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socket_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
     printf("Socket fd: %d\n", socket_desc);

    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ADDR);
    
    //print fd
   
    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldn't bind to the port\n");
        printf("Error : %s\n", strerror(errno));
        return -1;
    }
    printf("Done with binding\n");
    
    // Listen for clients:
    if(listen(socket_desc, 1) < 0){
        printf("Error while listening\n");
        return -1;
    }

    wait:

    printf("\nListening for incoming connections.....\n");
    
    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
    
    if (client_sock < 0){
        printf("Can't accept\n");
        return -1;
    }
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    // Receive client's message:
    if (recv(client_sock, message, 256, 0) < 0){
        printf("Couldn't receive\n");
        return -1;
    }
    

    printf("Msg from client: \n%s\n", message);

    int code = process(message, &dout);

    // Send a response:
    sprintf(response, "HTTP/1.1 %d OK\nContent-Type: text/html\nContent-Length: %lu\n\n%s",code, strlen(dout), dout);

    if (send(client_sock, response, strlen(response), 0) < 0){
        printf("Can't send\n");
        return -1;
    }

    printf("Html sent\n");
    close(client_sock);

    goto wait;
    
    // Closing the socket:
    
    close(socket_desc);


}
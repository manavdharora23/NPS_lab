#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char* argv[]) { 
    const char *server_name = "127.0.0.1";
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    servaddr.sin_family = AF_INET; 
    inet_pton(AF_INET, server_name, &servaddr.sin_addr);

    int port = atoi(argv[1]);
    servaddr.sin_port = htons(port);

    if((connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) == 0) {
        printf("Connected to server!\n");
        fflush(stdout);
    } else {
        printf("Invalid or busy server address\n");
        fflush(stdout);
        exit(0);
    };

    int cont = 0;
    char *buf = malloc(1024);
    char recevied[1024];
    printf("Client input: ");
    fgets(buf, 100, stdin);
    while(strcmp(buf, "quit\n") != 0) {
        send(sockfd, buf, 100, 0);
        if ((cont = recv(sockfd, recevied, 1024, 0)) > 0) {
            printf("Server response: %s\n", recevied);
            fflush(stdout);
            memset(recevied, 0, sizeof(recevied));
        }
        printf("Client input: ");
        fgets(buf, 100, stdin);
    };

    close(sockfd); 
}

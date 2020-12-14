#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <wait.h>

int main(int argc, char *argv[]) {
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
  
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 

    int port = atoi(argv[1]);
    servaddr.sin_port = htons(port);
  
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  
    listen(sockfd, 128);
    printf("Server started at %s...\n", inet_ntoa(servaddr.sin_addr));
    fflush(stdout);

    while(1) {
        socklen_t len = sizeof(cli);
        connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
        int i = getpeername(connfd, (struct sockaddr*)&cli, &len);

        printf("Client connected at port %d\n", htons(cli.sin_port));
        int n;
        char buf[100];
        int pid;

        if((pid=fork())==0){
            close(sockfd);
            while((n = recv(connfd, buf, 100, 0)) > 0) {
                printf("Client message: %s\n", buf);
                int pipes[2];
                pipe(&pipes[0]);
                if ((pid=fork()) == 0) {
                    dup2(pipes[1], STDOUT_FILENO);
                    close(pipes[0]);
                    execl("/bin/sh", "sh", "-c", buf, (char *) NULL);
                } else {
                    close(pipes[1]);
                    char cmd_output[1024];
                    int cmd_size = read(pipes[0], cmd_output, sizeof(cmd_output));
                    send(connfd, cmd_output, cmd_size, 0);
                    wait(NULL);
                }
                fflush(stdout);
            };
            exit(0);
        }
        close(connfd);
    }
    return 0;
}

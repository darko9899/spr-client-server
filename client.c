#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd)
{
    
    int num=0;
    int n=0;
    for (;;) {
        //bzero(num, sizeof(num));
        printf("Enter step for iteration: ");
        //n = 0;
        //while ((buff[n++] = getchar()) != '\n');
        scanf("%d", &num);
        write(sockfd, &num, sizeof(int));
        //bzero(num, sizeof(num));
        read(sockfd, &n, sizeof(int));
        char *buff[n];
        recv(sockfd,&buff,n*sizeof(char*),0);
        printf("From Server : %d\n", num);
        for(int i=0;i<n;i++){
            printf("%s",buff[i]);
        }
        printf("\n");
        if (num == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}
 
int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
 
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
 
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
 
    // function for chat
    func(sockfd);
 
    // close the socket
    close(sockfd);
}
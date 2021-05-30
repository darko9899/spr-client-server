#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8080
#define SIZE 2000
#define SA struct sockaddr

typedef struct Response {
    int size;
    char arr[SIZE];
} Response;
void func(int sockfd)
{
    int num=0;
    char buff[SIZE];
    for (;;) {
        Response *response= (Response*)malloc(sizeof(Response));
        printf("Enter step for iteration(0 for exit):");
        scanf("%d", &num);
        if(num<0){
            printf("Invalid step!!!\n");
            continue;
        }
        write(sockfd, &num, sizeof(int));
        bzero(response,sizeof(Response));
        bzero(buff,SIZE);
        read(sockfd,buff,SIZE);
        memcpy(response,buff,SIZE);
        printf("From Server : \n");
        
        printf("Number of elements : %d\n", response->size);
        printf("Result :\n%s\n",response->arr);
        
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
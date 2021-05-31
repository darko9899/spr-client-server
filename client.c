#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8081
#define SIZE 2000
#define SA struct sockaddr

typedef struct Response
{
    int size;
    char arr[SIZE];
} Response;
void chatFunc(int sockfd)
{
    int num = 0;
    char buff[SIZE];
    for (;;)
    {
        Response *response = (Response *)malloc(sizeof(Response));
        printf("Enter step for iteration:");
        scanf("%d", &num);
        if (num < 0)
        {
            printf("Invalid step!!!\n");
            continue;
        }
        write(sockfd, &num, sizeof(int));
        bzero(response, sizeof(Response));
        bzero(buff, SIZE);
        read(sockfd, buff, SIZE);
        memcpy(response, buff, SIZE);
        printf("From Server : \n");

        printf("Number of elements : %d\n", response->size);
        printf("Result :\n%s\n", response->arr);

        if (num == 0)
        {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Error: Socket creation failed!!!\n");
        exit(0);
    }
    else
        printf("Socket successfully created...\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Error: Connection with the server failed!!!\n");
        exit(0);
    }
    else
        printf("Connected to the server...\n");

    chatFunc(sockfd);

    close(sockfd);
}
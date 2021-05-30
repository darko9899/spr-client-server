#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <math.h>
#define MAX 128
#define PORT 8080
#define SIZE 2000
#define FILENAME "test.txt"
#define SA struct sockaddr

typedef struct Response {
    int size;
    char arr[SIZE];
} Response;
struct list {
    char *string;
    struct list *next;
};
typedef struct list LIST;

void makeResponse(int step,int sockfd)
{
    FILE *fp;
    char line[MAX];
    LIST *current, *head;
    
    char buff[SIZE];
    int j=0;
    char result[SIZE];
    Response *response= (Response*)malloc(sizeof(Response));
 
    head = current = NULL;
    fp = fopen(FILENAME, "r");
 
    while(fgets(line, sizeof(line), fp)){
        LIST *node = malloc(sizeof(LIST));
        node->string = strdup(line);//note : strdup is not standard function
        node->next =NULL;
 
        if(head == NULL){
            current = head = node;
        } else {
            current = current->next = node;
        }
        j++;
    }
    fclose(fp);
    
    double size = ceil((double)j/(double)step);
    
    
    bzero(result,SIZE);
    bzero(response,sizeof(Response));
    for(current = head; current ; current=current->next){
        strcat(response->arr,current->string);
        for(int i=0;i<step-1;i++){
                if (current->next==NULL){
                    break;
                }else{
                    current=current->next;
                }
 
        }
    }
    printf("To client :\n");
    printf("%s\n",response->arr);
    bzero(buff,SIZE);
    response->size=size;

    memcpy(buff,response,sizeof(buff));
    write(sockfd,buff,sizeof(buff));
    
   
}
 
// Function designed for chat between client and server.
void chatFunc(int sockfd)
{
    int num=0;
    // infinite loop for chat
    for (;;) {
        // read the message from client and copy it in buffer
        read(sockfd, &num, sizeof(int));
        // print buffer which contains the client contents
        printf("From client: %d\n", num);
        if (num == 0) {
            printf("Server Exit...\n");
            break;
        }
        makeResponse(num,sockfd);
    }
}
 
// Driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
 
    // socket create and verification
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
 
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
 
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
 
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");
 
    // Function for chatting between client and server
    chatFunc(connfd);
 
    // After chatting close the socket
    close(sockfd);
}
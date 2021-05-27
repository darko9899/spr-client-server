#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80
#define PORT 8080
#define FILENAME "test.txt"
#define SA struct sockaddr

struct list {
    char *string;
    struct list *next;
};
typedef struct list LIST;
int z=0;
char **printFile(int step)
{
    FILE *fp;
    char line[128];
    LIST *current, *head;
    char* str[50];
 
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
    }
    fclose(fp);
    //test print
    int j=0;
    for(current = head; current ; current=current->next){
        //printf("%s", current->string);
        str[j]=current->string;
        j++;
        for(int i=0;i<step-1;i++){
                if (current->next==NULL){
                    break;
                }else{
                    current=current->next;
                }
 
        }
    }
    z=j;
    char** str2=malloc(sizeof(char*)*j);
    for(int count=0;count<j;count++){
        str2[count]=malloc(MAX +1);
    }
    for(int c=0;c<j;c++){
        str2[c]=str[c];
        //printf("%s\n",str2[c]);
    }
    printf("\n");
    //need free for each node
    return str2;
   
}
 
// Function designed for chat between client and server.
void func(int sockfd)
{
    char buff[MAX];
    int n;
    int num=0;
    // infinite loop for chat
    for (;;) {
       // bzero(buff, MAX);
 
        // read the message from client and copy it in buffer
        read(sockfd, &num, sizeof(int));
        // print buffer which contains the client contents
        printf("\tFrom client: %d\t To client : ", num);
        char** str=printFile(num);
        for(int i=0;i<z;i++){
            printf("%s",str[i]);
        }
        printf("\n");
        //bzero(str, j);
        n = 0;
        // copy server message in the buffer
        /*while ((buff[n++] = getchar()) != '\n')
            ;*/
            //scanf("%d", &num);
            num=num+num;
            printf("%d\t", num);
 
        // and send that buffer to client
        write(sockfd, &z, sizeof(int));
        write(sockfd,&str,z*sizeof(char*));
 
        // if msg contains "Exit" then server exit and chat ended.
        if (num == 0) {
            printf("Server Exit...\n");
            break;
        }
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
    func(connfd);
 
    // After chatting close the socket
    close(sockfd);
}
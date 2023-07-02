#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<time.h>
#include<errno.h>

#define PORT_NUM 1488


//call - binary_name <address d.d.d.d> <port>
int main(int argc, char ** argv) {
    //init, parse args, call main processing loop
    int sock;
    char* p;
    uint16_t port;
    struct sockaddr_in server_addr;
    uint16_t payload[10];
    int payload_size = sizeof(payload);

    //init connection parameters
    inet_pton(AF_INET, argv[1], &(server_addr.sin_addr));
    server_addr.sin_family = AF_INET;
    port = strtol(argv[2], &p, 10);
    server_addr.sin_port = htons(port);

    char str_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(server_addr.sin_addr), str_addr, INET_ADDRSTRLEN);
    printf("Connecting to %s:%d\n", str_addr, port);

    printf("Generating numbers ");
    srand(time(NULL));
    for(int i = 0; i < 10; i++) {
        payload[i] = rand() % 100;
        printf("%d ",payload[i]);
    }
    printf("\n");

    
    //create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "Error creating socket.");
        exit(1);
    }
    
    //open connection/send array/receive array
    int result;
    result = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(result < 0) {
        printf("Error %s\n",strerror(errno));
    }
    send(sock, payload, payload_size, 0);
    printf("Sent the numbers\n");
    recv(sock, payload, payload_size, 0);
    printf("Received something\n");
    printf("Received sorted numbers ");
    for(int i = 0; i < 10; i++) {
        printf("%d ",payload[i]);
    }
    printf("\n");
    

    return(0);
    

}





//main init, parse args
//create 10 random numbers
//connect to server from args, sent the numbers
//read back from socket, print sorted numbers

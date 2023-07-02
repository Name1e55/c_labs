#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<time.h>

#define PORT_NUM 1488



int main(int argc, char ** argv) {
    //init, parse args, call main processing loop
    int sock;
    struct sockaddr_in server_addr;
    //const char* msg = argv[0];

    uint16_t payload[10];
    int payload_size = sizeof(payload);
    
    printf("\nGenerating numbers ");
    srand(time(NULL));
    for(int i = 0; i < 10; i++) {
        payload[i] = rand() % 100;
        printf("%d ",payload[i]);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "Error creating socket.");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(PORT_NUM);
    
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    send(sock, payload, payload_size, 0);
    recv(sock, payload, payload_size, 0);

    printf("\nReceived sorted numbers ");
    for(int i = 0; i < 10; i++) {
        printf("%d ",payload[i]);
    }
    

    return(0);
    

}





//main init, parse args
//create 10 random numbers
//connect to server from args, sent the numbers
//read back from socket, print sorted numbers
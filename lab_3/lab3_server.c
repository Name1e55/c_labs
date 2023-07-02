#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<math.h>
#include<unistd.h>

#define PORT_NUM 1488

void insertionSort(uint16_t arr[], int n);

int main(int argc, char ** argv) {
    //init, parse args, call main processing loop
    int sock;
    struct sockaddr_in host_addr, source_addr;
    socklen_t src_struct_len = sizeof(source_addr);
    int bytes_read;
    char buf[1024];
    char str_addr[INET_ADDRSTRLEN];
    uint16_t payload[10];
    int payload_size = sizeof(payload);
    
    printf("I'm alive, initializing.... \n");

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "Error creating socket.");
        exit(1);
    }
    host_addr.sin_family = AF_INET;
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    host_addr.sin_port = htons(PORT_NUM);
    if (bind(sock, (struct sockaddr*)&host_addr, sizeof(host_addr)) < 0) {
        fprintf(stderr, "Error binding socket.");
        exit(2);
    }

    listen(sock, 10);

    int accepted_socket = 0 ;

    while(1){
        //bytes_read = recvfrom(sock, payload, payload_size, 0, (struct sockaddr*)&source_addr, &src_struct_len);
        //buf[bytes_read] = '\0';
        accepted_socket = accept(sock, (struct sockaddr*)&source_addr, &src_struct_len);
        printf("Spawn child process\n");

        pid_t child_pid;

        if((child_pid = fork()) < 0) {
            fprintf(stderr, "Error spawning child process");
            exit(1);
        }

        if(child_pid > 0) {
            printf("Spawned child process with PID %d \n", child_pid);
            wait(NULL);
        }
        else {
            recv(accepted_socket, payload, payload_size, 0);

            inet_ntop(AF_INET, &(source_addr.sin_addr), str_addr, INET_ADDRSTRLEN);
            printf("Received from %s \n",str_addr);
            //printf("%s",buf);
            printf("\nReceived numbers ");
            for(int i = 0; i < 10; i++) {
                printf("%d ",payload[i]);
            }
            printf("\n");
            insertionSort(payload,10);
            printf("\nSorted numbers: ");
            for(int i = 0; i < 10; i++) {
                printf("%d ",payload[i]);
            }
            printf("\n");
            //connect(sock, (struct sockaddr*)&source_addr,sizeof(source_addr));
            send(accepted_socket, payload, payload_size, 0);
            exit(0);
        }
    }

 
    return(0);
    

}

//plain copy-paste
void insertionSort(uint16_t arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
 
        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

//main processing loop

//read from socket, get the numbers, print source address and the initial numbers
//spawn new process to sort received buffer and send sorted numbers back to origin


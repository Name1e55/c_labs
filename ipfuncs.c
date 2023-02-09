#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ipfuncs.h"
#include "ipdata.c"

typedef struct Address address_t;


// Separate IP address and netmask from input. Write into provided buffer as twin-uint32 struct
int parseInput(const char* inpStr, address_t* parsed_item) {
    char* addr;
    char* mask;
    uint8_t bytes[4];
    
    char* inputDupe = strdup(inpStr);

    addr = strtok(inputDupe, "/");
    if (addr != NULL) {
        mask = strtok(NULL, "/");
        // printf("Address - %s\n", addr);
        // printf("Netmask - %s\n", mask);
        
        char* tmp = strtok(addr, ".");
        for (int i=0 ; i<4; i++) {
            bytes[i] = atoi(tmp);
            tmp = strtok(NULL, ".");
        }
        
        // for (int j = 0; j<4; j++) {
        //     printf("%d\n", bytes[j]);
        // }
        
        parsed_item->IP = bytes[3] | (bytes[2] << 8) | (bytes[1] << 16) | (bytes [0] << 24);
        parsed_item->Netmask = ~((1 << (32 - atoi(mask))) - 1);


        // const int bytes = 4;
        // uint8_t octet[4];
        // char result[16];
        // for (int i=0; i<bytes; i++){
        //     octet[i] = parsed_item->IP >> (i*8);
        // }
        // sprintf(result, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
        // printf("IP:    %s \n",result);
        //
        // for (int i=0; i<bytes; i++){
        //     octet[i] = parsed_item->Netmask >> (i*8);
        // }
        // sprintf(result, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
        // printf("netmask:    %s \n",result);
        
    }
    else {
        free(inputDupe);
        puts("Some shit happened");
        return 1;
    }
    
    free(inputDupe);
    return 0;
}

int getNetwork(address_t* addr){
    uint32_t network;
    network = addr->IP & addr->Netmask;

    const int bytes = 4;
    uint8_t octet[4];
    char result[16];
    for (int i=0; i<bytes; i++){
        octet[i] = network >> (i*8);
    }
    sprintf(result, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
    printf("Network:    %s \n",result);

    return 0;
}

int checkPresetNetworks(address_t* item){
    address_t presetNetwork[4];
    char verdict[1000];
    char* pos = verdict;

    parseInput(Network_1, &presetNetwork[0]);
    parseInput(Network_2, &presetNetwork[1]);
    parseInput(Network_3, &presetNetwork[2]);
    parseInput(Network_4, &presetNetwork[3]);

    for (int i = 0; i < 4; i++) {
        if ((item->IP & item->Netmask) == (presetNetwork[i].IP & presetNetwork[i].Netmask)) {
            pos += sprintf(pos, "Address belongs to Network %d  \n",i+1);
        }
    }
    printf("%s",verdict);


    return 0;
}

int checkPrivateNetworks(address_t* item){
    address_t presetNetwork[5];
    char verdict[1000] = {0};
    char* pos = verdict;

    parseInput(Private_1, &presetNetwork[0]);
    parseInput(Private_2, &presetNetwork[1]);
    parseInput(Private_3, &presetNetwork[2]);
    parseInput(Private_4, &presetNetwork[3]);
    parseInput(Private_5, &presetNetwork[4]);

    for (int i = 0; i < 4; i++) {
        if ((item->IP & item->Netmask) == (presetNetwork[i].IP & presetNetwork[i].Netmask)) {
            pos += sprintf(pos, "Address belongs to Private Network %d  \n",i+1);
        }
    }
    printf("%d\n",verdict[0]);
    if (verdict[0] != 0) printf("wow");
    printf("%s",verdict);


    return 0;
}

int checkIfBroadcast(address_t* item){
    uint32_t network = item->IP & item->Netmask;
    if ((network ^ item->IP) == ~item->Netmask)
        printf("That's a broadcast!");
    else
        printf("Not a broadcast address");
    return 0;
}

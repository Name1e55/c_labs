#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ipdata.h"

// Too much structs to write, here's typedef
typedef struct Address address_t;


// Separate IP address and netmask from input. Write into provided buffer as twin-uint32 struct
int parseInput(const char* inpStr, address_t* parsed_item) {
    char* addr;
    char* mask;
    uint8_t bytes[4];
    
    // Copy input const to calm the compiler and provide safe runtime
    char* inputDupe = strdup(inpStr);

    addr = strtok(inputDupe, "/");
    if (addr != NULL) {
        mask = strtok(NULL, "/");
        
        char* tmp = strtok(addr, ".");
        for (int i=0 ; i<4; i++) {
            bytes[i] = atoi(tmp);
            tmp = strtok(NULL, ".");
        }
        
        // Put octets of our IP separately into uint32
        parsed_item->IP = bytes[3] | (bytes[2] << 8) | (bytes[1] << 16) | (bytes [0] << 24);
        // Cool bitmask hackery. Will bork if mask is >32. Check TBA in validation
        parsed_item->Netmask = ~((1 << (32 - atoi(mask))) - 1);
    }
    else {
        free(inputDupe);
        puts("Some shit happened");
        return 1;
    }
    
    free(inputDupe);
    return 0;
}

// Get network address from IP and netmask of provided item
int getNetwork(address_t* addr){
    uint32_t network;
    network = addr->IP & addr->Netmask;
    // Useful part ends higher, the rest is just for the output to get the right order and cast octets separately/
    // Let's pretend we don't know inet_pton()
    // But in reality I could not manage to make it work =((
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

// Check which subnet is bigger
address_t* whoIsBigger(address_t* first, address_t* second){
    if (first->Netmask < second->Netmask)
        return first;
    else
        return second;
}

// Parse preset networks the same as test item from input and then check for matches
int checkPresetNetworks(address_t* item){
    address_t presetNetwork[4];
    char verdict[1000];
    char* pos = verdict;
    address_t* bigNet;

    parseInput(Network_1, &presetNetwork[0]);
    parseInput(Network_2, &presetNetwork[1]);
    parseInput(Network_3, &presetNetwork[2]);
    parseInput(Network_4, &presetNetwork[3]);

    for (int i = 0; i < 4; i++) {
        // /16 net will include /24 net so we'll need to check
        // if our preset is bigger than the item to check
        bigNet = whoIsBigger(item, &presetNetwork[i]);
        if ((bigNet == &presetNetwork[i]) && (item->IP & bigNet->Netmask) == (presetNetwork[i].IP & bigNet->Netmask)) {
            pos += sprintf(pos, "Address belongs to Network %d  \n",i+1);
        }
    }
    if (verdict[0] != 0) 
        printf("%s",verdict);

    return 0;
}

// Same but with private subnets
int checkPrivateNetworks(address_t* item){
    address_t presetNetwork[5];
    char verdict[1000] = {0};
    char* pos = verdict;
    address_t* bigNet;

    parseInput(Private_1, &presetNetwork[0]);
    parseInput(Private_2, &presetNetwork[1]);
    parseInput(Private_3, &presetNetwork[2]);
    parseInput(Private_4, &presetNetwork[3]);
    parseInput(Private_5, &presetNetwork[4]);

    for (int i = 0; i < 5; i++) {
        bigNet = whoIsBigger(item, &presetNetwork[i]);
        if ((bigNet == &presetNetwork[i]) && (item->IP & bigNet->Netmask) == (presetNetwork[i].IP & bigNet->Netmask)) {
            pos += sprintf(pos, "Address belongs to Private Network %d  \n",i+1);
        }
    }
    if (verdict[0] != 0) 
        printf("%s",verdict);

    return 0;
}

// Broadcast has 111s in the end, mask in the beginning. Invert one - it slould match for broadcast
int checkIfBroadcast(address_t* item){
    uint32_t network = item->IP & item->Netmask;
    if ((network ^ item->IP) == ~item->Netmask)
        printf("That's a broadcast!");
    else
        printf("Not a broadcast address");
    return 0;
}

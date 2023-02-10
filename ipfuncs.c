#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ipdata.h"

// Too much structs to write, here's typedef
typedef struct Address address_t;

// Check if string is number
int isNumber(const char* str) {
    int len = strlen(str);
    for (int i=0; i<len; i++) {
        if (!isdigit(str[i])) {
            puts("There's a character in input!");
            return(1);
        }
    }
    return(0);
}

// Check separators count for IP part
int dotsCount(const char* str) {
    int dots = 0;
    while (*str != '\0') {
        if (*str++ == '.') dots++;
    }
    return dots;
}

// Check if IP part is a valid IP address
int validateIP(const char* str) {
    int dots = 0;
    int errCount = 0;
    char* inputDupe = strdup(str);

    dots = dotsCount(str);
    if (dots != 3) {
        puts("Invalid octets count.");
        return 1;
    }

    char* part;
    part = strtok(inputDupe, ".");
    while (part != NULL) {
        if (isNumber(part) != 0) return 1;
        if (strlen(part) > 1 && *part == '0') {
            puts("Address octet contains leading zeroes");
            return 1;
        }
        int range = atoi(part);
        if (range < 0 || range > 255) {
            puts("Octet is not in range");
            return 1;
        }
        part = strtok(NULL, ".");
    }
    free(inputDupe);
    
    return 0;
}

// Separate IP address and netmask from input. Write into provided buffer as twin-uint32 struct
int parseInput(const char* inpStr, address_t* parsed_item) {
    char* addr;
    char* mask;
    uint8_t octets[4];
    
    // Copy input const to calm the compiler and provide safe runtime
    char* inputDupe = strdup(inpStr);

    addr = strtok(inputDupe, "/");
    if (addr != NULL) {
        mask = strtok(NULL, "/");
        
    // Primitive input validation 
        if (validateIP(addr) != 0)
            return 1;
        if (mask == NULL){
            puts("Netmask separator (/) not found. \n");
            return 1;
        }
        if (strlen(mask) > 2){
            puts("Invalid netmask length\n");
            return 1;
        }
        int range = atoi(mask);
        if (range <0 || range > 32){
            puts("Invalid netmask range\n");
            return 1;
        }


        char* tmp = strtok(addr, ".");
        for (int i=0 ; i<4; i++) {
            octets[i] = atoi(tmp);
            tmp = strtok(NULL, ".");
        }
        
        // Put octets of our IP separately into uint32
        parsed_item->IP = octets[3] | (octets[2] << 8) | (octets[1] << 16) | (octets[0] << 24);
        // Some bitmask hackery. Will bork if mask is >32. Should be safe now with mask validation
        parsed_item->Netmask = ~((1 << (32 - atoi(mask))) - 1);
        // Here out input is kinda correct, so we fill char array to output it without conversions later
        strcpy(parsed_item->strVal,inpStr);
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
void getNetwork(address_t* addr){
    uint32_t network;
    network = addr->IP & addr->Netmask;
    // Useful part ends here, the rest is just for the output to get the right order and cast octets separately/
    // Let's pretend we don't know inet_pton()
    // But in reality I could not manage to make it work =((
    const int numBytes = 4;
    uint8_t octet[4];
    char result[16];
    for (int i = 0; i < numBytes; i++){
        octet[i] = network >> (i*8);
    }
    sprintf(result, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);
    printf("Address is from Network: %s \n",result);
}

// Check which subnet is bigger
address_t* whoIsBigger(address_t* first, address_t* second){
    if (first->Netmask < second->Netmask)
        return first;
    else
        return second;
}

// Parse preset networks the same as test item from input and then check for matches
void checkPresetNetworks(address_t* item){
    address_t presetNetwork[4];
    char verdict[1000] = {0};
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
            pos += sprintf(pos, "Address belongs to Network %d  %s \n",i+1,presetNetwork[i].strVal);
        }
    }
    if (verdict[0] != 0) 
        printf("%s",verdict);
}

// Parse preset private networks the same as test item from input and then check for matches
void checkPrivateNetworks(address_t* item){
    address_t presetNetwork[5];
    char verdict[1000] = {0};
    char* pos = verdict;
    address_t* bigNet;

    parseInput(Private_1, &presetNetwork[0]);
    parseInput(Private_2, &presetNetwork[1]);
    parseInput(Private_3, &presetNetwork[2]);
    parseInput(Private_4, &presetNetwork[3]);
    parseInput(Private_5, &presetNetwork[4]);

    // Same but with private subnets
    for (int i = 0; i < 5; i++) {
        bigNet = whoIsBigger(item, &presetNetwork[i]);
        if ((bigNet == &presetNetwork[i]) && (item->IP & bigNet->Netmask) == (presetNetwork[i].IP & bigNet->Netmask)) {
            pos += sprintf(pos, "Address belongs to Private Network %d  %s \n",i+1,presetNetwork[i].strVal);
        }
    }
    if (verdict[0] != 0) 
        printf("%s",verdict);
}

// Broadcast has 111s in the end, mask in the beginning. XOR address with it's network, invert - it slould match netmask for broadcast address
void checkIfBroadcast(address_t* item){
    uint32_t network = item->IP & item->Netmask;
    if ((network ^ item->IP) == ~item->Netmask)
        printf("That's a broadcast!\n");
    else
        printf("Not a broadcast address\n");
}

// Validate provided address string and do the actual stuff with it
int doStuff(const char* str){
    // We quit on q
    if(str[0] != 'q'){
        struct Address addr;
        if (0 == parseInput(str,&addr)){
            getNetwork(&addr);
            checkPresetNetworks(&addr);    
            checkPrivateNetworks(&addr);
            checkIfBroadcast(&addr);
        }
        return 0;
    }
    return 1;
}

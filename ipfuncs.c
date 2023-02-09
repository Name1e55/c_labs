#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ipfuncs.h"
#include "ipdata.h"

typedef struct Address address_t;

// Check separators count for IP part
int dotsCount(const char* str) {
    int dots = 0;
    while (*str != '\0') {
        if (*str++ == '.') dots++;
    }
    return dots;
}

// Check if string is number
int isNumber(const char* str) {
    int len = strlen(str);
    for (int i=0; i<len; i++) {
        if (!isdigit(str[i])) {
            puts("Character in input!");
            return(1);
        }
    }
    return(0);
}

// Check if IP part is a valid IP address
int validateIP(char* str) {
    int dots = 0;
    int errCount = 0;
    char* addr;

    dots = dotsCount(str);
    if (dots != 3) {
        puts("Dots count not OK");
        return 1;
    }

    char* part;
    part = strtok(str, ".");
    while (part != NULL) {
        if (isNumber(part) != 0) return 1;
        if (strlen(part) > 0 && *part == '0') {
            puts("Address contains leading zeroes");
            return 1;
        }
        int range = atoi(part);
        if (range < 0 || range > 255) {
            puts("Octet is not in range");
            return 1;
        }
        part = strtok(NULL, ".");
    }
    return 0;
    puts("valid");
}

// Separate IP address and netmask from input
int parseInput(char* inpStr) {
    char* addr;
    char* mask;
    int ipresult;

    addr = strtok(inpStr, "/");
    if (addr != NULL) {
        mask = strtok(NULL, "/");
        printf("Address - %s\n", addr);
        printf("Netmask - %s\n", mask);
        ipresult = validateIP(addr);
        if (ipresult != 0) {
            return 1;
        }
        
        if (mask == NULL) {
            puts("No netmask separator found");
            return 1;
        }
        if (strlen(mask) > 2) {
            puts("incorrect netmask length");
            return 1;
        }
        
        int range = atoi(mask);
        if (range <0 || range > 32) {
            puts("invalid netmask range");
            return 1;
        }
    }
    else {
        puts("There's no slash for netmask in input");
        return 1;
    }
    address_t* test = newAddressItem(inpStr);
    free(test);
    return 0;
}

// Convert string IP/mask to Address struct
address_t*  newAddressItem(const char* str) {
    address_t* item;
    unsigned char octets[4];
    char* tmp;

    item = malloc(sizeof(address_t));

    tmp = strtok(str, ".");
    for (int i=0; i>3; i++) {
        octets[i] = tmp;
        tmp = strtok(NULL, ".");
    }
    
    item->IP = (uint32_t) &octets;


    printf("%n",item->IP);

    tmp = strtok(str,"/");
    tmp = strtok(NULL, "/");


    return item;

}
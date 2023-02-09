#include <stdio.h>
//#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ipfuncs.h"
#include "ipdata.h"

typedef struct Address address_t;

// // Check separators count for IP part
// int dotsCount(const char* str) {
//     int dots = 0;
//     while (*str != '\0') {
//         if (*str++ == '.') dots++;
//     }
//     return dots;
// }
//
// // Check if string is number
// int isNumber(const char* str) {
//     int len = strlen(str);
//     for (int i=0; i<len; i++) {
//         if (!isdigit(str[i])) {
//             puts("Character in input!");
//             return(1);
//         }
//     }
//     return(0);
// }
//
// // Check if IP part is a valid IP address
// int validateIP(char* str) {
//     int dots = 0;
//
//     dots = dotsCount(str);
//     if (dots != 3) {
//         puts("Dots count not OK");
//         return 1;
//     }
//
//     char* part;
//     part = strtok(str, ".");
//     while (part != NULL) {
//         if (isNumber(part) != 0) return 1;
//         if (strlen(part) > 0 && *part == '0') {
//             puts("Address contains leading zeroes");
//             return 1;
//         }
//         int range = atoi(part);
//         if (range < 0 || range > 255) {
//             puts("Octet is not in range");
//             return 1;
//         }
//         part = strtok(NULL, ".");
//     }
//     return 0;
//     puts("valid");
// }

// Separate IP address and netmask from input
int parseInput(char* inpStr, address_t* parsed_item) {
    char* addr;
    char* mask;
    char bytes[4];
    int ipresult;
    // address_t item;

    addr = strtok(inpStr, "/");
    if (addr != NULL) {
        mask = strtok(NULL, "/");
        printf("Address - %s\n", addr);
        printf("Netmask - %s\n", mask);
        
        char* tmp = strtok(addr, ".");
        for (int i=0 ; i<4; i++) {
            bytes[i] = atoi(tmp);
            tmp = strtok(NULL, ".");
        }
        
        for (int j = 0; j<4; j++) {
            printf("%d\n", bytes[j]);
        }
        
        parsed_item->IP = bytes[3] | (bytes[2] << 8) | (bytes[1] << 16) | (bytes [0] << 24);
        parsed_item->Netmask = ~((1 << (32 - atoi(mask))) - 1);
        
        printf("%lu",(unsigned long)parsed_item->Netmask);

    }
    else {
        puts("Some shit happened");
        return 1;
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "ipfuncs.h"

char *inputIp(FILE* fp, size_t size) {
    char* str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(*str)*size);
    if (!str) return str;
    while (EOF!=(ch=fgetc(fp)) && ch != '\n') {
        str[len++] = ch;
        if (len == size) {
            str = realloc(str, sizeof(*str)*(size+=1));
            if (!str) return str;
        }
    }
    str[len++] = '\0';

    return realloc(str, sizeof(*str)*len);
}

int main (void) {
    // Input buffer
    char* m;

    // Buffers for IP address and netmask
    char address[16];
    char netmask[2];

    printf("Input something \n");
    m = inputIp(stdin,1);
    printf("%s\n",m);


//    int res = validateIPInput(m);

    parseInput(m);
    //printf(address, netmask);


//    validate input
//    print subnet
//    check if private
//    check if broadcast
//    chech if belongs to predefined nets

    free(m);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "ipfuncs.h"

char *inputIp(FILE* fp, size_t size) {
    char* str;
    int ch;
    size_t len = 0;
    str = (char*)realloc(NULL, sizeof(*str)*size);
    if (!str) return str;
    while (EOF!=(ch=fgetc(fp)) && ch != '\n') {
        str[len++] = ch;
        if (len == size) {
            str = (char*)realloc(str, sizeof(*str)*(size+=1));
            if (!str) return str;
        }
    }
    str[len++] = '\0';

    return (char*)realloc(str, sizeof(*str)*len);
}

int main (void) {
    // Input buffer
    char* m;

    printf("Input something \n");
    m = inputIp(stdin,1);
    // printf("%s\n",m);


//    int res = validateIPInput(m);
    struct Address addr;
    parseInput(m,&addr);
    // printf("%lu", (unsigned long) addr.IP);
    // printf("%lu", (unsigned long) addr.Netmask);

    getNetwork(&addr);
    checkPresetNetworks(&addr);    
    checkPrivateNetworks(&addr);
    checkIfBroadcast(&addr);
//    validate input
//    print subnet
//    check if private
//    check if broadcast
//    chech if belongs to predefined nets

    free(m);
    return 0;
}

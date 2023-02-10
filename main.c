#include <stdio.h>
#include <stdlib.h>
#include "ipfuncs.h"
#include "ipdata.h"

// Pure copy-paste in order to get dynamic string size for input.
// Sadly there's no additional validation yet, thus this stuff is completely useless.
char* inputIp(FILE* fp, size_t size) {
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

    printf("Input IP and mask as d.d.d.d/d or q to exit \n");
    m = inputIp(stdin,1);

    //I don't want to do typedef for single reference to struct =)
    while (doStuff(m) == 0){
        printf("\nInput another IP and mask as d.d.d.d/d or q to exit \n");
        m = inputIp(stdin,1);
    }

    // Free dynamically allocated memory for our input buffer.
    // Probably should do this after parseInput.
    free(m);
    return 0;
}

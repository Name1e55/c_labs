#ifndef IPDATA
#define IPDATA

#include <stdint.h>

// Struct for IP address and netmask stored bitwise in network order (little-endian)
struct Address {
    uint32_t IP;
    uint32_t Netmask;
} ;

extern char Network_1[];
extern char Network_2[];
extern char Network_3[];
extern char Network_4[];

extern char Private_1[];
extern char Private_2[];
extern char Private_3[];
extern char Private_4[];
extern char Private_5[];
#endif

#ifndef IPDATA
#define IPDATA

#include <stdint.h>

// Struct for IP address and netmask stored bitwise in network order (little-endian)
struct Address {
    uint32_t IP;
    uint32_t Netmask;
} ;

#endif

#ifndef IPFUNCS
#define IPFUNCS

// We need struct declaration to available
#include "ipdata.h"

// Don't need this since there's only doStuff needed outside the module now
//int parseInput(const char* inpStr, struct Address* parsed_item);
//int getNetwork(struct Address* addr);
//int checkPresetNetworks(struct Address* item);
//int checkPrivateNetworks(struct Address* item);
//int checkIfBroadcast(struct Address* item);
int doStuff(const char* str);

#endif

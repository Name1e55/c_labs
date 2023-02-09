#ifndef IPFUNCS
#define IPFUNCS

#include "ipdata.h"

int parseInput(const char* inpStr, struct Address* parsed_item);
int getNetwork(struct Address* addr);
int checkPresetNetworks(struct Address* item);
int checkPrivateNetworks(struct Address* item);
int checkIfBroadcast(struct Address* item);

#endif

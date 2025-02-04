#ifndef SHUFFLEGRASS_H
#define SHUFFLEGRASS_H

#include <z64.h>
#include <soh/OTRGlobals.h>

#ifdef __cplusplus
extern "C" {
#endif
void EnKusa_RandomizerInit(void* actorRef);
#ifdef __cplusplus
};
#endif

void ShuffleGrass_OnVanillaBehaviorHandler(GIVanillaBehavior id, bool* should, va_list originalArgs);

#endif //SHUFFLEGRASS_H

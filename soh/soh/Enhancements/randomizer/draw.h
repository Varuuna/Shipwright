#ifndef RANDODRAW_H
#define RANDODRAW_H
#pragma once

#include "../item-tables/ItemTableTypes.h"

typedef struct PlayState PlayState;

#ifdef __cplusplus
extern "C" {
#endif
void Randomizer_DrawSmallKey(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawMap(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawCompass(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawKeyRing(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawBossKey(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawBossSoul(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawDoubleDefense(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawMasterSword(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawTriforcePiece(PlayState* play, GetItemEntry getItemEntry);
void Randomizer_DrawTriforcePieceGI(PlayState* play, GetItemEntry getItemEntry);
void Randomizer_DrawOcarinaButton(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawBronzeScale(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawFishingPoleGI(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawSkeletonKey(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawMysteryItem(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawBombchuBagInLogic(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawBombchuBag(PlayState* play, GetItemEntry* getItemEntry);
void Randomizer_DrawOverworldKey(PlayState* play, GetItemEntry* getItemEntry);

#define GET_ITEM_MYSTERY                                                                                 \
    {                                                                                                    \
        ITEM_NONE_FE, 0, 0, 0, 0, MOD_RANDOMIZER, MOD_RANDOMIZER, ITEM_NONE_FE, 0, false, ITEM_FROM_NPC, \
            ITEM_CATEGORY_JUNK, ITEM_NONE_FE, MOD_RANDOMIZER, (CustomDrawFunc)Randomizer_DrawMysteryItem \
    }
#ifdef __cplusplus
};
#endif

#endif

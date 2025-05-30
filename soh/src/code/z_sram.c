#include "global.h"
#include "vt.h"

#include <string.h>
#include "soh/Enhancements/game-interactor/GameInteractor.h"
#include "soh/Enhancements/game-interactor/GameInteractor_Hooks.h"
#include "soh/Enhancements/randomizer/savefile.h"
#include "soh/OTRGlobals.h"
#include "soh/SaveManager.h"
#include "soh/ResourceManagerHelpers.h"

#define NUM_DUNGEONS 8
#define NUM_COWS 10

void Save_LoadFile(void);

void BossRush_InitSave(void);

/**
 *  Initialize new save.
 *  This save has an empty inventory with 3 hearts and single magic.
 */
void Sram_InitNewSave(void) {
    Save_InitFile(false);
}

/**
 *  Initialize debug save. This is also used on the Title Screen
 *  This save has a mostly full inventory with 10 hearts and single magic.
 *
 *  Some noteable flags that are set:
 *  Showed Mido sword/shield, met Deku Tree, Deku Tree mouth opened,
 *  used blue warp in Gohmas room, Zelda fled castle, light arrow cutscene watched,
 *  and set water level in Water Temple to lowest level.
 */
void Sram_InitDebugSave(void) {
    Save_InitFile(true);
}

void Sram_InitBossRushSave(void) {
    Save_InitFile(false);
    BossRush_InitSave();
}

static s16 sDungeonEntrances[] = {
    ENTR_DEKU_TREE_ENTRANCE,               // SCENE_DEKU_TREE
    ENTR_DODONGOS_CAVERN_ENTRANCE,         // SCENE_DODONGOS_CAVERN
    ENTR_JABU_JABU_ENTRANCE,               // SCENE_JABU_JABU
    ENTR_FOREST_TEMPLE_ENTRANCE,           // SCENE_FOREST_TEMPLE
    ENTR_FIRE_TEMPLE_ENTRANCE,             // SCENE_FIRE_TEMPLE
    ENTR_WATER_TEMPLE_ENTRANCE,            // SCENE_WATER_TEMPLE
    ENTR_SPIRIT_TEMPLE_ENTRANCE,           // SCENE_SPIRIT_TEMPLE
    ENTR_SHADOW_TEMPLE_ENTRANCE,           // SCENE_SHADOW_TEMPLE
    ENTR_BOTTOM_OF_THE_WELL_ENTRANCE,      // SCENE_BOTTOM_OF_THE_WELL
    ENTR_ICE_CAVERN_ENTRANCE,              // SCENE_ICE_CAVERN
    ENTR_GANONS_TOWER_0,                   // SCENE_GANONS_TOWER
    ENTR_GERUDO_TRAINING_GROUND_ENTRANCE,  // SCENE_GERUDO_TRAINING_GROUND
    ENTR_THIEVES_HIDEOUT_0,                // SCENE_THIEVES_HIDEOUT
    ENTR_INSIDE_GANONS_CASTLE_ENTRANCE,    // SCENE_INSIDE_GANONS_CASTLE
    ENTR_GANONS_TOWER_COLLAPSE_INTERIOR_0, // SCENE_GANONS_TOWER_COLLAPSE_INTERIOR
    ENTR_INSIDE_GANONS_CASTLE_COLLAPSE_0,  // SCENE_INSIDE_GANONS_CASTLE_COLLAPSE
};

/**
 *  Copy save currently on the buffer to Save Context and complete various tasks to open the save.
 *  This includes:
 *  - Set proper entrance depending on where the game was saved
 *  - If health is less than 3 hearts, give 3 hearts
 *  - If either scarecrow song is set, copy them from save context to the proper location
 *  - Handle a case where the player saved and quit after zelda cutscene but didnt get the song
 *  - Give and equip master sword if player is adult and doesnt have kokiri sword (bug?)
 *  - Revert any trade items that spoil
 */
void Sram_OpenSave() {
    u16 i;
    u16 j;
    u8* ptr;

    Save_LoadFile();

    switch (gSaveContext.savedSceneNum) {
        case SCENE_DEKU_TREE:
        case SCENE_DODONGOS_CAVERN:
        case SCENE_JABU_JABU:
        case SCENE_FOREST_TEMPLE:
        case SCENE_FIRE_TEMPLE:
        case SCENE_WATER_TEMPLE:
        case SCENE_SPIRIT_TEMPLE:
        case SCENE_SHADOW_TEMPLE:
        case SCENE_BOTTOM_OF_THE_WELL:
        case SCENE_ICE_CAVERN:
        case SCENE_GANONS_TOWER:
        case SCENE_GERUDO_TRAINING_GROUND:
        case SCENE_THIEVES_HIDEOUT:
        case SCENE_INSIDE_GANONS_CASTLE:
            gSaveContext.entranceIndex = sDungeonEntrances[gSaveContext.savedSceneNum];
            break;
        case SCENE_DEKU_TREE_BOSS:
            gSaveContext.entranceIndex = ENTR_DEKU_TREE_ENTRANCE;
            break;
        case SCENE_DODONGOS_CAVERN_BOSS:
            gSaveContext.entranceIndex = ENTR_DODONGOS_CAVERN_ENTRANCE;
            break;
        case SCENE_JABU_JABU_BOSS:
            gSaveContext.entranceIndex = ENTR_JABU_JABU_ENTRANCE;
            break;
        case SCENE_FOREST_TEMPLE_BOSS:
            gSaveContext.entranceIndex = ENTR_FOREST_TEMPLE_ENTRANCE;
            break;
        case SCENE_FIRE_TEMPLE_BOSS:
            gSaveContext.entranceIndex = ENTR_FIRE_TEMPLE_ENTRANCE;
            break;
        case SCENE_WATER_TEMPLE_BOSS:
            gSaveContext.entranceIndex = ENTR_WATER_TEMPLE_ENTRANCE;
            break;
        case SCENE_SPIRIT_TEMPLE_BOSS:
            gSaveContext.entranceIndex = ENTR_SPIRIT_TEMPLE_ENTRANCE;
            break;
        case SCENE_SHADOW_TEMPLE_BOSS:
            gSaveContext.entranceIndex = ENTR_SHADOW_TEMPLE_ENTRANCE;
            break;
        case SCENE_GANONS_TOWER_COLLAPSE_INTERIOR:
        case SCENE_INSIDE_GANONS_CASTLE_COLLAPSE:
        case SCENE_GANONDORF_BOSS:
        case SCENE_GANONS_TOWER_COLLAPSE_EXTERIOR:
        case SCENE_GANON_BOSS:
            gSaveContext.entranceIndex = ENTR_GANONS_TOWER_0;
            break;

        default:
            // Use the saved entrance value with remember save location, except when in grottos/fairy fountains
            if (CVarGetInteger(CVAR_ENHANCEMENT("RememberSaveLocation"), 0) &&
                gSaveContext.savedSceneNum != SCENE_FAIRYS_FOUNTAIN && gSaveContext.savedSceneNum != SCENE_GROTTOS) {
                break;
            }

            if (gSaveContext.savedSceneNum != SCENE_LINKS_HOUSE) {
                gSaveContext.entranceIndex =
                    (LINK_AGE_IN_YEARS == YEARS_CHILD) ? ENTR_LINKS_HOUSE_CHILD_SPAWN : ENTR_TEMPLE_OF_TIME_WARP_PAD;
            } else {
                gSaveContext.entranceIndex = ENTR_LINKS_HOUSE_CHILD_SPAWN;
            }
            break;
    }

    if (!CVarGetInteger(CVAR_ENHANCEMENT("PersistentMasks"), 0)) {
        gSaveContext.ship.maskMemory = PLAYER_MASK_NONE;
    }

    osSyncPrintf("scene_no = %d\n", gSaveContext.entranceIndex);
    osSyncPrintf(VT_RST);

    if (gSaveContext.health < 0x30) {
        gSaveContext.health =
            CVarGetInteger(CVAR_ENHANCEMENT("FullHealthSpawn"), 0) ? gSaveContext.healthCapacity : 0x30;
    }

    if (gSaveContext.scarecrowLongSongSet) {
        osSyncPrintf(VT_FGCOL(BLUE));
        osSyncPrintf("\n====================================================================\n");

        memcpy(gScarecrowCustomSongPtr, gSaveContext.scarecrowLongSong, sizeof(gSaveContext.scarecrowLongSong));

        ptr = (u8*)gScarecrowCustomSongPtr;
        for (i = 0; i < ARRAY_COUNT(gSaveContext.scarecrowLongSong); i++, ptr++) {
            osSyncPrintf("%d, ", *ptr);
        }

        osSyncPrintf("\n====================================================================\n");
        osSyncPrintf(VT_RST);
    }

    if (gSaveContext.scarecrowSpawnSongSet) {
        osSyncPrintf(VT_FGCOL(GREEN));
        osSyncPrintf("\n====================================================================\n");

        memcpy(gScarecrowSpawnSongPtr, gSaveContext.scarecrowSpawnSong, sizeof(gSaveContext.scarecrowSpawnSong));

        ptr = gScarecrowSpawnSongPtr;
        for (i = 0; i < ARRAY_COUNT(gSaveContext.scarecrowSpawnSong); i++, ptr++) {
            osSyncPrintf("%d, ", *ptr);
        }

        osSyncPrintf("\n====================================================================\n");
        osSyncPrintf(VT_RST);
    }

    // if zelda cutscene has been watched but lullaby was not obtained, restore cutscene and take away letter
    if ((Flags_GetEventChkInf(EVENTCHKINF_OBTAINED_ZELDAS_LETTER)) && !CHECK_QUEST_ITEM(QUEST_SONG_LULLABY) &&
        !IS_RANDO) {
        i = gSaveContext.eventChkInf[4] & ~1;
        gSaveContext.eventChkInf[4] = i;

        INV_CONTENT(ITEM_LETTER_ZELDA) = ITEM_CHICKEN;

        for (j = 1; j < ARRAY_COUNT(gSaveContext.equips.buttonItems); j++) {
            if (gSaveContext.equips.buttonItems[j] == ITEM_LETTER_ZELDA) {
                gSaveContext.equips.buttonItems[j] = ITEM_CHICKEN;
            }
        }
    }

    if (LINK_AGE_IN_YEARS == YEARS_ADULT && !CHECK_OWNED_EQUIP(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER)) {
        if (!IS_RANDO || !Randomizer_GetSettingValue(RSK_SHUFFLE_MASTER_SWORD)) {
            gSaveContext.inventory.equipment |= OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER);
            gSaveContext.equips.buttonItems[0] = ITEM_SWORD_MASTER;
            gSaveContext.equips.equipment &= ~(0xF << (EQUIP_TYPE_SWORD * 4));
            gSaveContext.equips.equipment |= EQUIP_VALUE_SWORD_MASTER << (EQUIP_TYPE_SWORD * 4);
        }
    }

    if (GameInteractor_Should(VB_REVERT_SPOILING_ITEMS, true)) {
        for (i = 0; i < ARRAY_COUNT(gSpoilingItems); i++) {
            if (INV_CONTENT(ITEM_TRADE_ADULT) == gSpoilingItems[i]) {
                INV_CONTENT(gSpoilingItemReverts[i]) = gSpoilingItemReverts[i];

                for (j = 1; j < ARRAY_COUNT(gSaveContext.equips.buttonItems); j++) {
                    if (gSaveContext.equips.buttonItems[j] == gSpoilingItems[i]) {
                        gSaveContext.equips.buttonItems[j] = gSpoilingItemReverts[i];
                    }
                }
            }
        }
    }

    gSaveContext.magicLevel = 0;
}

void Sram_InitSave(FileChooseContext* fileChooseCtx) {
    u16 offset;
    u16 j;
    u16* ptr;
    u16 checksum;

    if (fileChooseCtx->buttonIndex != 0 || !CVarGetInteger(CVAR_DEVELOPER_TOOLS("DebugEnabled"), 0)) {
        Sram_InitNewSave();
    } else {
        Sram_InitDebugSave();
    }

    gSaveContext.entranceIndex = ENTR_LINKS_HOUSE_CHILD_SPAWN;
    gSaveContext.linkAge = 1;
    gSaveContext.dayTime = 0x6AAB;
    gSaveContext.cutsceneIndex = 0xFFF1;
    if (ResourceMgr_GetGameRegion(0) == GAME_REGION_PAL && gSaveContext.language != LANGUAGE_JPN) {
        gSaveContext.ship.filenameLanguage = NAME_LANGUAGE_PAL;
    } else { // GAME_REGION_NTSC
        gSaveContext.ship.filenameLanguage =
            (gSaveContext.language == LANGUAGE_JPN) ? NAME_LANGUAGE_NTSC_JPN : NAME_LANGUAGE_NTSC_ENG;
    }

    if ((fileChooseCtx->buttonIndex == 0 && CVarGetInteger(CVAR_DEVELOPER_TOOLS("DebugEnabled"), 0))) {
        gSaveContext.cutsceneIndex = 0;
    }

    for (offset = 0; offset < 8; offset++) {
        gSaveContext.playerName[offset] = Save_GetSaveMetaInfo(fileChooseCtx->buttonIndex)->playerName[offset];
    }

    gSaveContext.n64ddFlag = fileChooseCtx->n64ddFlag;

    u8 currentQuest = fileChooseCtx->questType[fileChooseCtx->buttonIndex];

    if (currentQuest == QUEST_RANDOMIZER && (Randomizer_IsSeedGenerated() || Randomizer_IsSpoilerLoaded())) {
        gSaveContext.ship.quest.id = QUEST_RANDOMIZER;

        Randomizer_InitSaveFile();
    } else {
        gSaveContext.ship.quest.id = currentQuest;
    }

    Save_SaveFile();
    SaveManager_ThreadPoolWait();
}

void Sram_InitSram(GameState* gameState) {
    Save_Init();

    func_800F6700(gSaveContext.audioSetting);
}

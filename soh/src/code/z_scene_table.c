#include "global.h"

#include "scenes/overworld/spot00/spot00_scene.h"
#include "scenes/overworld/spot00/spot00_room_0.h"
#include "scenes/overworld/spot01/spot01_scene.h"
#include "scenes/overworld/spot07/spot07_scene.h"
#include "scenes/overworld/spot12/spot12_scene.h"
#include "scenes/overworld/spot16/spot16_scene.h"
#include "scenes/overworld/spot16/spot16_room_0.h"
#include "scenes/overworld/spot18/spot18_scene.h"
#include "scenes/overworld/spot20/spot20_scene.h"
#include "scenes/overworld/souko/souko_scene.h"

#include "scenes/dungeons/men/men_scene.h"
#include "scenes/dungeons/ddan/ddan_scene.h"
#include "scenes/dungeons/ydan/ydan_scene.h"
#include "scenes/dungeons/Bmori1/Bmori1_scene.h"
#include "scenes/dungeons/MIZUsin/MIZUsin_scene.h"
#include "scenes/dungeons/gerudoway/gerudoway_scene.h"
#include "scenes/dungeons/jyasinzou/jyasinzou_scene.h"
#include "scenes/indoors/miharigoya/miharigoya_scene.h"
#include "scenes/dungeons/ice_doukutu/ice_doukutu_scene.h"

#include "overlays/actors/ovl_Bg_Dodoago/z_bg_dodoago.h"

#include "soh/mq_asset_hacks.h"
#include "soh/OTRGlobals.h"
#include "soh/ResourceManagerHelpers.h"

// Entrance Table definition
#define DEFINE_ENTRANCE(_0, sceneId, spawn, continueBgm, displayTitleCard, endTransType, startTransType) \
    { sceneId, spawn,                                                                                    \
      (((continueBgm) ? ENTRANCE_INFO_CONTINUE_BGM_FLAG : 0) |                                           \
       ((displayTitleCard) ? ENTRANCE_INFO_DISPLAY_TITLE_CARD_FLAG : 0) |                                \
       (((endTransType) << ENTRANCE_INFO_END_TRANS_TYPE_SHIFT) & ENTRANCE_INFO_END_TRANS_TYPE_MASK) |    \
       (((startTransType) << ENTRANCE_INFO_START_TRANS_TYPE_SHIFT) & ENTRANCE_INFO_START_TRANS_TYPE_MASK)) },

EntranceInfo gEntranceTable[] = {
#include "tables/entrance_table.h"
};

#undef DEFINE_ENTRANCE

//#define TITLED_SCENE(name, title, unk_10, config, unk_12)                                            \
//    {                                                                                                \
//        (u32) _##name##SegmentRomStart, (u32)_##name##SegmentRomEnd, (u32)_##title##SegmentRomStart, \
//            (u32)_##title##SegmentRomEnd, unk_10, config, unk_12, 0                                  \
//    }

//#define TITLED_SCENE(name, title, unk_10, config, unk_12)                                            \
//    {                                                                                                \
//        {0, 0, #name}, {0, 0, #title}, unk_10, config, unk_12, 0                                     \
//    }

//#define UNTITLED_SCENE(name, unk_10, config, unk_12) \
    //{ (u32) _##name##SegmentRomStart, (u32)_##name##SegmentRomEnd, 0, 0, unk_10, config, unk_12, 0 }

//#define UNTITLED_SCENE(name, unk_10, config, unk_12) \
//    { { 0, 0, #name }, (u32)0, 0, 0, unk_10, config, unk_12, 0 }

#define DEFINE_SCENE(name, title, _2, config, unk_10, unk_12) \
    { { 0, 0, #name }, { 0, 0, #title }, unk_10, config, unk_12, 0 },

// Handle `none` as a special case for scenes without a title card
#define none ""

SceneTableEntry gSceneTable[] = {
#include "tables/scene_table.h"
};

#undef none

#undef DEFINE_SCENE

Gfx sDefaultDisplayList[] = {
    gsSPSegment(0x08, gEmptyDL),
    gsSPSegment(0x09, gEmptyDL),
    gsSPSegment(0x0A, gEmptyDL),
    gsSPSegment(0x0B, gEmptyDL),
    gsSPSegment(0x0C, gEmptyDL),
    gsSPSegment(0x0D, gEmptyDL),
    gsDPPipeSync(),
    gsDPSetPrimColor(0, 0, 128, 128, 128, 128),
    gsDPSetEnvColor(128, 128, 128, 128),
    gsSPEndDisplayList(),
};

// Computes next entrance index based on age and day time to set the fade out transition
void Scene_SetTransitionForNextEntrance(PlayState* play) {
    s16 entranceIndex;

    if (!IS_DAY) {
        if (!LINK_IS_ADULT) {
            entranceIndex = play->nextEntranceIndex + 1;
        } else {
            entranceIndex = play->nextEntranceIndex + 3;
        }
    } else {
        if (!LINK_IS_ADULT) {
            entranceIndex = play->nextEntranceIndex;
        } else {
            entranceIndex = play->nextEntranceIndex + 2;
        }
    }

    play->transitionType = ENTRANCE_INFO_START_TRANS_TYPE(gEntranceTable[entranceIndex].field); // Fade out
}

// Scene Draw Config 0
void func_80099550(PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    gSPDisplayList(POLY_OPA_DISP++, sDefaultDisplayList);
    gSPDisplayList(POLY_XLU_DISP++, sDefaultDisplayList);

    CLOSE_DISPS(play->state.gfxCtx);
}

void* D_8012A2F8[] = {
    gYdanTex_00BA18,
    gYdanTex_00CA18,
};

// Scene Draw Config 19
void func_800995DC(PlayState* play) {
    u32 gameplayFrames = play->gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - (gameplayFrames % 128), (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    { s32 pad; }

    if (ResourceMgr_IsGameMasterQuest()) {
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A2F8_MQ[gSaveContext.nightFlag]));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A2F8[gSaveContext.nightFlag]));
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 28
void func_80099760(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, (gameplayFrames * 2) % 256, 0, 64, 32, 1, 0,
                                (gameplayFrames * 2) % 128, 64, 32));

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

void* gDCEntranceTextures[] = {
    gDCDayEntranceTex,
    gDCNightEntranceTex,
};
void* sDCLavaFloorTextures[] = {
    gDCLavaFloor1Tex, gDCLavaFloor2Tex, gDCLavaFloor3Tex, gDCLavaFloor4Tex,
    gDCLavaFloor5Tex, gDCLavaFloor6Tex, gDCLavaFloor7Tex, gDCLavaFloor8Tex,
};

// Scene Draw Config 20 - Dodongo's Cavern
void func_80099878(PlayState* play) {
    u32 gameplayFrames;
    s32 pad;
    Gfx* displayListHead = Graph_Alloc(play->state.gfxCtx, 2 * sizeof(Gfx[3]));

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    if (ResourceMgr_IsGameMasterQuest()) {
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(gDCEntranceTextures_MQ[gSaveContext.nightFlag]));
        gSPSegment(POLY_OPA_DISP++, 0x09,
                   SEGMENTED_TO_VIRTUAL(sDCLavaFloorTextures_MQ[(s32)(gameplayFrames & 14) >> 1]));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(gDCEntranceTextures[gSaveContext.nightFlag]));
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sDCLavaFloorTextures[(s32)(gameplayFrames & 14) >> 1]));
    }
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, (gameplayFrames * 1) % 256, 0, 64, 32, 1, 0,
                                (gameplayFrames * 1) % 128, 64, 32));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 128, 32, 32, 1, 0,
                                (gameplayFrames * 2) % 128, 32, 32));

    { s32 pad2[2]; }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gSPSegment(POLY_OPA_DISP++, 0x0B, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 255, 255, 255, play->roomCtx.unk_74[BGDODOAGO_EYE_LEFT]);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_OPA_DISP++, 0x0C, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 255, 255, 255, play->roomCtx.unk_74[BGDODOAGO_EYE_RIGHT]);
    gSPEndDisplayList(displayListHead);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 30
void func_80099BD8(PlayState* play) {
    f32 temp;
    Gfx* displayListHead = Graph_Alloc(play->state.gfxCtx, 18 * sizeof(Gfx));

    OPEN_DISPS(play->state.gfxCtx);

    temp = play->roomCtx.unk_74[0] / 255.0f;

    gSPSegment(POLY_XLU_DISP++, 0x08, displayListHead);
    gSPSegment(POLY_OPA_DISP++, 0x08, displayListHead);
    gDPSetPrimColor(displayListHead++, 0, 0, 255 - (u8)(185.0f * temp), 255 - (u8)(145.0f * temp),
                    255 - (u8)(105.0f * temp), 255);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_XLU_DISP++, 0x09, displayListHead);
    gSPSegment(POLY_OPA_DISP++, 0x09, displayListHead);
    gDPSetPrimColor(displayListHead++, 0, 0, 76 + (u8)(6.0f * temp), 76 + (u8)(34.0f * temp), 76 + (u8)(74.0f * temp),
                    255);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_OPA_DISP++, 0x0A, displayListHead);
    gSPSegment(POLY_XLU_DISP++, 0x0A, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 0, 0, 0, play->roomCtx.unk_74[0]);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_OPA_DISP++, 0x0B, displayListHead);
    gSPSegment(POLY_XLU_DISP++, 0x0B, displayListHead);
    gDPSetPrimColor(displayListHead++, 0, 0, 89 + (u8)(166.0f * temp), 89 + (u8)(166.0f * temp),
                    89 + (u8)(166.0f * temp), 255);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 0, 0, 0, play->roomCtx.unk_74[0]);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_OPA_DISP++, 0x0C, displayListHead);
    gSPSegment(POLY_XLU_DISP++, 0x0C, displayListHead);
    gDPSetPrimColor(displayListHead++, 0, 0, 255 + (u8)(179.0f * temp), 255 + (u8)(179.0f * temp),
                    255 + (u8)(179.0f * temp), 255);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 0, 0, 0, play->roomCtx.unk_74[0]);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_OPA_DISP++, 0x0D, displayListHead);
    gSPSegment(POLY_XLU_DISP++, 0x0D, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 0, 0, 0, play->roomCtx.unk_74[1]);
    gSPEndDisplayList(displayListHead);

    CLOSE_DISPS(play->state.gfxCtx);

    if (gSaveContext.sceneSetupIndex == 5) {
        gCustomLensFlareOn = true;
        gCustomLensFlarePos.x = -20.0f;
        gCustomLensFlarePos.y = 1220.0f;
        gCustomLensFlarePos.z = -684.0f;
        gLensFlareScale = 10;
        gLensFlareColorIntensity = 8.0f;
        gLensFlareScreenFillAlpha = 200;
    }
}

// Scene Draw Config 31
void func_8009A45C(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 0, (gameplayFrames * 1) % 64, 256, 16));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - (gameplayFrames % 128), (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 32, 1, 0, 127 - (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x0B, Gfx_TexScroll(play->state.gfxCtx, 0, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0C,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 50) % 2048, 8, 512, 1, 0,
                                (gameplayFrames * 60) % 2048, 8, 512));
    gSPSegment(POLY_OPA_DISP++, 0x0D,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0, (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 32
void func_8009A798(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 0, (gameplayFrames * 2) % 256, 64, 64));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - (gameplayFrames * 1) % 128, (gameplayFrames * 1) % 256, 32,
                                64, 1, 0, 0, 32, 128));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 33
void func_8009A9DC(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 256, 32, 64,
                                1, gameplayFrames % 128, (gameplayFrames * 3) % 256, 32, 64));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 48
void func_8009AB98(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 0, gameplayFrames % 64, 256, 16));

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 39
void func_8009ACA8(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09, Gfx_TexScroll(play->state.gfxCtx, 0, gameplayFrames % 64, 256, 16));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 24
void func_8009AE30(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;

    if (play->sceneNum == SCENE_SHADOW_TEMPLE_BOSS) {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, 0, (gameplayFrames * 2) % 128, 0, 32, 32, 1,
                                    (gameplayFrames * 2) % 128, 0, 32, 32));
    } else {
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, 0, (gameplayFrames * 2) % 128, 0, 32, 32, 1,
                                    (gameplayFrames * 2) % 128, 0, 32, 32));
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

void* sThievesHideoutEntranceTextures[] = {
    gThievesHideoutDayEntranceTex,
    gThievesHideoutNightEntranceTex,
};

// Scene Draw Config 40
void func_8009AFE0(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x09, Gfx_TexScroll(play->state.gfxCtx, 0, (gameplayFrames * 3) % 128, 32, 32));

    { s32 pad[2]; }

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sThievesHideoutEntranceTextures[gSaveContext.nightFlag]));

    CLOSE_DISPS(play->state.gfxCtx);
}

void* D_8012A330[] = {
    gWaterTempleDayEntranceTex,
    gWaterTempleNightEntranceTex,
};

// Scene Draw Config 23 (Water Temple)
void func_8009B0FC(PlayState* play) {
    u32 gameplayFrames;
    s32 spB0;
    s32 spAC;

    OPEN_DISPS(play->state.gfxCtx);

    spB0 = (play->roomCtx.unk_74[1] >> 8) & 0xFF;
    spAC = play->roomCtx.unk_74[1] & 0xFF;
    gameplayFrames = play->gameplayFrames;

    if (ResourceMgr_IsGameMasterQuest()) {
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A330_MQ[gSaveContext.nightFlag]));
    } else {
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A330[gSaveContext.nightFlag]));
    }

    if (spB0 == 1) {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0,
                                            0, spAC));
    } else if (spB0 < 1) {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0,
                                            0, 255));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0,
                                            0, 160));
    }

    if (spB0 == 2) {
        gSPSegment(POLY_OPA_DISP++, 0x09,
                   Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0,
                                            0, spAC));
    } else if (spB0 < 2) {
        gSPSegment(POLY_OPA_DISP++, 0x09,
                   Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0,
                                            0, 255));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x09,
                   Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0,
                                            0, 160));
    }

    if (spB0 != 0) {
        gSPSegment(POLY_OPA_DISP++, 0x0A,
                   Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0,
                                            0, 160));
        gSPSegment(POLY_OPA_DISP++, 0x0B,
                   Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames * 3, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0,
                                            0, 180));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x0A,
                   Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, (gameplayFrames * 1) % 128, 0, 32, 32, 1, 0, 0, 32,
                                            32, 0, 0, 0, 160 + (s32)((spAC / 200.0f) * 95.0f)));
        gSPSegment(POLY_OPA_DISP++, 0x0B,
                   Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames * 3, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0,
                                            0, 185 + (s32)((spAC / 200.0f) * 70.0f)));
    }

    gSPSegment(POLY_XLU_DISP++, 0x0C,
               Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames * 1, gameplayFrames * 1, 32, 32, 1, 0,
                                        127 - (gameplayFrames * 1), 32, 32, 0, 0, 0, 128));
    gSPSegment(POLY_XLU_DISP++, 0x0D,
               Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames * 4, 0, 32, 32, 1, gameplayFrames * 4, 0,
                                        32, 32, 0, 0, 0, 128));

    { s32 pad[2]; }

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 29
void func_8009B86C(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, gameplayFrames * 1, 0, 32, 32, 1, 0, 0, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, play->roomCtx.unk_74[0]);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 145);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 34
void func_8009B9BC(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 0, gameplayFrames % 64, 4, 16));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 35
void func_8009BAA4(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));

    if (play->sceneNum == SCENE_CASTLE_COURTYARD_GUARDS_DAY) {
        gSPSegment(POLY_XLU_DISP++, 0x09, Gfx_TexScroll(play->state.gfxCtx, 0, (gameplayFrames * 10) % 256, 32, 64));
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 36
void func_8009BC44(PlayState* play) {
    u32 gameplayFrames;
    s8 sp83;

    sp83 = coss((play->gameplayFrames * 1500) & 0xFFFF) >> 8;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;

    if (play->sceneNum == SCENE_OUTSIDE_GANONS_CASTLE) {
        gSPSegment(POLY_XLU_DISP++, 0x09, Gfx_TexScroll(play->state.gfxCtx, 0, (gameplayFrames * 1) % 256, 64, 64));
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 255 - (gameplayFrames * 1) % 256, 64, 64, 1, 0,
                                    (gameplayFrames * 1) % 256, 64, 64));
    }

    gSPSegment(POLY_OPA_DISP++, 0x0B,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 255 - (gameplayFrames * 1) % 128, (gameplayFrames * 1) % 128, 32,
                                32, 1, (gameplayFrames * 1) % 128, (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    sp83 = (sp83 >> 1) + 192;
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, sp83, sp83, sp83, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Screen Shake for Ganon's Tower Collapse
void func_8009BEEC(PlayState* play) {
    s32 var;

    if (play->gameplayFrames % 128 == 13) {
        var = Quake_Add(GET_ACTIVE_CAM(play), 2);
        Quake_SetSpeed(var, 10000);
        Quake_SetQuakeValues(var, 4, 0, 0, 0);
        Quake_SetCountdown(var, 127);
    }

    if ((play->gameplayFrames % 64 == 0) && (Rand_ZeroOne() > 0.6f)) {
        var = Quake_Add(GET_ACTIVE_CAM(play), 3);
        Quake_SetSpeed(var, 32000.0f + (Rand_ZeroOne() * 3000.0f));
        Quake_SetQuakeValues(var, 10.0f - (Rand_ZeroOne() * 9.0f), 0, 0, 0);
        Quake_SetCountdown(var, 48.0f - (Rand_ZeroOne() * 15.0f));
    }
}

// Scene Draw Config 38
void func_8009C0AC(PlayState* play) {
    u32 gameplayFrames;
    s8 sp7B;

    sp7B = coss((play->gameplayFrames * 1500) & 0xFFFF) >> 8;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 512, 64, 128, 1, 0,
                                511 - (gameplayFrames * 1) % 512, 64, 128));
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 256, 32, 64, 1, 0,
                                255 - (gameplayFrames * 1) % 256, 32, 64));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 20) % 2048, 16, 512, 1, 0,
                                (gameplayFrames * 30) % 2048, 16, 512));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    sp7B = (sp7B >> 1) + 192;
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, sp7B, sp7B, sp7B, 128);

    CLOSE_DISPS(play->state.gfxCtx);

    if (Flags_GetSwitch(play, 0x37)) {
        if ((play->sceneNum == SCENE_GANON_BOSS) || (play->sceneNum == SCENE_GANONS_TOWER_COLLAPSE_EXTERIOR) ||
            (play->sceneNum == SCENE_GANONS_TOWER_COLLAPSE_INTERIOR) ||
            (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE_COLLAPSE)) {
            func_8009BEEC(play);
        }
    }
}

void* sIceCavernEntranceTextures[] = {
    gIceCavernDayEntranceTex,
    gIceCavernNightEntranceTex,
};

// Scene Draw Config 37
void func_8009C3EC(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    if (ResourceMgr_IsGameMasterQuest()) {
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sIceCavernEntranceTextures_MQ[gSaveContext.nightFlag]));
    } else {
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sIceCavernEntranceTextures[gSaveContext.nightFlag]));
    }
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    { s32 pad[2]; }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 42
void func_8009C608(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 0, (gameplayFrames * 1) % 64, 256, 16));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 60) % 2048, 8, 512, 1, 0,
                                (gameplayFrames * 50) % 2048, 8, 512));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - (gameplayFrames * 1) % 128, 0, 32, 32, 1,
                                (gameplayFrames * 1) % 128, 0, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0B,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 1023 - (gameplayFrames * 6) % 1024, 16, 256, 1, 0,
                                1023 - (gameplayFrames * 3) % 1024, 16, 256));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 43
void func_8009C8B8(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 32, 1, 0, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09, Gfx_TexScroll(play->state.gfxCtx, 0, 255 - (gameplayFrames * 10) % 256, 32, 64));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 47
void func_8009CAC0(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

void* sGTGEntranceTextures[] = {
    gGTGDayEntranceTex,
    gGTGNightEntranceTex,
};

// Scene Draw Config 27
void func_8009CC00(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    if (ResourceMgr_IsGameMasterQuest()) {
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sGTGEntranceTextures_MQ[gSaveContext.nightFlag]));
    } else {
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sGTGEntranceTextures[gSaveContext.nightFlag]));
    }
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    { s32 pad[2]; }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

Gfx* Gfx_TwoTexScrollPrimColor(GraphicsContext* gfxCtx, s32 tile1, u32 x1, u32 y1, s32 width1, s32 height1, s32 tile2,
                               u32 x2, u32 y2, s32 width2, s32 height2, s32 r, s32 g, s32 b, s32 a) {
    Gfx* displayList = Graph_Alloc(gfxCtx, 10 * sizeof(Gfx));

    x1 %= 2048;
    y1 %= 2048;
    x2 %= 2048;
    y2 %= 2048;

    gDPTileSync(displayList);
    gDPSetTileSize(displayList + 1, tile1, x1, y1, (x1 + ((width1 - 1) << 2)), (y1 + ((height1 - 1) << 2)));
    gDPTileSync(displayList + 2);
    gDPSetTileSize(displayList + 3, tile2, x2, y2, (x2 + ((width2 - 1) << 2)), (y2 + ((height2 - 1) << 2)));
    gDPSetPrimColor(displayList + 4, 0, 0, r, g, b, a);
    gSPEndDisplayList(displayList + 5);

    return displayList;
}

// Scene Draw Config 50
void func_8009CF84(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScrollPrimColor(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128,
                                         32, 32, 1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32, 255, 255,
                                         255, play->roomCtx.unk_74[0] + 127));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 41
void func_8009D0E8(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 127 - (gameplayFrames * 4) % 128, 0, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x09, Gfx_TexScroll(play->state.gfxCtx, 0, (gameplayFrames * 5) % 64, 16, 16));
    gSPSegment(POLY_OPA_DISP++, 0x0A, Gfx_TexScroll(play->state.gfxCtx, 0, 63 - (gameplayFrames * 2) % 64, 16, 16));
    gSPSegment(POLY_XLU_DISP++, 0x0B,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 127 - (gameplayFrames * 3) % 128, 32, 32, 1, 0, 0, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

void* sLonLonHouseEntranceTextures[] = {
    gLonLonHouseDayEntranceTex,
    gLonLonHouseNightEntranceTex,
};

// Scene Draw Config 44
void func_8009D31C(PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    { s32 pad[2]; }

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sLonLonHouseEntranceTextures[gSaveContext.nightFlag]));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

void* sGuardHouseView2Textures[] = {
    gGuardHouseOutSideView1DayTex,
    gGuardHouseOutSideView1NightTex,
};
void* sGuardHouseView1Textures[] = {
    gGuardHouseOutSideView2DayTex,
    gGuardHouseOutSideView2NightTex,
};

// Scene Draw Config 45
void func_8009D438(PlayState* play) {
    s32 var;

    OPEN_DISPS(play->state.gfxCtx);

    if (LINK_IS_ADULT) {
        var = 1;
    } else {
        var = gSaveContext.nightFlag;
    }

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sGuardHouseView1Textures[var]));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sGuardHouseView2Textures[var]));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 46
void func_8009D5B4(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 0, (gameplayFrames * 3) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 1023 - (gameplayFrames * 3) % 1024, 16, 256, 1, 0,
                                1023 - (gameplayFrames * 6) % 1024, 16, 256));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

void* sForestTempleEntranceTextures[] = {
    gForestTempleDayEntranceTex,
    gForestTempleNightEntranceTex,
};

// Scene Draw Config 22
void func_8009D758(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    if (ResourceMgr_IsGameMasterQuest()) {
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   SEGMENTED_TO_VIRTUAL(sForestTempleEntranceTextures_MQ[gSaveContext.nightFlag]));
    } else {
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sForestTempleEntranceTextures[gSaveContext.nightFlag]));
    }
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    { s32 pad[2]; }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

void* sSpiritTempleEntranceTextures[] = {
    gSpiritTempleDayEntranceTex,
    gSpiritTempleNightEntranceTex,
};

// Scene Draw Config 25
void func_8009D974(PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    { s32 pad[2]; }

    if (ResourceMgr_IsGameMasterQuest()) {
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   SEGMENTED_TO_VIRTUAL(sSpiritTempleEntranceTextures_MQ[gSaveContext.nightFlag]));
    } else {
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sSpiritTempleEntranceTextures[gSaveContext.nightFlag]));
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 1
void func_8009DA30(PlayState* play) {
    u32 gameplayFrames;
    Gfx* displayListHead;

    displayListHead = Graph_Alloc(play->state.gfxCtx, 3 * sizeof(Gfx));

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 10) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 10) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gSPSegment(POLY_XLU_DISP++, 0x0A, displayListHead);

    if ((gSaveContext.dayTime >= 0x4AAC) && (gSaveContext.dayTime <= 0xC555)) {
        gSPEndDisplayList(displayListHead);
    } else {
        if (gSaveContext.dayTime > 0xC555) {
            if (play->roomCtx.unk_74[0] != 255) {
                Math_StepToS(&play->roomCtx.unk_74[0], 255, 5);
            }
        } else if (gSaveContext.dayTime >= 0x4000) {
            if (play->roomCtx.unk_74[0] != 0) {
                Math_StepToS(&play->roomCtx.unk_74[0], 0, 10);
            }
        }

        gDPSetPrimColor(displayListHead++, 0, 0, 255, 255, 255, play->roomCtx.unk_74[0]);
        gSPDisplayList(displayListHead++, spot00_room_0DL_012B20);
        gSPEndDisplayList(displayListHead);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void* sKakarikoWindowTextures[] = {
    gKakarikoVillageDayWindowTex,
    gKakarikoVillageNightWindowTex,
};

// Scene Draw Config 2
void func_8009DD5C(PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    { s32 pad[2]; }

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sKakarikoWindowTextures[gSaveContext.nightFlag]));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 3
void func_8009DE78(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 6) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 6) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 4
void func_8009E0B8(PlayState* play) {
    u32 gameplayFrames;
    u8 spA3;
    u16 spA0;
    Gfx* displayListHead;

    spA3 = 128;
    spA0 = 500;
    displayListHead = Graph_Alloc(play->state.gfxCtx, 6 * sizeof(Gfx));

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 10) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 10) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    if (gSaveContext.sceneSetupIndex == 4) {
        spA3 = 255 - (u8)play->roomCtx.unk_74[0];
    } else if (gSaveContext.sceneSetupIndex == 6) {
        spA0 = play->roomCtx.unk_74[0] + 500;
    } else if (((gSaveContext.sceneSetupIndex < 4) || LINK_IS_ADULT) &&
               (Flags_GetEventChkInf(EVENTCHKINF_OBTAINED_KOKIRI_EMERALD_DEKU_TREE_DEAD))) {
        spA0 = 2150;
    }

    gSPSegment(POLY_OPA_DISP++, 0x0A, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 128, 128, 128, spA3);
    gSPEndDisplayList(displayListHead++);

    gSPSegment(POLY_XLU_DISP++, 0x0B, displayListHead);
    gSPSegment(POLY_OPA_DISP++, 0x0B, displayListHead);
    gDPPipeSync(displayListHead++);
    gDPSetEnvColor(displayListHead++, 128, 128, 128, spA0 * 0.1f);
    gSPEndDisplayList(displayListHead);

    gSPSegment(POLY_OPA_DISP++, 0x0C,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (s16)(-play->roomCtx.unk_74[0] * 0.02f), 32, 16, 1, 0,
                                (s16)(-play->roomCtx.unk_74[0] * 0.02f), 32, 16));

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 5
void func_8009E54C(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    if ((gSaveContext.sceneSetupIndex > 3) ||
        (LINK_IS_ADULT && !Flags_GetEventChkInf(EVENTCHKINF_RAISED_LAKE_HYLIA_WATER))) {
        play->roomCtx.unk_74[0] = 87;
    }

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, gameplayFrames, gameplayFrames, 32, 32, 1, 0, 0, 32, 32,
                                        0, 0, 0, play->roomCtx.unk_74[0] + 168));
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScrollEnvColor(play->state.gfxCtx, 0, -gameplayFrames, -gameplayFrames, 32, 32, 1, 0, 0, 16,
                                        64, 0, 0, 0, play->roomCtx.unk_74[0] + 168));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

void* sZorasDomainEntranceTextures[] = {
    gZorasDomainDayEntranceTex,
    gZorasDomainNightEntranceTex,
};

// Scene Draw Config 6
void func_8009E730(PlayState* play) {
    u32 gameplayFrames;
    u32 var;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    var = 127 - (gameplayFrames * 1) % 128;
    if (LINK_IS_ADULT) {
        var = 0;
    }
    gSPSegment(POLY_OPA_DISP++, 0x0C, Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 64, 32, 1, 0, var, 64, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sZorasDomainEntranceTextures[gSaveContext.nightFlag]));

    { s32 pad[2]; }

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 7
void func_8009E8C0(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, (gameplayFrames * 1) % 128, 0, 32, 32, 1, 0, 0, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 255 - (gameplayFrames * 2) % 256, 64, 64, 1, 0,
                                255 - (gameplayFrames * 2) % 256, 64, 64));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 128, 32, 32, 1, 0,
                                (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 8
void func_8009EAD8(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 3) % 1024, 32, 256, 1, 0,
                                (gameplayFrames * 3) % 1024, 32, 256));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 256, 64, 64, 1, 0,
                                (gameplayFrames * 1) % 256, 64, 64));
    gSPSegment(POLY_XLU_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 2) % 128, 32, 32, 1, 0,
                                (gameplayFrames * 2) % 128, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x0B,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 32, 1, 0, 127 - (gameplayFrames * 3) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0C,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 128, 32, 32, 1, 0,
                                (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x0D,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (gameplayFrames * 1) % 64, 16, 16, 1, 0,
                                (gameplayFrames * 1) % 64, 16, 16));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 9
void func_8009EE44(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(
        POLY_XLU_DISP++, 0x08,
        Gfx_TwoTexScroll(play->state.gfxCtx, 0, gameplayFrames % 128, 0, 32, 16, 1, gameplayFrames % 128, 0, 32, 16));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, gameplayFrames % 128, 32, 32, 1,
                                gameplayFrames % 128, gameplayFrames % 128, 32, 32));

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    bool playerHasCojiro = INV_CONTENT(ITEM_COJIRO) == ITEM_COJIRO;
    if (IS_RANDO && Randomizer_GetSettingValue(RSK_SHUFFLE_ADULT_TRADE)) {
        playerHasCojiro = Flags_GetRandomizerInf(RAND_INF_ADULT_TRADES_HAS_COJIRO);
    }
    if ((play->roomCtx.unk_74[0] == 0) && playerHasCojiro) {
        if (play->roomCtx.unk_74[1] == 50) {
            Player_PlaySfx(&GET_PLAYER(play)->actor, NA_SE_EV_CHICKEN_CRY_M);
            play->roomCtx.unk_74[0] = 1;
        }
        play->roomCtx.unk_74[1]++;
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 10
void func_8009F074(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 32, 1, 0, 127 - gameplayFrames % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// walls of GF room where they throw you when you get caught
void* D_8012A380[] = {
    gSpot12_009678Tex,
    gSpot12_00DE78Tex,
};

// Scene Draw Config 11
void func_8009F1B4(PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    { s32 pad[2]; }

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_8012A380[gSaveContext.nightFlag]));

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 12
void func_8009F270(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(
        POLY_OPA_DISP++, 0x08,
        Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, gameplayFrames % 128, 32, 32, 1, 0, gameplayFrames % 128, 32, 32));
    gSPSegment(
        POLY_XLU_DISP++, 0x09,
        Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, gameplayFrames % 128, 32, 32, 1, 0, gameplayFrames % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 13
void func_8009F40C(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 10) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 10) % 128, 32, 32));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 3) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 14
void func_8009F5D4(PlayState* play) {
    Gfx* displayListHead = Graph_Alloc(play->state.gfxCtx, 3 * sizeof(Gfx));

    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08, displayListHead);

    if ((gSaveContext.dayTime >= 0x4AAC) && (gSaveContext.dayTime <= 0xC000)) {
        gSPEndDisplayList(displayListHead);
    } else {
        if (gSaveContext.dayTime > 0xC000) {
            if (play->roomCtx.unk_74[0] != 255) {
                Math_StepToS(&play->roomCtx.unk_74[0], 255, 5);
            }
        } else if (gSaveContext.dayTime >= 0x4000) {
            if (play->roomCtx.unk_74[0] != 0) {
                Math_StepToS(&play->roomCtx.unk_74[0], 0, 10);
            }
        }

        gDPSetPrimColor(displayListHead++, 0, 0, 255, 255, 255, play->roomCtx.unk_74[0]);
        gSPDisplayList(displayListHead++, spot16_room_0DL_00AA48);
        gSPEndDisplayList(displayListHead);
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 15
void func_8009F7D4(PlayState* play) {
    s8 sp6F = coss((play->gameplayFrames * 1500) & 0xFFFF) >> 8;
    s8 sp6E = coss((play->gameplayFrames * 1500) & 0xFFFF) >> 8;
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    sp6F = (sp6F >> 1) + 192;
    sp6E = (sp6E >> 1) + 192;

    gSPSegment(
        POLY_OPA_DISP++, 0x08,
        Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, gameplayFrames % 128, 32, 32, 1, 0, gameplayFrames % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, sp6F, sp6E, 255, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

void* sGoronCityEntranceTextures[] = {
    gGoronCityDayEntranceTex,
    gGoronCityNightEntranceTex,
};

// Scene Draw Config 16
void func_8009F9D0(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 127 - gameplayFrames % 128, 32, 32, 1, gameplayFrames % 128,
                                0, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sGoronCityEntranceTextures[gSaveContext.nightFlag]));

    { s32 pad[2]; }

    CLOSE_DISPS(play->state.gfxCtx);
}

void* sLonLonRanchWindowTextures[] = {
    gLonLonRanchDayWindowTex,
    gLonLonRangeNightWindowsTex,
};

// Scene Draw Config 17
void func_8009FB74(PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    { s32 pad[2]; }

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sLonLonRanchWindowTextures[gSaveContext.nightFlag]));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 18
void func_8009FC90(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 127 - gameplayFrames % 128, 32, 32, 1,
                                127 - gameplayFrames % 128, 0, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, (gameplayFrames * 3) % 128, 127 - (gameplayFrames * 6) % 128, 32,
                                32, 1, (gameplayFrames * 6) % 128, 127 - (gameplayFrames * 3) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 64);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 64);

    CLOSE_DISPS(play->state.gfxCtx);
}

f32 D_8012A398 = 0.0f;

void func_8009FE58(PlayState* play) {
    static s16 D_8012A39C = 538;
    static s16 D_8012A3A0 = 4272;
    u32 gameplayFrames;
    f32 temp;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    if (play->sceneNum == SCENE_JABU_JABU) {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, 0, gameplayFrames % 128, (gameplayFrames * 2) % 128, 32, 32, 1,
                                    127 - gameplayFrames % 128, (gameplayFrames * 2) % 128, 32, 32));
        gSPSegment(POLY_OPA_DISP++, 0x0B,
                   Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 255 - (gameplayFrames * 4) % 256, 32, 64, 1, 0,
                                    255 - (gameplayFrames * 4) % 256, 32, 64));
    } else {
        gSPSegment(
            POLY_OPA_DISP++, 0x08,
            Gfx_TexScroll(play->state.gfxCtx, (127 - (gameplayFrames * 1)) % 128, (gameplayFrames * 1) % 128, 32, 32));
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    if (FrameAdvance_IsEnabled(play) != true) {

        D_8012A39C += 1820;
        D_8012A3A0 += 1820;

        temp = 0.020000001f;

        if (play->pauseCtx.state == 0) {
            View_SetDistortionOrientation(&play->view,
                                          ((360.00018f / 65535.0f) * (M_PI / 180.0f)) * temp * Math_CosS(D_8012A39C),
                                          ((360.00018f / 65535.0f) * (M_PI / 180.0f)) * temp * Math_SinS(D_8012A39C),
                                          ((360.00018f / 65535.0f) * (M_PI / 180.0f)) * temp * Math_SinS(D_8012A3A0));
            View_SetDistortionScale(&play->view, 1.f + (0.79999995f * temp * Math_SinS(D_8012A3A0)),
                                    1.f + (0.39999998f * temp * Math_CosS(D_8012A3A0)),
                                    1.f + (1 * temp * Math_CosS(D_8012A39C)));
            View_SetDistortionSpeed(&play->view, 0.95f);
        }

        switch (play->roomCtx.unk_74[0]) {
            case 0:
                break;
            case 1:
                if (play->roomCtx.unk_74[1] < 1200) {
                    play->roomCtx.unk_74[1] += 200;
                } else {
                    play->roomCtx.unk_74[0]++;
                }
                break;
            case 2:
                if (play->roomCtx.unk_74[1] > 0) {
                    play->roomCtx.unk_74[1] -= 30;
                } else {
                    play->roomCtx.unk_74[1] = 0;
                    play->roomCtx.unk_74[0] = 0;
                }
                break;
        }

        if (play->pauseCtx.state == 0) {
            D_8012A398 += 0.15f + (play->roomCtx.unk_74[1] * 0.001f);
        }
    }

    if (play->roomCtx.curRoom.num == 2) {
        Matrix_Scale(1.0f, sinf(D_8012A398) * 0.8f, 1.0f, MTXMODE_NEW);
    } else {
        Matrix_Scale(1.005f, sinf(D_8012A398) * 0.8f, 1.005f, MTXMODE_NEW);
    }

    gSPSegment(POLY_OPA_DISP++, 0x0D, MATRIX_NEWMTX(play->state.gfxCtx));

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 26
void func_800A0334(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 512, 32, 128,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 512, 32, 128));
    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, 127 - gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32,
                                1, gameplayFrames % 128, (gameplayFrames * 1) % 128, 32, 32));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

// Scene Draw Config 52
void func_800A055C(PlayState* play) {
    func_8009BEEC(play);
}

// Scene Draw Config 51
void func_800A057C(PlayState* play) {
    func_8009BEEC(play);
}

// Scene Draw Config 49
void func_800A059C(PlayState* play) {
    u32 gameplayFrames;

    OPEN_DISPS(play->state.gfxCtx);

    gameplayFrames = play->gameplayFrames;
    gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 127 - (gameplayFrames * 2) % 128, 0, 32, 64));
    gSPSegment(POLY_OPA_DISP++, 0x09, Gfx_TexScroll(play->state.gfxCtx, 0, (gameplayFrames * 2) % 512, 128, 128));

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 128, 128, 128, 128);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 128);

    CLOSE_DISPS(play->state.gfxCtx);
}

void (*sSceneDrawHandlers[])(PlayState*) = {
    func_80099550, func_8009DA30, func_8009DD5C, func_8009DE78, func_8009E0B8, func_8009E54C, func_8009E730,
    func_8009E8C0, func_8009EAD8, func_8009EE44, func_8009F074, func_8009F1B4, func_8009F270, func_8009F40C,
    func_8009F5D4, func_8009F7D4, func_8009F9D0, func_8009FB74, func_8009FC90, func_800995DC, func_80099878,
    func_8009FE58, func_8009D758, func_8009B0FC, func_8009AE30, func_8009D974, func_800A0334, func_8009CC00,
    func_80099760, func_8009B86C, func_80099BD8, func_8009A45C, func_8009A798, func_8009A9DC, func_8009B9BC,
    func_8009BAA4, func_8009BC44, func_8009C3EC, func_8009C0AC, func_8009ACA8, func_8009AFE0, func_8009D0E8,
    func_8009C608, func_8009C8B8, func_8009D31C, func_8009D438, func_8009D5B4, func_8009CAC0, func_8009AB98,
    func_800A059C, func_8009CF84, func_800A057C, func_800A055C,
};

void Scene_Draw(PlayState* play) {
    if (HREG(80) == 17) {
        if (HREG(95) != 17) {
            HREG(95) = 17;
            HREG(81) = 1;
            HREG(82) = 1;
            HREG(83) = 0;
            HREG(84) = 0;
            HREG(85) = 0;
            HREG(86) = 0;
            HREG(87) = 0;
            HREG(88) = 0;
            HREG(89) = 0;
            HREG(91) = 0;
            HREG(92) = 0;
            HREG(93) = 0;
            HREG(94) = 0;
        }

        OPEN_DISPS(play->state.gfxCtx);

        if (HREG(81) == 1) {
            gSPDisplayList(POLY_OPA_DISP++, sDefaultDisplayList);
            gSPDisplayList(POLY_XLU_DISP++, sDefaultDisplayList);
        }

        CLOSE_DISPS(play->state.gfxCtx);

        if (HREG(82) == 1) {
            sSceneDrawHandlers[play->sceneConfig](play);
        }
    } else {
        sSceneDrawHandlers[play->sceneConfig](play);
    }
}

#include "global.h"
#include "z64camera.h"

#include <string.h>

#include "scenes/indoors/tokinoma/tokinoma_scene.h"
#include "scenes/overworld/spot00/spot00_scene.h"
#include "scenes/overworld/spot01/spot01_scene.h"
#include "scenes/overworld/spot02/spot02_scene.h"
#include "scenes/overworld/spot04/spot04_scene.h"
#include "scenes/overworld/spot05/spot05_scene.h"
#include "scenes/overworld/spot06/spot06_scene.h"
#include "scenes/overworld/spot07/spot07_scene.h"
#include "scenes/overworld/spot08/spot08_scene.h"
#include "scenes/overworld/spot09/spot09_scene.h"
#include "scenes/overworld/spot11/spot11_scene.h"
#include "scenes/overworld/spot12/spot12_scene.h"
#include "scenes/overworld/spot15/spot15_scene.h"
#include "scenes/overworld/spot16/spot16_scene.h"
#include "scenes/overworld/spot17/spot17_scene.h"
#include "scenes/overworld/spot18/spot18_scene.h"
#include "scenes/overworld/spot20/spot20_scene.h"

#include "scenes/dungeons/bdan/bdan_scene.h"
#include "scenes/dungeons/ddan/ddan_scene.h"
#include "scenes/dungeons/ydan/ydan_scene.h"
#include "scenes/dungeons/ganontika/ganontika_scene.h"
#include "scenes/dungeons/ganon_tou/ganon_tou_scene.h"
#include "scenes/dungeons/jyasinboss/jyasinboss_scene.h"
#include "scenes/dungeons/ice_doukutu/ice_doukutu_scene.h"

#include "scenes/misc/hakaana_ouke/hakaana_ouke_scene.h"

#include "soh/Enhancements/randomizer/randomizer_entrance.h"
#include "soh/OTRGlobals.h"
#include "soh/ResourceManagerHelpers.h"
#include "soh/Enhancements/game-interactor/GameInteractor_Hooks.h"

u16 D_8011E1C0 = 0;
u16 D_8011E1C4 = 0;

typedef void (*CutsceneStateHandler)(PlayState*, CutsceneContext*);

void func_80064720(PlayState* play, CutsceneContext* csCtx);
void func_80064760(PlayState* play, CutsceneContext* csCtx);
void func_800647C0(PlayState* play, CutsceneContext* csCtx);
void func_80068C3C(PlayState* play, CutsceneContext* csCtx);
void func_80068D84(PlayState* play, CutsceneContext* csCtx);
void func_80068DC0(PlayState* play, CutsceneContext* csCtx);

CutsceneStateHandler sCsStateHandlers1[] = {
    func_80064720, func_80064760, func_80064720, func_80068D84, func_80064720,
};

CutsceneStateHandler sCsStateHandlers2[] = {
    func_80064720, func_800647C0, func_80068C3C, func_80068DC0, func_80068C3C,
};

u8 sTitleCsState = 0;

EntranceCutscene sEntranceCutsceneTable[] = {
    { ENTR_HYRULE_FIELD_WOODED_EXIT, 2, 0xA0, gHyruleFieldIntroCs },
    { ENTR_DEATH_MOUNTAIN_TRAIL_BOTTOM_EXIT, 2, 0xA1, gDMTIntroCs },
    { ENTR_KAKARIKO_VILLAGE_FRONT_GATE, 2, 0xA3, gKakarikoVillageIntroCs },
    { ENTR_ZORAS_DOMAIN_ENTRANCE, 2, 0xA4, gZorasDomainIntroCs },
    { ENTR_CASTLE_GROUNDS_SOUTH_EXIT, 1, 0xA5, gHyruleCastleIntroCs },
    { ENTR_GORON_CITY_UPPER_EXIT, 2, 0xA6, gGoronCityIntroCs },
    { ENTR_TEMPLE_OF_TIME_ENTRANCE, 2, 0xA7, gTempleOfTimeIntroCs },
    { ENTR_DEKU_TREE_ENTRANCE, 2, 0xA8, gDekuTreeIntroCs },
    { ENTR_HYRULE_FIELD_11, 0, 0x18, gHyruleFieldSouthEponaJumpCs },
    { ENTR_HYRULE_FIELD_13, 0, 0x18, gHyruleFieldEastEponaJumpCs },
    { ENTR_HYRULE_FIELD_12, 0, 0x18, gHyruleFieldWestEponaJumpCs },
    { ENTR_HYRULE_FIELD_15, 0, 0x18, gHyruleFieldGateEponaJumpCs },
    { ENTR_HYRULE_FIELD_16, 1, 0xA9, gHyruleFieldGetOoTCs },
    { ENTR_LAKE_HYLIA_NORTH_EXIT, 2, 0xB1, gLakeHyliaIntroCs },
    { ENTR_GERUDO_VALLEY_EAST_EXIT, 2, 0xB2, gGerudoValleyIntroCs },
    { ENTR_GERUDOS_FORTRESS_EAST_EXIT, 2, 0xB3, gGerudoFortressIntroCs },
    { ENTR_LON_LON_RANCH_ENTRANCE, 2, 0xB4, gLonLonRanchIntroCs },
    { ENTR_JABU_JABU_ENTRANCE, 2, 0xB5, gJabuJabuIntroCs },
    { ENTR_GRAVEYARD_ENTRANCE, 2, 0xB6, gGraveyardIntroCs },
    { ENTR_ZORAS_FOUNTAIN_TUNNEL_EXIT, 2, 0xB7, gZorasFountainIntroCs },
    { ENTR_DESERT_COLOSSUS_EAST_EXIT, 2, 0xB8, gDesertColossusIntroCs },
    { ENTR_DEATH_MOUNTAIN_CRATER_UPPER_EXIT, 2, 0xB9, gDeathMountainCraterIntroCs },
    { ENTR_CASTLE_GROUNDS_SOUTH_EXIT, 0, 0xBA, gGanonsCastleIntroCs },
    { ENTR_ROYAL_FAMILYS_TOMB_1, 2, 0x5A, gSunSongGraveSunSongTeachPart2Cs },
    { ENTR_INSIDE_GANONS_CASTLE_2, 2, 0xBB, gForestBarrierCs },
    { ENTR_INSIDE_GANONS_CASTLE_3, 2, 0xBC, gWaterBarrierCs },
    { ENTR_INSIDE_GANONS_CASTLE_4, 2, 0xBD, gShadowBarrierCs },
    { ENTR_INSIDE_GANONS_CASTLE_5, 2, 0xBE, gFireBarrierCs },
    { ENTR_INSIDE_GANONS_CASTLE_6, 2, 0xBF, gLightBarrierCs },
    { ENTR_INSIDE_GANONS_CASTLE_7, 2, 0xAD, gSpiritBarrierCs },
    { ENTR_SPIRIT_TEMPLE_BOSS_ENTRANCE, 0, 0xC0, gSpiritBossNabooruKnuckleIntroCs },
    { ENTR_GERUDOS_FORTRESS_17, 0, 0xC7, gGerudoFortressFirstCaptureCs },
    { ENTR_DEATH_MOUNTAIN_CRATER_GC_EXIT, 2, 0xB9, gDeathMountainCraterIntroCs },
    { ENTR_KOKIRI_FOREST_12, 2, 0xC6, gKokiriForestDekuSproutCs },
};

// Unused, seems to be an early list of dungeon entrance cutscene locations
void* D_8011E304[] = {
    gDekuTreeIntroCs, gJabuJabuIntroCs, gDcOpeningCs, gMinuetCs, gIceCavernSerenadeCs, gTowerBarrierCs,
};

u16 D_8015FCC0;
u16 D_8015FCC2;
u16 D_8015FCC4;
s16 D_8015FCC6;
u8 D_8015FCC8;
s16 sQuakeIndex;
u16 D_8015FCCC;      // only written to, never read
char D_8015FCD0[20]; // unreferenced
u8 D_8015FCE4;       // only written to, never read

void func_80068ECC(PlayState* play, CutsceneContext* csCtx);

void Cutscene_DrawDebugInfo(PlayState* play, Gfx** dlist, CutsceneContext* csCtx) {
    GfxPrint printer;
    s32 pad[2];

    GfxPrint_Init(&printer);
    GfxPrint_Open(&printer, *dlist);

    GfxPrint_SetPos(&printer, 22, 25);
    GfxPrint_SetColor(&printer, 255, 255, 55, 32);
    GfxPrint_Printf(&printer, "%s", "FLAME ");
    GfxPrint_SetColor(&printer, 255, 255, 255, 32);
    GfxPrint_Printf(&printer, "%06d", csCtx->frames);
    GfxPrint_SetColor(&printer, 50, 255, 255, 60);
    GfxPrint_SetPos(&printer, 4, 26);
    GfxPrint_Printf(&printer, "%s", "SKIP=(START) or (Cursole Right)");

    *dlist = GfxPrint_Close(&printer);
    GfxPrint_Destroy(&printer);
}

void func_8006450C(PlayState* play, CutsceneContext* csCtx) {
    csCtx->state = CS_STATE_IDLE;
    csCtx->unk_0C = 0.0f;
}

void func_80064520(PlayState* play, CutsceneContext* csCtx) {
    csCtx->state = CS_STATE_SKIPPABLE_INIT;
    csCtx->linkAction = NULL;
}

void func_80064534(PlayState* play, CutsceneContext* csCtx) {
    if (csCtx->state != CS_STATE_UNSKIPPABLE_EXEC) {
        csCtx->state = CS_STATE_UNSKIPPABLE_INIT;
    }
}

void func_80064558(PlayState* play, CutsceneContext* csCtx) {
    if (gSaveContext.cutsceneIndex < 0xFFF0) {
        sCsStateHandlers1[csCtx->state](play, csCtx);
    }
}

void func_800645A0(PlayState* play, CutsceneContext* csCtx) {
    Input* input = &play->state.input[0];

    if (CVarGetInteger(CVAR_DEVELOPER_TOOLS("DebugEnabled"), 0) && CHECK_BTN_ALL(input->press.button, BTN_DLEFT) &&
        (csCtx->state == CS_STATE_IDLE) && (gSaveContext.sceneSetupIndex >= 4)) {
        D_8015FCC8 = 0;
        gSaveContext.cutsceneIndex = 0xFFFD;
        gSaveContext.cutsceneTrigger = 1;
    }

    if (CVarGetInteger(CVAR_DEVELOPER_TOOLS("DebugEnabled"), 0) && CHECK_BTN_ALL(input->press.button, BTN_DUP) &&
        (csCtx->state == CS_STATE_IDLE) && (gSaveContext.sceneSetupIndex >= 4) && !gDbgCamEnabled) {
        D_8015FCC8 = 1;
        gSaveContext.cutsceneIndex = 0xFFFD;
        gSaveContext.cutsceneTrigger = 1;
    }

    if ((gSaveContext.cutsceneTrigger != 0) && (play->transitionTrigger == TRANS_TRIGGER_START)) {
        gSaveContext.cutsceneTrigger = 0;
    }

    if ((gSaveContext.cutsceneTrigger != 0) && (csCtx->state == CS_STATE_IDLE)) {
        osSyncPrintf("\nデモ開始要求 発令！"); // "Cutscene start request announcement!"
        gSaveContext.cutsceneIndex = 0xFFFD;
        gSaveContext.cutsceneTrigger = 1;
    }

    if (gSaveContext.cutsceneIndex >= 0xFFF0) {
        func_80068ECC(play, csCtx);
        sCsStateHandlers2[csCtx->state](play, csCtx);
    }
}

void func_80064720(PlayState* play, CutsceneContext* csCtx) {
}

u32 func_8006472C(PlayState* play, CutsceneContext* csCtx, f32 target) {
    return Math_StepToF(&csCtx->unk_0C, target, 0.1f);
}

void func_80064760(PlayState* play, CutsceneContext* csCtx) {
    Interface_ChangeAlpha(1);
    ShrinkWindow_SetVal(0x20);

    if (func_8006472C(play, csCtx, 1.0f)) {
        Audio_SetCutsceneFlag(1);
        csCtx->state++;
    }
}

void func_800647C0(PlayState* play, CutsceneContext* csCtx) {
    func_80068C3C(play, csCtx);
    Interface_ChangeAlpha(1);
    ShrinkWindow_SetVal(0x20);

    if (func_8006472C(play, csCtx, 1.0f)) {
        Audio_SetCutsceneFlag(1);
        csCtx->state++;
    }
}

// Command 3: Misc. Actions
void func_80064824(PlayState* play, CutsceneContext* csCtx, CsCmdBase* cmd) {
    Player* player = GET_PLAYER(play);
    f32 temp;
    u8 sp3F = 0;

    if ((csCtx->frames < cmd->startFrame) || ((csCtx->frames >= cmd->endFrame) && (cmd->endFrame != cmd->startFrame))) {
        return;
    }

    temp = Environment_LerpWeight(cmd->endFrame - 1, cmd->startFrame, csCtx->frames);

    if (csCtx->frames == cmd->startFrame) {
        sp3F = 1;
    }

    switch (cmd->base) {
        case 1:
            if (sp3F != 0) {
                Audio_SetNatureAmbienceChannelIO(NATURE_CHANNEL_RAIN, CHANNEL_IO_PORT_4, 0x3F);
                Audio_SetNatureAmbienceChannelIO(NATURE_CHANNEL_RAIN, CHANNEL_IO_PORT_1, 1);
                play->envCtx.unk_EE[0] = 20;
            }
            break;
        case 2:
            if (sp3F != 0) {
                Audio_SetNatureAmbienceChannelIO(NATURE_CHANNEL_LIGHTNING, CHANNEL_IO_PORT_0, 0);
                Environment_AddLightningBolts(play, 3);
                gLightningStrike.state = LIGHTNING_STRIKE_START;
            }
            break;
        case 3:
            if (sp3F != 0) {
                Flags_SetEnv(play, 0);
                if (gSaveContext.entranceIndex == ENTR_TEMPLE_OF_TIME_ENTRANCE ||
                    (IS_RANDO && gSaveContext.entranceIndex == ENTR_TEMPLE_OF_TIME_WARP_PAD)) {
                    Flags_SetEnv(play, 2);
                }
            }
            break;
        case 6:
            if (play->envCtx.adjFogFar < 12800) {
                play->envCtx.adjFogFar += 35;
            }
            break;
        case 7:
            if (sp3F != 0) {
                play->envCtx.unk_19 = 1;
                play->envCtx.unk_17 = 1;
                play->envCtx.unk_18 = 0;
                play->envCtx.unk_1A = 0x3C;
                play->envCtx.unk_21 = 1;
                play->envCtx.unk_1F = 0;
                play->envCtx.unk_20 = 1;
                play->envCtx.unk_22 = play->envCtx.unk_24 = 0x3C;
            }
            break;
        case 8:
            if (play->roomCtx.unk_74[0] < 0x80) {
                play->roomCtx.unk_74[0] += 4;
            }
            break;
        case 9:
            play->envCtx.unk_EE[3] = 16;
            break;
        case 10:
            Flags_SetEnv(play, 1);
            break;
        case 11:
            if (play->roomCtx.unk_74[0] < 0x672) {
                play->roomCtx.unk_74[0] += 0x14;
            }
            if (csCtx->frames == 0x30F) {
                Sfx_PlaySfxCentered(NA_SE_EV_DEKU_DEATH);
            } else if (csCtx->frames == 0x2CD) {
                play->roomCtx.unk_74[0] = 0;
            }
            break;
        case 12:
            if (sp3F != 0) {
                if (csCtx->state != CS_STATE_UNSKIPPABLE_EXEC) {
                    csCtx->state = CS_STATE_UNSKIPPABLE_INIT;
                }
            }
            break;
        case 13:
            if (play->roomCtx.unk_74[1] == 0) {
                Sfx_PlaySfxCentered(NA_SE_EV_TRIFORCE_FLASH);
            }
            if (play->roomCtx.unk_74[1] < 0xFF) {
                play->roomCtx.unk_74[1] += 5;
            }
            break;
        case 14:
            if (sp3F != 0) {
                Play_SetViewpoint(play, 1);
            }
            break;
        case 15:
            if (sp3F != 0) {
                TitleCard_InitPlaceName(play, &play->actorCtx.titleCtx, player->giObjectSegment, 160, 120, 144, 24, 20);
            }
            break;
        case 16:
            if (sp3F != 0) {
                sQuakeIndex = Quake_Add(GET_ACTIVE_CAM(play), 6);
                Quake_SetSpeed(sQuakeIndex, 0x7FFF);
                Quake_SetQuakeValues(sQuakeIndex, 4, 0, 1000, 0);
                Quake_SetCountdown(sQuakeIndex, 800);
            }
            break;
        case 17:
            if (sp3F != 0) {
                Quake_RemoveFromIdx(sQuakeIndex);
            }
            break;
        case 18:
            play->envCtx.unk_EE[0] = 0;
            play->envCtx.gloomySkyMode = 2;
            if (gSaveContext.dayTime < 0x4AAB) {
                gSaveContext.dayTime += 30;
            }
            if (play->envCtx.unk_EE[1] == 0) {
                gWeatherMode = 0;
                Audio_SetNatureAmbienceChannelIO(NATURE_CHANNEL_RAIN, CHANNEL_IO_PORT_1, 0);
            }
            break;
        case 19:
            Flags_SetEventChkInf(EVENTCHKINF_PLAYED_SONG_OF_STORMS_IN_WINDMILL);
            break;
        case 20:
            Flags_SetEventChkInf(EVENTCHKINF_DRAINED_WELL_IN_KAKARIKO);
            break;
        case 21:
            Flags_SetEventChkInf(EVENTCHKINF_RAISED_LAKE_HYLIA_WATER);
            break;
        case 22:
            gVisMonoColor.r = 255;
            gVisMonoColor.g = 255;
            gVisMonoColor.b = 255;
            gVisMonoColor.a = 255;
            break;
        case 23:
            gVisMonoColor.r = 255;
            gVisMonoColor.g = 180;
            gVisMonoColor.b = 100;
            gVisMonoColor.a = 255.0f * temp;
            break;
        case 24:
            play->roomCtx.curRoom.segment = NULL;
            break;
        case 25:
            gSaveContext.dayTime += 30;
            if ((gSaveContext.dayTime) > 0xCAAA) {
                gSaveContext.dayTime = 0xCAAA;
            }
            break;
        case 26:
            if ((gSaveContext.dayTime < 0x3000) || (gSaveContext.dayTime >= 0x4555)) {
                if ((gSaveContext.dayTime >= 0x4555) && (gSaveContext.dayTime < 0xAAAB)) {
                    play->envCtx.unk_BF = 1;
                } else if ((gSaveContext.dayTime >= 0xAAAB) && (gSaveContext.dayTime < 0xC556)) {
                    play->envCtx.unk_BF = 2;
                } else {
                    play->envCtx.unk_BF = 3;
                }
            }
            break;
        case 27:
            if (play->state.frames & 8) {
                if (play->envCtx.adjAmbientColor[0] < 40) {
                    play->envCtx.adjAmbientColor[0] += 2;
                    play->envCtx.adjLight1Color[1] -= 3;
                    play->envCtx.adjLight1Color[2] -= 3;
                }
            } else {
                if (play->envCtx.adjAmbientColor[0] > 2) {
                    play->envCtx.adjAmbientColor[0] -= 2;
                    play->envCtx.adjLight1Color[1] += 3;
                    play->envCtx.adjLight1Color[2] += 3;
                }
            }
            break;
        case 28:
            play->unk_11DE9 = 1;
            break;
        case 29:
            play->unk_11DE9 = 0;
            break;
        case 30:
            Flags_SetEnv(play, 3);
            break;
        case 31:
            Flags_SetEnv(play, 4);
            break;
        case 32:
            if (sp3F != 0) {
                play->envCtx.sandstormState = SANDSTORM_FILL;
            }
            Sfx_PlaySfxCentered2(NA_SE_EV_SAND_STORM - SFX_FLAG);
            break;
        case 33:
            gSaveContext.sunsSongState = SUNSSONG_START;
            break;
        case 34:
            if (IS_DAY) {
                gSaveContext.dayTime -= gTimeIncrement;
            } else {
                gSaveContext.dayTime -= gTimeIncrement * 2;
            }
            break;
        case 35:
            func_800EE824();
            csCtx->frames = cmd->startFrame - 1;
            break;
    }
}

// Command 4: Set Environment Lighting
void Cutscene_Command_SetLighting(PlayState* play, CutsceneContext* csCtx, CsCmdEnvLighting* cmd) {
    if (csCtx->frames == cmd->startFrame) {
        play->envCtx.unk_BF = cmd->setting - 1;
        play->envCtx.unk_D8 = 1.0f;
    }
}

// Command 0x56: Play Background Music
void Cutscene_Command_PlayBGM(PlayState* play, CutsceneContext* csCtx, CsCmdMusicChange* cmd) {
    if (csCtx->frames == cmd->startFrame) {
        func_800F595C(cmd->sequence - 1);
    }
}

// Command 0x57: Stop Background Music
void Cutscene_Command_StopBGM(PlayState* play, CutsceneContext* csCtx, CsCmdMusicChange* cmd) {
    if (csCtx->frames == cmd->startFrame) {
        func_800F59E8(cmd->sequence - 1);
    }
}

// Command 0x7C: Fade Background Music over duration
void Cutscene_Command_FadeBGM(PlayState* play, CutsceneContext* csCtx, CsCmdMusicFade* cmd) {
    u8 var1;

    if ((csCtx->frames == cmd->startFrame) && (csCtx->frames < cmd->endFrame)) {
        var1 = cmd->endFrame - cmd->startFrame;

        if (cmd->type == 3) {
            Audio_QueueSeqCmd(var1 << 0x10 | (0x1 << 28 | SEQ_PLAYER_FANFARE << 24 | 0xFF));
        } else {
            Audio_QueueSeqCmd(var1 << 0x10 | (0x1 << 28 | SEQ_PLAYER_BGM_MAIN << 24 | 0xFF));
        }
    }
}

// Command 9: ?
void Cutscene_Command_09(PlayState* play, CutsceneContext* csCtx, CsCmdUnknown9* cmd) {
    if (csCtx->frames == cmd->startFrame) {
        func_800AA000(0.0f, cmd->unk_06, cmd->unk_07, cmd->unk_08);
    }
}

// Command 0x8C: Set Time of Day & Environment Time
void func_80065134(PlayState* play, CutsceneContext* csCtx, CsCmdDayTime* cmd) {
    s16 temp1;
    s16 temp2;

    if (csCtx->frames == cmd->startFrame) {
        temp1 = (cmd->hour * 60.0f) / (360.0f / 0x4000);
        temp2 = (cmd->minute + 1) / (360.0f / 0x4000);

        gSaveContext.dayTime = temp1 + temp2;
        gSaveContext.skyboxTime = temp1 + temp2;
        LUSLOG_INFO("SET TIME %d", gSaveContext.dayTime);
    }
}

// Command 0x3E8: Code Execution (& Terminates Cutscene?)
void Cutscene_Command_Terminator(PlayState* play, CutsceneContext* csCtx, CsCmdBase* cmd) {
    Player* player = GET_PLAYER(play);
    s32 temp = 0;

    bool shouldSkipCommand = false;

    if (cmd->base == 8 && !GameInteractor_Should(VB_PLAY_PULL_MASTER_SWORD_CS, true)) {
        shouldSkipCommand = true;
    }

    if (cmd->base == 24 && !GameInteractor_Should(VB_PLAY_DROP_FISH_FOR_JABU_CS, true)) {
        shouldSkipCommand = true;
    }

    bool debugCsSkip = (CHECK_BTN_ALL(play->state.input[0].press.button, BTN_START) &&
                        (gSaveContext.fileNum != 0xFEDC) && CVarGetInteger(CVAR_DEVELOPER_TOOLS("DebugEnabled"), 0));

    if ((gSaveContext.gameMode != GAMEMODE_NORMAL) && (gSaveContext.gameMode != GAMEMODE_END_CREDITS) &&
        (play->sceneNum != SCENE_HYRULE_FIELD) && (csCtx->frames > 20) &&
        (CHECK_BTN_ALL(play->state.input[0].press.button, BTN_A) ||
         CHECK_BTN_ALL(play->state.input[0].press.button, BTN_B) ||
         CHECK_BTN_ALL(play->state.input[0].press.button, BTN_START)) &&
        (gSaveContext.fileNum != 0xFEDC) && (play->transitionTrigger == TRANS_TRIGGER_OFF)) {
        Audio_PlaySoundGeneral(NA_SE_SY_PIECE_OF_HEART, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                               &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
        temp = 1;
    }

    bool playCutscene = false;
    if ((!CVarGetInteger(CVAR_ENHANCEMENT("CreditsFix"), 1) || ResourceMgr_GetGameRegion(0) == GAME_REGION_NTSC) &&
        (cmd->startFrame == csCtx->frames)) {
        playCutscene = true;
    } else if (CVarGetInteger(CVAR_ENHANCEMENT("CreditsFix"), 1)) {
        u16 delay = 0;

        // HACK:  Align visual timing with audio during credits sequence
        switch (cmd->base) {
            case 55: // Gerudo fortress (second scene of credits roll)
                delay = 20;
                break;
            case 56: // Kakariko village
                delay = 40;
                break;
            case 57: // Death mountain trail
                delay = 20;
                break;
            case 58: // Goron city
                delay = 20;
                break;
            case 59: // Lake hylia
                delay = 20;
                break;
            case 62: // Kokiri forest (houses)
                delay = 40;
                break;
            case 63: // Kokiri forest (deku tree)
                delay = 40;
                break;
            case 74: // First gorons dancing
                delay = 100;
                break;
            case 75: // Magic carpet guy and old shop keepers
                delay = 180;
                break;
            case 77: // Sad mido and king zora (plays after scene 78)
                delay = 100;
                break;
            case 78: // Second gorons dancing
                delay = 160;
                break;
        }
        if (cmd->startFrame + delay == csCtx->frames) {
            playCutscene = true;
        }
    }

    if (playCutscene || (temp != 0) || ((csCtx->frames > 20) && (shouldSkipCommand || debugCsSkip))) {

        csCtx->state = CS_STATE_UNSKIPPABLE_EXEC;
        Audio_SetCutsceneFlag(0);
        gSaveContext.cutsceneTransitionControl = 1;

        osSyncPrintf("\n分岐先指定！！=[%d]番", cmd->base); // "Future fork designation=No. [%d]"

        if ((gSaveContext.gameMode != GAMEMODE_NORMAL) && (csCtx->frames != cmd->startFrame)) {
            gSaveContext.forceRisingButtonAlphas = 1;
        }

        gSaveContext.cutsceneIndex = 0;

        switch (cmd->base) {
            case 1:
                play->nextEntranceIndex = ENTR_CUTSCENE_MAP_0;
                gSaveContext.cutsceneIndex = 0xFFF1;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 2:
                play->nextEntranceIndex = ENTR_CUTSCENE_MAP_0;
                gSaveContext.cutsceneIndex = 0xFFF0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FILL_WHITE;
                break;
            case 3:
                play->nextEntranceIndex = ENTR_GERUDO_VALLEY_EAST_EXIT;
                gSaveContext.cutsceneIndex = 0xFFF1;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FILL_WHITE;
                break;
            case 4:
                play->nextEntranceIndex = ENTR_DEATH_MOUNTAIN_TRAIL_BOTTOM_EXIT;
                gSaveContext.cutsceneIndex = 0xFFF0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FILL_WHITE;
                break;
            case 5:
                play->nextEntranceIndex = ENTR_KOKIRI_FOREST_0;
                gSaveContext.cutsceneIndex = 0xFFF0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FILL_WHITE;
                break;
            case 6:
                play->nextEntranceIndex = ENTR_CUTSCENE_MAP_0;
                gSaveContext.cutsceneIndex = 0xFFF2;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FILL_WHITE;
                break;
            case 7:
                play->nextEntranceIndex = ENTR_KOKIRI_FOREST_0;
                gSaveContext.cutsceneIndex = 0xFFF2;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_INSTANT;
                break;
            case 8:
                if (CVarGetInteger(CVAR_ENHANCEMENT("BetterFarore"), 0)) {
                    FaroresWindData tempFW = gSaveContext.ship.backupFW;
                    gSaveContext.ship.backupFW = gSaveContext.fw;
                    gSaveContext.fw = tempFW;
                } else {
                    gSaveContext.fw.set = 0;
                    gSaveContext.respawn[RESPAWN_MODE_TOP].data = 0;
                }
                if (GameInteractor_Should(VB_PLAY_PULL_MASTER_SWORD_CS,
                                          !Flags_GetEventChkInf(EVENTCHKINF_PULLED_MASTER_SWORD_FROM_PEDESTAL))) {
                    Flags_SetEventChkInf(EVENTCHKINF_PULLED_MASTER_SWORD_FROM_PEDESTAL);
                    play->nextEntranceIndex = ENTR_CUTSCENE_MAP_0;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    gSaveContext.cutsceneIndex = 0xFFF3;
                    play->transitionType = TRANS_TYPE_INSTANT;
                } else {
                    if (gSaveContext.sceneSetupIndex < 4) {
                        if (!LINK_IS_ADULT) {
                            play->linkAgeOnLoad = 0;
                        } else {
                            play->linkAgeOnLoad = 1;
                        }
                    }
                    play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_2;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    play->transitionType = TRANS_TYPE_FADE_WHITE;
                    gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                }
                break;
            case 9:
                play->nextEntranceIndex = ENTR_GERUDO_VALLEY_EAST_EXIT;
                gSaveContext.cutsceneIndex = 0xFFF0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FILL_BROWN;
                break;
            case 10:
                play->nextEntranceIndex = ENTR_LINKS_HOUSE_CHILD_SPAWN;
                gSaveContext.cutsceneIndex = 0xFFF0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 11:
                play->nextEntranceIndex = ENTR_KOKIRI_FOREST_0;
                gSaveContext.cutsceneIndex = 0xFFF3;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 12:
                play->nextEntranceIndex = ENTR_DEATH_MOUNTAIN_TRAIL_DODONGO_BLUE_WARP;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 13:
                play->nextEntranceIndex = ENTR_ZORAS_FOUNTAIN_JABU_JABU_BLUE_WARP;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 14:
                play->nextEntranceIndex = ENTR_KOKIRI_FOREST_DEKU_TREE_BLUE_WARP;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 15:
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF4;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 16:
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF5;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 17:
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF6;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 18:
                Flags_SetEventChkInf(EVENTCHKINF_ENTERED_MASTER_SWORD_CHAMBER);
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_4;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 19:
                play->nextEntranceIndex = ENTR_DEATH_MOUNTAIN_TRAIL_BOTTOM_EXIT;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                gSaveContext.cutsceneIndex = 0x8000;
                break;
            case 21:
                play->nextEntranceIndex = ENTR_LAKE_HYLIA_NORTH_EXIT;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF0;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 22:
                if (GameInteractor_Should(VB_GIVE_ITEM_REQUIEM_OF_SPIRIT, true)) {
                    Item_Give(play, ITEM_SONG_REQUIEM);
                }
                play->nextEntranceIndex = ENTR_DESERT_COLOSSUS_EAST_EXIT;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF0;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 23:
                play->nextEntranceIndex = ENTR_CUTSCENE_MAP_0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF8;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 24:
                if (IS_RANDO && Randomizer_GetSettingValue(RSK_SHUFFLE_ENTRANCES)) {
                    play->nextEntranceIndex = Entrance_OverrideNextIndex(ENTR_JABU_JABU_ENTRANCE);
                } else {
                    play->nextEntranceIndex = ENTR_JABU_JABU_ENTRANCE;
                }
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 25:
                play->linkAgeOnLoad = 0;
                play->nextEntranceIndex = ENTR_CHAMBER_OF_THE_SAGES_0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF0;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 26:
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF4;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 27:
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF5;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 28:
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF6;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 29:
                play->nextEntranceIndex = ENTR_CHAMBER_OF_THE_SAGES_0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.chamberCutsceneNum = 0;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 30:
                play->nextEntranceIndex = ENTR_CHAMBER_OF_THE_SAGES_0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                if (GameInteractor_Should(VB_GIVE_ITEM_FIRE_MEDALLION, true)) {
                    Item_Give(play, ITEM_MEDALLION_FIRE);
                }
                gSaveContext.chamberCutsceneNum = 1;
                break;
            case 31:
                play->nextEntranceIndex = ENTR_CHAMBER_OF_THE_SAGES_0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                gSaveContext.chamberCutsceneNum = 2;
                break;
            case 32:
                play->linkAgeOnLoad = 1;
                play->nextEntranceIndex = ENTR_HYRULE_FIELD_PAST_BRIDGE_SPAWN;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF2;
                play->transitionType = TRANS_TYPE_INSTANT;
                break;
            case 33:
                play->nextEntranceIndex = ENTR_HYRULE_FIELD_PAST_BRIDGE_SPAWN;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 34:
                play->nextEntranceIndex = ENTR_CUTSCENE_MAP_0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF3;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 35:
                play->nextEntranceIndex = ENTR_HYRULE_FIELD_PAST_BRIDGE_SPAWN;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF0;
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                break;
            case 38:
                play->nextEntranceIndex = ENTR_CUTSCENE_MAP_0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF4;
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                break;
            case 39:
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF9;
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                break;
            case 40:
                play->linkAgeOnLoad = 0;
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFFA;
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                break;
            case 41:
                play->nextEntranceIndex = ENTR_LAKE_HYLIA_5;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 42:
                play->nextEntranceIndex = ENTR_KAKARIKO_VILLAGE_FRONT_GATE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF2;
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                break;
            case 43:
                play->nextEntranceIndex = ENTR_WINDMILL_AND_DAMPES_GRAVE_2;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                break;
            case 44:
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_3;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE_INSTANT;
                break;
            case 46:
                Flags_SetEventChkInf(EVENTCHKINF_ENTERED_MASTER_SWORD_CHAMBER);
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_4;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                break;
            case 47:
                if (GameInteractor_Should(VB_GIVE_ITEM_NOCTURNE_OF_SHADOW, true)) {
                    Item_Give(play, ITEM_SONG_NOCTURNE);
                }
                Flags_SetEventChkInf(EVENTCHKINF_LEARNED_NOCTURNE_OF_SHADOW);
                play->nextEntranceIndex = ENTR_KAKARIKO_VILLAGE_FRONT_GATE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF1;
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                break;
            case 48:
                play->nextEntranceIndex = ENTR_DESERT_COLOSSUS_4;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_SANDSTORM_END;
                gSaveContext.nextTransitionType = TRANS_TYPE_SANDSTORM_END;
                break;
            case 49:
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_5;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                break;
            case 50:
                play->nextEntranceIndex = ENTR_KAKARIKO_VILLAGE_13;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE_INSTANT;
                break;
            case 51:
                play->nextEntranceIndex = ENTR_HYRULE_FIELD_PAST_BRIDGE_SPAWN;
                gSaveContext.cutsceneIndex = 0xFFF8;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_CIRCLE(TCA_NORMAL, TCC_WHITE, TCS_SLOW);
                break;
            case 52:
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
                gSaveContext.cutsceneIndex = 0xFFF7;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_INSTANT;
                break;
            case 53:
                play->nextEntranceIndex = ENTR_HYRULE_FIELD_16;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 54:
                gSaveContext.gameMode = GAMEMODE_END_CREDITS;
                Audio_SetSoundBanksMute(0x6F);
                play->linkAgeOnLoad = 1;
                play->nextEntranceIndex = ENTR_GERUDO_VALLEY_EAST_EXIT;
                gSaveContext.cutsceneIndex = 0xFFF2;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 55:
                play->nextEntranceIndex = ENTR_GERUDOS_FORTRESS_EAST_EXIT;
                gSaveContext.cutsceneIndex = 0xFFF1;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 56:
                play->nextEntranceIndex = ENTR_KAKARIKO_VILLAGE_FRONT_GATE;
                gSaveContext.cutsceneIndex = 0xFFF4;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 57:
                play->nextEntranceIndex = ENTR_DEATH_MOUNTAIN_TRAIL_BOTTOM_EXIT;
                gSaveContext.cutsceneIndex = 0xFFF3;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 58:
                play->nextEntranceIndex = ENTR_GORON_CITY_UPPER_EXIT;
                gSaveContext.cutsceneIndex = 0xFFF1;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 59:
                play->nextEntranceIndex = ENTR_LAKE_HYLIA_NORTH_EXIT;
                gSaveContext.cutsceneIndex = 0xFFF1;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 60:
                play->nextEntranceIndex = ENTR_ZORAS_FOUNTAIN_JABU_JABU_BLUE_WARP;
                gSaveContext.cutsceneIndex = 0xFFF2;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 61:
                play->nextEntranceIndex = ENTR_ZORAS_DOMAIN_ENTRANCE;
                gSaveContext.cutsceneIndex = 0xFFF0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 62:
                play->linkAgeOnLoad = 0;
                play->nextEntranceIndex = ENTR_KOKIRI_FOREST_0;
                gSaveContext.cutsceneIndex = 0xFFF6;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 63:
                play->nextEntranceIndex = ENTR_KOKIRI_FOREST_0;
                gSaveContext.cutsceneIndex = 0xFFF7;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 64:
                play->nextEntranceIndex = ENTR_HYRULE_FIELD_PAST_BRIDGE_SPAWN;
                gSaveContext.cutsceneIndex = 0xFFF5;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 65:
                play->linkAgeOnLoad = 1;
                play->nextEntranceIndex = ENTR_LON_LON_RANCH_ENTRANCE;
                gSaveContext.cutsceneIndex = 0xFFF2;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 66:
                play->nextEntranceIndex = ENTR_KAKARIKO_VILLAGE_OWL_DROP;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 67:
                play->nextEntranceIndex = ENTR_HYRULE_FIELD_OWL_DROP;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 68:
                play->nextEntranceIndex = ENTR_CUTSCENE_MAP_0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF5;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 69:
                play->nextEntranceIndex = ENTR_KOKIRI_FOREST_12;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 70:
                play->nextEntranceIndex = ENTR_DEATH_MOUNTAIN_TRAIL_BOTTOM_EXIT;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF4;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 71:
                gSaveContext.equips.equipment |= EQUIP_VALUE_TUNIC_KOKIRI << (EQUIP_TYPE_TUNIC * 4);
                Player_SetEquipmentData(play, player);
                gSaveContext.equips.equipment |= EQUIP_VALUE_BOOTS_KOKIRI << (EQUIP_TYPE_BOOTS * 4);
                Player_SetEquipmentData(play, player);
                play->linkAgeOnLoad = 1;
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF1;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 72:
                play->nextEntranceIndex = ENTR_CASTLE_COURTYARD_ZELDA_0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF0;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 73:
                play->linkAgeOnLoad = 1;
                play->nextEntranceIndex = ENTR_LON_LON_RANCH_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF2;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 74:
                play->nextEntranceIndex = ENTR_LON_LON_RANCH_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF3;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 75:
                play->linkAgeOnLoad = 1;
                play->nextEntranceIndex = ENTR_LON_LON_RANCH_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF4;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 76:
                play->linkAgeOnLoad = 0;
                play->nextEntranceIndex = ENTR_LON_LON_RANCH_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF5;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 77:
                play->linkAgeOnLoad = 1;
                play->nextEntranceIndex = ENTR_LON_LON_RANCH_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF6;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 78:
                play->nextEntranceIndex = ENTR_LON_LON_RANCH_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF7;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 79:
            case 80:
            case 81:
            case 82:
            case 83:
            case 84:
            case 85:
            case 86:
            case 87:
            case 88:
            case 89:
            case 90:
            case 91:
            case 92:
            case 93:
                play->nextEntranceIndex = ENTR_LON_LON_RANCH_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 94:
                play->nextEntranceIndex = ENTR_LON_LON_RANCH_1;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 95:
                if ((Flags_GetEventChkInf(EVENTCHKINF_USED_FOREST_TEMPLE_BLUE_WARP)) &&
                    (Flags_GetEventChkInf(EVENTCHKINF_USED_FIRE_TEMPLE_BLUE_WARP)) &&
                    (Flags_GetEventChkInf(EVENTCHKINF_USED_WATER_TEMPLE_BLUE_WARP))) {
                    play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    gSaveContext.cutsceneIndex = 0xFFF3;
                    play->transitionType = TRANS_TYPE_FADE_BLACK;
                } else {
                    switch (gSaveContext.sceneSetupIndex) {
                        case 8:
                            play->nextEntranceIndex = ENTR_SACRED_FOREST_MEADOW_SOUTH_EXIT;
                            play->transitionTrigger = TRANS_TRIGGER_START;
                            play->transitionType = TRANS_TYPE_FADE_BLACK;
                            break;
                        case 9:
                            play->nextEntranceIndex = ENTR_DEATH_MOUNTAIN_CRATER_UPPER_EXIT;
                            play->transitionTrigger = TRANS_TRIGGER_START;
                            play->transitionType = TRANS_TYPE_FADE_BLACK;
                            break;
                        case 10:
                            play->nextEntranceIndex = ENTR_LAKE_HYLIA_NORTH_EXIT;
                            play->transitionTrigger = TRANS_TRIGGER_START;
                            gSaveContext.cutsceneIndex = 0xFFF0;
                            play->transitionType = TRANS_TYPE_FADE_WHITE;
                            break;
                    }
                }
                break;
            case 96:
                if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SHADOW)) {
                    play->nextEntranceIndex = ENTR_CHAMBER_OF_THE_SAGES_0;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    gSaveContext.cutsceneIndex = 0xFFF1;
                    play->transitionType = TRANS_TYPE_FADE_WHITE_FAST;
                } else {
                    Flags_SetEventChkInf(EVENTCHKINF_OBTAINED_SPIRIT_MEDALLION);
                    play->nextEntranceIndex = ENTR_DESERT_COLOSSUS_SPIRIT_TEMPLE_BLUE_WARP;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    play->transitionType = TRANS_TYPE_FADE_WHITE;
                    gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                }
                break;
            case 97:
                if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) {
                    play->nextEntranceIndex = ENTR_CHAMBER_OF_THE_SAGES_0;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    gSaveContext.cutsceneIndex = 0xFFF1;
                    play->transitionType = TRANS_TYPE_FADE_WHITE_FAST;
                } else {
                    play->nextEntranceIndex = ENTR_GRAVEYARD_SHADOW_TEMPLE_BLUE_WARP;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    play->transitionType = TRANS_TYPE_FADE_WHITE;
                    gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                }
                break;
            case 98:
                play->nextEntranceIndex = ENTR_DEATH_MOUNTAIN_CRATER_FIRE_TEMPLE_BLUE_WARP;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 99:
                play->nextEntranceIndex = ENTR_SACRED_FOREST_MEADOW_FOREST_TEMPLE_BLUE_WARP;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 100:
                play->nextEntranceIndex = ENTR_KOKIRI_FOREST_0;
                gSaveContext.cutsceneIndex = 0xFFF8;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 101:
                play->nextEntranceIndex = ENTR_DESERT_COLOSSUS_6;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_SANDSTORM_END;
                break;
            case 102:
                play->nextEntranceIndex = ENTR_TEMPLE_OF_TIME_6;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 103:
                play->nextEntranceIndex = ENTR_HYRULE_FIELD_PAST_BRIDGE_SPAWN;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF3;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 104:
                switch (sTitleCsState) {
                    case 0:
                        play->nextEntranceIndex = ENTR_SPIRIT_TEMPLE_BOSS_ENTRANCE;
                        play->transitionTrigger = TRANS_TRIGGER_START;
                        gSaveContext.cutsceneIndex = 0xFFF2;
                        play->transitionType = TRANS_TYPE_FADE_BLACK;
                        sTitleCsState++;
                        break;
                    case 1:
                        play->nextEntranceIndex = ENTR_DEATH_MOUNTAIN_CRATER_UPPER_EXIT;
                        play->transitionTrigger = TRANS_TRIGGER_START;
                        gSaveContext.cutsceneIndex = 0xFFF1;
                        play->transitionType = TRANS_TYPE_FADE_BLACK;
                        sTitleCsState++;
                        break;
                    case 2:
                        play->nextEntranceIndex = ENTR_CUTSCENE_MAP_0;
                        play->transitionTrigger = TRANS_TRIGGER_START;
                        gSaveContext.cutsceneIndex = 0xFFF6;
                        play->transitionType = TRANS_TYPE_FADE_BLACK;
                        sTitleCsState = 0;
                        break;
                }
                break;
            case 105:
                play->nextEntranceIndex = ENTR_GRAVEYARD_ENTRANCE;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.cutsceneIndex = 0xFFF1;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 106:
                play->nextEntranceIndex = ENTR_ROYAL_FAMILYS_TOMB_1;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 107:
                play->nextEntranceIndex = ENTR_INSIDE_GANONS_CASTLE_2;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 108:
                play->nextEntranceIndex = ENTR_INSIDE_GANONS_CASTLE_3;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 109:
                play->nextEntranceIndex = ENTR_INSIDE_GANONS_CASTLE_4;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 110:
                play->nextEntranceIndex = ENTR_INSIDE_GANONS_CASTLE_5;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 111:
                play->nextEntranceIndex = ENTR_INSIDE_GANONS_CASTLE_6;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 112:
                play->nextEntranceIndex = ENTR_INSIDE_GANONS_CASTLE_7;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 113:
                if (Flags_GetEventChkInf(EVENTCHKINF_COMPLETED_FOREST_TRIAL) &&
                    Flags_GetEventChkInf(EVENTCHKINF_COMPLETED_WATER_TRIAL) &&
                    Flags_GetEventChkInf(EVENTCHKINF_COMPLETED_SHADOW_TRIAL) &&
                    Flags_GetEventChkInf(EVENTCHKINF_COMPLETED_FIRE_TRIAL) &&
                    Flags_GetEventChkInf(EVENTCHKINF_COMPLETED_LIGHT_TRIAL) &&
                    Flags_GetEventChkInf(EVENTCHKINF_COMPLETED_SPIRIT_TRIAL)) {
                    play->csCtx.segment = SEGMENTED_TO_VIRTUAL(gTowerBarrierCs);
                    play->csCtx.frames = 0;
                    gSaveContext.cutsceneTrigger = 1;
                    gSaveContext.cutsceneIndex = 0xFFFF;
                    csCtx->state = CS_STATE_UNSKIPPABLE_INIT;
                } else {
                    gSaveContext.cutsceneIndex = 0xFFFF;
                    csCtx->state = CS_STATE_UNSKIPPABLE_INIT;
                }
                break;
            case 114:
                play->nextEntranceIndex = ENTR_HYRULE_FIELD_WOODED_EXIT;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 115:
                play->nextEntranceIndex = ENTR_HYRULE_FIELD_17;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_BLACK;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 116:
                if (Flags_GetEventChkInf(EVENTCHKINF_OBTAINED_SPIRIT_MEDALLION)) {
                    play->nextEntranceIndex = ENTR_GRAVEYARD_SHADOW_TEMPLE_BLUE_WARP;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    play->transitionType = TRANS_TYPE_FADE_WHITE;
                } else {
                    play->nextEntranceIndex = ENTR_DESERT_COLOSSUS_SPIRIT_TEMPLE_BLUE_WARP;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    play->transitionType = TRANS_TYPE_FADE_WHITE;
                }
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 117:
                gSaveContext.gameMode = GAMEMODE_END_CREDITS;
                Audio_SetSoundBanksMute(0x6F);
                play->linkAgeOnLoad = 0;
                play->nextEntranceIndex = ENTR_HYRULE_FIELD_PAST_BRIDGE_SPAWN;
                gSaveContext.cutsceneIndex = 0xFFF7;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
            case 118:
                gSaveContext.respawn[RESPAWN_MODE_DOWN].entranceIndex = ENTR_GANON_BOSS_0;
                Play_TriggerVoidOut(play);
                gSaveContext.respawnFlag = -2;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK;
                break;
            case 119:
                gSaveContext.dayTime = 0x8000;
                gSaveContext.skyboxTime = 0x8000;
                play->nextEntranceIndex = ENTR_CASTLE_COURTYARD_ZELDA_1;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                break;
        }
    }
}

// Command 0x2D: Transition Effects
void Cutscene_Command_TransitionFX(PlayState* play, CutsceneContext* csCtx, CsCmdBase* cmd) {
    f32 temp;

    if ((csCtx->frames >= cmd->startFrame) && (csCtx->frames <= cmd->endFrame)) {
        play->envCtx.fillScreen = true;
        temp = Environment_LerpWeight(cmd->endFrame, cmd->startFrame, csCtx->frames);

        switch (cmd->base) {
            case 1:
            case 5:
                play->envCtx.screenFillColor[0] = 160;
                play->envCtx.screenFillColor[1] = 160;
                play->envCtx.screenFillColor[2] = 160;
                if (cmd->base == 1) {
                    play->envCtx.screenFillColor[3] = 255.0f * temp;
                    if ((temp == 0.0f) && (gSaveContext.entranceIndex == ENTR_CHAMBER_OF_THE_SAGES_0)) {
                        Audio_PlaySoundGeneral(NA_SE_SY_WHITE_OUT_S, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                               &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                    } else if ((temp == 0.0f) &&
                               ((gSaveContext.entranceIndex == ENTR_TEMPLE_OF_TIME_ENTRANCE) ||
                                (gSaveContext.entranceIndex == ENTR_CASTLE_GROUNDS_SOUTH_EXIT) ||
                                (gSaveContext.entranceIndex == ENTR_GREAT_FAIRYS_FOUNTAIN_SPELLS_FARORES_ZF))) {
                        Audio_PlaySoundGeneral(NA_SE_EV_WHITE_OUT, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                               &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                    } else if ((temp == 0.0f) && (play->sceneNum == SCENE_INSIDE_GANONS_CASTLE)) {
                        Sfx_PlaySfxCentered2(NA_SE_EV_WHITE_OUT);
                    }
                } else {
                    play->envCtx.screenFillColor[3] = (1.0f - temp) * 255.0f;
                }
                break;
            case 2:
            case 6:
                play->envCtx.screenFillColor[0] = 0;
                play->envCtx.screenFillColor[1] = 0;
                play->envCtx.screenFillColor[2] = 255;
                if (cmd->base == 2) {
                    play->envCtx.screenFillColor[3] = 255.0f * temp;
                } else {
                    play->envCtx.screenFillColor[3] = (1.0f - temp) * 255.0f;
                }
                break;
            case 3:
            case 7:
                play->envCtx.screenFillColor[0] = 255;
                play->envCtx.screenFillColor[1] = 0;
                play->envCtx.screenFillColor[2] = 0;
                if (cmd->base == 3) {
                    play->envCtx.screenFillColor[3] = (1.0f - temp) * 255.0f;
                } else {
                    play->envCtx.screenFillColor[3] = 255.0f * temp;
                }
                break;
            case 4:
            case 8:
                play->envCtx.screenFillColor[0] = 0;
                play->envCtx.screenFillColor[1] = 255;
                play->envCtx.screenFillColor[2] = 0;
                if (cmd->base == 4) {
                    play->envCtx.screenFillColor[3] = (1.0f - temp) * 255.0f;
                } else {
                    play->envCtx.screenFillColor[3] = 255.0f * temp;
                }
                break;
            case 9:
                gSaveContext.cutsceneTransitionControl = 1;
                break;
            case 10:
            case 11:
                play->envCtx.screenFillColor[0] = 0;
                play->envCtx.screenFillColor[1] = 0;
                play->envCtx.screenFillColor[2] = 0;
                if (cmd->base == 10) {
                    play->envCtx.screenFillColor[3] = (1.0f - temp) * 255.0f;
                } else {
                    play->envCtx.screenFillColor[3] = 255.0f * temp;
                }
                break;
            case 12:
                gSaveContext.cutsceneTransitionControl = 255.0f - (155.0f * temp);
                break;
            case 13:
                play->envCtx.screenFillColor[0] = 0;
                play->envCtx.screenFillColor[1] = 0;
                play->envCtx.screenFillColor[2] = 0;
                play->envCtx.screenFillColor[3] = 255.0f - ((1.0f - temp) * 155.0f);
                break;
        }
    }
}

// Command 0x1 & 0x5: Camera Positions
size_t Cutscene_Command_CameraPositions(PlayState* play, CutsceneContext* csCtx, u8* cmd, u8 relativeToLink) {
    s32 shouldContinue = true;
    CsCmdBase* cmdBase = (CsCmdBase*)cmd;
    size_t size;

    cmd += sizeof(CutsceneData) * 2;
    size = sizeof(CutsceneData) * 2;

    if ((cmdBase->startFrame < csCtx->frames) && (csCtx->frames < cmdBase->endFrame) &&
        ((csCtx->unk_18 < cmdBase->startFrame) || (csCtx->unk_18 >= 0xF000))) {
        csCtx->unk_1B = 1;
        csCtx->cameraPosition = (CutsceneCameraPoint*)cmd;
        if (csCtx->unk_1A != 0) {
            csCtx->unk_18 = cmdBase->startFrame;
            if (D_8015FCC8 != 0) {
                Play_CameraChangeSetting(play, csCtx->unk_14, CAM_SET_CS_0);
                Play_ChangeCameraStatus(play, D_8015FCC6, CAM_STAT_WAIT);
                Play_ChangeCameraStatus(play, csCtx->unk_14, CAM_STAT_ACTIVE);
                Camera_ResetAnim(Play_GetCamera(play, csCtx->unk_14));
                Camera_SetCSParams(Play_GetCamera(play, csCtx->unk_14), csCtx->cameraFocus, csCtx->cameraPosition,
                                   GET_PLAYER(play), relativeToLink);
            }
        }
    }

    while (shouldContinue) {
        if (((CutsceneCameraPoint*)cmd)->continueFlag == CS_CMD_STOP) {
            shouldContinue = false;
        }
        cmd += sizeof(CutsceneData) * 4;
        size += sizeof(CutsceneData) * 4;
    }

    return size;
}

// Command 0x2 & 0x6: Camera Focus Points
size_t Cutscene_Command_CameraFocus(PlayState* play, CutsceneContext* csCtx, u8* cmd, u8 relativeToLink) {
    s32 shouldContinue = true;
    CsCmdBase* cmdBase = (CsCmdBase*)cmd;
    size_t size;

    cmd += sizeof(CutsceneData) * 2;
    size = sizeof(CutsceneData) * 2;

    if ((cmdBase->startFrame < csCtx->frames) && (csCtx->frames < cmdBase->endFrame) &&
        ((D_8015FCC0 < cmdBase->startFrame) || (D_8015FCC0 >= 0xF000))) {
        csCtx->unk_1A = 1;
        csCtx->cameraFocus = (CutsceneCameraPoint*)cmd;
        if (csCtx->unk_1B != 0) {
            D_8015FCC0 = cmdBase->startFrame;
            if (D_8015FCC8 != 0) {
                Play_CameraChangeSetting(play, csCtx->unk_14, CAM_SET_CS_0);
                Play_ChangeCameraStatus(play, D_8015FCC6, CAM_STAT_WAIT);
                Play_ChangeCameraStatus(play, csCtx->unk_14, CAM_STAT_ACTIVE);
                Camera_ResetAnim(Play_GetCamera(play, csCtx->unk_14));
                Camera_SetCSParams(Play_GetCamera(play, csCtx->unk_14), csCtx->cameraFocus, csCtx->cameraPosition,
                                   GET_PLAYER(play), relativeToLink);
            }
        }
    }

    while (shouldContinue) {
        if (((CutsceneCameraPoint*)cmd)->continueFlag == CS_CMD_STOP) {
            shouldContinue = false;
        }
        cmd += sizeof(CutsceneData) * 4;
        size += sizeof(CutsceneData) * 4;
    }

    return size;
}

// Command 0x7: ? (Related to camera positons)
size_t Cutscene_Command_07(PlayState* play, CutsceneContext* csCtx, u8* cmd, u8 unused) {
    CsCmdBase* cmdBase = (CsCmdBase*)cmd;
    size_t size;
    Vec3f sp3C;
    Vec3f sp30;
    Camera* sp2C;
    f32 sp28;

    cmd += sizeof(CutsceneData) * 2;
    size = sizeof(CutsceneData) * 2;

    if ((cmdBase->startFrame < csCtx->frames) && (csCtx->frames < cmdBase->endFrame) &&
        ((D_8015FCC2 < cmdBase->startFrame) || (D_8015FCC2 >= 0xF000))) {
        csCtx->unk_1B = 1;
        csCtx->cameraPosition = (CutsceneCameraPoint*)cmd;
        if (csCtx->unk_1A != 0) {
            D_8015FCC2 = cmdBase->startFrame;
            if (D_8015FCC8 != 0) {
                sp2C = Play_GetCamera(play, csCtx->unk_14);
                sp2C->player = NULL;
                Play_ChangeCameraStatus(play, MAIN_CAM, CAM_STAT_WAIT);
                Play_ChangeCameraStatus(play, csCtx->unk_14, CAM_STAT_ACTIVE);
                Play_CameraChangeSetting(play, csCtx->unk_14, CAM_SET_FREE0);
                sp28 = csCtx->cameraFocus->cameraRoll * 1.40625f;
                Camera_SetParam(sp2C, 64, &sp28);
                sp3C.x = csCtx->cameraFocus->pos.x;
                sp3C.y = csCtx->cameraFocus->pos.y;
                sp3C.z = csCtx->cameraFocus->pos.z;
                sp30.x = csCtx->cameraPosition->pos.x;
                sp30.y = csCtx->cameraPosition->pos.y;
                sp30.z = csCtx->cameraPosition->pos.z;
                Play_CameraSetAtEye(play, csCtx->unk_14, &sp3C, &sp30);
                Play_CameraSetFov(play, csCtx->unk_14, csCtx->cameraPosition->viewAngle);
            }
        }
    }

    size += sizeof(CutsceneData) * 4;

    return size;
}

// Command 0x8: ? (Related to camera focus points)
size_t Cutscene_Command_08(PlayState* play, CutsceneContext* csCtx, u8* cmd, u8 unused) {
    CsCmdBase* cmdBase = (CsCmdBase*)cmd;
    size_t size;
    Vec3f sp3C;
    Vec3f sp30;
    Camera* sp2C;
    f32 sp28;

    cmd += sizeof(CutsceneData) * 2;
    size = sizeof(CutsceneData) * 2;

    if ((cmdBase->startFrame < csCtx->frames) && (csCtx->frames < cmdBase->endFrame) &&
        ((D_8015FCC4 < cmdBase->startFrame) || (D_8015FCC4 >= 0xF000))) {
        csCtx->unk_1A = 1;
        csCtx->cameraFocus = (CutsceneCameraPoint*)cmd;
        if (csCtx->unk_1B != 0) {
            D_8015FCC4 = cmdBase->startFrame;
            if (D_8015FCC8 != 0) {
                sp2C = Play_GetCamera(play, csCtx->unk_14);
                sp2C->player = NULL;
                Play_ChangeCameraStatus(play, MAIN_CAM, CAM_STAT_WAIT);
                Play_ChangeCameraStatus(play, csCtx->unk_14, CAM_STAT_ACTIVE);
                Play_CameraChangeSetting(play, csCtx->unk_14, CAM_SET_FREE0);
                sp3C.x = csCtx->cameraFocus->pos.x;
                sp3C.y = csCtx->cameraFocus->pos.y;
                sp3C.z = csCtx->cameraFocus->pos.z;
                sp30.x = csCtx->cameraPosition->pos.x;
                sp30.y = csCtx->cameraPosition->pos.y;
                sp30.z = csCtx->cameraPosition->pos.z;
                Play_CameraSetAtEye(play, csCtx->unk_14, &sp3C, &sp30);
                Play_CameraSetFov(play, csCtx->unk_14, csCtx->cameraPosition->viewAngle);
            }
        }
    }

    size += sizeof(CutsceneData) * 4;

    return size;
}

// Command 0x13: Textbox
void Cutscene_Command_Textbox(PlayState* play, CutsceneContext* csCtx, CsCmdTextbox* cmd) {
    u8 dialogState;
    s16 originalCsFrames;

    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
        if (cmd->type != 2) {
            if (D_8011E1C0 != cmd->base) {
                D_8011E1C0 = cmd->base;
                if ((cmd->type == 3) && CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE)) {
                    Message_StartTextbox(play, cmd->textId1, NULL);
                } else if ((cmd->type == 4) && CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
                    Message_StartTextbox(play, cmd->textId1, NULL);
                } else {
                    GetItemEntry getItemEntry = GET_ITEM_NONE;
                    if (IS_RANDO) {
                        switch (cmd->base) {
                            case 0x80:
                                getItemEntry = Randomizer_GetItemFromKnownCheck(RC_QUEEN_GOHMA, RG_KOKIRI_EMERALD);
                                break;
                            case 0x81:
                                getItemEntry = Randomizer_GetItemFromKnownCheck(RC_KING_DODONGO, RG_GORON_RUBY);
                                break;
                            case 0x82:
                                getItemEntry = Randomizer_GetItemFromKnownCheck(RC_BARINADE, RG_ZORA_SAPPHIRE);
                                break;
                            case 0x3E:
                                getItemEntry = Randomizer_GetItemFromKnownCheck(RC_PHANTOM_GANON, RG_FOREST_MEDALLION);
                                break;
                            case 0x3C:
                                getItemEntry = Randomizer_GetItemFromKnownCheck(RC_VOLVAGIA, RG_FIRE_MEDALLION);
                                break;
                            case 0x3D:
                                getItemEntry = Randomizer_GetItemFromKnownCheck(RC_MORPHA, RG_WATER_MEDALLION);
                                break;
                            case 0x3F:
                                getItemEntry = Randomizer_GetItemFromKnownCheck(RC_TWINROVA, RG_SPIRIT_MEDALLION);
                                break;
                            case 0x41:
                                getItemEntry = Randomizer_GetItemFromKnownCheck(RC_BONGO_BONGO, RG_SHADOW_MEDALLION);
                                break;
                            case 0x40:
                                getItemEntry = Randomizer_GetItemFromKnownCheck(RC_GIFT_FROM_RAURU, RG_LIGHT_MEDALLION);
                                break;
                            case 0x72:
                                getItemEntry =
                                    Randomizer_GetItemFromKnownCheck(RC_TOT_LIGHT_ARROWS_CUTSCENE, RG_LIGHT_ARROWS);
                                break;
                        }
                        if (getItemEntry.getItemId != GI_NONE) {
                            // cmd->base = getItemEntry.textId;
                            // GET_PLAYER(play)->getItemEntry = getItemEntry;
                        }
                    }
                    Message_StartTextbox(play, cmd->base, NULL);
                    if (IS_RANDO && getItemEntry.getItemId != GI_NONE) {
                        // GET_PLAYER(play)->getItemEntry = (GetItemEntry)GET_ITEM_NONE;
                    }
                }
                return;
            }
        } else {
            if (D_8011E1C4 != cmd->base) {
                D_8011E1C4 = cmd->base;
                func_8010BD58(play, cmd->base);
                return;
            }
        }

        if (csCtx->frames >= cmd->endFrame) {
            originalCsFrames = csCtx->frames;
            dialogState = Message_GetState(&play->msgCtx);

            if ((dialogState != TEXT_STATE_CLOSING) && (dialogState != TEXT_STATE_NONE) &&
                (dialogState != TEXT_STATE_SONG_DEMO_DONE) && (dialogState != TEXT_STATE_8)) {
                csCtx->frames--;

                if ((dialogState == TEXT_STATE_CHOICE) && Message_ShouldAdvance(play)) {
                    if (play->msgCtx.choiceIndex == 0) {
                        if (cmd->textId1 != 0xFFFF) {
                            // LUSLOG_INFO("Cutscene_Command_Textbox D: base:0x%x textId1:0x%x textId2:0x%x", cmd->base,
                            // cmd->textId1, cmd->textId2);
                            Message_ContinueTextbox(play, cmd->textId1);
                        } else {
                            csCtx->frames++;
                        }
                    } else {
                        if (cmd->textId2 != 0xFFFF) {
                            // LUSLOG_INFO("Cutscene_Command_Textbox E: base:0x%x textId1:0x%x textId2:0x%x", cmd->base,
                            // cmd->textId1, cmd->textId2);
                            Message_ContinueTextbox(play, cmd->textId2);
                        } else {
                            csCtx->frames++;
                        }
                    }
                }

                if (dialogState == TEXT_STATE_9) {
                    if (cmd->textId1 != 0xFFFF) {
                        // LUSLOG_INFO("Cutscene_Command_Textbox F: base:0x%x textId1:0x%x textId2:0x%x", cmd->base,
                        // cmd->textId1, cmd->textId2);
                        Message_ContinueTextbox(play, cmd->textId1);
                    } else {
                        csCtx->frames++;
                    }
                }

                if (dialogState == TEXT_STATE_EVENT) {
                    if (Message_ShouldAdvance(play)) {
                        func_8010BD58(play, cmd->base);
                    }
                }
            }

            if (csCtx->frames == originalCsFrames) {
                Interface_ChangeAlpha(1);
                D_8011E1C0 = 0;
                D_8011E1C4 = 0;
            }
        }
    }
}

void Cutscene_ProcessCommands(PlayState* play, CutsceneContext* csCtx, u8* cutscenePtr) {
    s16 i;
    s32 totalEntries;
    s32 cmdType;
    s32 cmdEntries;
    CsCmdBase* cmd;
    s32 cutsceneEndFrame;
    s16 j;

    if (ResourceMgr_OTRSigCheck(cutscenePtr))
        cutscenePtr = ResourceMgr_LoadCSByName(cutscenePtr);

    memcpy(&totalEntries, cutscenePtr, sizeof(CutsceneData));
    cutscenePtr += sizeof(CutsceneData);
    memcpy(&cutsceneEndFrame, cutscenePtr, sizeof(CutsceneData));
    cutscenePtr += sizeof(CutsceneData);

    if ((cutsceneEndFrame < csCtx->frames) && (csCtx->state != CS_STATE_UNSKIPPABLE_EXEC)) {
        csCtx->state = CS_STATE_UNSKIPPABLE_INIT;
        return;
    }

    if (CVarGetInteger(CVAR_DEVELOPER_TOOLS("DebugEnabled"), 0) &&
        CHECK_BTN_ALL(play->state.input[0].press.button, BTN_DRIGHT)) {
        csCtx->state = CS_STATE_UNSKIPPABLE_INIT;
        return;
    }

    for (i = 0; i < totalEntries; i++) {
        memcpy(&cmdType, cutscenePtr, sizeof(CutsceneData));
        cutscenePtr += sizeof(CutsceneData);

        if (cmdType == -1) {
            return;
        }

        // printf("CmdType: %04X\n", cmdType);

        switch (cmdType) {
            case CS_CMD_MISC:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    func_80064824(play, csCtx, (void*)cutscenePtr);
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_SET_LIGHTING:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    Cutscene_Command_SetLighting(play, csCtx, (void*)cutscenePtr);
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_PLAYBGM:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    Cutscene_Command_PlayBGM(play, csCtx, (void*)cutscenePtr);
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_STOPBGM:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    Cutscene_Command_StopBGM(play, csCtx, (void*)cutscenePtr);
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_FADEBGM:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    Cutscene_Command_FadeBGM(play, csCtx, (void*)cutscenePtr);
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_09:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    Cutscene_Command_09(play, csCtx, (void*)cutscenePtr);
                    cutscenePtr += sizeof(CutsceneData) * 3;
                }
                break;
            case CS_CMD_SETTIME:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    func_80065134(play, csCtx, (void*)cutscenePtr);
                    cutscenePtr += sizeof(CutsceneData) * 3;
                }
                break;
            case CS_CMD_SET_PLAYER_ACTION:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
                        csCtx->linkAction = (void*)cutscenePtr;
                    }
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_SET_ACTOR_ACTION_1:
            case 17:
            case 18:
            case 23:
            case 34:
            case 39:
            case 46:
            case 76:
            case 85:
            case 93:
            case 105:
            case 107:
            case 110:
            case 119:
            case 123:
            case 138:
            case 139:
            case 144:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
                        csCtx->npcActions[0] = (void*)cutscenePtr;
                    }
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_SET_ACTOR_ACTION_2:
            case 16:
            case 24:
            case 35:
            case 40:
            case 48:
            case 64:
            case 68:
            case 70:
            case 78:
            case 80:
            case 94:
            case 116:
            case 118:
            case 120:
            case 125:
            case 131:
            case 141:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
                        csCtx->npcActions[1] = (void*)cutscenePtr;
                    }
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_SET_ACTOR_ACTION_3:
            case 36:
            case 41:
            case 50:
            case 67:
            case 69:
            case 72:
            case 74:
            case 81:
            case 106:
            case 117:
            case 121:
            case 126:
            case 132:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
                        csCtx->npcActions[2] = (void*)cutscenePtr;
                    }
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_SET_ACTOR_ACTION_4:
            case 37:
            case 42:
            case 51:
            case 53:
            case 63:
            case 65:
            case 66:
            case 75:
            case 82:
            case 108:
            case 127:
            case 133:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
                        csCtx->npcActions[3] = (void*)cutscenePtr;
                    }
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_SET_ACTOR_ACTION_5:
            case 38:
            case 43:
            case 47:
            case 54:
            case 79:
            case 83:
            case 128:
            case 135:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
                        csCtx->npcActions[4] = (void*)cutscenePtr;
                    }
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_SET_ACTOR_ACTION_6:
            case 55:
            case 77:
            case 84:
            case 90:
            case 129:
            case 136:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
                        csCtx->npcActions[5] = (void*)cutscenePtr;
                    }
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_SET_ACTOR_ACTION_7:
            case 52:
            case 57:
            case 58:
            case 88:
            case 115:
            case 130:
            case 137:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
                        csCtx->npcActions[6] = (void*)cutscenePtr;
                    }
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_SET_ACTOR_ACTION_8:
            case 60:
            case 89:
            case 111:
            case 114:
            case 134:
            case 142:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
                        csCtx->npcActions[7] = (void*)cutscenePtr;
                    }
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_SET_ACTOR_ACTION_9:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
                        csCtx->npcActions[8] = (void*)cutscenePtr;
                    }
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_SET_ACTOR_ACTION_10:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if ((cmd->startFrame < csCtx->frames) && (csCtx->frames <= cmd->endFrame)) {
                        csCtx->npcActions[9] = (void*)cutscenePtr;
                    }
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
            case CS_CMD_CAM_EYE:
                cutscenePtr += Cutscene_Command_CameraPositions(play, csCtx, (void*)cutscenePtr, 0);
                break;
            case CS_CMD_CAM_EYE_REL_TO_PLAYER:
                cutscenePtr += Cutscene_Command_CameraPositions(play, csCtx, (void*)cutscenePtr, 1);
                break;
            case CS_CMD_CAM_AT:
                cutscenePtr += Cutscene_Command_CameraFocus(play, csCtx, (void*)cutscenePtr, 0);
                break;
            case CS_CMD_CAM_AT_REL_TO_PLAYER:
                cutscenePtr += Cutscene_Command_CameraFocus(play, csCtx, (void*)cutscenePtr, 1);
                break;
            case CS_CMD_07:
                cutscenePtr += Cutscene_Command_07(play, csCtx, (void*)cutscenePtr, 0);
                break;
            case CS_CMD_08:
                cutscenePtr += Cutscene_Command_08(play, csCtx, (void*)cutscenePtr, 0);
                break;
            case CS_CMD_TERMINATOR:
                cutscenePtr += 4;
                Cutscene_Command_Terminator(play, csCtx, (void*)cutscenePtr);
                cutscenePtr += 8;
                break;
            case CS_CMD_TEXTBOX:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += 4;
                for (j = 0; j < cmdEntries; j++) {
                    cmd = (CsCmdBase*)cutscenePtr;
                    if (cmd->base != 0xFFFF) {
                        Cutscene_Command_Textbox(play, csCtx, (void*)cutscenePtr);
                    }
                    cutscenePtr += sizeof(CutsceneData) * 3;
                }
                break;
            case CS_CMD_SCENE_TRANS_FX:
                cutscenePtr += sizeof(CutsceneData);
                Cutscene_Command_TransitionFX(play, csCtx, (void*)cutscenePtr);
                cutscenePtr += sizeof(CutsceneData) * 2;
                break;
            default:
                memcpy(&cmdEntries, cutscenePtr, sizeof(CutsceneData));
                cutscenePtr += sizeof(CutsceneData);
                for (j = 0; j < cmdEntries; j++) {
                    cutscenePtr += sizeof(CutsceneData) * 12;
                }
                break;
        }
    }
}

void func_80068C3C(PlayState* play, CutsceneContext* csCtx) {
    Gfx* displayList;
    Gfx* prevDisplayList;

    if (gSaveContext.cutsceneIndex >= 0xFFF0) {

        if (BREG(0) != 0) {
            OPEN_DISPS(play->state.gfxCtx);

            prevDisplayList = POLY_OPA_DISP;
            displayList = Graph_GfxPlusOne(POLY_OPA_DISP);
            gSPDisplayList(OVERLAY_DISP++, displayList);
            Cutscene_DrawDebugInfo(play, &displayList, csCtx);
            gSPEndDisplayList(displayList++);
            Graph_BranchDlist(prevDisplayList, displayList);
            POLY_OPA_DISP = displayList;

            CLOSE_DISPS(play->state.gfxCtx);
        }

        csCtx->frames++;
        if (dREG(95) != 0) {
            Cutscene_ProcessCommands(play, csCtx, gDebugCutsceneScript);
        } else {
            Cutscene_ProcessCommands(play, csCtx, play->csCtx.segment);
        }
    }
}

void func_80068D84(PlayState* play, CutsceneContext* csCtx) {
    if (func_8006472C(play, csCtx, 0.0f)) {
        Audio_SetCutsceneFlag(0);
        csCtx->state = CS_STATE_IDLE;
    }
}

void func_80068DC0(PlayState* play, CutsceneContext* csCtx) {
    s16 i;

    if (func_8006472C(play, csCtx, 0.0f)) {
        csCtx->linkAction = NULL;

        for (i = 0; i < 10; i++) {
            csCtx->npcActions[i] = NULL;
        }

        osSyncPrintf("\n\n\n\n\nやっぱりここかいな"); // "Right here, huh"
        gSaveContext.cutsceneIndex = 0;
        gSaveContext.gameMode = GAMEMODE_NORMAL;

        if (D_8015FCC8 != 0) {
            switch (gSaveContext.entranceIndex) {
                case ENTR_HYRULE_FIELD_11:
                case ENTR_HYRULE_FIELD_12:
                case ENTR_HYRULE_FIELD_13:
                case ENTR_HYRULE_FIELD_15:
                    Play_CopyCamera(play, D_8015FCC6, csCtx->unk_14);
            }

            Play_ChangeCameraStatus(play, D_8015FCC6, CAM_STAT_ACTIVE);
            Play_ClearCamera(play, csCtx->unk_14);
            func_8005B1A4(play->cameraPtrs[D_8015FCC6]);
        }

        Audio_SetCutsceneFlag(0);
        csCtx->state = CS_STATE_IDLE;
    }
}

void func_80068ECC(PlayState* play, CutsceneContext* csCtx) {
    u8 i;

    if ((gSaveContext.cutsceneTrigger != 0) && (csCtx->state == CS_STATE_IDLE) && !Player_InCsMode(play)) {
        gSaveContext.cutsceneIndex = 0xFFFD;
    }

    if ((gSaveContext.cutsceneIndex >= 0xFFF0) && (csCtx->state == CS_STATE_IDLE)) {
        Flags_UnsetEnv(play, 0);

        D_8011E1C0 = 0;
        D_8011E1C4 = 0;
        csCtx->unk_12 = 0;
        csCtx->linkAction = NULL;

        for (i = 0; i < 10; i++) {
            csCtx->npcActions[i] = NULL;
        }

        csCtx->state++;

        if (csCtx->state == CS_STATE_SKIPPABLE_INIT) {
            Audio_SetCutsceneFlag(1);

            csCtx->frames = 0xFFFF;
            csCtx->unk_18 = 0xFFFF;
            D_8015FCC0 = 0xFFFF;
            D_8015FCC2 = 0xFFFF;
            D_8015FCC4 = 0xFFFF;
            csCtx->unk_1A = 0;
            csCtx->unk_1B = 0;
            D_8015FCC6 = play->activeCamera;

            if (D_8015FCC8 != 0) {
                csCtx->unk_14 = Play_CreateSubCamera(play);
            }

            if (gSaveContext.cutsceneTrigger == 0) {
                Interface_ChangeAlpha(1);
                ShrinkWindow_SetVal(0x20);
                ShrinkWindow_SetCurrentVal(0x20);
                csCtx->state++;
            }

            func_80068C3C(play, csCtx);
        }

        gSaveContext.cutsceneTrigger = 0;
    }
}

void func_80069048(PlayState* play) {
    s16 i;

    D_8015FCCC = 0;
    for (i = 0; i < 20; i++) {
        ; // Empty Loop
    }
    D_8015FCE4 = 0;
}

void func_8006907C(PlayState* play) {
    if (D_8015FCCC != 0) {
        D_8015FCCC = 0;
    }
}

void Cutscene_HandleEntranceTriggers(PlayState* play) {
    EntranceCutscene* entranceCutscene;
    u8 requiredAge;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(sEntranceCutsceneTable); i++) {
        entranceCutscene = &sEntranceCutsceneTable[i];
        requiredAge = entranceCutscene->ageRestriction;
        if (GameInteractor_Should(VB_ALLOW_ENTRANCE_CS_FOR_EITHER_AGE, requiredAge == 2, entranceCutscene->entrance)) {
            requiredAge = gSaveContext.linkAge;
        }

        if ((gSaveContext.entranceIndex == entranceCutscene->entrance) &&
            (!Flags_GetEventChkInf(entranceCutscene->flag) || (entranceCutscene->flag == EVENTCHKINF_EPONA_OBTAINED)) &&
            (gSaveContext.cutsceneIndex < 0xFFF0) && ((u8)gSaveContext.linkAge == requiredAge) &&
            (gSaveContext.respawnFlag <= 0)) {
            Flags_SetEventChkInf(entranceCutscene->flag);
            if (GameInteractor_Should(VB_PLAY_ENTRANCE_CS, true, entranceCutscene->flag, entranceCutscene->entrance)) {
                Cutscene_SetSegment(play, entranceCutscene->segAddr);
                gSaveContext.cutsceneTrigger = 2;
                gSaveContext.showTitleCard = false;
            }
            break;
        }
    }
}

void Cutscene_HandleConditionalTriggers(PlayState* play) {
    osSyncPrintf("\ngame_info.mode=[%d] restart_flag", ((void)0, gSaveContext.respawnFlag));
    LUSLOG_INFO("Cutscene_HandleConditionalTriggers - entranceIndex: %#x cutsceneIndex: %#x",
                gSaveContext.entranceIndex, gSaveContext.cutsceneIndex);

    if (!GameInteractor_Should(VB_PLAY_TRANSITION_CS, true)) {
        return;
    }

    if ((gSaveContext.gameMode == GAMEMODE_NORMAL) && (gSaveContext.respawnFlag <= 0) &&
        (gSaveContext.cutsceneIndex < 0xFFF0)) {
        if ((gSaveContext.entranceIndex == ENTR_DESERT_COLOSSUS_OUTSIDE_TEMPLE) &&
            !Flags_GetEventChkInf(EVENTCHKINF_LEARNED_REQUIEM_OF_SPIRIT)) {
            Flags_SetEventChkInf(EVENTCHKINF_LEARNED_REQUIEM_OF_SPIRIT);
            gSaveContext.entranceIndex = ENTR_DESERT_COLOSSUS_EAST_EXIT;
            gSaveContext.cutsceneIndex = 0xFFF0;
        } else if (GameInteractor_Should(VB_BE_ELIGIBLE_FOR_NOCTURNE_OF_SHADOW,
                                         ((gSaveContext.entranceIndex == ENTR_KAKARIKO_VILLAGE_FRONT_GATE) &&
                                          LINK_IS_ADULT &&
                                          Flags_GetEventChkInf(EVENTCHKINF_USED_FOREST_TEMPLE_BLUE_WARP) &&
                                          Flags_GetEventChkInf(EVENTCHKINF_USED_FIRE_TEMPLE_BLUE_WARP) &&
                                          Flags_GetEventChkInf(EVENTCHKINF_USED_WATER_TEMPLE_BLUE_WARP) &&
                                          !Flags_GetEventChkInf(EVENTCHKINF_BONGO_BONGO_ESCAPED_FROM_WELL)))) {
            Flags_SetEventChkInf(EVENTCHKINF_BONGO_BONGO_ESCAPED_FROM_WELL);
            gSaveContext.cutsceneIndex = 0xFFF0;
        } else if ((gSaveContext.entranceIndex == ENTR_LOST_WOODS_BRIDGE_EAST_EXIT) &&
                   !Flags_GetEventChkInf(EVENTCHKINF_SPOKE_TO_SARIA_ON_BRIDGE)) {
            Flags_SetEventChkInf(EVENTCHKINF_SPOKE_TO_SARIA_ON_BRIDGE);
            if (GameInteractor_Should(VB_GIVE_ITEM_FAIRY_OCARINA, true)) {
                Item_Give(play, ITEM_OCARINA_FAIRY);
            }
            gSaveContext.entranceIndex = ENTR_LOST_WOODS_SOUTH_EXIT;
            gSaveContext.cutsceneIndex = 0xFFF0;
        } else if (GameInteractor_Should(
                       VB_BE_ELIGIBLE_FOR_LIGHT_ARROWS,
                       (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT) && CHECK_QUEST_ITEM(QUEST_MEDALLION_SHADOW) &&
                        LINK_IS_ADULT &&
                        !Flags_GetEventChkInf(EVENTCHKINF_RETURNED_TO_TEMPLE_OF_TIME_WITH_ALL_MEDALLIONS) &&
                        (gEntranceTable[((void)0, gSaveContext.entranceIndex)].scene == SCENE_TEMPLE_OF_TIME)))) {
            Flags_SetEventChkInf(EVENTCHKINF_RETURNED_TO_TEMPLE_OF_TIME_WITH_ALL_MEDALLIONS);
            gSaveContext.entranceIndex = ENTR_TEMPLE_OF_TIME_ENTRANCE;
            gSaveContext.cutsceneIndex = 0xFFF8;
        } else if (!Flags_GetEventChkInf(EVENTCHKINF_WATCHED_GANONS_CASTLE_COLLAPSE_CAUGHT_BY_GERUDO) &&
                   (gEntranceTable[((void)0, gSaveContext.entranceIndex)].scene == SCENE_GANON_BOSS)) {
            Flags_SetEventChkInf(EVENTCHKINF_WATCHED_GANONS_CASTLE_COLLAPSE_CAUGHT_BY_GERUDO);
            gSaveContext.entranceIndex = ENTR_GANON_BOSS_0;
            gSaveContext.cutsceneIndex = 0xFFF0;
        }
    }
}

void Cutscene_SetSegment(PlayState* play, void* segment) {
    if (SEGMENT_NUMBER(segment) != 0) {
        play->csCtx.segment = SEGMENTED_TO_VIRTUAL(segment);
    } else {
        play->csCtx.segment = segment;
    }
}

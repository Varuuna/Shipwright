#include "z_kaleido_scope.h"
#include "textures/icon_item_static/icon_item_static.h"
#include "textures/parameter_static/parameter_static.h"
#include "soh/Enhancements/cosmetics/cosmeticsTypes.h"
#include "soh/Enhancements/enhancementTypes.h"

static u8 sChildUpgrades[] = { UPG_BULLET_BAG, UPG_BOMB_BAG, UPG_STRENGTH, UPG_SCALE };
static u8 sAdultUpgrades[] = { UPG_QUIVER, UPG_BOMB_BAG, UPG_STRENGTH, UPG_SCALE };

static u8 sChildUpgradeItemBases[] = { ITEM_BULLET_BAG_30, ITEM_BOMB_BAG_20, ITEM_BRACELET, ITEM_SCALE_SILVER };
static u8 sAdultUpgradeItemBases[] = { ITEM_QUIVER_30, ITEM_BOMB_BAG_20, ITEM_BRACELET, ITEM_SCALE_SILVER };

static u8 sUpgradeItemOffsets[] = { 0x00, 0x03, 0x06, 0x09 };

static u8 sEquipmentItemOffsets[] = {
    0x00, 0x00, 0x01, 0x02, 0x00, 0x03, 0x04, 0x05, 0x00, 0x06, 0x07, 0x08, 0x00, 0x09, 0x0A, 0x0B,
};

// Vertices for A button indicator (coordinates 0.75x the texture size)
// pt (-97, -36)
static Vtx sStrengthAButtonVtx[] = {
    VTX(-9, 6, 0, 0 << 5, 0 << 5, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(9, 6, 0, 24 << 5, 0 << 5, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(-9, -6, 0, 0 << 5, 16 << 5, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(9, -6, 0, 24 << 5, 16 << 5, 0xFF, 0xFF, 0xFF, 0xFF),
};

static s16 sEquipTimer = 0;

extern int gPauseLinkFrameBuffer;

void KaleidoScope_DrawEquipmentImage(PlayState* play, void* source, u32 width, u32 height) {
    PauseContext* pauseCtx = &play->pauseCtx;
    u8* curTexture;
    s32 vtxIndex;
    s32 textureCount;
    s32 textureHeight;
    s32 remainingSize;
    s32 textureSize;
    s32 pad;
    s32 i;

    OPEN_DISPS(play->state.gfxCtx);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetTextureFilter(POLY_OPA_DISP++, G_TF_POINT);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

    curTexture = source;
    remainingSize = width * height * 2;
    textureHeight = 4096 / (width * 2);
    textureSize = width * textureHeight * 2;
    textureCount = remainingSize / textureSize;
    if ((remainingSize % textureSize) != 0) {
        textureCount += 1;
    }

    vtxIndex = 80;

    gDPSetTileCustom(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, textureHeight, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                     G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    remainingSize -= textureSize;

    textureCount = 1;

    // VERTEX Y EXTEND
    pauseCtx->equipVtx[vtxIndex + 2].v.ob[1] -= 80;
    pauseCtx->equipVtx[vtxIndex + 3].v.ob[1] -= 80;

    for (i = 0; i < textureCount; i++) {
        gSPVertex(POLY_OPA_DISP++, &pauseCtx->equipVtx[vtxIndex], 4, 0);

        gDPSetTextureImage(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, curTexture);

        gDPLoadSync(POLY_OPA_DISP++);
        gDPLoadTile(POLY_OPA_DISP++, G_TX_LOADTILE, 0, 0, (width - 1) << 2, (textureHeight - 1) << 2);

        gDPSetTextureImageFB(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, gPauseLinkFrameBuffer);
        gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);

        curTexture += textureSize;

        if ((remainingSize - textureSize) < 0) {
            if (remainingSize > 0) {
                textureHeight = remainingSize / (s32)(width * 2);
                remainingSize -= textureSize;

                gDPSetTileCustom(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, textureHeight, 0,
                                 G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                 G_TX_NOLOD, G_TX_NOLOD);
            }
        } else {
            remainingSize -= textureSize;
        }

        vtxIndex += 4;
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void KaleidoScope_DrawAButton(PlayState* play, Vtx* vtx, int16_t xTranslate, int16_t yTranslate) {
    PauseContext* pauseCtx = &play->pauseCtx;
    OPEN_DISPS(play->state.gfxCtx);
    Matrix_Push();

    Matrix_Translate(xTranslate, yTranslate, 0, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, MATRIX_NEWMTX(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    Color_RGB8 aButtonColor = { 0, 100, 255 };
    if (CVarGetInteger(CVAR_COSMETIC("HUD.AButton.Changed"), 0)) {
        aButtonColor = CVarGetColor24(CVAR_COSMETIC("HUD.AButton.Value"), aButtonColor);
    } else if (CVarGetInteger(CVAR_COSMETIC("DefaultColorScheme"), COLORSCHEME_N64) == COLORSCHEME_GAMECUBE) {
        aButtonColor = (Color_RGB8){ 0, 255, 100 };
    }

    gSPVertex(POLY_OPA_DISP++, vtx, 4, 0);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, aButtonColor.r, aButtonColor.g, aButtonColor.b, pauseCtx->alpha);
    gDPLoadTextureBlock(POLY_OPA_DISP++, gABtnSymbolTex, G_IM_FMT_IA, G_IM_SIZ_8b, 24, 16, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 4, 4, G_TX_NOLOD, G_TX_NOLOD);
    gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);
    Matrix_Pop();
    gSPMatrix(POLY_OPA_DISP++, MATRIX_NEWMTX(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    CLOSE_DISPS(play->state.gfxCtx);
}

void KaleidoScope_DrawPlayerWork(PlayState* play) {
    PauseContext* pauseCtx = &play->pauseCtx;
    Vec3f pos;
    Vec3s rot;
    f32 scale;

    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        pos.x = 2.0f;
        pos.y = -130.0f;
        pos.z = -150.0f;
        scale = 0.046f;
    } else if (CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD) != EQUIP_VALUE_SWORD_MASTER &&
               !CVarGetInteger(CVAR_GENERAL("PauseMenuAnimatedLinkTriforce"), 0)) {
        pos.x = 25.0f;
        pos.y = -228.0f;
        pos.z = 60.0f;
        scale = 0.056f;
    } else {
        pos.x = 20.0f;
        pos.y = -180.0f;
        pos.z = -40.0f;
        scale = 0.047f;
    }

    // SOH [Port] Draw the pause Link on a separate framebuffer starting in the work buffer
    OPEN_DISPS(play->state.gfxCtx);
    gsSPSetFB(WORK_DISP++, gPauseLinkFrameBuffer);

    rot.y = 32300;
    rot.x = rot.z = 0;
    Player_DrawPause(play, pauseCtx->playerSegment, &pauseCtx->playerSkelAnime, &pos, &rot, scale,
                     SWORD_EQUIP_TO_PLAYER(CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD)),
                     TUNIC_EQUIP_TO_PLAYER(CUR_EQUIP_VALUE(EQUIP_TYPE_TUNIC)),
                     SHIELD_EQUIP_TO_PLAYER(CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD)),
                     BOOTS_EQUIP_TO_PLAYER(CUR_EQUIP_VALUE(EQUIP_TYPE_BOOTS)));

    gsSPResetFB(WORK_DISP++);
    CLOSE_DISPS(play->state.gfxCtx);
}

void KaleidoScope_DrawEquipment(PlayState* play) {
    PauseContext* pauseCtx = &play->pauseCtx;
    Input* input = &play->state.input[0];
    u16 i;
    u16 j;
    u16 k;
    u16 bit;
    u16 temp;
    u16 point;
    u16 rowStart;
    u16 pad;
    s16 cursorMoveResult;
    u16 cursorItem;
    u16 cursorSlot = 0;
    s16 cursorPoint;
    s16 cursorX;
    s16 cursorY;
    s16 oldCursorPoint;
    bool dpad = (CVarGetInteger(CVAR_SETTING("DPadOnPause"), 0) && !CHECK_BTN_ALL(input->cur.button, BTN_CUP));
    bool pauseAnyCursor =
        (CVarGetInteger(CVAR_ENHANCEMENT("PauseAnyCursor"), 0) == PAUSE_ANY_CURSOR_RANDO_ONLY && IS_RANDO) ||
        (CVarGetInteger(CVAR_ENHANCEMENT("PauseAnyCursor"), 0) == PAUSE_ANY_CURSOR_ALWAYS_ON);

    OPEN_DISPS(play->state.gfxCtx);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, ZREG(39), ZREG(40), ZREG(41), pauseCtx->alpha);
    gDPSetEnvColor(POLY_OPA_DISP++, ZREG(43), ZREG(44), ZREG(45), 0);

    for (i = 0, j = 64; i < 4; i++, j += 4) {
        if (CUR_EQUIP_VALUE(i) != 0) {
            gDPPipeSync(POLY_OPA_DISP++);
            gSPVertex(POLY_OPA_DISP++, &pauseCtx->equipVtx[j], 4, 0);

            POLY_OPA_DISP = KaleidoScope_QuadTextureIA8(POLY_OPA_DISP, gEquippedItemOutlineTex, 32, 32, 0);
        }
    }

    if ((pauseCtx->state == 6) && (pauseCtx->unk_1E4 == 0) && (pauseCtx->pageIndex == PAUSE_EQUIP)) {
        oldCursorPoint = pauseCtx->cursorPoint[PAUSE_EQUIP];
        pauseCtx->cursorColorSet = 0;

        if (pauseCtx->cursorSpecialPos == 0) {
            pauseCtx->nameColorSet = 0;

            cursorItem = pauseCtx->cursorItem[PAUSE_EQUIP];
            if ((cursorItem >= ITEM_SWORD_KOKIRI) && (cursorItem <= ITEM_BOOTS_HOVER)) {
                pauseCtx->cursorColorSet = 8;
            }

            cursorPoint = pauseCtx->cursorPoint[PAUSE_EQUIP];
            cursorX = pauseCtx->cursorX[PAUSE_EQUIP];
            cursorY = pauseCtx->cursorY[PAUSE_EQUIP];

            cursorMoveResult = 0;
            while (cursorMoveResult == 0) {
                if ((pauseCtx->stickRelX < -30) || (dpad && CHECK_BTN_ALL(input->press.button, BTN_DLEFT))) {
                    if (pauseCtx->cursorX[PAUSE_EQUIP] != 0) {
                        pauseCtx->cursorX[PAUSE_EQUIP] -= 1;
                        pauseCtx->cursorPoint[PAUSE_EQUIP] -= 1;

                        if (pauseCtx->cursorX[PAUSE_EQUIP] == 0) {
                            if (pauseCtx->cursorY[PAUSE_EQUIP] == 0) {
                                if (CUR_UPG_VALUE(UPG_BULLET_BAG) != 0) {
                                    cursorMoveResult = 1;
                                }
                            } else {
                                if (CUR_UPG_VALUE(pauseCtx->cursorY[PAUSE_EQUIP]) != 0) {
                                    cursorMoveResult = 1;
                                }
                            }
                        } else if ((gBitFlags[pauseCtx->cursorPoint[PAUSE_EQUIP] - 1] &
                                    gSaveContext.inventory.equipment) ||
                                   pauseAnyCursor) {
                            cursorMoveResult = 2;
                        }
                    } else {
                        pauseCtx->cursorX[PAUSE_EQUIP] = cursorX;
                        pauseCtx->cursorY[PAUSE_EQUIP] += 1;

                        if (pauseCtx->cursorY[PAUSE_EQUIP] >= 4) {
                            pauseCtx->cursorY[PAUSE_EQUIP] = 0;
                        }

                        pauseCtx->cursorPoint[PAUSE_EQUIP] =
                            pauseCtx->cursorX[PAUSE_EQUIP] + (pauseCtx->cursorY[PAUSE_EQUIP] * 4);

                        if (pauseCtx->cursorPoint[PAUSE_EQUIP] >= 16) {
                            pauseCtx->cursorPoint[PAUSE_EQUIP] = pauseCtx->cursorX[PAUSE_EQUIP];
                        }

                        if (cursorY == pauseCtx->cursorY[PAUSE_EQUIP]) {
                            pauseCtx->cursorX[PAUSE_EQUIP] = cursorX;
                            pauseCtx->cursorPoint[PAUSE_EQUIP] = cursorPoint;
                            KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_LEFT);
                            cursorMoveResult = 3;
                        }
                    }
                } else if ((pauseCtx->stickRelX > 30) || (dpad && CHECK_BTN_ALL(input->press.button, BTN_DRIGHT))) {
                    if (pauseCtx->cursorX[PAUSE_EQUIP] < 3) {
                        pauseCtx->cursorX[PAUSE_EQUIP] += 1;
                        pauseCtx->cursorPoint[PAUSE_EQUIP] += 1;

                        if (pauseCtx->cursorX[PAUSE_EQUIP] == 0) {
                            if (CUR_UPG_VALUE(pauseCtx->cursorY[PAUSE_EQUIP]) != 0) {
                                cursorMoveResult = 1;
                            }
                        } else if ((gBitFlags[pauseCtx->cursorPoint[PAUSE_EQUIP] - 1] &
                                    gSaveContext.inventory.equipment) ||
                                   pauseAnyCursor) {
                            cursorMoveResult = 2;
                        }
                    } else {
                        pauseCtx->cursorX[PAUSE_EQUIP] = cursorX;
                        pauseCtx->cursorY[PAUSE_EQUIP] += 1;

                        if (pauseCtx->cursorY[PAUSE_EQUIP] >= 4) {
                            pauseCtx->cursorY[PAUSE_EQUIP] = 0;
                        }

                        pauseCtx->cursorPoint[PAUSE_EQUIP] =
                            pauseCtx->cursorX[PAUSE_EQUIP] + (pauseCtx->cursorY[PAUSE_EQUIP] * 4);

                        if (pauseCtx->cursorPoint[PAUSE_EQUIP] >= 16) {
                            pauseCtx->cursorPoint[PAUSE_EQUIP] = pauseCtx->cursorX[PAUSE_EQUIP];
                        }

                        if (cursorY == pauseCtx->cursorY[PAUSE_EQUIP]) {
                            pauseCtx->cursorX[PAUSE_EQUIP] = cursorX;
                            pauseCtx->cursorPoint[PAUSE_EQUIP] = cursorPoint;
                            KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_RIGHT);
                            cursorMoveResult = 3;
                        }
                    }
                } else {
                    cursorMoveResult = 4;
                }
            }

            cursorPoint = pauseCtx->cursorPoint[PAUSE_EQUIP];
            cursorY = pauseCtx->cursorY[PAUSE_EQUIP];

            if (cursorMoveResult) {}

            cursorMoveResult = 0;
            while (cursorMoveResult == 0) {
                if ((pauseCtx->stickRelY > 30) || (dpad && CHECK_BTN_ALL(input->press.button, BTN_DUP))) {
                    if (pauseCtx->cursorY[PAUSE_EQUIP] != 0) {
                        pauseCtx->cursorY[PAUSE_EQUIP] -= 1;
                        pauseCtx->cursorPoint[PAUSE_EQUIP] -= 4;

                        if (pauseCtx->cursorX[PAUSE_EQUIP] == 0) {
                            if (pauseCtx->cursorY[PAUSE_EQUIP] == 0) {
                                if (CUR_UPG_VALUE(UPG_BULLET_BAG) != 0) {
                                    cursorMoveResult = 1;
                                }
                            } else if (CUR_UPG_VALUE(pauseCtx->cursorY[PAUSE_EQUIP]) != 0) {
                                cursorMoveResult = 1;
                            }
                        } else if ((gBitFlags[pauseCtx->cursorPoint[PAUSE_EQUIP] - 1] &
                                    gSaveContext.inventory.equipment) ||
                                   pauseAnyCursor) {
                            cursorMoveResult = 2;
                        }
                    } else {
                        pauseCtx->cursorY[PAUSE_EQUIP] = cursorY;
                        pauseCtx->cursorPoint[PAUSE_EQUIP] = cursorPoint;
                        cursorMoveResult = 3;
                    }
                } else if ((pauseCtx->stickRelY < -30) || (dpad && CHECK_BTN_ALL(input->press.button, BTN_DDOWN))) {
                    if (pauseCtx->cursorY[PAUSE_EQUIP] < 3) {
                        pauseCtx->cursorY[PAUSE_EQUIP] += 1;
                        pauseCtx->cursorPoint[PAUSE_EQUIP] += 4;

                        if (pauseCtx->cursorX[PAUSE_EQUIP] == 0) {
                            if (CUR_UPG_VALUE(pauseCtx->cursorY[PAUSE_EQUIP]) != 0) {
                                cursorMoveResult = 1;
                            }
                        } else if ((gBitFlags[pauseCtx->cursorPoint[PAUSE_EQUIP] - 1] &
                                    gSaveContext.inventory.equipment) ||
                                   pauseAnyCursor) {
                            cursorMoveResult = 2;
                        }
                    } else {
                        pauseCtx->cursorY[PAUSE_EQUIP] = cursorY;
                        pauseCtx->cursorPoint[PAUSE_EQUIP] = cursorPoint;
                        cursorMoveResult = 3;
                    }
                } else {
                    cursorMoveResult = 4;
                }
            }
        } else if (pauseCtx->cursorSpecialPos == PAUSE_CURSOR_PAGE_LEFT) {
            if ((pauseCtx->stickRelX > 30) || (dpad && CHECK_BTN_ALL(input->press.button, BTN_DRIGHT))) {
                pauseCtx->nameDisplayTimer = 0;
                pauseCtx->cursorSpecialPos = 0;

                Audio_PlaySoundGeneral(NA_SE_SY_CURSOR, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                       &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);

                cursorPoint = cursorX = cursorY = 0;
                while (true) {
                    if (cursorX == 0) {
                        if (cursorY == 0) {
                            if (CUR_UPG_VALUE(UPG_BULLET_BAG) != 0) {
                                pauseCtx->cursorPoint[PAUSE_EQUIP] = cursorPoint;
                                pauseCtx->cursorX[PAUSE_EQUIP] = cursorX;
                                pauseCtx->cursorY[PAUSE_EQUIP] = cursorY;
                                break;
                            }
                        } else if (CUR_UPG_VALUE(cursorY) != 0) {
                            pauseCtx->cursorPoint[PAUSE_EQUIP] = cursorPoint;
                            pauseCtx->cursorX[PAUSE_EQUIP] = cursorX;
                            pauseCtx->cursorY[PAUSE_EQUIP] = cursorY;
                            break;
                        }
                    } else if (gBitFlags[cursorPoint - 1] & gSaveContext.inventory.equipment) {
                        pauseCtx->cursorPoint[PAUSE_EQUIP] = cursorPoint;
                        pauseCtx->cursorX[PAUSE_EQUIP] = cursorX;
                        pauseCtx->cursorY[PAUSE_EQUIP] = cursorY;
                        break;
                    }

                    cursorY = cursorY + 1;
                    cursorPoint = cursorPoint + 4;
                    if (cursorY < 4) {
                        continue;
                    }

                    cursorY = 0;
                    cursorPoint = cursorX + 1;
                    cursorX = cursorPoint;
                    if (cursorX < 4) {
                        continue;
                    }

                    KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_RIGHT);
                    break;
                }
            }
        } else {
            if ((pauseCtx->stickRelX < -30) || (dpad && CHECK_BTN_ALL(input->press.button, BTN_DLEFT))) {
                pauseCtx->nameDisplayTimer = 0;
                pauseCtx->cursorSpecialPos = 0;
                Audio_PlaySoundGeneral(NA_SE_SY_CURSOR, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                       &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);

                cursorPoint = cursorX = 3;
                cursorY = 0;
                while (true) {
                    if (cursorX == 0) {
                        if (CUR_UPG_VALUE(cursorY) != 0) {
                            pauseCtx->cursorPoint[PAUSE_EQUIP] = cursorPoint;
                            pauseCtx->cursorX[PAUSE_EQUIP] = cursorX;
                            pauseCtx->cursorY[PAUSE_EQUIP] = cursorY;
                            break;
                        }
                    } else if (gBitFlags[cursorPoint - 1] & gSaveContext.inventory.equipment) {
                        pauseCtx->cursorPoint[PAUSE_EQUIP] = cursorPoint;
                        pauseCtx->cursorX[PAUSE_EQUIP] = cursorX;
                        pauseCtx->cursorY[PAUSE_EQUIP] = cursorY;
                        break;
                    }

                    cursorY = cursorY + 1;
                    cursorPoint = cursorPoint + 4;
                    if (cursorY < 4) {
                        continue;
                    }

                    cursorY = 0;
                    cursorPoint = cursorX - 1;
                    cursorX = cursorPoint;
                    if (cursorX >= 0) {
                        continue;
                    }

                    KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_LEFT);
                    break;
                }
            }
        }

        if (pauseCtx->cursorX[PAUSE_EQUIP] == 0) {
            pauseCtx->cursorColorSet = 0;

            if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
                if ((pauseCtx->cursorY[PAUSE_EQUIP] == 0) && (CUR_UPG_VALUE(UPG_BULLET_BAG) != 0)) {
                    cursorItem = ITEM_BULLET_BAG_30 + CUR_UPG_VALUE(UPG_BULLET_BAG) - 1;
                } else {
                    cursorItem = ITEM_QUIVER_30 + sUpgradeItemOffsets[pauseCtx->cursorY[PAUSE_EQUIP]] +
                                 CUR_UPG_VALUE(pauseCtx->cursorY[PAUSE_EQUIP]) - 1;
                    osSyncPrintf("H_arrowcase_1 + non_equip_item_table = %d\n", cursorItem);
                }
            } else {
                if ((pauseCtx->cursorY[PAUSE_EQUIP] == 0) && (CUR_UPG_VALUE(UPG_QUIVER) == 0)) {
                    cursorItem = ITEM_BULLET_BAG_30 + CUR_UPG_VALUE(UPG_BULLET_BAG) - 1;
                } else {
                    cursorItem = ITEM_QUIVER_30 + sUpgradeItemOffsets[pauseCtx->cursorY[PAUSE_EQUIP]] +
                                 CUR_UPG_VALUE(pauseCtx->cursorY[PAUSE_EQUIP]) - 1;
                    osSyncPrintf("大人 H_arrowcase_1 + non_equip_item_table = %d\n", cursorItem);
                }
            }
        } else {
            cursorItem = ITEM_SWORD_KOKIRI + sEquipmentItemOffsets[pauseCtx->cursorPoint[PAUSE_EQUIP]];
            osSyncPrintf("ccc=%d\n", cursorItem);

            if (pauseCtx->cursorSpecialPos == 0) {
                pauseCtx->cursorColorSet = 8;
            }
        }

        if ((pauseCtx->cursorY[PAUSE_EQUIP] == 0) && (pauseCtx->cursorX[PAUSE_EQUIP] == 3)) {
            if (gSaveContext.bgsFlag != 0) {
                cursorItem = ITEM_HEART_PIECE_2;
            } else if (CHECK_OWNED_EQUIP_ALT(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BROKENGIANTKNIFE)) {
                cursorItem = ITEM_SWORD_KNIFE;
            }
        }

        cursorSlot = pauseCtx->cursorPoint[PAUSE_EQUIP];

        pauseCtx->cursorItem[PAUSE_EQUIP] = cursorItem;
        pauseCtx->cursorSlot[PAUSE_EQUIP] = cursorSlot;

        osSyncPrintf("kscope->select_name[Display_Equipment] = %d\n", pauseCtx->cursorItem[PAUSE_EQUIP]);

        if (!(CHECK_AGE_REQ_EQUIP(pauseCtx->cursorY[PAUSE_EQUIP], pauseCtx->cursorX[PAUSE_EQUIP]))) {
            pauseCtx->nameColorSet = 1;
        }

        if (pauseCtx->cursorItem[PAUSE_EQUIP] == ITEM_BRACELET) {
            if (LINK_AGE_IN_YEARS == YEARS_CHILD || IS_RANDO) {
                pauseCtx->nameColorSet = 0;
            } else {
                pauseCtx->nameColorSet = 1;
            }
        }

        if ((pauseCtx->cursorX[PAUSE_EQUIP] == 0) && (pauseCtx->cursorY[PAUSE_EQUIP] == 0)) {
            if (LINK_AGE_IN_YEARS != YEARS_CHILD) {
                if ((cursorItem >= ITEM_BULLET_BAG_30) && (cursorItem <= ITEM_BULLET_BAG_50)) {
                    pauseCtx->nameColorSet = 1;
                } else {
                    pauseCtx->nameColorSet = 0;
                }
            } else {
                pauseCtx->nameColorSet = 0;
            }
        }

        KaleidoScope_SetCursorVtx(pauseCtx, cursorSlot * 4, pauseCtx->equipVtx);

        // Allow Toggling of Strength when Pressing A on Strength Upgrade Slot
        if ((pauseCtx->cursorSpecialPos == 0) && (pauseCtx->state == 6) && (pauseCtx->unk_1E4 == 0) &&
            CHECK_BTN_ALL(input->press.button, BTN_A) && (pauseCtx->cursorX[PAUSE_EQUIP] == 0) &&
            (pauseCtx->cursorY[PAUSE_EQUIP] == 2) && CVarGetInteger(CVAR_ENHANCEMENT("ToggleStrength"), 0)) {
            CVarSetInteger(CVAR_ENHANCEMENT("StrengthDisabled"),
                           !CVarGetInteger(CVAR_ENHANCEMENT("StrengthDisabled"), 0));
            // Equip success sound
            Audio_PlaySoundGeneral(NA_SE_SY_DECIDE, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                   &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            // Wait 10 frames before accepting input again
            pauseCtx->unk_1E4 = 7;
            sEquipTimer = 10;
        }

        u16 buttonsToCheck = BTN_A | BTN_CLEFT | BTN_CDOWN | BTN_CRIGHT;
        if (CVarGetInteger(CVAR_ENHANCEMENT("DpadEquips"), 0) &&
            (!CVarGetInteger(CVAR_SETTING("DPadOnPause"), 0) || CHECK_BTN_ALL(input->cur.button, BTN_CUP))) {
            buttonsToCheck |= BTN_DUP | BTN_DDOWN | BTN_DLEFT | BTN_DRIGHT;
        }

        if ((pauseCtx->cursorSpecialPos == 0) && (cursorItem != PAUSE_ITEM_NONE) && (pauseCtx->state == 6) &&
            (pauseCtx->unk_1E4 == 0) && CHECK_BTN_ANY(input->press.button, buttonsToCheck) &&
            (pauseCtx->cursorX[PAUSE_EQUIP] != 0)) {

            if (CHECK_AGE_REQ_EQUIP(pauseCtx->cursorY[PAUSE_EQUIP], pauseCtx->cursorX[PAUSE_EQUIP])) {
                if (CHECK_BTN_ALL(input->press.button, BTN_A)) {

                    // #Region SoH [Enhancements]
                    // Allow Link to remove his equipment from the equipment subscreen by toggling on/off
                    // Shields will be un-equipped entirely, and tunics/boots will revert to Kokiri Tunic/Kokiri Boots
                    // Only BGS/Giant's Knife is affected, and it will revert to Master Sword.

                    // If we have the feature toggled on
                    if (CVarGetInteger(CVAR_ENHANCEMENT("EquipmentCanBeRemoved"), 0)) {

                        if (CVarGetInteger(CVAR_ENHANCEMENT("SwordToggle"), SWORD_TOGGLE_NONE) ==
                                SWORD_TOGGLE_BOTH_AGES ||
                            (CVarGetInteger(CVAR_ENHANCEMENT("SwordToggle"), SWORD_TOGGLE_NONE) ==
                             SWORD_TOGGLE_CHILD) &&
                                LINK_IS_CHILD) {
                            // If we're on the "swords" section of the equipment screen AND we're on a
                            // currently-equipped sword
                            if (pauseCtx->cursorY[PAUSE_EQUIP] == 0 &&
                                pauseCtx->cursorX[PAUSE_EQUIP] == CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD)) {
                                Inventory_ChangeEquipment(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_NONE);
                                gSaveContext.equips.buttonItems[0] = ITEM_NONE;
                                Flags_SetInfTable(INFTABLE_SWORDLESS);
                                goto RESUME_EQUIPMENT_SWORD; // Skip to here so we don't re-equip it
                            }
                        } else {
                            // If we're on the "swords" section of the equipment screen AND we're on a
                            // currently-equipped BGS/Giant's Knife
                            if (pauseCtx->cursorY[PAUSE_EQUIP] == 0 && pauseCtx->cursorX[PAUSE_EQUIP] == 3 &&
                                CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD) == EQUIP_VALUE_SWORD_BIGGORON &&
                                CHECK_OWNED_EQUIP(EQUIP_TYPE_SWORD,
                                                  EQUIP_INV_SWORD_MASTER)) { // And we have the Master Sword
                                Inventory_ChangeEquipment(
                                    EQUIP_TYPE_SWORD,
                                    EQUIP_VALUE_SWORD_MASTER); // "Unequip" it by equipping Master Sword
                                gSaveContext.equips.buttonItems[0] = ITEM_SWORD_MASTER;
                                Flags_UnsetInfTable(INFTABLE_SWORDLESS);
                                goto RESUME_EQUIPMENT_SWORD; // Skip to here so we don't re-equip it
                            }
                        }

                        // If we're on the "shields" section of the equipment screen AND we're on a currently-equipped
                        // shield
                        if (pauseCtx->cursorY[PAUSE_EQUIP] == 1 &&
                            pauseCtx->cursorX[PAUSE_EQUIP] == CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD)) {
                            Inventory_ChangeEquipment(EQUIP_TYPE_SHIELD, EQUIP_VALUE_SHIELD_NONE); // Unequip it
                            goto RESUME_EQUIPMENT; // Skip to here so we don't re-equip it
                        }

                        // If we're on the "tunics" section of the equipment screen AND we're on a currently-equipped
                        // tunic
                        if (pauseCtx->cursorY[PAUSE_EQUIP] == 2 &&
                            pauseCtx->cursorX[PAUSE_EQUIP] == CUR_EQUIP_VALUE(EQUIP_TYPE_TUNIC)) {
                            Inventory_ChangeEquipment(
                                EQUIP_TYPE_TUNIC, EQUIP_VALUE_TUNIC_KOKIRI); // "Unequip" it (by equipping Kokiri Tunic)
                            goto RESUME_EQUIPMENT;                           // Skip to here so we don't re-equip it
                        }

                        // If we're on the "boots" section of the equipment screen AND we're on currently-equipped boots
                        if (pauseCtx->cursorY[PAUSE_EQUIP] == 3 &&
                            pauseCtx->cursorX[PAUSE_EQUIP] == CUR_EQUIP_VALUE(EQUIP_TYPE_BOOTS)) {
                            Inventory_ChangeEquipment(
                                EQUIP_TYPE_BOOTS, EQUIP_VALUE_BOOTS_KOKIRI); // "Unequip" it (by equipping Kokiri Boots)
                            goto RESUME_EQUIPMENT;                           // Skip to here so we don't re-equip it
                        }
                    }

                    if (CHECK_OWNED_EQUIP(pauseCtx->cursorY[PAUSE_EQUIP], pauseCtx->cursorX[PAUSE_EQUIP] - 1)) {
                        Inventory_ChangeEquipment(pauseCtx->cursorY[PAUSE_EQUIP], pauseCtx->cursorX[PAUSE_EQUIP]);
                    } else {
                        goto EQUIP_FAIL;
                    }

                RESUME_EQUIPMENT:
                    if (pauseCtx->cursorY[PAUSE_EQUIP] == 0) {
                        gSaveContext.infTable[29] = 0;
                        gSaveContext.equips.buttonItems[0] = cursorItem;

                        if ((pauseCtx->cursorX[PAUSE_EQUIP] == 3) && (gSaveContext.bgsFlag != 0)) {
                            gSaveContext.equips.buttonItems[0] = ITEM_SWORD_BGS;
                            gSaveContext.swordHealth = 8;
                        } else {
                            if (gSaveContext.equips.buttonItems[0] == ITEM_HEART_PIECE_2) {
                                gSaveContext.equips.buttonItems[0] = ITEM_SWORD_BGS;
                            }
                            if ((gSaveContext.equips.buttonItems[0] == ITEM_SWORD_BGS) && (gSaveContext.bgsFlag == 0) &&
                                CHECK_OWNED_EQUIP_ALT(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BROKENGIANTKNIFE)) {
                                gSaveContext.equips.buttonItems[0] = ITEM_SWORD_KNIFE;
                            }
                        }
                    RESUME_EQUIPMENT_SWORD:
                        Interface_LoadItemIcon1(play, 0);
                    }

                    Audio_PlaySoundGeneral(NA_SE_SY_DECIDE, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                           &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                    pauseCtx->unk_1E4 = 7;
                    sEquipTimer = 10;
                } else if (CVarGetInteger(CVAR_ENHANCEMENT("AssignableTunicsAndBoots"), 0) != 0) {
                    // Only allow assigning tunic and boots to c-buttons
                    if (pauseCtx->cursorY[PAUSE_EQUIP] > 1) {
                        if (CHECK_OWNED_EQUIP(pauseCtx->cursorY[PAUSE_EQUIP], pauseCtx->cursorX[PAUSE_EQUIP] - 1)) {
                            u16 slot = 0;
                            switch (cursorItem) {
                                case ITEM_TUNIC_KOKIRI:
                                    slot = SLOT_TUNIC_KOKIRI;
                                    break;
                                case ITEM_TUNIC_GORON:
                                    slot = SLOT_TUNIC_GORON;
                                    break;
                                case ITEM_TUNIC_ZORA:
                                    slot = SLOT_TUNIC_ZORA;
                                    break;
                                case ITEM_BOOTS_KOKIRI:
                                    slot = SLOT_BOOTS_KOKIRI;
                                    break;
                                case ITEM_BOOTS_IRON:
                                    slot = SLOT_BOOTS_IRON;
                                    break;
                                case ITEM_BOOTS_HOVER:
                                    slot = SLOT_BOOTS_HOVER;
                                    break;
                                default:
                                    break;
                            }
                            KaleidoScope_SetupItemEquip(play, cursorItem, slot,
                                                        pauseCtx->equipVtx[cursorSlot * 4].v.ob[0] * 10,
                                                        pauseCtx->equipVtx[cursorSlot * 4].v.ob[1] * 10);
                        } else {
                            Audio_PlaySoundGeneral(NA_SE_SY_ERROR, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                                   &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                        }
                    }
                }
            } else {
            EQUIP_FAIL:
                if (CHECK_BTN_ALL(input->press.button, BTN_A)) {
                    Audio_PlaySoundGeneral(NA_SE_SY_ERROR, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                           &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                } else if ((CVarGetInteger(CVAR_ENHANCEMENT("AssignableTunicsAndBoots"), 0) != 0) &&
                           (pauseCtx->cursorY[PAUSE_EQUIP] > 1)) {
                    Audio_PlaySoundGeneral(NA_SE_SY_ERROR, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                           &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                }
            }
        }

        if (oldCursorPoint != pauseCtx->cursorPoint[PAUSE_EQUIP]) {
            Audio_PlaySoundGeneral(NA_SE_SY_CURSOR, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                   &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
        }
    } else if ((pauseCtx->unk_1E4 == 7) && (pauseCtx->pageIndex == PAUSE_EQUIP)) {
        KaleidoScope_SetCursorVtx(pauseCtx, pauseCtx->cursorSlot[PAUSE_EQUIP] * 4, pauseCtx->equipVtx);
        pauseCtx->cursorColorSet = 8;

        sEquipTimer--;
        if (sEquipTimer == 0) {
            pauseCtx->unk_1E4 = 0;
        }
    }

    // Grey Out Strength Upgrade Name when Disabled
    // Do not Grey Out Strength Upgrade Name when Enabled
    // This needs to be outside the previous block since otherwise the nameColorSet is cleared to 0 by other menu pages
    // when toggling
    if ((pauseCtx->pageIndex == PAUSE_EQUIP) && (pauseCtx->cursorX[PAUSE_EQUIP] == 0) &&
        (pauseCtx->cursorY[PAUSE_EQUIP] == 2) && CVarGetInteger(CVAR_ENHANCEMENT("ToggleStrength"), 0)) {
        if (CVarGetInteger(CVAR_ENHANCEMENT("StrengthDisabled"), 0)) {
            pauseCtx->nameColorSet = 1;
        } else {
            pauseCtx->nameColorSet = 0;
        }
    }

    for (rowStart = 0, i = 0, point = 4; i < 4; i++, rowStart += 4, point += 16) {

        for (k = 0, temp = rowStart + 1, bit = rowStart, j = point; k < 3; k++, bit++, j += 4, temp++) {

            if ((gBitFlags[bit] & gSaveContext.inventory.equipment) && (pauseCtx->cursorSpecialPos == 0)) {
                if (CHECK_AGE_REQ_EQUIP(i, k + 1)) {
                    if (temp == cursorSlot) {
                        pauseCtx->equipVtx[j].v.ob[0] = pauseCtx->equipVtx[j + 2].v.ob[0] =
                            pauseCtx->equipVtx[j].v.ob[0] - 2;
                        pauseCtx->equipVtx[j + 1].v.ob[0] = pauseCtx->equipVtx[j + 3].v.ob[0] =
                            pauseCtx->equipVtx[j + 1].v.ob[0] + 4;
                        pauseCtx->equipVtx[j].v.ob[1] = pauseCtx->equipVtx[j + 1].v.ob[1] =
                            pauseCtx->equipVtx[j].v.ob[1] + 2;
                        pauseCtx->equipVtx[j + 2].v.ob[1] = pauseCtx->equipVtx[j + 3].v.ob[1] =
                            pauseCtx->equipVtx[j + 2].v.ob[1] - 4;
                    }
                }
            }
        }
    }

    // Add zoom effect to strength item if cursor is hovering over it when toggle option is on
    if ((pauseCtx->cursorX[PAUSE_EQUIP] == 0) && (pauseCtx->cursorY[PAUSE_EQUIP] == 2) &&
        CVarGetInteger(CVAR_ENHANCEMENT("ToggleStrength"), 0) && pauseCtx->cursorSpecialPos == 0) {
        u8 row = 2;
        u8 column = 0;
        u8 equipVtxIndex = 16 * row + 4 * column;
        pauseCtx->equipVtx[equipVtxIndex].v.ob[0] = pauseCtx->equipVtx[equipVtxIndex + 2].v.ob[0] =
            pauseCtx->equipVtx[equipVtxIndex].v.ob[0] - 2;
        pauseCtx->equipVtx[equipVtxIndex + 1].v.ob[0] = pauseCtx->equipVtx[equipVtxIndex + 3].v.ob[0] =
            pauseCtx->equipVtx[equipVtxIndex + 1].v.ob[0] + 4;
        pauseCtx->equipVtx[equipVtxIndex].v.ob[1] = pauseCtx->equipVtx[equipVtxIndex + 1].v.ob[1] =
            pauseCtx->equipVtx[equipVtxIndex].v.ob[1] + 2;
        pauseCtx->equipVtx[equipVtxIndex + 2].v.ob[1] = pauseCtx->equipVtx[equipVtxIndex + 3].v.ob[1] =
            pauseCtx->equipVtx[equipVtxIndex + 2].v.ob[1] - 4;
    }

    Gfx_SetupDL_42Opa(play->state.gfxCtx);

    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

    for (rowStart = 0, j = 0, temp = 0, i = 0; i < 4; i++, rowStart += 4, j += 16) {
        gSPVertex(POLY_OPA_DISP++, &pauseCtx->equipVtx[j], 16, 0);
        bool drawGreyItems = !CVarGetInteger(CVAR_CHEAT("TimelessEquipment"), 0);
        if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
            point = CUR_UPG_VALUE(sChildUpgrades[i]);
            if ((point != 0) && (CUR_UPG_VALUE(sChildUpgrades[i]) != 0)) {
                // Grey Out the Gauntlets as Child
                // Grey Out Strength Upgrades when Disabled and the Toggle Strength Option is on
                if ((drawGreyItems &&
                     ((sChildUpgradeItemBases[i] + CUR_UPG_VALUE(sChildUpgrades[i]) - 1) == ITEM_GAUNTLETS_SILVER ||
                      (sChildUpgradeItemBases[i] + CUR_UPG_VALUE(sChildUpgrades[i]) - 1) == ITEM_GAUNTLETS_GOLD)) ||
                    (CVarGetInteger(CVAR_ENHANCEMENT("ToggleStrength"), 0) &&
                     CVarGetInteger(CVAR_ENHANCEMENT("StrengthDisabled"), 0) && sChildUpgrades[i] == UPG_STRENGTH)) {
                    gDPSetGrayscaleColor(POLY_OPA_DISP++, 109, 109, 109, 255);
                    gSPGrayscale(POLY_OPA_DISP++, true);
                }
                KaleidoScope_DrawQuadTextureRGBA32(play->state.gfxCtx,
                                                   gItemIcons[sChildUpgradeItemBases[i] + point - 1], 32, 32, 0);
                gSPGrayscale(POLY_OPA_DISP++, false);
            }
        } else {
            if ((i == 0) &&
                (CUR_UPG_VALUE(sAdultUpgrades[i]) ==
                 0)) { // If the player doesn't have the bow, load the current slingshot ammo upgrade instead.
                if (drawGreyItems) {
                    gDPSetGrayscaleColor(POLY_OPA_DISP++, 109, 109, 109, 255); // Grey Out Slingshot Bullet Bags
                    gSPGrayscale(POLY_OPA_DISP++, true);
                }
                KaleidoScope_DrawQuadTextureRGBA32(
                    play->state.gfxCtx, gItemIcons[sChildUpgradeItemBases[i] + CUR_UPG_VALUE(sChildUpgrades[i]) - 1],
                    32, 32, 0);
                gSPGrayscale(POLY_OPA_DISP++, false);
            } else if (CUR_UPG_VALUE(sAdultUpgrades[i]) != 0) {
                // Grey Out the Goron Bracelet when Not Randomized and Toggle Strength Option is off
                // Grey Out Strength Upgrades when Disabled and the Toggle Strength Option is on
                if ((drawGreyItems &&
                     (((sAdultUpgradeItemBases[i] + CUR_UPG_VALUE(sAdultUpgrades[i]) - 1) == ITEM_BRACELET &&
                       !(IS_RANDO) && !CVarGetInteger(CVAR_ENHANCEMENT("ToggleStrength"), 0)))) ||
                    (CVarGetInteger(CVAR_ENHANCEMENT("ToggleStrength"), 0) &&
                     CVarGetInteger(CVAR_ENHANCEMENT("StrengthDisabled"), 0) && sAdultUpgrades[i] == UPG_STRENGTH)) {
                    gDPSetGrayscaleColor(POLY_OPA_DISP++, 109, 109, 109, 255);
                    gSPGrayscale(POLY_OPA_DISP++, true);
                }
                KaleidoScope_DrawQuadTextureRGBA32(
                    play->state.gfxCtx, gItemIcons[sAdultUpgradeItemBases[i] + CUR_UPG_VALUE(sAdultUpgrades[i]) - 1],
                    32, 32, 0);
                gSPGrayscale(POLY_OPA_DISP++, false);
            }
        }
        // Draw inventory screen icons
        for (k = 0, bit = rowStart, point = 4; k < 3; k++, point += 4, temp++, bit++) {

            int itemId = ITEM_SWORD_KOKIRI + temp;
            bool age_restricted = !CHECK_AGE_REQ_ITEM(itemId);
            if (age_restricted) {
                gDPSetGrayscaleColor(POLY_OPA_DISP++, 109, 109, 109, 255);
                gSPGrayscale(POLY_OPA_DISP++, true);
            }
            if (((u32)i == 0) && (k == 2) && (gSaveContext.bgsFlag != 0)) {
                KaleidoScope_DrawQuadTextureRGBA32(play->state.gfxCtx, gItemIconSwordBiggoronTex, 32, 32, point);
            } else if ((i == 0) && (k == 2) && (gBitFlags[bit + 1] & gSaveContext.inventory.equipment)) {
                KaleidoScope_DrawQuadTextureRGBA32(play->state.gfxCtx, gItemIconBrokenGiantsKnifeTex, 32, 32, point);
            } else if (gBitFlags[bit] & gSaveContext.inventory.equipment) {
                KaleidoScope_DrawQuadTextureRGBA32(play->state.gfxCtx, gItemIcons[itemId], 32, 32, point);
            }
            gSPGrayscale(POLY_OPA_DISP++, false);
        }
    }

    // Render A button indicator when hovered over strength
    if ((pauseCtx->cursorX[PAUSE_EQUIP] == 0) && (pauseCtx->cursorY[PAUSE_EQUIP] == 2) &&
        CVarGetInteger(CVAR_ENHANCEMENT("ToggleStrength"), 0) && pauseCtx->cursorSpecialPos == 0 &&
        pauseCtx->unk_1E4 == 0 && pauseCtx->state == 6) {
        u8 row = 2;
        u8 column = 0;
        u8 equipVtxIndex = 16 * row + 4 * column;
        // Get Bottom Bisector of the Quad
        s16 translateX =
            (pauseCtx->equipVtx[equipVtxIndex].v.ob[0] + pauseCtx->equipVtx[equipVtxIndex + 1].v.ob[0]) / 2;
        // Add 4 since the icon will be zoomed in on
        s16 translateY = pauseCtx->equipVtx[equipVtxIndex + 2].v.ob[1] + 4;
        KaleidoScope_DrawAButton(play, sStrengthAButtonVtx, translateX, translateY);
    }

    KaleidoScope_DrawPlayerWork(play);

    // if ((pauseCtx->unk_1E4 == 7) && (sEquipTimer == 10)) {
    // KaleidoScope_SetupPlayerPreRender(play);
    //}

    if ((pauseCtx->unk_1E4 == 7) && (sEquipTimer == 9)) {
        //! @bug: This function shouldn't take any arguments
        // KaleidoScope_ProcessPlayerPreRender(play);
    }

    // gSPInvalidateTexCache(POLY_OPA_DISP++, 0);
    gSPInvalidateTexCache(POLY_OPA_DISP++, pauseCtx->iconItemSegment);
    // gSPInvalidateTexCache(POLY_OPA_DISP++, pauseCtx->iconItem24Segment);
    gSPInvalidateTexCache(POLY_OPA_DISP++, pauseCtx->nameSegment);

    // gSPSegment(POLY_OPA_DISP++, 0x07, pauseCtx->playerSegment);
    gSPSegment(POLY_OPA_DISP++, 0x08, pauseCtx->iconItemSegment);
    gSPSegment(POLY_OPA_DISP++, 0x09, pauseCtx->iconItem24Segment);
    gSPSegment(POLY_OPA_DISP++, 0x0A, pauseCtx->nameSegment);
    gSPSegment(POLY_OPA_DISP++, 0x0B, play->interfaceCtx.mapSegment);
    // gSPSegment(POLY_OPA_DISP++, 0x0C, pauseCtx->iconItemAltSegment);

    Gfx_SetupDL_42Opa(play->state.gfxCtx);
    KaleidoScope_DrawEquipmentImage(play, pauseCtx->playerSegment, PAUSE_EQUIP_PLAYER_WIDTH, PAUSE_EQUIP_PLAYER_HEIGHT);

    if (gUpgradeMasks[0]) {}

    CLOSE_DISPS(play->state.gfxCtx);
}

#include "global.h"
#include <string.h>
#include "soh/OTRGlobals.h"
#include "soh/ResourceManagerHelpers.h"

// unused
Gfx sCircleNullDList[] = {
    gsSPEndDisplayList(),
};

//#include "code/fbdemo_circle/z_fbdemo_circle.c"
#include "code/fbdemo_circle/z_fbdemo_circle.h"

Gfx sTransCircleDL[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                          G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsDPSetOtherMode(G_AD_DISABLE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                     G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2),
    gsDPSetCombineMode(G_CC_BLENDPEDECALA, G_CC_BLENDPEDECALA),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(SEG_ADDR(8, 0), G_IM_FMT_I, G_IM_SIZ_8b, 16, 64, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 4, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPDisplayList(SEG_ADDR(9, 0)),
    // OTRTODO: Add proper gsSPVertexOTRFilepath macro
    { G_VTX_OTR_FILEPATH << 24, (uintptr_t)sTransCircleVtx },
    { 32, 0 << 16 | 0 },
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 4, 0),
    gsSP2Triangles(3, 5, 6, 0, 5, 7, 8, 0),
    gsSP2Triangles(7, 9, 10, 0, 9, 11, 12, 0),
    gsSP2Triangles(11, 13, 14, 0, 13, 15, 16, 0),
    gsSP2Triangles(15, 17, 18, 0, 17, 19, 20, 0),
    gsSP2Triangles(19, 21, 22, 0, 21, 23, 24, 0),
    gsSP2Triangles(23, 25, 26, 0, 25, 27, 28, 0),
    gsSP1Triangle(27, 29, 30, 0),
    // OTRTODO: Add proper gsSPVertexOTRFilepath macro
    { G_VTX_OTR_FILEPATH << 24, (uintptr_t)sTransCircleVtx },
    { 3, 0 << 16 | 31 },
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

void TransitionCircle_Start(void* thisx) {
    TransitionCircle* this = (TransitionCircle*)thisx;

    this->isDone = false;

    switch (this->appearanceType) {
        case TCA_WAVE:
            this->texture = sTransCircleWaveTex;
            break;
        case TCA_RIPPLE:
            this->texture = sTransCircleRippleTex;
            break;
        case TCA_STARBURST:
            this->texture = sTransCircleStarburstTex;
            break;
        default:
            this->texture = sTransCircleNormalTex;
            break;
    }

    if (this->speedType == TCS_FAST) {
        this->speed = 20;
    } else {
        this->speed = 10;
    }

    if (this->colorType == TCC_BLACK) {
        this->color.rgba = RGBA8(0, 0, 0, 255);
    } else if (this->colorType == TCC_WHITE) {
        this->color.rgba = RGBA8(160, 160, 160, 255);
    } else if (this->colorType == TCC_GRAY) {
        // yes, really.
        this->color.r = 100;
        this->color.g = 100;
        this->color.b = 100;
        this->color.a = 255;
    } else {
        this->speed = 40;
        this->color.rgba = this->appearanceType == TCA_WAVE ? RGBA8(0, 0, 0, 255) : RGBA8(160, 160, 160, 255);
    }
    if (this->direction != 0) {
        this->texY = 0;
        if (this->colorType == TCC_SPECIAL) {
            this->texY = 0xFA;
        }
    } else {
        this->texY = 0x1F4;
        if (this->appearanceType == TCA_RIPPLE) {
            Audio_PlaySoundGeneral(NA_SE_OC_SECRET_WARP_OUT, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                   &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
        }
    }
    guPerspective(&this->projection, &this->normal, 60.0f, (4.0f / 3.0f), 10.0f, 12800.0f, 1.0f);
    guLookAt(&this->lookAt, 0.0f, 0.0f, 400.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void* TransitionCircle_Init(void* thisx) {
    TransitionCircle* this = (TransitionCircle*)thisx;

    memset(this, 0, sizeof(*this));
    return this;
}

void TransitionCircle_Destroy(void* thisx) {
}

void TransitionCircle_Update(void* thisx, s32 updateRate) {
    TransitionCircle* this = (TransitionCircle*)thisx;
    s32 temp_t2;
    s32 temp_t3;

    if (this->direction != 0) {
        if (this->texY == 0) {
            if (this->appearanceType == TCA_RIPPLE) {
                Audio_PlaySoundGeneral(NA_SE_OC_SECRET_WARP_IN, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                       &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            }
        }
        this->texY += this->speed * 3 / updateRate;
        if (this->texY >= 0x1F4) {
            this->texY = 0x1F4;
            this->isDone = true;
        }
    } else {
        this->texY -= this->speed * 3 / updateRate;
        if (this->colorType != TCC_SPECIAL) {
            if (this->texY <= 0) {
                this->texY = 0;
                this->isDone = true;
            }
        } else {
            if (this->texY < 0xFB) {
                this->texY = 0xFA;
                this->isDone = true;
            }
        }
    }
}

void TransitionCircle_Draw(void* thisx, Gfx** gfxP) {
    Gfx* gfx = *gfxP;
    Mtx* modelView;
    TransitionCircle* this = (TransitionCircle*)thisx;
    Gfx* texScroll;
    // These variables are a best guess based on the other transition types.
    f32 tPos = 0.0f;
    f32 rot = 0.0f;
    f32 scale = 14.8f;

    modelView = this->modelView[this->frame];

    this->frame ^= 1;
    gDPPipeSync(gfx++);
    texScroll = Gfx_BranchTexScroll(&gfx, this->texX, this->texY, 16, 64);
    gSPSegment(gfx++, 9, texScroll);
    gSPSegment(gfx++, 8, this->texture);
    gDPSetColor(gfx++, G_SETPRIMCOLOR, this->color.rgba);
    gDPSetColor(gfx++, G_SETENVCOLOR, this->color.rgba);
    gSPMatrix(gfx++, &this->projection, G_MTX_PROJECTION | G_MTX_LOAD);
    gSPPerspNormalize(gfx++, this->normal);
    gSPMatrix(gfx++, &this->lookAt, G_MTX_PROJECTION | G_MTX_NOPUSH | G_MTX_MUL);

    float aspectRatio = OTRGetAspectRatio();

    if (scale != 1.0f) {
        guScale(&modelView[0], scale * aspectRatio, scale * aspectRatio, 1.0f);
        gSPMatrix(gfx++, &modelView[0], G_MTX_LOAD);
    }

    if (rot != 0.0f) {
        guRotate(&modelView[1], rot, 0.0f, 0.0f, 1.0f);
        gSPMatrix(gfx++, &modelView[1], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    }

    if ((tPos != 0.0f) || (tPos != 0.0f)) {
        guTranslate(&modelView[2], tPos, tPos, 0.0f);
        gSPMatrix(gfx++, &modelView[2], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    }
    gSPDisplayList(gfx++, sTransCircleDL);
    gDPPipeSync(gfx++);
    *gfxP = gfx;
}

s32 TransitionCircle_IsDone(void* thisx) {
    TransitionCircle* this = (TransitionCircle*)thisx;

    return this->isDone;
}

void TransitionCircle_SetType(void* thisx, s32 type) {
    TransitionCircle* this = (TransitionCircle*)thisx;

    if (type & TC_SET_PARAMS) {
        // SetType is called twice for circles, the actual direction value will be set on the second call.
        // The direction set here will be overwritten on that second call.
        this->direction = (type >> 5) & 0x1;
        this->colorType = (type >> 3) & 0x3;
        this->speedType = type & 0x1;
        this->appearanceType = (type >> 1) & 0x3;
    } else if (type == 1) {
        this->direction = 1;
    } else {
        this->direction = 0;
    }
}

void TransitionCircle_SetColor(void* thisx, u32 color) {
    TransitionCircle* this = (TransitionCircle*)thisx;

    this->color.rgba = color;
}

void TransitionCircle_SetEnvColor(void* thisx, u32 envColor) {
    TransitionCircle* this = (TransitionCircle*)thisx;

    this->envColor.rgba = envColor;
}

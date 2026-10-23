/**
 * @file render_objects.c renders hud elements
 * A more suitable name may be print.c
 **/

#include <ultra64.h>
#include <mk64.h>
#include <PR/gbi.h>
#include <macros.h>
#include <defines.h>
#include <common_structs.h>
#include <actor_types.h>
#include "camera.h"
#include "memory.h"
#include "math_util.h"
#include "math_util_2.h"
#include "objects.h"
#include "path.h"
#include "bomb_kart.h"
#include <assets/common_data.h>
#include "render_player.h"
#include "animation.h"
#include "cpu_vehicles_camera_path.h"
#include "code_80057C60.h"
#include "code_8006E9C0.h"
#include "render_objects.h"
#include "update_objects.h"
#include "menu_items.h"
#include "collision.h"
#include "main.h"
#include "menus.h"
#include "code_80086E70.h"
#include "code_800029B0.h"
#include <assets/data_800E45C0.h>
#include "courses/all_course_data.h"
#include <vehicles.h>
#include "data/some_data.h"
#include "OverKart5Hooks.h"

void KwsetTile(Vec3f pos, Vec3su orientation, f32 Scale, Vtx* vtx) {
    KWCreateAffine3D(pos, orientation, Scale);
    gSPVertex(DisplayListHead++, vtx, 4, 0);
    gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
}

void Kwpolygon3D(Vec3f pos, Vec3su orientation, f32 Scale, Gfx* gfx) {
    KWCreateAffine3D(pos, orientation, Scale);
    gSPDisplayList(DisplayListHead++, D_0D0077A0);
    gSPDisplayList(DisplayListHead++, gfx);
}

UNUSED void Kwpolygon3DBoth(Vec3f pos, Vec3su orientation, f32 Arg2, Gfx* gfx) {
    KWCreateAffine3D(pos, orientation, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D0077A0);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BOTH);
    gSPDisplayList(DisplayListHead++, gfx);
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

void Kwpolygon3DSlight(Vec3f arg0, Vec3su arg1, f32 Arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D0077D0);
    gSPDisplayList(DisplayListHead++, gfx);
}

UNUSED void Kwpolygon3DTexFlight(Vec3f arg0, Vec3su arg1, f32 Arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D0077F8);
    gSPDisplayList(DisplayListHead++, gfx);
}

UNUSED void Kwpolygon3DTexSlight(Vec3f arg0, Vec3su arg1, f32 Arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007828);
    gSPDisplayList(DisplayListHead++, gfx);
}

UNUSED void Kwpolygon3DTexSlightBoth(Vec3f arg0, Vec3su arg1, f32 Arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007828);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BOTH);
    gSPDisplayList(DisplayListHead++, gfx);
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

void Kwpolygon3DIceblock(Vec3f arg0, Vec3su arg1, f32 Arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007850);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BOTH);
    gSPDisplayList(DisplayListHead++, gfx);
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

void Kwpolygon3DIcekage(Vec3f arg0, Vec3su arg1, f32 Arg2, Gfx* gfx, s32 arg4) {
    KWCreateAffine3DIceKage(arg0, arg1, Arg2);

    gSPDisplayList(DisplayListHead++, D_0D007878);
    gDPSetPrimColor(DisplayListHead++, 0, 0, 0xFF, 0xFF, 0xFF, arg4);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BOTH);
    gSPDisplayList(DisplayListHead++, gfx);
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

UNUSED void Kwpolygon3DShade(Vec3f arg0, Vec3su arg1, f32 Arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D0078A0);
    gSPDisplayList(DisplayListHead++, gfx);
}

UNUSED void KwshadeTriangle(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, Vtx* vtx) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D0078A0);
    gSPVertex(DisplayListHead++, vtx, 3, 0);
    gSPDisplayList(DisplayListHead++, D_0D006930);
}

UNUSED void KwshadeTile(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, Vtx* vtx) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D0078A0);
    gSPVertex(DisplayListHead++, vtx, 4, 0);
    gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
}

UNUSED void KwshadeXLUTile(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, Vtx* vtx, s32 arg5) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D0078A0);

    gDPSetRenderMode(DisplayListHead++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    vtx[0].v.cn[3] = arg5;
    vtx[1].v.cn[3] = arg5;
    vtx[2].v.cn[3] = arg5;
    vtx[3].v.cn[3] = arg5;
    gSPVertex(DisplayListHead++, vtx, 4, 0);
    gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
}

UNUSED void KWShutter1(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, Vtx* vtx, s32 arg5) {
    vtx[1].v.ob[0] = arg5;
    vtx[2].v.ob[0] = arg5;
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D0078A0);
    gDPSetRenderMode(DisplayListHead++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gSPVertex(DisplayListHead++, vtx, 4, 0);
    gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
}

UNUSED void KWShutter2(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, Vtx* vtx, s32 arg5) {
    vtx[0].v.ob[0] = arg5;
    vtx[3].v.ob[0] = arg5;
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D0078A0);
    gDPSetRenderMode(DisplayListHead++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gSPVertex(DisplayListHead++, vtx, 4, 0);
    gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
}

s32 KwgetMaskval(s32 arg0) {
    s32 temp_a1;
    s32 phi_v0;
    s32 phi_v1 = 0;

    phi_v0 = arg0;
    do {
        phi_v1++;
        temp_a1 = phi_v0 / 2;
        phi_v0 = temp_a1;
    } while (temp_a1 != 1);
    return phi_v1;
}

void KwloadtextureblockRGBA32B(u8* Texture, s32 Width, s32 Height) {
    gDPLoadTextureBlock(DisplayListHead++, Texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, Width, Height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void KwloadtexturetileRGBA32B(u8* Texture, s32 Width, s32 Height) {
    gDPLoadTextureTile(DisplayListHead++, Texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, Width, Height, 0, 0, Width - 1,
                       Height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
}

void KWLoadTextureBlockRGBA16B(u16* Texture, s32 Width, s32 Height) {
    gDPLoadTextureBlock(DisplayListHead++, Texture, G_IM_FMT_RGBA, G_IM_SIZ_16b, Width, Height, 0,
                        G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void KwloadtextureblockRGBA16BMr(u16* Texture, s32 Width, s32 Height, s32 someMask) {
    gDPLoadTextureBlock(DisplayListHead++, Texture, G_IM_FMT_RGBA, G_IM_SIZ_16b, Width, Height, 0,
                        G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, someMask, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void KwloadtexturetileRGBA16B(u16* Texture, s32 Width, s32 Height) {
    gDPLoadTextureTile(DisplayListHead++, Texture, G_IM_FMT_RGBA, G_IM_SIZ_16b, Width, Height, 0, 0, Width - 1,
                       Height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
}

void KwloadtextureblockIA16B(u8* Texture, s32 Width, s32 Height) {
    gDPLoadTextureBlock(DisplayListHead++, Texture, G_IM_FMT_IA, G_IM_SIZ_16b, Width, Height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void KwloadtexturetileIA16B(u8* Texture, s32 Width, s32 Height) {
    gDPLoadTextureTile(DisplayListHead++, Texture, G_IM_FMT_IA, G_IM_SIZ_16b, Width, Height, 0, 0, Width - 1,
                       Height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
}

void KwloadtextureblockIA8B(u8* Texture, s32 Width, s32 Height) {
    gDPLoadTextureBlock(DisplayListHead++, Texture, G_IM_FMT_IA, G_IM_SIZ_8b, Width, Height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void KwloadtexturetileIA8B(u8* Texture, s32 Width, s32 Height) {
    gDPLoadTextureTile(DisplayListHead++, Texture, G_IM_FMT_IA, G_IM_SIZ_8b, Width, Height, 0, 0, Width - 1,
                       Height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
}

void KwloadtextureblockA8B(u8* Texture, s32 Width, s32 Height) {
    gDPLoadTextureBlock(DisplayListHead++, Texture, G_IM_FMT_I, G_IM_SIZ_8b, Width, Height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void KwloadtextureblockIA4B(u8* Texture, s32 Width, s32 Height) {
    // This macro ought to be equivalent to the block of macros below but it doesn't match
    // See comment above the `gDPLoadBlock` macro
    // gDPLoadTextureBlock_4b(DisplayListHead++, Texture, G_IM_FMT_I, Width, Height, 0, G_TX_NOMIRROR | G_TX_CLAMP,
    // G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gDPSetTextureImage(DisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, Texture);
    gDPSetTile(DisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 0, G_TX_RENDERTILE, G_TX_LOADTILE, 0,
               G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
               G_TX_NOLOD);
    gDPLoadSync(DisplayListHead++);
    // The last argument to this macro really should be `CALC_DXT_4b(Width)` but that creates a massive diff
    gDPLoadBlock(DisplayListHead++, G_TX_LOADTILE, 0, 0, (((Width * Height) + 3) >> 2) - 1,
                 ((Width / 16) + 2047) / (Width / 16));
    gDPPipeSync(DisplayListHead++);
    gDPSetTile(DisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_4b, (((Width >> 1) + 7) >> 3), G_TX_RENDERTILE,
               G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP,
               G_TX_NOMASK, G_TX_NOLOD);
    gDPSetTileSize(DisplayListHead++, G_TX_RENDERTILE, 0, 0, (Width - 1) << G_TEXTURE_IMAGE_FRAC,
                   (Height - 1) << G_TEXTURE_IMAGE_FRAC);
}

UNUSED void KwloadtexturetileIA4B(u8* Texture, s32 Width, s32 Height) {
    gDPLoadTextureTile_4b(DisplayListHead++, Texture, G_IM_FMT_IA, Width, Height, 0, 0, Width - 1, Height - 1, 0,
                          G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                          G_TX_NOLOD);
}

void KwloadtextureblockI8B(u8* Texture, s32 Width, s32 Height) {
    gDPLoadTextureBlock(DisplayListHead++, Texture, G_IM_FMT_I, G_IM_SIZ_8b, Width, Height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void KWLoadTextureBlockI4b(u8* image, s32 Width, s32 Height) {
    // This macro ought to be equivalent to the block of macros below but it doesn't match
    // See comment above the `gDPLoadBlock` macro
    // gDPLoadTextureBlock_4b(DisplayListHead++, image, G_IM_FMT_I, Width, Height, 0, G_TX_NOMIRROR | G_TX_CLAMP,
    // G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gDPSetTextureImage(DisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 1, image);
    gDPSetTile(DisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 0, G_TX_RENDERTILE, G_TX_LOADTILE, 0,
               G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
               G_TX_NOLOD);
    gDPLoadSync(DisplayListHead++);
    // The last argument to this macro really should be `CALC_DXT_4b(Width)` but that creates a massive diff
    gDPLoadBlock(DisplayListHead++, G_TX_LOADTILE, 0, 0, (((Width * Height) + 3) >> 2) - 1,
                 ((Width / 16) + 2047) / (Width / 16));
    gDPPipeSync(DisplayListHead++);
    gDPSetTile(DisplayListHead++, G_IM_FMT_I, G_IM_SIZ_4b, (((Width >> 1) + 7) >> 3), G_TX_RENDERTILE, G_TX_RENDERTILE,
               0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
               G_TX_NOLOD);
    gDPSetTileSize(DisplayListHead++, G_TX_RENDERTILE, 0, 0, (Width - 1) << G_TEXTURE_IMAGE_FRAC,
                   (Height - 1) << G_TEXTURE_IMAGE_FRAC);
}

// Appears to be a complete copy of `KwloadtextureblockA4B`?
void KwloadtextureblockA4B(u8* image, s32 Width, s32 Height) {
    // This macro ought to be equivalent to the block of macros below but it doesn't match
    // See comment above the `gDPLoadBlock` macro
    // gDPLoadTextureBlock_4b(DisplayListHead++, image, G_IM_FMT_I, Width, Height, 0, G_TX_NOMIRROR | G_TX_CLAMP,
    // G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gDPSetTextureImage(DisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 1, image);
    gDPSetTile(DisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 0, G_TX_RENDERTILE, G_TX_LOADTILE, 0,
               G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
               G_TX_NOLOD);
    gDPLoadSync(DisplayListHead++);
    // The last argument to this macro really should be `CALC_DXT_4b(Width)` but that creates a massive diff
    gDPLoadBlock(DisplayListHead++, G_TX_LOADTILE, 0, 0, (((Width * Height) + 3) >> 2) - 1,
                 ((Width / 16) + 2047) / (Width / 16));
    gDPPipeSync(DisplayListHead++);
    gDPSetTile(DisplayListHead++, G_IM_FMT_I, G_IM_SIZ_4b, (((Width >> 1) + 7) >> 3), G_TX_RENDERTILE, G_TX_RENDERTILE,
               0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
               G_TX_NOLOD);
    gDPSetTileSize(DisplayListHead++, G_TX_RENDERTILE, 0, 0, (Width - 1) << G_TEXTURE_IMAGE_FRAC,
                   (Height - 1) << G_TEXTURE_IMAGE_FRAC);
}

void KwloadtextureblockA4BXflip(u8* image, s32 Width, s32 Height) {
    // This macro ought to be equivalent to the block of macros below but it doesn't match
    // See comment above the `gDPLoadBlock` macro
    // gDPLoadTextureBlock_4b(DisplayListHead++, image, G_IM_FMT_I, Width, Height, 0, G_TX_NOMIRROR | G_TX_CLAMP,
    // G_TX_MIRROR | G_TX_WRAP, G_TX_NOMASK, masks, G_TX_NOLOD, G_TX_NOLOD);
    s32 masks = KwgetMaskval(Width);

    gDPSetTextureImage(DisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 1, image);
    gDPSetTile(DisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 0, G_TX_RENDERTILE, G_TX_LOADTILE, 0,
               G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_MIRROR | G_TX_WRAP, masks, G_TX_NOLOD);
    gDPLoadSync(DisplayListHead++);
    // The last argument to this macro really should be `CALC_DXT_4b(Width)` but that creates a massive diff
    gDPLoadBlock(DisplayListHead++, G_TX_LOADTILE, 0, 0, (((Width * Height) + 3) >> 2) - 1,
                 ((Width / 16) + 2047) / (Width / 16));
    gDPPipeSync(DisplayListHead++);
    gDPSetTile(DisplayListHead++, G_IM_FMT_I, G_IM_SIZ_4b, (((Width >> 1) + 7) >> 3), G_TX_RENDERTILE, G_TX_RENDERTILE,
               0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_MIRROR | G_TX_WRAP, masks, G_TX_NOLOD);
    gDPSetTileSize(DisplayListHead++, G_TX_RENDERTILE, 0, 0, (Width - 1) << G_TEXTURE_IMAGE_FRAC,
                   (Height - 1) << G_TEXTURE_IMAGE_FRAC);
}

void KwloadtextureblockCI8B(u8* Texture, s32 Width, s32 Height) {
    gDPLoadTextureBlock(DisplayListHead++, Texture, G_IM_FMT_CI, G_IM_SIZ_8b, Width, Height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void KwloadtextureblockCI8BMr(u8* Texture, s32 Width, s32 Height, s32 someMask) {
    gDPLoadTextureBlock(DisplayListHead++, Texture, G_IM_FMT_CI, G_IM_SIZ_8b, Width, Height, 0,
                        G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, someMask, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

UNUSED void KwloadtexturetileCI8B(u8* Texture, s32 Width, s32 Height) {
    gDPLoadTextureTile(DisplayListHead++, Texture, G_IM_FMT_CI, G_IM_SIZ_8b, Width, Height, 0, 0, Width - 1,
                       Height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
}

UNUSED void KwloadtextureblockMulti16B(u8* image1, u8* image2, s32 Width, s32 Height) {
    gDPSetCombineLERP(DisplayListHead++, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL1, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);

    gDPLoadMultiBlock(DisplayListHead++, image2, 0x100, G_TX_RENDERTILE, G_IM_FMT_I, G_IM_SIZ_8b, Width, Height, 0,
                      G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                      G_TX_NOLOD);
    gDPLoadTextureBlock(DisplayListHead++, image1, G_IM_FMT_RGBA, G_IM_SIZ_16b, Width, Height, 0,
                        G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    gDPSetTile(DisplayListHead++, G_IM_FMT_I, G_IM_SIZ_8b, (Width + 7) >> 3, 0x0100, 1, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    ;
    gDPSetTileSize(DisplayListHead++, 1, 0, 0, (Width - 1) << G_TEXTURE_IMAGE_FRAC,
                   (Height - 1) << G_TEXTURE_IMAGE_FRAC);
}

void Kwsettexturetile(Vtx* arg0) {
    gSPVertex(DisplayListHead++, arg0, 4, 0);
    gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
}

void Kwsettexturetriangle(Vtx* arg0) {
    gSPVertex(DisplayListHead++, arg0, 3, 0);
    gSPDisplayList(DisplayListHead++, D_0D006930);
}

UNUSED void Kwtile3DShade(Vec3f arg0, Vec3su arg1, f32 Arg2, Vtx* Arg3) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D0078A0);
    gSPVertex(DisplayListHead++, Arg3, 4, 0);
    gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
}

UNUSED void Kwline3DShade(Vec3f arg0, Vec3su arg1, f32 Arg2, Vtx* Arg3) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D0078D0);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BOTH);
    gSPVertex(DisplayListHead++, Arg3, 4, 0);
    gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

void KWTextureRGBASubPT(u16* Texture, Vtx* arg1, s32 Width, s32 Arg3, s32 Height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u16* img = Texture;

    for (heightIndex = 0; heightIndex < Arg3 / Height; heightIndex++) {
        KWLoadTextureBlockRGBA16B(img, Width, Height);
        Kwsettexturetile(&arg1[vertexIndex]);
        img += Width * Height;
        vertexIndex += 4;
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureRGBASubBL(u16* Texture, Vtx* arg1, s32 Width, s32 Arg3, s32 Height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u16* img = Texture;

    for (heightIndex = 0; heightIndex < Arg3 / Height; heightIndex++) {
        KWLoadTextureBlockRGBA16B(img, Width, Height);
        Kwsettexturetile(&arg1[vertexIndex]);
        img += Width * (Height - 1);
        vertexIndex += 4;
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureRGBASubBLMR(u16* Texture, Vtx* arg1, s32 Width, s32 Arg3, s32 Height, s32 someMask) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u16* img = Texture;

    for (heightIndex = 0; heightIndex < Arg3 / Height; heightIndex++) {
        KwloadtextureblockRGBA16BMr(img, Width, Height, someMask);
        Kwsettexturetile(&arg1[vertexIndex]);
        img += Width * (Height - 1);
        vertexIndex += 4;
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

//! @todo tlut/Texture unconfirmed. This could be texture1 and texture2
UNUSED void KWTextureRGBMASubBL(u8* tlut, u8* Texture, Vtx* Arg2, s32 Width, s32 arg4, s32 Height) {
    s32 var_s0 = 0;
    u8* img1 = tlut;
    u8* img2 = Texture;
    s32 temp_lo_2;
    s32 var;
    s32 i;

    gSPDisplayList(DisplayListHead++, D_0D008138);

    for (i = 0; i < arg4 / Height; i++) {
        KwloadtextureblockMulti16B(img1, img2, Width, Height);
        Kwsettexturetile(&Arg2[var_s0]);
        var = Height - 1;
        temp_lo_2 = (Width * var);
        img1 += temp_lo_2 * 2;
        img2 += temp_lo_2;
        var_s0 += 4;
    }
    gSPTexture(DisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
    gSPDisplayList(DisplayListHead++, D_0D008120);
}

void KWTextureRGBA32SubPT(u8* Texture, Vtx* arg1, s32 Width, s32 Arg3, s32 Height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = Texture;

    for (heightIndex = 0; heightIndex < Arg3 / Height; heightIndex++) {
        KwloadtextureblockRGBA32B(img, Width, Height);
        Kwsettexturetile(&arg1[vertexIndex]);
        img += Width * Height * 4;
        vertexIndex += 4;
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureRGBA32SubBL(u8* Texture, Vtx* arg1, s32 Width, s32 Arg3, s32 Height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = Texture;

    for (heightIndex = 0; heightIndex < Arg3 / Height; heightIndex++) {
        KwloadtextureblockRGBA32B(img, Width, Height);
        Kwsettexturetile(&arg1[vertexIndex]);
        img += Width * (Height - 1) * 4;
        vertexIndex += 4;
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTexture2DRGBA(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u16* Texture, Vtx* arg5, s32 arg6, s32 arg7, UNUSED s32 arg8,
                   s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007928);
    KWTextureRGBASubPT(Texture, arg5, arg6, arg7, arg9);
}

void Kwtexture2DFlashRGBAPt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u16* Texture, Vtx* arg5, s32 arg6, s32 arg7, UNUSED s32 arg8,
                   s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007968);
    IceMode(KwflashR, KwflashG, KwflashB, 128, 128, 128, 255);
    KWTextureRGBASubPT(Texture, arg5, arg6, arg7, arg9);
}

UNUSED void Kwtexture2DRGBABl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u16* Texture, Vtx* arg5, s32 arg6, s32 arg7,
                          UNUSED s32 arg8, s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007948);
    KWTextureRGBASubBL(Texture, arg5, arg6, arg7, arg9);
}

UNUSED void Kwtexture2DRGBAAABl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u16* Texture, Vtx* arg5, s32 arg6, s32 arg7,
                          UNUSED s32 arg8, s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D0079A8);
    KWTextureRGBASubBL(Texture, arg5, arg6, arg7, arg9);
}

void Kwtexture2DRGBAXLU(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 arg4, u16* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                   UNUSED s32 arg9, s32 argA) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D0079E8);

    SubAlphaMode(arg4);
    KWTextureRGBASubBL(Texture, arg6, arg7, arg8, argA);
}

UNUSED void Kwtexture2DRgbmaBl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                          UNUSED s32 arg9, s32 argA) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007948);
    KWTextureRGBMASubBL(tlut, Texture, arg6, arg7, arg8, argA);
}

void Kwtexture2DTriangleRGBA(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u16* Texture, Vtx* arg5, s32 arg6, s32 arg7) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007948);
    KWLoadTextureBlockRGBA16B(Texture, arg6, arg7);
    Kwsettexturetriangle(arg5);
}

UNUSED void KWTexture2DRGBA32PT(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7,
                          UNUSED s32 arg8, s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007968);
    KWTextureRGBA32SubPT(Texture, arg5, arg6, arg7, arg9);
}

UNUSED void KWTexture2DRGBA32BL(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7,
                          UNUSED s32 arg8, s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007948);
    KWTextureRGBA32SubBL(Texture, arg5, arg6, arg7, arg9);
}

void Kwtexture3DRGBABl(Vec3f arg0, Vec3su arg1, f32 Arg2, u16* Texture, Vtx* arg4, s32 arg5, s32 arg6, UNUSED s32 arg7,
                   s32 arg8) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007948);
    KWTextureRGBASubBL(Texture, arg4, arg5, arg6, arg8);
}

UNUSED void Kwtexture3DRGBAAAZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, u16* Texture, Vtx* arg4, s32 arg5, s32 arg6,
                          UNUSED s32 arg7, s32 arg8) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, KWTextureRGBAXLUAAZBBlInit);
    KWTextureRGBASubBL(Texture, arg4, arg5, arg6, arg8);
}

void Kwtexture3DRGBAAAZBBlMr(Vec3f arg0, Vec3su arg1, f32 Arg2, u16* Texture, Vtx* arg4, s32 arg5, s32 arg6, UNUSED s32 arg7,
                   s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, KWTextureRGBAXLUAAZBBlInit);
    KWTextureRGBASubBLMR(Texture, arg4, arg5, arg6, arg8, arg9);
}

UNUSED void Kwtexture3DRGBAAAZBBlBt(Vec3f arg0, Vec3su arg1, f32 Arg2, u16* Texture, Vtx* arg4, s32 arg5, s32 arg6,
                          UNUSED s32 arg7, s32 arg8) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, KWTextureRGBAXLUAAZBBlInit);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BOTH);
    KWTextureRGBASubBL(Texture, arg4, arg5, arg6, arg8);
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

void Kwtexture3DTriangleRGBA(Vec3f arg0, Vec3su arg1, f32 Arg2, u16* Texture, Vtx* arg4, s32 arg5, s32 arg6) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007948);
    KWLoadTextureBlockRGBA16B(Texture, arg5, arg6);
    Kwsettexturetriangle(arg4);
}

UNUSED void KWTexture3DRGBA32AAZBBL(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* Texture, Vtx* arg4, s32 arg5, s32 arg6,
                          UNUSED s32 arg7, s32 arg8) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, KWTextureRGBAXLUAAZBBlInit);
    KWTextureRGBA32SubBL(Texture, arg4, arg5, arg6, arg8);
}

UNUSED void Kwtexture8X8RGBABl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u16* Texture) {
    Kwtexture2DRGBABl(arg0, arg1, Arg2, Arg3, Texture, VtxKTile8X8, 8, 8, 8, 8);
}

UNUSED void Kwtexture2D16X16RGBABl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u16* Texture) {
    Kwtexture2DRGBABl(arg0, arg1, Arg2, Arg3, Texture, VtxKTile16X16, 16, 16, 16, 16);
}

UNUSED void Kwtexture3D16X16RGBABl(Vec3f arg0, Vec3su arg1, f32 Arg2, u16* Texture) {
    Kwtexture3DRGBABl(arg0, arg1, Arg2, Texture, VtxKTile16X16, 16, 16, 16, 16);
}

UNUSED void KwtextureTriangle16X16RGBA(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u16* Texture, Vtx* arg5) {
    Kwtexture2DTriangleRGBA(arg0, arg1, Arg2, Arg3, Texture, arg5, 16, 16);
}

UNUSED void Kwtexture3DTriangle16X16RGBA(Vec3f arg0, Vec3su arg1, f32 Arg2, u16* Texture, Vtx* arg4) {
    Kwtexture3DTriangleRGBA(arg0, arg1, Arg2, Texture, arg4, 16, 16);
}

UNUSED void Kwtexture2D32X32RGBA(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u16* Texture) {
    Kwtexture2DRGBABl(arg0, arg1, Arg2, Arg3, Texture, VtxKTile32X32, 32, 32, 32, 32);
}

UNUSED void Kwtexture2D64X32RGBABl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u16* Texture) {
    Kwtexture2DRGBABl(arg0, arg1, Arg2, Arg3, Texture, D_0D005FB0, 64, 32, 64, 32);
}

UNUSED void Kwtexture3D64X32RGBABl(Vec3f arg0, Vec3su arg1, f32 Arg2, u16* Texture) {
    Kwtexture3DRGBABl(arg0, arg1, Arg2, Texture, D_0D005FB0, 64, 32, 64, 32);
}

UNUSED void Kwtexture2D64X64RGBABl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u16* Texture) {
    Kwtexture2DRGBABl(arg0, arg1, Arg2, Arg3, Texture, VtxKTile64X64BL, 64, 64, 64, 32);
}

UNUSED void Kwtexture2D64X64RGBAXLU(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 arg4, u16* Texture) {
    Kwtexture2DRGBAXLU(arg0, arg1, Arg2, Arg3, arg4, Texture, VtxKTile64X64BL, 64, 64, 64, 32);
}

void KwloadtextureCI8AAZBBl(u8* tlut, u8* Texture, s32 Width, s32 Height) {
    gSPDisplayList(DisplayListHead++, D_0D007D78);
    gDPLoadTLUT_pal256(DisplayListHead++, tlut);
    KwloadtextureblockCI8B(Texture, Width, Height);
}

void KwloadtextureCI8AAZBBlMr(u8* tlut, u8* arg1, s32 Arg2, s32 Arg3, s32 arg4) {
    gSPDisplayList(DisplayListHead++, D_0D007D78);
    gDPLoadTLUT_pal256(DisplayListHead++, tlut);
    KwloadtextureblockCI8BMr(arg1, Arg2, Arg3, arg4);
}

void KWTextureCI8SubPT(u8* tlut, u8* Texture, Vtx* Arg2, UNUSED s32 Arg3, s32 arg4, s32 Width, s32 Height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = Texture;

    gDPLoadTLUT_pal256(DisplayListHead++, tlut);
    for (heightIndex = 0; heightIndex < arg4 / Height; heightIndex++) {
        KwloadtextureblockCI8B(img, Width, Height);
        gSPVertex(DisplayListHead++, &Arg2[vertexIndex], 4, 0);
        gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
        img += Width * Height;
        vertexIndex += 4;
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureCI8SubBL(u8* tlut, u8* Texture, Vtx* Arg2, UNUSED s32 Arg3, s32 arg4, s32 Width,
                                    s32 Height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = Texture;

    gDPLoadTLUT_pal256(DisplayListHead++, tlut);
    for (heightIndex = 0; heightIndex < arg4 / Height; heightIndex++) {
        KwloadtextureblockCI8B(img, Width, Height);
        gSPVertex(DisplayListHead++, &Arg2[vertexIndex], 4, 0);
        gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
        img += Width * (Height - 1);
        vertexIndex += 4;
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureCI8SubBLMR(u8* tlut, u8* Texture, Vtx* Arg2, UNUSED s32 Arg3, s32 arg4, s32 Width, s32 Height, s32 someMask) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = Texture;

    gDPLoadTLUT_pal256(DisplayListHead++, tlut);
    for (heightIndex = 0; heightIndex < arg4 / Height; heightIndex++) {
        KwloadtextureblockCI8BMr(img, Width, Height, someMask);
        gSPVertex(DisplayListHead++, &Arg2[vertexIndex], 4, 0);
        gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
        img += Width * (Height - 1);
        vertexIndex += 4;
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureCI8SubBLCutX(u8* tlut, u8* Texture, Vtx* Arg2, s32 Arg3, s32 Width, s32 Height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = Texture;

    gDPLoadTLUT_pal256(DisplayListHead++, tlut);
    for (heightIndex = 0; heightIndex < Arg3 / Height; heightIndex++) {
        // Something seems off about arguments here, but if it matches it matches
        KwloadtextureblockCI8B(img, Height, Width);
        gSPVertex(DisplayListHead++, &Arg2[vertexIndex], 4, 0);
        gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
        img += Height * Width;
        vertexIndex += 4;
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void Kwtexture2DCI8Pt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, KWSpriteRGBAPtInit);
    KWTextureCI8SubPT(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

void KWTexture2DCI8BL(s32 x, s32 y, u16 Angle, f32 Size, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8, s32 arg9,
                   s32 argA) {
    Kwcreateaffine2D(x, y, Angle, Size);
    gSPDisplayList(DisplayListHead++, KWSpriteRGBAAABlInit);
    KWTextureCI8SubBL(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

void Kwtexture2DCI8Av(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007CF8);
    KWTextureCI8SubBL(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

void KWTexture2DCI8AAPT(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007D18);
    KWTextureCI8SubPT(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

void Kwtexture2DCI8AABl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007D38);
    KWTextureCI8SubBL(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

UNUSED void Kwtexture2DCI8AABlCutx(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                          s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007D38);
    KWTextureCI8SubBLCutX(tlut, Texture, arg6, arg7, arg8, arg9);
}

void KWTexture2DCI8XLUPT(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 arg4, u8* tlut, u8* Texture, Vtx* arg7, s32 arg8,
                   s32 arg9, s32 argA, s32 argB) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007DB8);
    SubAlphaMode(arg4);
    KWTextureCI8SubPT(tlut, Texture, arg7, arg8, arg9, argA, argB);
}

UNUSED void KWTexture2DCI8XLUBL(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 arg4, u8* tlut, u8* Texture, Vtx* arg7, s32 arg8,
                          s32 arg9, s32 argA, s32 argB) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007DD8);
    SubAlphaMode(arg4);
    KWTextureCI8SubBL(tlut, Texture, arg7, arg8, arg9, argA, argB);
}

void KWTexture2DCI8AAXLUPT(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 arg4, u8* tlut, u8* Texture, Vtx* arg7, s32 arg8,
                   s32 arg9, s32 argA, s32 argB) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007E38);
    SubAlphaMode(arg4);
    KWTextureCI8SubPT(tlut, Texture, arg7, arg8, arg9, argA, argB);
}

UNUSED void Kwtexture2DCI8AAXLUBl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 arg4, u8* tlut, u8* Texture, Vtx* arg7, s32 arg8,
                          s32 arg9, s32 argA, s32 argB) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007E58);
    SubAlphaMode(arg4);
    KWTextureCI8SubBL(tlut, Texture, arg7, arg8, arg9, argA, argB);
}

UNUSED void Kwtexture3DCI8Pt(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7,
                          s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, KWSpriteRGBAPtInit);
    KWTextureCI8SubPT(tlut, Texture, arg5, arg6, arg7, arg8, arg9);
}

void Kwtexture3DCI8Bl(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, KWSpriteRGBAAABlInit);
    KWTextureCI8SubBL(tlut, Texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void Kwtexture3DCI8AAPt(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7,
                          s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007D18);
    KWTextureCI8SubPT(tlut, Texture, arg5, arg6, arg7, arg8, arg9);
}

void Kwtexture3DCI8AABl(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007D38);
    KWTextureCI8SubBL(tlut, Texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void Kwtexture3DCI8ZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7,
                          s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007D58);
    KWTextureCI8SubBL(tlut, Texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void Kwtexture3DCI8AAZBPt(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7,
                          s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007D98);
    KWTextureCI8SubPT(tlut, Texture, arg5, arg6, arg7, arg8, arg9);
}

void Kwtexture3DCI8AAZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7,
                        s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007D78);
    KWTextureCI8SubBL(tlut, Texture, arg5, arg6, arg7, arg8, arg9);
}

void Kwtexture3DCI8AAZBBlMr(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007D78);
    KWTextureCI8SubBLMR(tlut, Texture, arg5, arg6, arg7, arg8, arg9, argA);
}

UNUSED void Kwtexture3DCI8AAZBBlCutx(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7,
                          s32 arg8) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007D78);
    KWTextureCI8SubBLCutX(tlut, Texture, arg5, arg6, arg7, arg8);
}

UNUSED void Kwtexture3DCI8XLUPt(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7,
                          s32 arg8, s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007DB8);
    SubAlphaMode(Arg3);
    KWTextureCI8SubPT(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

void Kwtexture3DCI8XLUBl(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007DD8);
    SubAlphaMode(Arg3);
    KWTextureCI8SubBL(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

UNUSED void Kwtexture3DCI8XLUAAPt(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7,
                          s32 arg8, s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007E38);
    SubAlphaMode(Arg3);
    KWTextureCI8SubPT(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

void Kwtexture3DCI8XLUAABl(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007E58);
    SubAlphaMode(Arg3);
    KWTextureCI8SubBL(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

void Kwtexture3DCI8XLUZBPt(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007DF8);
    SubAlphaMode(Arg3);
    KWTextureCI8SubPT(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

void Kwtexture3DCI8XLUZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007E18);
    SubAlphaMode(Arg3);
    KWTextureCI8SubBL(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

void Kwtexture3DCI8XLUAAZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007E98);
    SubAlphaMode(Arg3);
    KWTextureCI8SubBL(tlut, Texture, arg6, arg7, arg8, arg9, argA);
}

void Kwtexture3DCI8PclAAZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007E98);
    gDPSetAlphaCompare(DisplayListHead++, G_AC_DITHER);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    SubAlphaMode(Arg3);
    KWTextureCI8SubBL(tlut, Texture, arg6, arg7, arg8, arg9, argA);

    gDPSetAlphaCompare(DisplayListHead++, G_AC_NONE);
}

UNUSED void Kwtexture2D24X48CI8Pt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture2DCI8Pt(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 24, 48, 24, 48);
}

UNUSED void Kwtexture32X32CI8Pt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture2DCI8Pt(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 32, 32, 32, 32);
}

UNUSED void Kwtexture3D48X40CI8XLUAAZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture3DCI8XLUAAZBBl(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 48, 48, 48, 40);
}

UNUSED void Kwtexture2D48X48CI8Pt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture2DCI8Pt(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 48, 48, 48, 48);
}

UNUSED void Kwtexture2D64X32CI8Pt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture2DCI8Pt(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 64, 32, 64, 32);
}

UNUSED void Kwtexture2D64X64CI8Pt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture2DCI8Pt(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 64, 64, 64, 32);
}

UNUSED void Kwtexture2D64X64CI8Bl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    KWTexture2DCI8BL(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 64, 64, 64, 32);
}

UNUSED void Kwtexture2D64X64CI8Av(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture2DCI8Av(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 64, 64, 64, 32);
}

UNUSED void Kwtexture2D64X64CI8AAPt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    KWTexture2DCI8AAPT(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 64, 64, 64, 32);
}

UNUSED void Kwtexture2D64X64CI8AABl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture2DCI8AABl(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 64, 64, 64, 32);
}

UNUSED void Kwtexture2D64X64CI8XLUPt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 arg4, u8* tlut, u8* Texture, Vtx* arg7) {
    KWTexture2DCI8XLUPT(arg0, arg1, Arg2, Arg3, arg4, tlut, Texture, arg7, 64, 64, 64, 32);
}

UNUSED void Kwtexture2D64X64CI8AAXLUPt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 arg4, u8* tlut, u8* Texture, Vtx* arg7) {
    KWTexture2DCI8AAXLUPT(arg0, arg1, Arg2, Arg3, arg4, tlut, Texture, arg7, 64, 64, 64, 32);
}

UNUSED void Kwtexture3D64X64CI8Bl(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5) {
    Kwtexture3DCI8Bl(arg0, arg1, Arg2, tlut, Texture, arg5, 64, 64, 64, 32);
}

UNUSED void Kwtexture3D64X64CI8AABl(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5) {
    Kwtexture3DCI8AABl(arg0, arg1, Arg2, tlut, Texture, arg5, 64, 64, 64, 32);
}

UNUSED void Kwtexture3D64X64CI8AAZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5) {
    Kwtexture3DCI8AAZBBl(arg0, arg1, Arg2, tlut, Texture, arg5, 64, 64, 64, 32);
}

UNUSED void Kwtexture3D64X64CI8XLUBl(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture3DCI8XLUBl(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 64, 64, 64, 32);
}

UNUSED void Kwtexture3D64X64CI8XLUAABl(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture3DCI8XLUAABl(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 64, 64, 64, 32);
}

UNUSED void Kwtexture3D64X64CI8XLUZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture3DCI8XLUZBBl(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 64, 64, 64, 32);
}

UNUSED void Kwtexture3D64X64CI8XLUAAZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, s32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture3DCI8XLUAAZBBl(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 64, 64, 64, 32);
}

UNUSED void Kwtexture2D72X48CI8Bl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    KWTexture2DCI8BL(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 72, 48, 72, 24);
}

UNUSED void Kwtexture2D72X48CI8AAPt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    KWTexture2DCI8AAPT(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 72, 48, 72, 24);
}

UNUSED void Kwtexture2D72X48CI8AABl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* tlut, u8* Texture, Vtx* arg6) {
    Kwtexture2DCI8AABl(arg0, arg1, Arg2, Arg3, tlut, Texture, arg6, 72, 48, 72, 24);
}

UNUSED void Kwtexture3D72X48CI8AAZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* tlut, u8* Texture, Vtx* arg5) {
    Kwtexture3DCI8AAZBBl(arg0, arg1, Arg2, tlut, Texture, arg5, 72, 48, 72, 24);
}

void KWTextureIA16Sub(u16* Texture, Vtx* arg1, s32 Arg2, s32 Arg3, s32 Width, s32 Height) {
    s32 heightIndex;
    s32 widthIndex;
    s32 vertexIndex = 0;
    u8* img = (u8 *) Texture;

    for (heightIndex = 0; heightIndex < Arg3 / Height; heightIndex++) {
        for (widthIndex = 0; widthIndex < Arg2 / Width; widthIndex++) {
            KwloadtextureblockIA16B(img, Width, Height);
            gSPVertex(DisplayListHead++, &arg1[vertexIndex], 4, 0);
            gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
            img += Width * Height * 2;
            vertexIndex += 4;
        }
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KwtextiledrawIA16(u8* Texture, Vtx* arg1, s32 Arg2, s32 Arg3, s32 Width, s32 Height) {
    s32 heightIndex;
    s32 widthIndex;
    s32 vertexIndex = 0;
    u8* img = Texture;

    for (heightIndex = 0; heightIndex < Arg3 / Height; heightIndex++) {
        for (widthIndex = 0; widthIndex < Arg2 / Width; widthIndex++) {
            KwloadtexturetileIA16B(img, Width, Height);
            gSPVertex(DisplayListHead++, &arg1[vertexIndex], 4, 0);
            gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
            img += Width * Height * 2;
            vertexIndex += 4;
        }
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureIA8Sub(u8* Texture, Vtx* arg1, s32 Arg2, s32 Arg3, s32 Width, s32 Height) {
    s32 heightIndex;
    s32 widthIndex;
    s32 vertexIndex = 0;
    u8* img = Texture;

    for (heightIndex = 0; heightIndex < Arg3 / Height; heightIndex++) {
        for (widthIndex = 0; widthIndex < Arg2 / Width; widthIndex++) {
            KwloadtextureblockIA8B(img, Width, Height);
            gSPVertex(DisplayListHead++, &arg1[vertexIndex], 4, 0);
            gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
            img += Width * Height;
            vertexIndex += 4;
        }
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KwtextiledrawIA8(u8* Texture, Vtx* arg1, s32 Arg2, s32 Arg3, s32 Width, s32 Height) {
    s32 heightIndex;
    s32 widthIndex;
    s32 vertexIndex = 0;
    u8* img = Texture;

    for (heightIndex = 0; heightIndex < Arg3 / Height; heightIndex++) {
        for (widthIndex = 0; widthIndex < Arg2 / Width; widthIndex++) {
            KwloadtexturetileIA8B(img, Width, Height);
            gSPVertex(DisplayListHead++, &arg1[vertexIndex], 4, 0);
            gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
            img += Width * Height;
            vertexIndex += 4;
        }
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureIA4Sub(u8* Texture, Vtx* arg1, s32 Arg2, s32 Arg3, s32 Width, s32 Height) {
    s32 i;
    s32 j;
    s32 var_s2 = 0;
    u8* img = Texture;

    for (i = 0; i < Arg3 / Height; i++) {
        for (j = 0; j < Arg2 / Width; j++) {

            KwloadtextureblockIA4B(img, Width, Height);
            gSPVertex(DisplayListHead++, &arg1[var_s2], 4, 0);
            gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
            img += (Width * Height) / 2;
            var_s2 += 4;
        }
    }

    gSPTexture(DisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureA8Sub(u8* Texture, Vtx* arg1, s32 Arg2, s32 Arg3, s32 Width, s32 Height) {
    s32 heightIndex;
    s32 widthIndex;
    s32 vertexIndex = 0;
    u8* img = Texture;

    for (heightIndex = 0; heightIndex < Arg3 / Height; heightIndex++) {
        for (widthIndex = 0; widthIndex < Arg2 / Width; widthIndex++) {
            KwloadtextureblockI8B(img, Width, Height);
            gSPVertex(DisplayListHead++, &arg1[vertexIndex], 4, 0);
            gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
            img += Width * Height;
            vertexIndex += 4;
        }
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureA4Sub(u8* Texture, Vtx* arg1, s32 Arg2, s32 Arg3, s32 Width, s32 Height) {
    s32 i;
    s32 j;
    s32 var_s2 = 0;
    u8* img = Texture;

    for (i = 0; i < Arg3 / Height; i++) {
        for (j = 0; j < Arg2 / Width; j++) {
            KWLoadTextureBlockI4b(img, Width, Height);
            gSPVertex(DisplayListHead++, &arg1[var_s2], 4, 0);
            gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
            img += (Width * Height) / 2;
            var_s2 += 4;
        }
    }
    gSPTexture(DisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}

void Kwtexture2DIA16Pt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A40);
    KWTextureIA16Sub((u16 *) Texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void Kwtexture2DIA16TilePt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                          s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A40);
    KwtextiledrawIA16(Texture, arg5, arg6, arg7, arg8, arg9);
}

void Kwtexture2DIA8Pt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A40);
    KWTextureIA8Sub(Texture, arg5, arg6, arg7, arg8, arg9);
}

void Kwtexture2DIA8Bl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A60);
    KWTextureIA8Sub(Texture, arg5, arg6, arg7, arg8, arg9);
}

void Kwtexture2DIA8AAPt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A80);
    KWTextureIA8Sub(Texture, arg5, arg6, arg7, arg8, arg9);
}

void Kwtexture2DIA8AABl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007AA0);
    KWTextureIA8Sub(Texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void Kwtexture2DIA8XLUPt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 red, s32 Green, s32 Blue, s32 Alpha, u8* Texture,
                          Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A40);
    ToumeIMode(red, Green, Blue, Alpha);
    KWTextureIA8Sub(Texture, arg9, argA, argB, argC, argD);
}

UNUSED void Kwtexture2DIA8XLUBl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 red, s32 Green, s32 Blue, s32 Alpha, u8* Texture,
                          Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A60);
    ToumeIMode(red, Green, Blue, Alpha);
    KWTextureIA8Sub(Texture, arg9, argA, argB, argC, argD);
}

UNUSED void Kwtexture2DIA8XLUAAZBBl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 red, s32 Green, s32 Blue, s32 Alpha, u8* Texture,
                          Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007B00);
    ToumeIMode(red, Green, Blue, Alpha);
    KWTextureIA8Sub(Texture, arg9, argA, argB, argC, argD);
}

UNUSED void Kwtexture2DIA8TilePt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                          s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A40);
    KwtextiledrawIA8(Texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void Kwtexture2DIA8TileBl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                          s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A60);
    KwtextiledrawIA8(Texture, arg5, arg6, arg7, arg8, arg9);
}

void Kwtexture2DIA4Pt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A40);
    KWTextureIA4Sub(Texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void Kwtexture2DIA4XLUBl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 red, s32 Green, s32 Blue, s32 Alpha, u8* Texture,
                          Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A60);
    ToumeIMode(red, Green, Blue, Alpha);
    KWTextureIA4Sub(Texture, arg9, argA, argB, argC, argD);
}
UNUSED void Kwtexture2DA4Pt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                          s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A40);
    gDPSetCombineLERP(DisplayListHead++, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0);
    KWTextureA4Sub(Texture, arg5, arg6, arg7, arg8, arg9);
}

void Kwtexture2DA4Bl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A60);
    gDPSetCombineLERP(DisplayListHead++, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0);
    KWTextureA4Sub(Texture, arg5, arg6, arg7, arg8, arg9);
}

void Kwtexture2DA4XLUPt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 red, s32 Green, s32 Blue, s32 Alpha, u8* Texture,
                   Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A40);
    SplashMode(red, Green, Blue, Alpha);
    KWTextureA4Sub(Texture, arg9, argA, argB, argC, argD);
}

void Kwtexture2DA4XLUBl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, s32 red, s32 Green, s32 Blue, s32 Alpha, u8* Texture,
                   Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    Kwcreateaffine2D(arg0, arg1, Arg2, Arg3);
    gSPDisplayList(DisplayListHead++, D_0D007A60);
    SplashMode(red, Green, Blue, Alpha);
    KWTextureA4Sub(Texture, arg9, argA, argB, argC, argD);
}

void Kwtexture3DIA8Pt(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* Texture, Vtx* arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007A40);
    KWTextureIA8Sub(Texture, arg4, arg5, arg6, arg7, arg8);
}

UNUSED void Kwtexture3DIA8Bl(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* Texture, Vtx* arg4, s32 arg5, s32 arg6, s32 arg7,
                          s32 arg8) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007A60);
    KWTextureIA8Sub(Texture, arg4, arg5, arg6, arg7, arg8);
}

UNUSED void Kwtexture3DIA8ZBPt(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* Texture, Vtx* arg4, s32 arg5, s32 arg6, s32 arg7,
                          s32 arg8) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007AC0);
    KWTextureIA8Sub(Texture, arg4, arg5, arg6, arg7, arg8);
}

UNUSED void Kwtexture3DIA8ZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* Texture, Vtx* arg4, s32 arg5, s32 arg6, s32 arg7,
                          s32 arg8) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007AE0);
    KWTextureIA8Sub(Texture, arg4, arg5, arg6, arg7, arg8);
}

UNUSED void Kwtexture3D16X16IA8Pt(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* Texture, Vtx* arg4) {
    Kwtexture3DIA8Pt(arg0, arg1, Arg2, Texture, arg4, 16, 16, 16, 16);
}

void KwdrawKage3D(Collision* arg0, Vec3f arg1, f32 Arg2) {
    if (KwcalcBump(arg0, arg1) != 0) {
        D_80183E50[0] = arg1[0];
        D_80183E50[1] = CalcHeight(arg1[0], 0.0f, arg1[2], arg0->LastZX) + 0.8;
        D_80183E50[2] = arg1[2];
        KWCreateAffine3DHole(D_80183E50, arg0->BumpZX, Arg2);
        gSPDisplayList(DisplayListHead++, ShadowModel2);
    }
}

void KwdrawKage3DFast(s32 ObjectIndex, f32 Scale) {
    AnmObject* object;

    if ((KwanmBitcheckflag(ObjectIndex, 0x00000020) != 0) &&
        (KwanmBitcheckflag(ObjectIndex, 0x00800000) != 0)) {
        object = &DynamicObjects[ObjectIndex];
        D_80183E50[0] = object->pos[0];
        D_80183E50[1] = object->Ground + 0.8;
        D_80183E50[2] = object->pos[2];
        KWCreateAffine3D(D_80183E50, object->Kageang, Scale);
        gSPDisplayList(DisplayListHead++, D_0D007B20);
    }
}

void KwdrawKage3DFlat(s32 ObjectIndex, f32 arg1) {
    AnmObject* object;

    if (KwanmBitcheckflag(ObjectIndex, 0x00000020) != 0) {
        object = &DynamicObjects[ObjectIndex];
        D_80183E50[0] = object->pos[0];
        D_80183E50[1] = object->Ground + 0.8;
        D_80183E50[2] = object->pos[2];
        D_80183E98[0] = 0x4000;
        D_80183E98[1] = 0;
        D_80183E98[2] = 0;
        KWCreateAffine3D(D_80183E50, D_80183E98, arg1);
        gSPDisplayList(DisplayListHead++, D_0D007B20);
    }
}

void KwdrawKage3DBump(s32 ObjectIndex, f32 arg1) {
    Mat4 sp30;
    AnmObject* object;

    if ((KwanmBitcheckflag(ObjectIndex, 0x00000020) != 0) &&
        (KwanmBitcheckflag(ObjectIndex, 0x00800000) != 0)) {
        object = &DynamicObjects[ObjectIndex];
        D_80183E50[0] = object->pos[0];
        D_80183E50[1] = object->Ground + 0.8;
        D_80183E50[2] = object->pos[2];
        KwcreateRotationMatrix(sp30, object->PosEnde, D_80183E50, 0U, arg1);
        Affine2Mtx(&DynamicP->MTXHud[Modelingnum], sp30);
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(DisplayListHead++, ShadowModel2);
    }
}

void KwdrawHole3D(f32 arg0) {
    KWCreateAffine3DHole(D_80183E50, D_80183E70, arg0);
    gSPDisplayList(DisplayListHead++, D_0D007C10);
}

UNUSED void Kwtexture3DA8ZBBl(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* Texture, Vtx* arg4, s32 arg5, s32 arg6, s32 arg7,
                          s32 arg8) {
    KWCreateAffine3D(arg0, arg1, Arg2);
    gSPDisplayList(DisplayListHead++, D_0D007AE0);
    SparkMode(D_80165860, D_8016586C, D_80165878);
    KWTextureA8Sub(Texture, arg4, arg5, arg6, arg7, arg8);
}

UNUSED void Kwtexture2D16X16IA16(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5) {
    Kwtexture2DIA16Pt(arg0, arg1, Arg2, Arg3, Texture, arg5, 16, 16, 16, 16);
}

UNUSED void Kwtexture2D16X16IA8(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5) {
    Kwtexture2DIA8Pt(arg0, arg1, Arg2, Arg3, Texture, arg5, 16, 16, 16, 16);
}

UNUSED void Kwtexture2D16X16IA4(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* arg4, Vtx* arg5) {
    Kwtexture2DIA4Pt(arg0, arg1, Arg2, Arg3, arg4, arg5, 16, 16, 16, 16);
}

UNUSED void Kwtexture2D32X32IA16(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5) {
    Kwtexture2DIA16Pt(arg0, arg1, Arg2, Arg3, Texture, arg5, 32, 32, 32, 32);
}

UNUSED void Kwtexture2D32X32IA8(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* arg4, Vtx* arg5) {
    Kwtexture2DIA8Pt(arg0, arg1, Arg2, Arg3, arg4, arg5, 32, 32, 32, 32);
}

UNUSED void Kwtexture2D32X32IA4(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* arg4, Vtx* arg5) {
    Kwtexture2DIA4Pt(arg0, arg1, Arg2, Arg3, arg4, arg5, 32, 32, 32, 32);
}

UNUSED void Kwtexture3D32X32IA8(Vec3f arg0, Vec3su arg1, f32 Arg2, u8* Texture, Vtx* arg4) {
    Kwtexture3DIA8Pt(arg0, arg1, Arg2, Texture, arg4, 32, 32, 32, 32);
}

UNUSED void Kwtexture2D64X32IA16(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5) {
    Kwtexture2DIA16Pt(arg0, arg1, Arg2, Arg3, Texture, arg5, 64, 32, 64, 32);
}

UNUSED void Kwtexture2D64X32IA8(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* arg4, Vtx* arg5) {
    Kwtexture2DIA8Pt(arg0, arg1, Arg2, Arg3, arg4, arg5, 64, 32, 64, 32);
}

UNUSED void Kwtexture2D64X32IA8Bl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* arg4, Vtx* arg5) {
    Kwtexture2DIA8Bl(arg0, arg1, Arg2, Arg3, arg4, arg5, 64, 32, 64, 32);
}

UNUSED void Kwtexture2D64X32IA8AAPt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5) {
    Kwtexture2DIA8AAPt(arg0, arg1, Arg2, Arg3, Texture, arg5, 64, 32, 64, 32);
}

UNUSED void Kwtexture2D64X32IA8AABl(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5) {
    Kwtexture2DIA8AABl(arg0, arg1, Arg2, Arg3, Texture, arg5, 64, 32, 64, 32);
}

UNUSED void Kwtexture2D64X64IA8(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5) {
    Kwtexture2DIA8Pt(arg0, arg1, Arg2, Arg3, Texture, arg5, 64, 64, 64, 64);
}

UNUSED void Kwtexture2D64X64IA16(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5) {
    Kwtexture2DIA16Pt(arg0, arg1, Arg2, Arg3, Texture, arg5, 64, 64, 64, 32);
}

UNUSED void Kwtexture2D64X96IA8Pt(s32 arg0, s32 arg1, u16 Arg2, f32 Arg3, u8* Texture, Vtx* arg5) {
    Kwtexture2DIA8Pt(arg0, arg1, Arg2, Arg3, Texture, arg5, 64, 96, 64, 48);
}

UNUSED void Renderghost(void) {
    gDPSetRenderMode(DisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
}

void InitcolpalCI8(u8* tlut) {
    gDPLoadTLUT_pal256(DisplayListHead++, tlut);
    gDPLoadSync(DisplayListHead++);
    gDPSetTexturePersp(DisplayListHead++, G_TP_NONE);
}

void KWSetPrimColor(s32 red, s32 Green, s32 Blue, s32 Alpha) {
    gDPSetPrimColor(DisplayListHead++, 0, 0, red, Green, Blue, Alpha);
}

UNUSED void KWSetEnvColor(s32 red, s32 Green, s32 Blue, s32 Alpha) {
    gDPSetEnvColor(DisplayListHead++, red, Green, Blue, Alpha);
}

void SetColorRender(s32 primRed, s32 primGreen, s32 primBlue, s32 envRed, s32 envGreen, s32 envBlue, s32 primAlpha) {
    gDPSetPrimColor(DisplayListHead++, 0, 0, primRed, primGreen, primBlue, primAlpha);
    gDPSetEnvColor(DisplayListHead++, envRed, envGreen, envBlue, 0xFF);
}

UNUSED void SubColorMode(s32 red, s32 Green, s32 Blue) {
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(DisplayListHead++, 0, 0, red, Green, Blue, 0xFF);
}

void SubAlphaMode(s32 Alpha) {
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(DisplayListHead++, 0, 0, 0xFF, 0xFF, 0xFF, Alpha);
}

void ToumeIAMode(s32 Alpha) {
    gDPSetCombineLERP(DisplayListHead++, 0, 0, 0, TEXEL0, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0,
                      PRIMITIVE, 0);
    gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, Alpha);
}

void ToumeIMode(u32 red, u32 Green, u32 Blue, u32 Alpha) {
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(DisplayListHead++, 0, 0, red, Green, Blue, Alpha);
}

void SmokeMode(s32 Alpha) {
    gDPSetCombineLERP(DisplayListHead++, 0, 0, 0, 1, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, 1, TEXEL0, 0, PRIMITIVE, 0);
    gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, Alpha);
}

void SplashMode(s32 red, s32 Green, s32 Blue, s32 Alpha) {
    gDPSetCombineLERP(DisplayListHead++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0,
                      PRIMITIVE, 0);
    gDPSetPrimColor(DisplayListHead++, 0, 0, red, Green, Blue, Alpha);
}

void RadarMode(s32 red, s32 Green, s32 Blue) {
    gDPSetCombineLERP(DisplayListHead++, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0);
    gDPSetPrimColor(DisplayListHead++, 0, 0, red, Green, Blue, 0xFF);
}

UNUSED void RankMode(s32 red, s32 Green, s32 Blue, s32 Alpha) {
    gDPSetCombineLERP(DisplayListHead++, 1, 0, SHADE, PRIMITIVE, 0, 0, 0, TEXEL0, 1, 0, SHADE, PRIMITIVE, 0, 0, 0,
                      TEXEL0);
    gDPSetPrimColor(DisplayListHead++, 0, 0, red, Green, Blue, Alpha);
}

UNUSED void WaterMode(s32 Alpha) {
    gDPSetCombineLERP(DisplayListHead++, 0, 0, 0, SHADE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, SHADE, TEXEL0, 0, PRIMITIVE,
                      0);
    gDPSetPrimColor(DisplayListHead++, 0, 0, 0xFF, 0xFF, 0xFF, Alpha);
}

UNUSED void ColAddMode(s32 red, s32 Green, s32 Blue, s32 Alpha) {
    gDPSetPrimColor(DisplayListHead++, 0, 0, red, Green, Blue, Alpha);
    gDPSetCombineLERP(DisplayListHead++, 1, PRIMITIVE_ALPHA, TEXEL0, PRIMITIVE, 0, 0, 0, TEXEL0, 1, PRIMITIVE_ALPHA,
                      TEXEL0, PRIMITIVE, 0, 0, 0, TEXEL0);
}

void IceMode(s32 primRed, s32 primGreen, s32 primBlue, s32 envRed, s32 envGreen, s32 envBlue, s32 primAlpha) {
    gDPSetPrimColor(DisplayListHead++, 0, 0, primRed, primGreen, primBlue, primAlpha);
    gDPSetEnvColor(DisplayListHead++, envRed, envGreen, envBlue, 0xFF);
    gDPSetCombineLERP(DisplayListHead++, 1, ENVIRONMENT, TEXEL0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0, 1, ENVIRONMENT,
                      TEXEL0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0);
}

void SparkMode(s32 red, s32 Green, s32 Blue) {
    gDPSetCombineLERP(DisplayListHead++, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0);
    gDPSetPrimColor(DisplayListHead++, 0, 0, red, Green, Blue, 0xFF);
}

void Color2Mode(u32 primRed, u32 primGreen, u32 primBlue, u32 envRed, u32 envGreen, u32 envBlue, u32 primAlpha) {
    gDPSetPrimColor(DisplayListHead++, 0, 0, primRed, primGreen, primBlue, primAlpha);
    gDPSetEnvColor(DisplayListHead++, envRed, envGreen, envBlue, 0xFF);
    gDPSetCombineLERP(DisplayListHead++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                      PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
}

void KWRectangle(s32 x, s32 y, s32 Width, s32 Height, s32 s, s32 w, s32 Mode) {

    s32 xh = (((x + Width) - 1) << 2);
    s32 yh = (((y + Height) - 1) << 2);
    s32 xl = ((x * 4));
    s32 yl = y * 4;

    s32 xh2 = (((x + Width)) << 2);
    s32 yh2 = ((y + Height) << 2);

    // If no cycle Mode is set, render Texture rectangle in copy Mode
    if (Mode == 0) {
        gSPTextureRectangle(DisplayListHead++, xl, yl, xh, yh, G_TX_RENDERTILE, s << 5, (w << 5), 4 << 10, 1 << 10);
        return;
    }
    // Render Texture rectangle in default cycle Mode (1 cycle or 2 cycle)
    gSPTextureRectangle(DisplayListHead++, xl, yl, xh2, yh2, G_TX_RENDERTILE, s << 5, (w << 5), 1 << 10, 1 << 10);
}

void SprDrawNoclip(s32 x, s32 y, s32 Width, s32 Height, s32 Mode) {
    // (0, 0) means Texture coordinates will be rendered from the top Left corner
    KWRectangle(x, y, Width, Height, 0, 0, Mode);
}

void SprDrawClip(s32 arg0, s32 arg1, s32 Arg2, s32 Arg3, s32 arg4) {
    UNUSED s32 pad[2];
    s32 sp2C;
    s32 var_a1;
    s32 var_v0;
    s32 var_v1;

    if ((-Arg2 < arg0) && (-Arg3 < arg1)) {
        var_v0 = 0;
        var_v1 = 0;
        sp2C = arg0;
        var_a1 = arg1;
        if (arg0 < 0) {
            var_v1 = -arg0;
            sp2C = 0;
        }
        if (arg1 < 0) {
            var_v0 = -arg1;
            var_a1 = 0;
        }
        KWRectangle(sp2C, var_a1, Arg2 - var_v1, Arg3 - var_v0, var_v1, var_v0, arg4);
    }
}

void SprDrawClipXflip(s32 arg0, s32 arg1, s32 Arg2, s32 Arg3, s32 arg4) {
    UNUSED s32 pad[2];
    s32 sp2C;
    s32 phi_a1;
    s32 phi_v1;
    s32 phi_v0;

    if ((-Arg2 < arg0) && (-Arg3 < arg1)) {
        phi_v0 = 0;
        phi_v1 = 0;
        sp2C = arg0;
        phi_a1 = arg1;
        if (arg0 < 0) {
            phi_v1 = -arg0;
            sp2C = 0;
        }
        if (arg1 < 0) {
            phi_v0 = -arg1;
            phi_a1 = 0;
        }
        KWRectangle(sp2C, phi_a1, Arg2 - phi_v1, Arg3 - phi_v0, phi_v1 + Arg2, phi_v0, arg4);
    }
}

void SprDrawClipST(s32 arg0, s32 arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6) {
    UNUSED s32 pad[2];
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 phi_a3;
    s32 phi_v0;
    s32 phi_v1;

    if ((-Arg2 < arg0) && (-Arg3 < arg1)) {
        sp34 = arg0;
        sp30 = arg1;
        phi_v0 = arg4;
        sp2C = Arg2;
        phi_a3 = Arg3;
        phi_v1 = arg5;
        if (arg0 < 0) {
            phi_v0 = arg4 - arg0;
            sp34 = 0;
            sp2C = Arg2 + arg0;
        }
        if (arg1 < 0) {
            phi_v1 = arg5 - arg1;
            sp30 = 0;
            phi_a3 = Arg3 + arg1;
        }
        KWRectangle(sp34, sp30, sp2C, phi_a3, phi_v0, phi_v1, arg6);
    }
}

UNUSED void func_8004BB34(void) {
}

void SprDrawClipScale(s32 arg0, s32 arg1, s32 Arg2, s32 Arg3, f32 arg4) {
    s16 t;
    s16 s;
    s16 temp_t9;
    s32 var_t0;
    s32 var_t1;
    s32 xl;
    s32 yl;
    UNUSED s32 Thing0;
    UNUSED s32 Thing1;

    var_t0 = (Arg2 * 4 * arg4) + 0.5;
    var_t1 = (Arg3 * 4 * arg4) + 0.5;
    xl = (arg0 * 4) - (var_t0 / 2);
    yl = (arg1 * 4) - (var_t1 / 2);
    if (-var_t0 < xl) {
        t = 0;
        if (-var_t1 < yl) {
            s = 0;
            if (xl < 0) {
                var_t0 += xl;
                s = (-xl * 8) / arg4;
                xl = 0;
            }
            if (yl < 0) {
                var_t1 += yl;
                t = (-yl * 8) / arg4;
                yl = 0;
            }
            temp_t9 = (1024.0f / arg4) + 0.5;
            gSPTextureRectangle(DisplayListHead++, xl, yl, xl + var_t0, yl + var_t1, 0, s, t, temp_t9, temp_t9);
        }
    }
}

UNUSED void KwspriteRGBAMorph(s32 x, s32 y, u32 Width, u32 Height, s32 Alpha, u16* texture1, u16* texture2) {
    gSPDisplayList(DisplayListHead++, D_0D007F38);
    gSPDisplayList(DisplayListHead++, D_0D008138);
    gDPSetTextureLOD(DisplayListHead++, G_TL_TILE);
    gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, Alpha);
    gDPSetCombineLERP(DisplayListHead++, TEXEL1, TEXEL0, PRIMITIVE_ALPHA, TEXEL0, TEXEL1, TEXEL0, PRIMITIVE, TEXEL0, 0,
                      0, 0, COMBINED, 0, 0, 0, COMBINED);
    gDPLoadMultiTile(DisplayListHead++, texture1, 0, G_TX_RENDERTILE, G_IM_FMT_RGBA, G_IM_SIZ_16b, Width, Height, 0, 0,
                     Width - 1, Height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
                     G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadMultiTile(DisplayListHead++, texture2, 256, G_TX_RENDERTILE + 1, G_IM_FMT_RGBA, G_IM_SIZ_16b, Width, Height,
                     0, 0, Width - 1, Height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP,
                     G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    SprDrawNoclip(x, y, Width, Height, 2);
    gSPDisplayList(DisplayListHead++, D_0D008120);
}

void DrawLineHorizontal(s16 arg0, s16 arg1, s16 Arg2, u16 red, u16 Green, u16 Blue, u16 Alpha) {
    gDPSetPrimColor(DisplayListHead++, 0, 0, red, Green, Blue, Alpha);
    gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
    gDPSetTexturePersp(DisplayListHead++, G_TP_NONE);
    gDPSetCombineMode(DisplayListHead++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(DisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    SprDrawClip(arg0, arg1, Arg2, 1, 1);
}

void DrawLineVertical(s16 arg0, s16 arg1, s16 Arg2, u16 red, u16 Green, u16 Blue, u16 Alpha) {
    gDPSetPrimColor(DisplayListHead++, 0, 0, red, Green, Blue, Alpha);
    gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
    gDPSetTexturePersp(DisplayListHead++, G_TP_NONE);
    gDPSetCombineMode(DisplayListHead++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(DisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    SprDrawClip(arg0, arg1, 1, Arg2, 1);
}

void KwspriteSub(u32 arg0, u32 arg1, u16* Texture, u32 Width, u32 arg4, u32 Height, s32 arg6) {
    s32 i;
    u16* img2;

    arg0 -= (Width / 2);
    arg1 -= (arg4 / 2);
    img2 = Texture;

    for (i = 0; (u32) i < (arg4 / Height); i++) {
        KWLoadTextureBlockRGBA16B(img2, Width, Height);
        SprDrawClip(arg0, arg1, Width, Height, arg6);
//! @todo fakematch?
#ifdef AVOID_UB
        img2 += (Width * Height);
#else
        img2 += (Width * Height) ^ ((arg4 / Height) * 0);
#endif
        arg1 += Height;
    }
}

UNUSED void func_8004C354() {
}

UNUSED void func_8004C35C() {
}

void KWSprite(s32 x, s32 y, u32 Width, u32 Height, u16* Texture) {
    gSPDisplayList(DisplayListHead++, D_0D008108);
    gSPDisplayList(DisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(DisplayListHead++, G_AC_THRESHOLD);
    KWLoadTextureBlockRGBA16B(Texture, Width, Height);
    SprDrawClip(x - (Width >> 1), y - (Height >> 1), Width, Height, 0);
    gSPDisplayList(DisplayListHead++, D_0D007EB8);
}

void KwspriteFlash(s32 x, s32 y, u32 Width, u32 Height, u16* Texture) {

    gSPDisplayList(DisplayListHead++, D_0D007F38);
    IceMode(KwflashR, KwflashG, KwflashB, 0x80, 0x80, 0x80, 0xFF);
    KWLoadTextureBlockRGBA16B(Texture, Width, Height);
    SprDrawClip(x - (Width >> 1), y - (Height >> 1), Width, Height, 1);
    gSPDisplayList(DisplayListHead++, D_0D007EB8);
}

UNUSED void KwspriteT(s32 arg0, s32 arg1, u32 Arg2, u32 Arg3, u16* Texture) {

    gSPDisplayList(DisplayListHead++, D_0D008108);
    gSPDisplayList(DisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(DisplayListHead++, G_AC_THRESHOLD);
    KwloadtexturetileRGBA16B(Texture, Arg2, Arg3);
    SprDrawClip(arg0 - (Arg2 >> 1), arg1 - (Arg3 >> 1), Arg2, Arg3, 0);
    gSPDisplayList(DisplayListHead++, D_0D007EB8);
}

void Kwsprite32B(s32 arg0, s32 arg1, u32 Arg2, u32 Arg3, u8* Texture) {

    gSPDisplayList(DisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(DisplayListHead++, G_AC_THRESHOLD);
    KwloadtextureblockRGBA32B(Texture, Arg2, Arg3);
    SprDrawClip(arg0 - (Arg2 >> 1), arg1 - (Arg3 >> 1), Arg2, Arg3, 1);
    gSPDisplayList(DisplayListHead++, D_0D007EB8);
}

void KWSpriteTile32B(s16 x, s16 y, u8* Texture, u32 Width, u32 Height) {
    s32 texSizeLess;
    s32 i;
    s32 centerY;
    s32 centerX;
    s32 numTextureBlocks;
    u32 texSize;
    s32 heightDiv;
    s32 Size;
    u8* textureCopy;

    centerX = x - (Width / 2);
    centerY = y - (Height / 2);
    textureCopy = Texture;
    gSPDisplayList(DisplayListHead++, D_0D007EF8);
    gDPSetRenderMode(DisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    texSize = Width * Height * 4;
    numTextureBlocks = texSize / 4096;
    if (texSize % 4096) {
        numTextureBlocks++;
    }
    heightDiv = Height / numTextureBlocks;
    Size = numTextureBlocks;
    for (i = 0; i < Size; i++) {
        KwloadtexturetileRGBA32B(textureCopy, Width, heightDiv);
        SprDrawNoclip(centerX, centerY, Width, heightDiv, 1);
        textureCopy += (Width * heightDiv * 4);
        texSizeLess = texSize - (Width * heightDiv * 4);
        if (texSizeLess < 0) {
            heightDiv = texSize / Width;
        } else {
            texSize -= (Width * heightDiv * 4);
        }


        centerY += heightDiv;
    }

    gSPDisplayList(DisplayListHead++, D_0D007EB8);
    // FAKE
    centerX++;
    centerX--;
}

void KwdisplayTitlelogo(s16 x, s16 y) {
    KWSpriteTile32B(x, y, Logoptr, 256, 128);
}

UNUSED void KWSpriteDiv(s32 arg0, s32 arg1, u16* Texture, s32 Arg3, s32 arg4, s32 arg5) {
    gSPDisplayList(DisplayListHead++, D_0D008108);
    gSPDisplayList(DisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(DisplayListHead++, G_AC_THRESHOLD);
    KwspriteSub(arg0, arg1, Texture, Arg3, arg4, arg5, 0);
    gSPDisplayList(DisplayListHead++, D_0D007EB8);
}

void KWSpriteXLU(s32 arg0, s32 arg1, s32 Arg2, u16* Texture, s32 arg4, s32 arg5, UNUSED s32 arg6, s32 arg7) {
    gSPDisplayList(DisplayListHead++, D_0D007F38);
    SubAlphaMode(Arg2);
    KwspriteSub(arg0, arg1, Texture, arg4, arg5, arg7, 1);
}

void KWSpriteScale(s32 arg0, s32 arg1, f32 Arg2, u16* Texture, s32 arg4, s32 arg5) {
    gSPDisplayList(DisplayListHead++, D_0D007F78);
    KWLoadTextureBlockRGBA16B(Texture, arg4, arg5);
    SprDrawClipScale(arg0, arg1, arg4, arg5, Arg2);
}

void KWSprite8x8(s32 x, s32 y, u16* Texture) {
    KWSprite(x, y, 8, 8, Texture);
}

UNUSED void Kwsprite8X16(s32 x, s32 y, u16* Texture) {
    KWSprite(x, y, 8, 16, Texture);
}

UNUSED void Kwsprite16X16(s32 x, s32 y, u16* Texture) {
    KWSprite(x, y, 16, 16, Texture);
}

void Kwsprite32X8(s32 x, s32 y, u16* Texture) {
    KWSprite(x, y, 32, 8, Texture);
}

void Kwsprite32X16(s32 x, s32 y, u16* Texture) {
    KWSprite(x, y, 32, 16, Texture);
}

UNUSED void Kwsprite32X16Sc(s32 arg0, s32 arg1, f32 Arg2, u16* Texture) {
    KWSpriteScale(arg0, arg1, Arg2, Texture, 32, 16);
}

UNUSED void Kwsprite32X32(s32 x, s32 y, u16* Texture) {
    KWSprite(x, y, 32, 32, Texture);
}

UNUSED void Kwsprite32X3232B(s32 arg0, s32 arg1, u8* Texture) {
    Kwsprite32B(arg0, arg1, 32, 32, Texture);
}

UNUSED void Kwsprite40X32(s32 x, s32 y, u16* Texture) {
    KWSprite(x, y, 40, 32, Texture);
}

UNUSED void Kwsprite48X48(s32 x, s32 y, u16* Texture) {
    KWSpriteDiv(x, y, Texture, 48, 48, 24);
}

UNUSED void Kwsprite64X32(s32 x, s32 y, u16* Texture) {
    KWSprite(x, y, 64, 32, Texture);
}

UNUSED void Kwsprite64X32Sc(s32 arg0, s32 arg1, f32 Arg2, u16* Texture) {
    KWSpriteScale(arg0, arg1, Arg2, Texture, 64, 32);
}

UNUSED void Kwsprite64X64(s32 arg0, s32 arg1, u16* Texture) {
    KWSpriteDiv(arg0, arg1, Texture, 64, 64, 32);
}

UNUSED void KwspriteIA16(s32 arg0, s32 arg1, UNUSED u8* Texture, s32 Width, s32 arg4, s32 Height) {
    UNUSED s32 pad;
    s32 var_s0;
    s32 i;
    u8* img;

    var_s0 = arg1 - (arg4 / 2);
    gSPDisplayList(DisplayListHead++, D_0D007FE0);

    for (i = 0; i < arg4 / Height; i++) {
        KwloadtextureblockIA16B(img, Width, Height);
        SprDrawClip(arg0 - (Width / 2), var_s0, Width, Height, 1);
        img += Width * Height * 2;
        var_s0 += Height;
    }
}

UNUSED void KwspriteIA8Common(s32 arg0, s32 arg1, u8* Texture, s32 Width, s32 arg4, s32 Height) {
    s32 var_s0 = arg1 - (arg4 / 2);
    s32 i;
    u8* img = Texture;

    for (i = 0; i < arg4 / Height; i++) {
        KwloadtextureblockIA8B(img, Width, Height);
        SprDrawClip(arg0 - (Width / 2), var_s0, Width, Height, 1);
        img += Width * Height;
        var_s0 += Height;
    }
}

UNUSED void KwspriteIA8Pt(s32 arg0, s32 arg1, u8* Texture, s32 Arg3, s32 arg4, UNUSED s32 arg5, s32 arg6) {
    gSPDisplayList(DisplayListHead++, D_0D007FE0);
    KwspriteIA8Common(arg0, arg1, Texture, Arg3, arg4, arg6);
}

UNUSED void KwspriteIA8Bl(s32 arg0, s32 arg1, u8* Texture, s32 Arg3, s32 arg4, UNUSED s32 arg5, s32 arg6) {
    gSPDisplayList(DisplayListHead++, D_0D008000);
    KwspriteIA8Common(arg0, arg1, Texture, Arg3, arg4, arg6);
}

UNUSED void KwspriteIA8XLUPt(s32 arg0, s32 arg1, u8* Texture, s32 red, s32 Green, s32 Blue, s32 Alpha, s32 arg7, s32 arg8,
                          UNUSED s32 arg9, s32 argA) {
    gSPDisplayList(DisplayListHead++, D_0D007FE0);
    ToumeIMode(red, Green, Blue, Alpha);
    KwspriteIA8Common(arg0, arg1, Texture, arg7, arg8, argA);
}

UNUSED void func_8004D0CC(void) {
}

UNUSED void KwspriteIA4(s32 arg0, s32 arg1, u8* Texture, s32 Width, s32 arg4, s32 Height) {
    s32 var_s0;
    u8* img;
    s32 i;

    var_s0 = arg1 - (arg4 / 2);
    img = Texture;
    gSPDisplayList(DisplayListHead++, D_0D007FE0);

    for (i = 0; i < arg4 / Height; i++) {
        KwloadtextureblockIA4B(img, Width, Height);
        SprDrawClip(arg0 - (Width / 2), var_s0, Width, Height, 1);
        img += Width * Height;
        var_s0 += Height;
    }
}

void KwspriteIA4XLUPt(s32 arg0, s32 arg1, u8* Texture, s32 Arg3, s32 arg4, s32 arg5, s32 arg6, s32 Width, s32 arg8,
                   UNUSED s32 arg9, s32 Height) {
    s32 var_s3;
    u8* img;
    s32 i;

    var_s3 = arg1 - (arg8 / 2);
    img = Texture;
    gSPDisplayList(DisplayListHead++, D_0D007FE0);
    ToumeIMode(Arg3, arg4, arg5, arg6);

    for (i = 0; i < arg8 / Height; i++) {
        KwloadtextureblockIA4B(img, Width, Height);
        SprDrawClip(arg0 - (Width / 2), var_s3, Width, Height, 1);
        img += (Width * Height) / 2;
        var_s3 += Height;
    }
}

void KwspriteA4XLUPt(s32 arg0, s32 arg1, u8* Texture, s32 Arg3, s32 arg4, s32 arg5, s32 arg6, s32 Width, s32 arg8,
                   UNUSED s32 arg9, s32 Height) {
    s32 var_s3;
    u8* img;
    s32 i;

    var_s3 = arg1 - (arg8 / 2);
    img = Texture;
    gSPDisplayList(DisplayListHead++, D_0D007FE0);
    SplashMode(Arg3, arg4, arg5, arg6);

    for (i = 0; i < arg8 / Height; i++) {
        KwloadtextureblockA4B(img, Width, Height);
        SprDrawClip(arg0 - (Width / 2), var_s3, Width, Height, 1);
        img += (Width * Height) / 2;
        var_s3 += Height;
    }
}

void KwspriteA4XLUPtXflip(s32 arg0, s32 arg1, u8* Texture, s32 Arg3, s32 arg4, s32 arg5, s32 arg6, s32 Width, s32 arg8,
                   UNUSED s32 arg9, s32 Height) {
    s32 var_s3;
    u8* img;
    s32 i;

    var_s3 = arg1 - (arg8 / 2);
    img = Texture;
    gSPDisplayList(DisplayListHead++, D_0D007FE0);
    SplashMode(Arg3, arg4, arg5, arg6);

    for (i = 0; i < arg8 / Height; i++) {
        KwloadtextureblockA4BXflip(img, Width, Height);
        SprDrawClipXflip(arg0 - (Width / 2), var_s3, Width, Height, 1);
        img += (Width * Height) / 2;
        var_s3 += Height;
    }
}

void KwspriteScaleA4XLUBl(s32 arg0, s32 arg1, u8* Texture, f32 Arg3, s32 arg4, s32 arg5, s32 arg6, UNUSED s32 arg7, s32 Width,
                   s32 arg9, UNUSED s32 argA, s32 Height) {
    s32 i;
    s32 var_s3;
    u8* textureCopy;

    var_s3 = arg1 - (arg9 / 2);
    textureCopy = Texture;
    gSPDisplayList(DisplayListHead++, D_0D008000);
    RadarMode(arg4, arg5, arg6);
    for (i = 0; i < (arg9 / Height); i++) {
        KwloadtextureblockA4B(textureCopy, Width, Height);
        SprDrawClipScale(arg0, arg1, Width, arg9, Arg3);
        textureCopy += (Width * Height) / 2;
        var_s3 += Height;
    }
}

void KwspriteDrawYraIA16(s32 arg0, s32 arg1, u8* Texture, s32 Arg3, s32 arg4) {
    s32 sp5C;
    f32 temp_f20;
    s16 temp_s7;
    s16 var_s1;
    u16 temp_s0;
    s32 temp_s5;
    s32 var_s3;
    u8* img;
    UNUSED s32 test[3];
    s32 i;

    D_801656B0 += D_80165710;
    temp_f20 = D_8018D00C;
    temp_s7 = D_80165708;
    var_s1 = D_801656B0;
    img = Texture;
    var_s3 = arg1 - (arg4 / 2);
    gSPDisplayList(DisplayListHead++, D_0D007FE0);

    sp5C = Arg3 * 2;
    for (i = 0; i < arg4; i++) {
        temp_s0 = var_s1;
        temp_s5 = (s32) ((SinT(temp_s0) * temp_f20) + (f32) (arg0 - (Arg3 / 2)));
        SinT(temp_s0);
        KwloadtextureblockIA16B(img, Arg3, 1);
        SprDrawClip(temp_s5, var_s3, Arg3, 1, 1);

        var_s1 += temp_s7;
        var_s3 += 1;
        img += sp5C;
    }
}

void KwspriteDrawYraIA8(s32 arg0, s32 arg1, u8* Texture, s32 Arg3, s32 arg4) {
    f32 temp_f20;
    s16 temp_s7;
    s16 var_s1;
    u16 temp_s0;
    s32 temp_s6;
    s32 var_s4;
    u8* img;
    s32 i;
    s32 var;

    D_801656B0 += D_80165710;
    temp_f20 = D_8018D00C;
    temp_s7 = D_80165708;
    var_s1 = D_801656B0;
    img = Texture;
    var = Arg3 / 2;
    var_s4 = arg1 - (arg4 / 2);

    gSPDisplayList(DisplayListHead++, D_0D007FE0);

    for (i = 0; i < arg4; i++) {
        temp_s0 = var_s1;
        temp_s6 = (s32) ((SinT(temp_s0) * temp_f20) + (f32) (arg0 - (var)));
        SinT(temp_s0);
        KwloadtextureblockIA8B(img, Arg3, 1);
        SprDrawClip(temp_s6, var_s4, Arg3, 1, 1);
        var_s1 += temp_s7;
        img = &img[Arg3];
        var_s4 += 1;
    }
}

UNUSED void KwspriteDrawYraIA4(s32 arg0, s32 arg1, u8* Texture, s32 Arg3, s32 arg4) {
    f32 temp_f20;
    s16 temp_s7;
    s16 var_s1;
    u16 temp_s0;
    s32 temp_s6;
    u8* img;
    s32 var_s4;
    s32 var;
    s32 i;

    D_801656B0 += D_80165710;
    temp_f20 = D_8018D00C;
    temp_s7 = D_80165708;
    var_s1 = (s16) D_801656B0;
    img = Texture;
    var = Arg3 / 2;
    var_s4 = arg1 - (arg4 / 2);

    gSPDisplayList(DisplayListHead++, D_0D007FE0);
    for (i = 0; i < arg4; i++) {
        temp_s0 = var_s1;
        temp_s6 = (s32) ((SinT(temp_s0) * temp_f20) + (f32) (arg0 - (var)));
        SinT(temp_s0);
        KwloadtextureblockIA4B(img, Arg3, 1);
        SprDrawClip(temp_s6, var_s4, Arg3, 1, 1);
        var_s1 += temp_s7;
        img += Arg3;
        var_s4 += 1;
    }
}

UNUSED void Kwsprite8X160IA8(s32 arg0, s32 arg1, u8* Texture) {
    KwspriteIA8Pt(arg0, arg1, Texture, 8, 160, 8, 160);
}

UNUSED void Kwsprite12X160IA8(s32 arg0, s32 arg1, u8* Texture) {
    KwspriteIA8Pt(arg0, arg1, Texture, 12, 160, 12, 160);
}

UNUSED void Kwsprite12X192IA8(s32 arg0, s32 arg1, u8* Texture) {
    KwspriteIA8Pt(arg0, arg1, Texture, 12, 192, 12, 192);
}

UNUSED void Kwsprite16X16IA(s32 arg0, s32 arg1, u8* Texture) {
    KwspriteIA16(arg0, arg1, Texture, 16, 16, 16);
}

UNUSED void Kwsprite16X160IA8(s32 arg0, s32 arg1, u8* Texture) {
    KwspriteIA8Pt(arg0, arg1, Texture, 16, 160, 16, 160);
}

UNUSED void Kwsprite32X32IA(s32 arg0, s32 arg1, u8* Texture) {
    KwspriteIA16(arg0, arg1, Texture, 32, 32, 32);
}

UNUSED void Kwsprite32X32IA8(s32 arg0, s32 arg1, u8* Texture) {
    KwspriteIA8Pt(arg0, arg1, Texture, 32, 32, 32, 32);
}

UNUSED void Kwsprite32X32IA4(s32 arg0, s32 arg1, u8* Texture) {
    KwspriteIA4(arg0, arg1, Texture, 32, 32, 32);
}

UNUSED void Kwsprite32X32YraIA16(s32 arg0, s32 arg1, u8* Texture) {
    KwspriteDrawYraIA16(arg0, arg1, Texture, 32, 32);
}

UNUSED void Kwsprite32X32YraIA8(s32 arg0, s32 arg1, u8* Texture) {
    KwspriteDrawYraIA8(arg0, arg1, Texture, 32, 32);
}

UNUSED void Kwsprite32X32YraIA4(s32 arg0, s32 arg1, u8* Arg2) {
    KwspriteDrawYraIA4(arg0, arg1, Arg2, 32, 32);
}

UNUSED void Kwsprite64X32IA(s32 arg0, s32 arg1, u8* Arg2) {
    KwspriteIA16(arg0, arg1, Arg2, 64, 32, 32);
}

UNUSED void Kwsprite64X64IA(s32 arg0, s32 arg1, u8* Arg2) {
    KwspriteIA16(arg0, arg1, Arg2, 64, 64, 32);
}

UNUSED void Kwsprite64X96IA8(s32 arg0, s32 arg1, u8* Texture) {
    KwspriteIA8Pt(arg0, arg1, Texture, 64, 96, 64, 48);
}

UNUSED void Kwsprite112X32IA8(s32 arg0, s32 arg1, u8* Arg2) {
    KwspriteIA8Pt(arg0, arg1, Arg2, 112, 32, 112, 32);
}

UNUSED void Kwsprite128X32IA8(s32 arg0, s32 arg1, u8* Arg2) {
    KwspriteIA8Pt(arg0, arg1, Arg2, 128, 32, 128, 32);
}

void SprDrawCI8(s32 arg0, s32 arg1, u8* Texture, s32 Width, s32 arg4, s32 Height) {
    s32 var_s0 = var_s0 = arg1 - (arg4 / 2);
    u8* img = Texture;
    s32 i;

    for (i = 0; i < arg4 / Height; i++) {
        KwloadtextureblockCI8B(img, Width, Height);
        SprDrawClip(arg0 - (Width / 2), var_s0, Width, Height, 1);
        img += Width * Height;
        var_s0 += Height;
    }
}

void SprDrawCI8Yra(s32 arg0, s32 arg1, u8* Texture, s32 Arg3, s32 arg4) {
    f32 temp_f20;
    s16 temp_s7;
    s16 var_s1;
    u16 temp_s0;
    s32 var_s4;
    u8* img;
    u32 temp_s6;
    s32 i;
    s32 var;

    D_801656B0 += D_80165710;
    temp_f20 = D_8018D00C;
    var_s1 = (s16) D_801656B0;
    temp_s7 = D_80165708;
    img = Texture;
    var = Arg3 / 2;
    var_s4 = arg1 - (arg4 / 2);

    for (i = 0; i < arg4; i++) {
        temp_s0 = var_s1;
        temp_s6 = (u32) ((SinT(temp_s0) * temp_f20) + (f32) (arg0 - var));
        SinT(temp_s0);
        KwloadtextureblockCI8B(img, Arg3, 1);
        SprDrawClip(temp_s6, var_s4, Arg3, 1, 1);
        var_s1 += temp_s7;
        img += Arg3;
        var_s4 += 1;
    }
}

UNUSED void func_8004E238(void) {
}

void SprDrawSubCI8(s32 arg0, s32 arg1, u8* tlut, u8* Texture, s32 arg4, s32 arg5, s32 arg6) {
    gSPDisplayList(DisplayListHead++, KWSpriteRGBAPtInit);
    InitcolpalCI8(tlut);
    SprDrawCI8(arg0, arg1, Texture, arg4, arg5, arg6);
}

void SprDrawSubCI8XLU(s32 arg0, s32 arg1, s32 Arg2, u8* tlut, u8* Texture, s32 arg5, s32 arg6, s32 arg7) {
    gSPDisplayList(DisplayListHead++, D_0D007DB8);
    SubAlphaMode(Arg2);
    InitcolpalCI8(tlut);
    SprDrawCI8(arg0, arg1, Texture, arg5, arg6, arg7);
}

void SprDrawSubCI8Yra(s32 arg0, s32 arg1, u8* tlut, u8* Texture, s32 arg4, s32 arg5) {
    gSPDisplayList(DisplayListHead++, D_0D007DB8);
    SubAlphaMode(D_8016589C);
    InitcolpalCI8(tlut);
    SprDrawCI8Yra(arg0, arg1, Texture, arg4, arg5);
}

UNUSED void func_8004E3B8(void) {
}

UNUSED void KwspriteCI8Pt(s32 arg0, s32 arg1, u8* tlut, u8* Texture, s32 arg4, s32 arg5, UNUSED s32 arg6, s32 arg7) {
    SprDrawSubCI8(arg0, arg1, tlut, Texture, arg4, arg5, arg7);
}

UNUSED void KwspriteCI8XLUPt(s32 arg0, s32 arg1, s32 Arg2, u8* tlut, u8* Texture, s32 arg5, s32 arg6, UNUSED s32 arg7,
                          s32 arg8) {
    SprDrawSubCI8XLU(arg0, arg1, Arg2, tlut, Texture, arg5, arg6, arg8);
}

UNUSED void Kwsprite8X128CI8(s32 arg0, s32 arg1, u8* tlut, u8* Texture) {
    SprDrawSubCI8(arg0, arg1, tlut, Texture, 8, 128, 128);
}

UNUSED void Kwsprite32X32CI8(s32 arg0, s32 arg1, u8* tlut, u8* Texture) {
    SprDrawSubCI8(arg0, arg1, tlut, Texture, 32, 32, 32);
}

UNUSED void Kwsprite32X64CI8(s32 arg0, s32 arg1, u8* tlut, u8* Texture) {
    SprDrawSubCI8(arg0, arg1, tlut, Texture, 32, 64, 64);
}

void Kwsprite40X32CI8(s32 arg0, s32 arg1, u8* tlut, u8* Texture) {
    SprDrawSubCI8(arg0, arg1, tlut, Texture, 40, 32, 32);
}

UNUSED void Kwsprite48X48CI8(s32 arg0, s32 arg1, u8* tlut, u8* Texture) {
    SprDrawSubCI8(arg0, arg1, tlut, Texture, 48, 48, 24);
}

UNUSED void Kwsprite64X32CI8(s32 arg0, s32 arg1, u8* tlut, u8* Texture) {
    SprDrawSubCI8(arg0, arg1, tlut, Texture, 64, 32, 32);
}

UNUSED void Kwsprite64X64CI8(s32 arg0, s32 arg1, u8* tlut, u8* Texture) {
    SprDrawSubCI8(arg0, arg1, tlut, Texture, 64, 64, 32);
}

UNUSED void Kwsprite64X64CI8XLU(s32 arg0, s32 arg1, s32 Arg2, u8* tlut, u8* Texture) {
    SprDrawSubCI8XLU(arg0, arg1, Arg2, tlut, Texture, 64, 64, 32);
}

UNUSED void Kwsprite64X64CI8Ra(s32 arg0, s32 arg1, u8* tlut, u8* Arg3) {
    SprDrawSubCI8Yra(arg0, arg1, tlut, Arg3, 64, 64);
}

UNUSED void Kwsprite320X240CI8(s32 arg0, s32 arg1, u8* tlut, u8* Texture) {
    SprDrawSubCI8(arg0, arg1, tlut, Texture, SCREEN_WIDTH, SCREEN_HEIGHT, 6);
}

void KWDisplayItembox(s32 PlayerId) {
    s32 ObjectIndex;
    AnmObject* object;
    Hud* temp_v0;

    ObjectIndex = ItemBoxAllocPtr[PlayerId];
    object = &DynamicObjects[ObjectIndex];
    if (object->Anmptr >= 2) {
        temp_v0 = &PlayerHUD[PlayerId];
        Kwsprite40X32CI8(temp_v0->Addix + temp_v0->ix, temp_v0->Addiy + temp_v0->iy,
                      (u8*) object->Texaddr, object->Indexaddr);
    }
}

void KWDisplayItemboxs(s32 PlayerId) {
    s32 ObjectIndex;
    AnmObject* object;
    Hud* temp_v0;

    ObjectIndex = ItemBoxAllocPtr[PlayerId];
    object = &DynamicObjects[ObjectIndex];
    if (object->Anmptr >= 2) {
        temp_v0 = &PlayerHUD[PlayerId];
        KWTexture2DCI8BL(temp_v0->Addix + temp_v0->ix, temp_v0->Addiy + temp_v0->iy, 0U,
                      temp_v0->is, (u8*) object->Texaddr, (u8*) object->Indexaddr, VtxKTile40X32,
                      0x00000028, 0x00000020, 0x00000028, 0x00000020);
    }
}

void KwdisplaySyuukai(s32 PlayerId) {
    Kwsprite32X8((s32) PlayerHUD[PlayerId].cx, PlayerHUD[PlayerId].cy + 3,
                             CommonTextureHUDLap);
    Kwsprite32X16(PlayerHUD[PlayerId].cx + 0x1C, (s32) PlayerHUD[PlayerId].cy,
                              LapList[PlayerHUD[PlayerId].Lapcnt]);
}

void KWDisplayRank(s32 PlayerId) {
    if (PlayerHUD[PlayerId].Ranksw != 0) {
        if (PlayerHUD[PlayerId].Goal != 3) {
            Kwtexture2DA4XLUBl(PlayerHUD[PlayerId].rx + PlayerHUD[PlayerId].rgx,
                          PlayerHUD[PlayerId].ry + PlayerHUD[PlayerId].rgy, 0U,
                          PlayerHUD[PlayerId].rs, 0x000000FF, D_800E55F8[KWRank[PlayerId]], 0, 0x000000FF,
                          common_texture_hud_place[KWRank[PlayerId]], D_0D0068F0, 0x00000080, 0x00000040,
                          0x00000080, 0x00000040);
        } else {
            Kwtexture2DA4XLUBl(PlayerHUD[PlayerId].rx + PlayerHUD[PlayerId].rgx,
                          PlayerHUD[PlayerId].ry + PlayerHUD[PlayerId].rgy, 0U,
                          PlayerHUD[PlayerId].rs, 0x000000FF, D_800E55F8[KW8GFTimer], 0, 0x000000FF,
                          common_texture_hud_place[GPCurrentRaceRankByPlayerId[PlayerId]], D_0D0068F0, 0x00000080,
                          0x00000040, 0x00000080, 0x00000040);
        }
    }
}

void KwdisplaySrank(s32 PlayerId) {
    if (PlayerHUD[PlayerId].Ranksw != 0) {
        if (PlayerHUD[PlayerId].Goal != 3) {
            Kwtexture2DA4XLUBl(PlayerHUD[PlayerId].rx + PlayerHUD[PlayerId].rgx,
                          PlayerHUD[PlayerId].ry + PlayerHUD[PlayerId].rgy, 0U,
                          PlayerHUD[PlayerId].rs, 0x000000FF,
                          D_800E5618[GPCurrentRaceRankByPlayerId[PlayerId]], 0, 0x000000FF,
                          D_0D015258[GPCurrentRaceRankByPlayerId[PlayerId]], D_0D006030, 0x00000040, 0x00000040,
                          0x00000040, 0x00000040);
        } else {
            Kwtexture2DA4XLUBl(PlayerHUD[PlayerId].rx + PlayerHUD[PlayerId].rgx,
                          PlayerHUD[PlayerId].ry + PlayerHUD[PlayerId].rgy, 0U,
                          PlayerHUD[PlayerId].rs, 0x000000FF, D_800E5618[KW4GFTimer], 0, 0x000000FF,
                          D_0D015258[GPCurrentRaceRankByPlayerId[PlayerId]], D_0D006030, 0x00000040, 0x00000040,
                          0x00000040, 0x00000040);
        }
    }
}

void func_8004EB30(UNUSED s32 arg0) {
}

void KwdisplayZanzou(s32 PlayerId) {
    Hud* temp_s0;

    temp_s0 = &PlayerHUD[PlayerId];
    if ((u8) temp_s0->Tptr2 != 0) {
        KWSpriteXLU(temp_s0->Lap1CompletionTimeX - 0x13, temp_s0->ty + 8, 0x00000080,
                      CommonTextureHUDTime, 0x00000020, 0x00000010, 0x00000020, 0x00000010);
        KWPrintLapTimeXLU((s32) temp_s0->Lap1CompletionTimeX, (s32) temp_s0->ty, 0x00000080, (s32) temp_s0->Totaltime);
    }
    if ((u8) temp_s0->Tptr3 != 0) {
        KWSpriteXLU(temp_s0->Lap2CompletionTimeX - 0x13, temp_s0->ty + 8, 0x00000050,
                      CommonTextureHUDTime, 0x00000020, 0x00000010, 0x00000020, 0x00000010);
        KWPrintLapTimeXLU((s32) temp_s0->Lap2CompletionTimeX, (s32) temp_s0->ty, 0x00000050, (s32) temp_s0->Totaltime);
    }
    if ((u8) temp_s0->Cptr2 != 0) {
        KWSpriteXLU((s32) temp_s0->cx2, temp_s0->cy + 3, 0x00000080, CommonTextureHUDLap,
                      0x00000020, 8, 0x00000020, 8);
        KWSpriteXLU(temp_s0->cx2 + 0x1C, (s32) temp_s0->cy, 0x00000080,
                      LapList[temp_s0->Lapcnt], 0x00000020, 0x00000010, 0x00000020, 0x00000010);
    }
    if ((u8) temp_s0->Cptr3 != 0) {
        KWSpriteXLU((s32) temp_s0->cx3, temp_s0->cy + 3, 0x00000050, CommonTextureHUDLap,
                      0x00000020, 8, 0x00000020, 8);
        KWSpriteXLU(temp_s0->cx3 + 0x1C, (s32) temp_s0->cy, 0x00000050,
                      LapList[temp_s0->Lapcnt], 0x00000020, 0x00000010, 0x00000020, 0x00000010);
    }
}

void KwdisplaySpeedmeter(s32 arg0) {
    Kwtexture2DA4XLUPt(PlayerHUD[arg0].mx, PlayerHUD[arg0].my, 0U, 1.0f, MapR, MapG,
                  MapB, 0xFF, CommonTextureSpeedometer, D_0D0064B0, 64, 96, 64, 48);
    Kwtexture2DA4Bl(D_8018CFEC, D_8018CFF4, D_8016579E, 1.0f, CommonTextureSpeedometerNeedle, D_0D005FF0, 0x40, 0x20,
                  0x40, 0x20);
}

void KwdisplayRadarmap(s32 arg0) {
    if (ScreenFlip != 0) {
        KwspriteA4XLUPtXflip(MapX[arg0] + Rzerox, MapY[arg0] + Rzeroy, (u8*) MapTexture,
                      (s32) MapR, (s32) MapG, (s32) MapB, 0x000000FF, (s32) MapWidth,
                      (s32) MapHeight, (s32) MapWidth, (s32) MapHeight);
    } else {
        KwspriteA4XLUPt(MapX[arg0] + Rzerox, MapY[arg0] + Rzeroy, (u8*) MapTexture,
                      (s32) MapR, (s32) MapG, (s32) MapB, 0x000000FF, (s32) MapWidth,
                      (s32) MapHeight, (s32) MapWidth, (s32) MapHeight);
    }
}

void KwdisplayKawanoradarmap(s32 arg0) {
    s16 temp_t0;
    s16 temp_v0;

    temp_v0 = D_800E5548[arg0 * 2];
    temp_t0 = D_800E5548[arg0 * 2 + 1];
    KwspriteA4XLUPt(0x00000104, 0x0000003C, D_8018D248[arg0], 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, temp_v0,
                  temp_t0, temp_v0, temp_t0);
}

void KwdisplayRadarsline(s32 arg0) {
    f32 var_f0;
    f32 var_f2;

    var_f2 = ((MapX[arg0] + Rzerox) - (MapWidth / 2)) + StartX;
    var_f0 = ((MapY[arg0] + Rzeroy) - (MapHeight / 2)) + StartY;
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) { /* irregular */
        case COURSE_MARIO_RACEWAY:
            var_f0 = var_f0 - 2.0;
            break;
        case COURSE_CHOCO_MOUNTAIN:
            var_f0 = var_f0 - 16.0;
            break;
        case COURSE_KALAMARI_DESERT:
            var_f0 = var_f0 + 4.0;
            break;
    }
    RadarFinishLine(var_f2, var_f0, CommonTextureMinimapFinishLine);
#else

#endif
}

void KwdisplayRadarplayerSub(s32 arg0, s32 PlayerId, s32 CharacterId) {
    f32 Thing0;
    f32 Thing1;
    s16 x;
    s16 y;
    Player *Player = &PlayerOne[PlayerId];

    if (Player->Flag & (1 << 15)) {
        Thing0 = HijackScaleMinimapX(Player->Position[0] * MapScale);
        Thing1 = HijackScaleMinimapY(Player->Position[2] * MapScale);
        x = ((MapX[arg0] + Rzerox) - (MapWidth / 2)) + StartX + (s16)(Thing0);
        y = ((MapY[arg0] + Rzeroy) - (MapHeight / 2)) + StartY + (s16)(Thing1);
        // huh?
        if (CharacterId != 8) {
            if ((GPCurrentRaceRankByPlayerId[PlayerId] == 0) && (GameMode != 3) && (GameMode != 1)) {
                Kwtexture2DFlashRGBAPt(x, y, Player->Direction[1] + DEGREES(180), 1.0f, &CommonTextureMinimapKartMario[CharacterId * 64], VtxKTile8X8, 8, 8, 8, 8);
            } else {
                KWTexture2DRGBA(x, y, Player->Direction[1] + DEGREES(180), 1.0f, &CommonTextureMinimapKartMario[CharacterId * 64], VtxKTile8X8, 8, 8, 8, 8);
            }
        } else {
            if (GPCurrentRaceRankByPlayerId[PlayerId] == 0) {
                KwspriteFlash(x, y, 8, 8, CommonTextureMinimapProgressDot);
            } else {
                KWSprite(x, y, 8, 8, CommonTextureMinimapProgressDot);
            }
        }
    }
}

// WTF is up with the PlayerOne access in this function?
void KwdisplayRadarplayer(s32 arg0) {
    UNUSED Player* Player;
    s32 PlayerId;
    s32 idx;

    switch (GameMode) { /* irregular */
        case GRAND_PRIX:
            for (idx = Kwkartnum - 1; idx >= 0; idx--) {
                PlayerId = RankToNum[idx];
                if ((PlayerOne + PlayerId)->Flag & IS_CPU_PLAYER) {
                    KwdisplayRadarplayerSub(arg0, PlayerId, 8);
                }
            }
            for (idx = Kwkartnum - 1; idx >= 0; idx--) {
                PlayerId = RankToNum[idx];
                if (((PlayerOne + PlayerId)->Flag & IS_CPU_PLAYER) != IS_CPU_PLAYER) {
                    KwdisplayRadarplayerSub(arg0, PlayerId, (PlayerOne + PlayerId)->Kart);
                }
            }
            break;
        case TIME_TRIALS:
            for (idx = 0; idx < 8; idx++) {
                if (((PlayerOne + idx)->Flag & IS_GHOST) == IS_GHOST) {
                    KwdisplayRadarplayerSub(arg0, idx, 8);
                }
            }
            KwdisplayRadarplayerSub(arg0, 0, PlayerOne->Kart);
            break;
        case VERSUS:
            for (idx = PlayerCount - 1; idx >= 0; idx--) {
                PlayerId = RankToNum[idx];
                KwdisplayRadarplayerSub(arg0, PlayerId, (PlayerOne + PlayerId)->Kart);
            }
            break;
        case BATTLE:
            for (idx = 0; idx < PlayerCount; idx++) {
                if (!((PlayerOne + idx)->Flag & IS_BOMB)) {
                    KwdisplayRadarplayerSub(arg0, idx, (PlayerOne + idx)->Kart);
                }
            }
            break;
    }
}

s32 Kwdiv(s32* arg0, s32 arg1) {
    s32 temp_v0;
    s32 ret;

    temp_v0 = *arg0;
    if (temp_v0 != 0) {
        ret = temp_v0 / arg1;
        *arg0 = temp_v0 % arg1;
    } else {
        *arg0 = 0;
        ret = 0;
    }
    return ret;
}

void Kwgettimer(s32 arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 sp24;

    sp24 = arg0;
    if (arg0 >= 599999) {
        sp24 = 599999;
    }
    D_801657D0[0] = Kwdiv(&sp24, 60000);
    D_801657D0[1] = Kwdiv(&sp24, 6000);
    D_801657D0[3] = Kwdiv(&sp24, 1000);
    D_801657D0[4] = Kwdiv(&sp24, 100);
    D_801657D0[6] = Kwdiv(&sp24, 10);
    D_801657D0[7] = sp24;
    D_801657D0[2] = 10;
    D_801657D0[5] = 11;
}

void KwprintlaptimeSub(s32 arg0, s32 arg1) {
    s32 i;
    s32 phi_s1 = arg0;

    for (i = 0; i < 8; i++) {
        SprDrawClipST(phi_s1, arg1, 8, 16, D_801657D0[i] * 8, 0, 0);
        phi_s1 += 8;
    }
}

void Kwprintlaptime(s32 arg0, s32 arg1, s32 Arg2) {
    gSPDisplayList(DisplayListHead++, D_0D008108);
    gSPDisplayList(DisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(DisplayListHead++, G_AC_THRESHOLD);
    KWLoadTextureBlockRGBA16B(CommonTextureHUDNormalDigit, 104, 16);
    Kwgettimer(Arg2);
    KwprintlaptimeSub(arg0, arg1);
    gSPDisplayList(DisplayListHead++, D_0D007EB8);
}

void KwprintlaptimeXLUSub(s32 arg0, s32 arg1) {
    s32 phi_s1 = arg0;
    s32 i;

    for (i = 0; i < 8; i++) {
        SprDrawClipST(phi_s1, arg1, 8, 16, D_801657D0[i] * 8, 0, 1);
        phi_s1 += 8;
    }
}

void KWPrintLapTimeXLU(s32 arg0, s32 arg1, s32 Arg2, s32 Arg3) {
    gSPDisplayList(DisplayListHead++, D_0D007F38);
    SubAlphaMode(Arg2);
    KWLoadTextureBlockRGBA16B(CommonTextureHUDNormalDigit, 104, 16);
    Kwgettimer(Arg3);
    KwprintlaptimeXLUSub(arg0, arg1);
}

void KwprintlaptimeFlash(s32 arg0, s32 arg1, s32 Arg2) {
    gSPDisplayList(DisplayListHead++, D_0D007F38);
    IceMode(KwflashR, KwflashG, KwflashB, 128, 128, 128, 255);
    KWLoadTextureBlockRGBA16B(CommonTextureHUDNormalDigit, 104, 16);
    Kwgettimer(Arg2);
    KwprintlaptimeXLUSub(arg0, arg1);
}

void KWDisplayTotalTime(s32 PlayerId) {
    s32 var_s0;

    if ((GameMode != 2) && (GameMode != 3)) {
        if (Kwgoalcnt == PlayerHUD[PlayerId].Goal) {
            if (ReplayFlag == 0) {
                for (var_s0 = 0; var_s0 < 3; var_s0++) {
                    if (D_80165658[var_s0] == 0) {
                        Kwprintlaptime(PlayerHUD[PlayerId].lapCompletionTimeXs[var_s0],
                                    PlayerHUD[PlayerId].ty + (var_s0 * 0x10),
                                    PlayerHUD[PlayerId].Laptimebuf[var_s0]);
                    } else {
                        KwprintlaptimeFlash(PlayerHUD[PlayerId].lapCompletionTimeXs[var_s0],
                                            PlayerHUD[PlayerId].ty + (var_s0 * 0x10),
                                            PlayerHUD[PlayerId].Laptimebuf[var_s0]);
                    }
                }
                Kwsprite32X16(PlayerHUD[PlayerId].totalTimeX - 0x13, PlayerHUD[PlayerId].ty + 0x38,
                                          CommonTextureHUDTotalTime);
                if (D_801657E5 != 0) {
                    KwprintlaptimeFlash(PlayerHUD[PlayerId].totalTimeX, PlayerHUD[PlayerId].ty + 0x30,
                                        PlayerHUD[PlayerId].Totaltime);
                } else {
                    Kwprintlaptime(PlayerHUD[PlayerId].totalTimeX, PlayerHUD[PlayerId].ty + 0x30,
                                PlayerHUD[PlayerId].Totaltime);
                }
            }
        } else {
            if (PlayerHUD[PlayerId].Flashtm == 0) {
                Kwsprite32X16(PlayerHUD[PlayerId].timerX - 0x13, PlayerHUD[PlayerId].ty + 8,
                                          CommonTextureHUDTime);
                Kwprintlaptime(PlayerHUD[PlayerId].timerX, PlayerHUD[PlayerId].ty, PlayerHUD[PlayerId].Totaltime);
            } else {
                Kwsprite32X16(PlayerHUD[PlayerId].timerX - 0x13, PlayerHUD[PlayerId].ty + 8,
                                          CommonTextureHUDLapTime);
                if (D_801657E3 != 0) {
                    KwprintlaptimeFlash(PlayerHUD[PlayerId].timerX, PlayerHUD[PlayerId].ty,
                                        PlayerHUD[PlayerId].Laptime);
                } else if (PlayerHUD[PlayerId].Dispsw == 0) {
                    Kwprintlaptime(PlayerHUD[PlayerId].timerX, PlayerHUD[PlayerId].ty, PlayerHUD[PlayerId].Laptime);
                }
            }
        }
    }
}

void KwdisplayLap88(s16 lapX, s16 lapY, s8 lap) {
    gSPDisplayList(DisplayListHead++, D_0D008108);
    gSPDisplayList(DisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(DisplayListHead++, G_AC_THRESHOLD);
    KWLoadTextureBlockRGBA16B(CommonTextureHUD123, 32, 8);
    // Display Current lap. Ex. 1/3

    SprDrawClipST(lapX, lapY, 8, 8, lap * 8, 0, 0); // display the digit
    SprDrawClipST(lapX + 8, lapY, 8, 8, 24, 0, 0);  // display the /
    SprDrawClipST(lapX + 16, lapY, 8, 8, 16, 0, 0); // display the 3
    gSPDisplayList(DisplayListHead++, D_0D007EB8);
}

void KwdisplayKaopanelSub(f32 arg0, f32 arg1, s16 Arg2, s16 Arg3, s16 CharacterId, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    if ((CourseID == COURSE_YOSHI_VALLEY) && (Arg3 < 3) && (arg8 == 0)) {
        Kwcreateaffine2D((s32) arg0, (s32) arg1, 0U, 1.0f);
        gSPDisplayList(DisplayListHead++, D_0D007DB8);
        ToumeIMode(0x000000FF, 0x000000FF, 0x000000FF, Kwa);
        gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTPortraitBombKartAndQuestionMark);
        KwloadtextureblockCI8B(CommonTexturePortraitQuestionMark, 0x00000020, 0x00000020);
        gSPDisplayList(DisplayListHead++, KWSetTile32x32);
    } else {
        Kwcreateaffine2D(arg0, arg1, 0U, 1.0f);
        gSPDisplayList(DisplayListHead++, D_0D007DB8);
        ToumeIMode(0x000000FF, 0x000000FF, 0x000000FF, arg5);
        gDPLoadTLUT_pal256(DisplayListHead++, KaoColpalList[CharacterId]);
        KwloadtextureblockCI8B(KaoIndexList[CharacterId], 0x00000020, 0x00000020);
        if (arg7 != 0) {
            gSPDisplayList(DisplayListHead++, KWSetTile32x32XFlip);
        } else {
            gSPDisplayList(DisplayListHead++, KWSetTile32x32);
        }
        if (arg6 != 0) {
            Kwcreateaffine2D(arg0, arg1, 0U, 1.0f);
            gSPDisplayList(DisplayListHead++, D_0D007A60);
            ToumeIMode(Wakur, Wakug, Wakub, 0x000000FF);
            KwloadtextureblockIA4B(CommonTextureCharacterPortraitBorder, 0x20, 0x20);
            gSPDisplayList(DisplayListHead++, KWSetTile32x32);
        }
        gSPDisplayList(DisplayListHead++, D_0D007DB8);
        ToumeIMode(0x000000FF, 0x000000FF, 0x000000FF, arg5);
        gSPDisplayList(DisplayListHead++, KWSpriteRGBAPtInit);
        gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTHUDTypeCRankFont);
        KwloadtextureblockCI8B(common_texture_hud_type_C_rank_font[Arg2], 0x00000010, 0x00000010);
        if (arg7 != 0) {
            Kwcreateaffine2D((s32) (arg0 + 9.0f), (s32) (arg1 + 7.0f), 0U, 1.0f);
        } else {
            Kwcreateaffine2D((s32) (arg0 - 9.0f), (s32) (arg1 + 7.0f), 0U, 1.0f);
        }
        gSPDisplayList(DisplayListHead++, KWSetTile16x16);
    }
}

void KwdisplayKaopanel(void) {
    s16 temp_v0;
    s16 CharacterId;
    s32 var_s0;
    s32 lapCount;
    s32 var_a0;

    if (Kwgoalpanelsw == 0) {
        for (var_s0 = 0; var_s0 < 4; var_s0++) {
            var_a0 = 0;
            if (HUDCharpicRankY1[var_s0] >= 0.0f) {
                if (D_8018D078[var_s0] < 0.0) {
                    var_a0 = 1;
                }
                temp_v0 = RankToNum[var_s0];
                CharacterId = GPCurrentRaceCharacterIdByRank[var_s0];
                lapCount = LapCountByPlayerId[temp_v0];
                if (CharacterId == PlayerOne->Kart) {
                    KwdisplayKaopanelSub(Panelx[var_s0], HUDCharpicRankY1[var_s0], var_s0, lapCount, CharacterId, 0x000000FF, 1,
                                  var_a0, 0);
                } else {
                    KwdisplayKaopanelSub(Panelx[var_s0], HUDCharpicRankY1[var_s0], var_s0, lapCount, CharacterId, Kwa, 0,
                                  var_a0, 0);
                }
            }
        }
    } else {
        for (var_s0 = 0; var_s0 < 8; var_s0++) {
            var_a0 = 0;
            if (HUDCharpicRankY1[var_s0] >= 0.0f) {
                if (D_8018D078[var_s0] <= 0.0) {
                    var_a0 = 1;
                }
                temp_v0 = RankToNum[var_s0];
                // ????
                CharacterId = (PlayerOne + temp_v0)->Kart;
                lapCount = LapCountByPlayerId[temp_v0];
                if (temp_v0 == 0) {
                    KwdisplayKaopanelSub(Panelx[var_s0], HUDCharpicRankY1[var_s0], var_s0, lapCount, CharacterId, 0x000000FF, 1,
                                  var_a0, 1);
                } else {
                    KwdisplayKaopanelSub(Panelx[var_s0], HUDCharpicRankY1[var_s0], var_s0, lapCount, CharacterId, 0x000000FF, 0,
                                  var_a0, 1);
                }
            }
        }
    }
    gSPTexture(DisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}

s32 KwcalcEnemycarZahyou(u16 arg0, s32* arg1, s32* Arg2) {
    s32 var_v0 = 0;
    s32 thing = 0;
    s32 test = LapCountByPlayerId[arg0];

    if (test < 3) {
        if (PlayerCount == 1) {
            if (test >= 0) {
                thing = (s32) (RateCountF[arg0] * 928);
            }
            if (thing < 0x104) {
                *arg1 = thing;
                *Arg2 = 0;
                var_v0 = 1;
            } else if (thing < 0x1D0) {
                *arg1 = 0x00000104;
                *Arg2 = thing - 0x104;
                var_v0 = 2;
            } else if (thing < 0x2D4) {
                *arg1 = 0x2D4 - thing;
                *Arg2 = 0x000000CC;
                var_v0 = 3;
            } else {
                *arg1 = 0;
                *Arg2 = 0x3A0 - thing;
                var_v0 = 4;
            }
        } else {
            if (test >= 0) {
                thing = (s32) (RateCountF[arg0] * 260);
            }
            *arg1 = thing;
            *Arg2 = 0;
        }
    } else if (PlayerCount == 1) {
        *arg1 = 0x00000020;
        *Arg2 = (GPCurrentRaceRankByPlayerId[arg0] * 0x14) + 0x20;
    } else {
        thing = (s32) (RateCountF[arg0] * 260);
        *arg1 = thing;
        *Arg2 = 0;
    }
    return var_v0;
}

void KwcalcBombkartZahyou(u16 bombIndex, s32* arg1, s32* Arg2) {
    s32 temp_v0 = Bom[bombIndex].PathPointIndex;
    s32 var_v1 = 0;

    if (temp_v0 != 0) {
        var_v1 = (s32) (temp_v0 * 0x3A0) / (s32) CenterLineMax;
    }
    if (var_v1 < 0x104) {
        *arg1 = var_v1;
        *Arg2 = 0;
    } else if (var_v1 < 0x1D0) {
        *arg1 = 0x00000104;
        *Arg2 = var_v1 - 0x104;
    } else if (var_v1 < 0x2D4) {
        *arg1 = 0x2D4 - var_v1;
        *Arg2 = 0x000000CC;
    } else {
        *arg1 = 0;
        *Arg2 = 0x3A0 - var_v1;
    }
}

void KwdisplayRoad(void) {
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 temp_v1;
    s16 var_s0;
    UNUSED s16 stackPadding;
    u16 var_s0_2;
    u16 var_s1;
    u16 var_s2;

    if (GameMode == TIME_TRIALS) {
        var_s0 = RealRankToNum[0];
    } else {
        var_s0 = RankToNum[0];
    }
    sp4C = KwcalcEnemycarZahyou(var_s0, &sp54, &sp50);
    temp_v1 = LapCountByPlayerId[var_s0];
    if (temp_v1 > 0) {
        if (temp_v1 == 1) {
            var_s0_2 = 0;
            var_s1 = 0;
            var_s2 = 0x000000FF;
        } else {
            if (temp_v1 == 2) {
                var_s0_2 = 0x00FF;
                var_s1 = 0x000000FF;
                var_s2 = 0;
            } else {
                var_s0_2 = 0x00FF;
                var_s1 = 0;
                var_s2 = 0;
            }
        }
        DrawLineHorizontal(0x0020, 0x0012, 0x0104, var_s0_2, var_s1, var_s2, 0x000000FF);
        DrawLineVertical(0x0124, 0x0012, 0x00CC, var_s0_2, var_s1, var_s2, 0x000000FF);
        DrawLineHorizontal(0x0020, 0x00DE, 0x0104, var_s0_2, var_s1, var_s2, 0x000000FF);
        DrawLineVertical(0x0020, 0x0012, 0x00CC, var_s0_2, var_s1, var_s2, 0x000000FF);
    }
    if ((temp_v1 < 0) || (temp_v1 >= 3)) {
        return;
    }
    switch (temp_v1) {
        case 0:
            var_s0_2 = 0;
            var_s1 = 0;
            var_s2 = 0x00FF;
            break;
        case 1:
            var_s0_2 = 0x00FF;
            var_s1 = 0x00FF;
            var_s2 = 0;
            break;
        case 2:
            var_s0_2 = 0x00FF;
            var_s1 = 0;
            var_s2 = 0;
            break;
        default:
            break;
    }
    switch (sp4C) {
        case 1:
            DrawLineHorizontal(0x0020, 0x0012, sp54, var_s0_2, var_s1, var_s2, 0x000000FF);
            break;
        case 2:
            DrawLineHorizontal(0x0020, 0x0012, 0x0104, var_s0_2, var_s1, var_s2, 0x000000FF);
            DrawLineVertical(0x0124, 0x0012, sp50, var_s0_2, var_s1, var_s2, 0x000000FF);
            break;
        case 3:
            DrawLineHorizontal(0x0020, 0x0012, 0x0104, var_s0_2, var_s1, var_s2, 0x000000FF);
            DrawLineVertical(0x0124, 0x0012, 0x00CC, var_s0_2, var_s1, var_s2, 0x000000FF);
            DrawLineHorizontal(sp54 + 0x20, 0x00DE, 0x104 - sp54, var_s0_2, var_s1, var_s2, 0x000000FF);
            break;
        case 4:
            DrawLineHorizontal(0x0020, 0x0012, 0x0104, var_s0_2, var_s1, var_s2, 0x000000FF);
            DrawLineVertical(0x0124, 0x0012, 0x00CC, var_s0_2, var_s1, var_s2, 0x000000FF);
            DrawLineHorizontal(0x0020, 0x00DE, 0x0104, var_s0_2, var_s1, var_s2, 0x000000FF);
            DrawLineVertical(0x0020, sp50 + 0x12, 0xCC - sp50, var_s0_2, var_s1, var_s2, 0x000000FF);
            break;
        case 0:
        default:
            break;
    }
}

void KwdisplayBombpanel(void) {
    UNUSED s32 stackPadding0;
    s32 sp88;
    s32 sp84;
    UNUSED s32 stackPadding1;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < NUM_BOMB_KARTS_VERSUS; var_s1++) {
        if ((Bom[var_s1].State != BOMB_STATE_EXPLODED) && (Bom[var_s1].State != BOMB_STATE_INACTIVE)) {
            KwcalcBombkartZahyou(var_s1, &sp88, &sp84);
            gSPDisplayList(DisplayListHead++, D_0D007DB8);
            gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTPortraitBombKartAndQuestionMark);
            KwloadtextureblockCI8B(CommonTexturePortraitBombKart, 0x00000020, 0x00000020);
            Kwcreateaffine2D(sp88 + 0x20, sp84 + 0x12, 0U, 0.6f);
            gSPDisplayList(DisplayListHead++, KWSetTile32x32);
        }
    }
}

void KwdisplayEnemycarSub(s32 PlayerId, s32 arg1) {
    s32 ObjectIndex;
    s32 spD0;
    s32 spCC;
    Player *Dummy = &PlayerOne[PlayerId];
    s32 spC4;
    s32 lapCount;
    s32 CharacterId;
    s32 spB8;
    s32 temp_v0_2;
    AnmObject* object;
    Player *Player = &PlayerOne[PlayerId];

    lapCount = LapCountByPlayerId[PlayerId];
    CharacterId = Player->Kart;
    ObjectIndex = D_8018CE10[PlayerId].ObjectIndex;

    if (PlayerCount == 1) {
        spC4 = 0x00000012;
    } else {
        spC4 = 0x00000078;
    }

    temp_v0_2 = KwcalcEnemycarZahyou(PlayerId, &spD0, &spCC);
    if ((temp_v0_2 == 2) || (temp_v0_2 == 3)) {
        spB8 = 1;
    } else {
        spB8 = 0;
    }

    if ((CourseID == COURSE_YOSHI_VALLEY) && (lapCount < 3)) {
        gSPDisplayList(DisplayListHead++, D_0D007DB8);
        gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTPortraitBombKartAndQuestionMark);
        KwloadtextureblockCI8B(CommonTexturePortraitQuestionMark, 0x00000020, 0x00000020);
        object = &DynamicObjects[ObjectIndex];
        object->pos[0] = object->PosSpline[0] + ((f32) (spD0 + 0x20));
        object->pos[1] = object->PosSpline[1] + ((f32) (spC4 + spCC));
        object->pos[2] = object->PosSpline[2];
        KWCreateAffine3D(object->pos, object->ang, object->Scale);
        gSPDisplayList(DisplayListHead++, KWSetTile32x32);
    } else {
        gDPLoadTLUT_pal256(DisplayListHead++, KaoColpalList[CharacterId]);
        gSPDisplayList(DisplayListHead++, D_0D007DB8);
        if (Player->SlipFlag & STAR) {
            IceMode((s32) KwflashR, (s32) KwflashG, (s32) KwflashB, 0x00000080, 0x00000080, 0x00000080,
                          (s32) DynamicObjects[ObjectIndex].Alpha);
        } else {
            SubAlphaMode((s32) DynamicObjects[ObjectIndex].Alpha);
        }
        KwloadtextureblockCI8B(KaoIndexList[CharacterId], 0x00000020, 0x00000020);
        object = &DynamicObjects[ObjectIndex];
        object->pos[0] = object->PosSpline[0] + ((f32) (spD0 + 0x20));
        object->pos[1] = object->PosSpline[1] + ((f32) (spC4 + spCC));
        object->pos[2] = object->PosSpline[2];
        KWCreateAffine3D(object->pos, object->ang, object->Scale);
        if (spB8 != 0) {
            gSPDisplayList(DisplayListHead++, KWSetTile32x32XFlip);
        } else {
            gSPDisplayList(DisplayListHead++, KWSetTile32x32);
        }
        gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTHUDTypeCRankTinyFont);
        KwloadtextureblockCI8B(common_texture_hud_type_C_rank_tiny_font[arg1 + 1], 8, 8);
        if (spB8 != 0) {
            Kwcreateaffine2D(spD0 + 0x26, (spC4 + spCC) + 4, 0U, 1.0f);
        } else {
            Kwcreateaffine2D(spD0 + 0x1B, (spC4 + spCC) + 4, 0U, 1.0f);
        }
        gSPDisplayList(DisplayListHead++, KWSetTile8x8);
        if ((Player == PlayerOne) && (ScreenSplitB == SCREEN_MODE_1P)) {
            gSPDisplayList(DisplayListHead++, D_0D007A40);
            ToumeIMode(Wakur, Wakug, Wakub, 0x000000FF);
            KwloadtextureblockIA4B(CommonTextureCharacterPortraitBorder, 0x00000020, 0x00000020);
            KWCreateAffine3D(object->pos, object->ang, object->Scale);
            gSPDisplayList(DisplayListHead++, KWSetTile32x32);
        }
    }
}

void KwdisplayEnemycar(void) {
    s32 temp_a0;
    s32 var_s0;
    s32 var_s1;
    s32 var_s3;
    Player* Player;

    if (ScreenSplitB == 0) {
        KwdisplayRoad();
    }
    var_s3 = 8;
    if ((PlayerCount == 2) && (ScreenSplitA == 2)) {
        var_s3 = 0;
    }
    for (var_s0 = var_s3 - 1, var_s1 = 0; var_s1 < var_s3; var_s1++, var_s0--) {
        temp_a0 = RankToNum[var_s0];
        Player = &PlayerOne[temp_a0];
        if ((Player->Flag & EXISTS) && ((temp_a0 != 0) || (PlayerCount != 1))) {
            KwdisplayEnemycarSub(temp_a0, var_s0);
        }
    }
    if (GameMode == 1) {
        KwdisplayEnemycarSub(0, RealRank[0]);
    } else if (PlayerCount == 1) {
        KwdisplayEnemycarSub(0, GPCurrentRaceRankByPlayerId[0]);
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KwdisplayLeaf(UNUSED s32 CameraId) {
    s32 someIndex;
    s32 leafIndex;
    AnmObject* object;

    gSPDisplayList(DisplayListHead++, KWTextureRGBAXLUAAZBBlInit);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BOTH);
    KWLoadTextureBlockRGBA16B(CommonTextureParticleLeaf, 0x00000020, 0x00000010);
    for (someIndex = 0; someIndex < gLeafParticle_SIZE; someIndex++) {
        leafIndex = Leafallocptr[someIndex];
        if (leafIndex != -1) {
            object = &DynamicObjects[leafIndex];
            if ((object->Anmptr >= 2) && (object->Chartptr == 7) && (Modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
                KWCreateAffine3DAnm(leafIndex);
                gSPDisplayList(DisplayListHead++, D_0D0069C8);
            }
        }
    }
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KwdisplaySnow(void) {
    s32 someIndex;
    s32 snowflakeIndex;

    gSPDisplayList(DisplayListHead++, D_0D007AE0);
    gDPSetCombineLERP(DisplayListHead++, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0);
    KwloadtextureblockA4B((HotSwapID > 0) ? (u8*) g_SnowParticleTex : D_0D0293D8, 0x10, 0x10);
    for (someIndex = 0; someIndex < NUM_SNOWFLAKES; someIndex++) {
        snowflakeIndex = EffectAllocArray1[someIndex];
        if (DynamicObjects[snowflakeIndex].Anmptr >= 2) {
            KWCreateAffine3DAnm(snowflakeIndex);
            gSPDisplayList(DisplayListHead++, KWSetTile16x16);
        }
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KwdisplayKumoDraw(s32 ObjectIndex, s16 arg1, s16 Arg2) {
    UNUSED s32 pad[1];
    if (DynamicObjects[ObjectIndex].Flag & 0x10) {
        if (Kumotype != DynamicObjects[ObjectIndex].Chartptr) {
            Kumotype = DynamicObjects[ObjectIndex].Chartptr;
            KWLoadTextureBlockI4b(DynamicObjects[ObjectIndex].Indexaddr, DynamicObjects[ObjectIndex].Texsizey,
                          DynamicObjects[ObjectIndex].Fvalptr);
        }
        Kwcreateaffine2D(arg1, Arg2, 0U, DynamicObjects[ObjectIndex].Scale);
        gSPVertex(DisplayListHead++, DynamicObjects[ObjectIndex].Vtxaddr, 4, 0);
        gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
    }
}

void KwdisplayStarDraw(s32 ObjectIndex, s16 arg1, s16 Arg2) {
    if (DynamicObjects[ObjectIndex].Flag & 0x10) {
        if (Kumotype != DynamicObjects[ObjectIndex].Chartptr) {
            Kumotype = DynamicObjects[ObjectIndex].Chartptr;
            KWLoadTextureBlockI4b(DynamicObjects[ObjectIndex].Indexaddr, DynamicObjects[ObjectIndex].Texsizey,
                          DynamicObjects[ObjectIndex].Fvalptr);
        }
        KWSetPrimColor(0x000000FF, 0x000000FF, 0x000000FF, DynamicObjects[ObjectIndex].Alpha);
        Kwcreateaffine2D(arg1, Arg2, 0U, DynamicObjects[ObjectIndex].Scale);
        gSPVertex(DisplayListHead++, DynamicObjects[ObjectIndex].Vtxaddr, 4, 0);
        gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
    }
}

void KwdisplayKumoSub(s16 arg0, s32 arg1) {
    s32 var_s0;
    s32 ObjectIndex;
    AnmObject* object;

    Kumotype = 0xFF;
    gSPDisplayList(DisplayListHead++, D_0D007A60);
    if ((u8) Kwhoshisw != 0) {
        SplashMode(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
        for (var_s0 = 0; var_s0 < CloudCount1P; var_s0++) {
            ObjectIndex = CloudAllocate[arg1 + var_s0];
            object = &DynamicObjects[ObjectIndex];
            KwdisplayStarDraw(ObjectIndex, object->ix, arg0 - object->iy);
        }
    } else {
        KumoColorMode(0x000000FF, 0x000000FF, 0x000000FF);
        for (var_s0 = 0; var_s0 < CloudCount1P; var_s0++) {
            ObjectIndex = CloudAllocate[arg1 + var_s0];
            object = &DynamicObjects[ObjectIndex];
            KwdisplayKumoDraw(ObjectIndex, object->ix, arg0 - object->iy);
        }
    }
}

void KwdisplayKumo2PudSub(s16 arg0, s32 arg1) {
    s16 var_s5;
    s32 var_s0;
    s32 ObjectIndex;
    AnmObject* object;

    if (D_801658FE == 0) {
        if (CourseID == COURSE_KOOPA_BEACH) {
            var_s5 = arg0;
        } else if (CourseID == COURSE_MOO_MOO_FARM) {
            var_s5 = arg0 - 0x10;
        } else if (CourseID == COURSE_YOSHI_VALLEY) {
            var_s5 = arg0 - 0x10;
        } else {
            var_s5 = arg0 + 0x10;
        }
    } else if (CourseID == COURSE_KOOPA_BEACH) {
        var_s5 = arg0 * 2;
    } else {
        var_s5 = arg0 + 0x20;
    }
    Kumotype = 0xFF;
    gSPDisplayList(DisplayListHead++, D_0D007A60);
    if ((u8) Kwhoshisw != 0) {
        SplashMode(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
        for (var_s0 = 0; var_s0 < CloudCount1P; var_s0++) {
            ObjectIndex = CloudAllocate[arg1 + var_s0];
            object = &DynamicObjects[ObjectIndex];
            KwdisplayStarDraw(ObjectIndex, object->ix, (var_s5 - object->iy) / 2);
        }
    } else {
        KumoColorMode(0x000000FF, 0x000000FF, 0x000000FF);
        for (var_s0 = 0; var_s0 < CloudCount1P; var_s0++) {
            ObjectIndex = CloudAllocate[arg1 + var_s0];
            object = &DynamicObjects[ObjectIndex];
            KwdisplayKumoDraw(ObjectIndex, object->ix, (var_s5 - object->iy) / 2);
        }
    }
}

void KwdisplayKumo1P(void) {
    KwdisplayKumoSub(240 - D_800DC5EC->Horizon, 0); // 28
}

void KwdisplayKumo2Pl(void) {
    s16 temp_a0;

    temp_a0 = 0xF0 - D_800DC5EC->Horizon;
    if (CourseID == COURSE_KOOPA_BEACH) {
        temp_a0 = temp_a0 - 0x30;
    } else if (CourseID == COURSE_MOO_MOO_FARM) {
        temp_a0 = temp_a0 - 0x40;
    } else if (CourseID == COURSE_YOSHI_VALLEY) {
        temp_a0 = temp_a0 - 0x40;
    } else {
        temp_a0 = temp_a0 - 0x30;
    }
    KwdisplayKumoSub(temp_a0, 0);
}

void KwdisplayKumo2Pr(void) {
    s16 temp_a0;

    temp_a0 = 0xF0 - D_800DC5F0->Horizon;
    if (CourseID == COURSE_KOOPA_BEACH) {
        temp_a0 = temp_a0 - 0x30;
    } else if (CourseID == COURSE_MOO_MOO_FARM) {
        temp_a0 = temp_a0 - 0x40;
    } else if (CourseID == COURSE_YOSHI_VALLEY) {
        temp_a0 = temp_a0 - 0x40;
    } else {
        temp_a0 = temp_a0 - 0x30;
    }
    KwdisplayKumoSub(temp_a0, CloudCount1P);
}

void KwdisplayKumo2Pu(void) {
    KwdisplayKumo2PudSub(240 - D_800DC5EC->Horizon, 0);
}

void KwdisplayKumo2Pd(void) {
    KwdisplayKumo2PudSub(240 - D_800DC5F0->Horizon, CloudCount1P);
}

void KwsetPukupukuLight(s32 arg0) {
    if (DynamicObjects[arg0].Chartptr == 0) {
        PukupukuMaterial[0].l[0].l.dir[0] = PukupukuMaterial[1].l[0].l.dir[0] = PukupukuMaterial[2].l[0].l.dir[0] =
            PukupukuMaterial[3].l[0].l.dir[0] = 0;
        PukupukuMaterial[0].l[0].l.dir[1] = PukupukuMaterial[1].l[0].l.dir[1] = PukupukuMaterial[2].l[0].l.dir[1] =
            PukupukuMaterial[3].l[0].l.dir[1] = -0x78;
        PukupukuMaterial[0].l[0].l.dir[2] = PukupukuMaterial[1].l[0].l.dir[2] = PukupukuMaterial[2].l[0].l.dir[2] =
            PukupukuMaterial[3].l[0].l.dir[2] = 0;
    } else {
        PukupukuMaterial[0].l[0].l.dir[0] = PukupukuMaterial[1].l[0].l.dir[0] = PukupukuMaterial[2].l[0].l.dir[0] =
            PukupukuMaterial[3].l[0].l.dir[0] = 0x63;
        PukupukuMaterial[0].l[0].l.dir[1] = PukupukuMaterial[1].l[0].l.dir[1] = PukupukuMaterial[2].l[0].l.dir[1] =
            PukupukuMaterial[3].l[0].l.dir[1] = 0x42;
        PukupukuMaterial[0].l[0].l.dir[2] = PukupukuMaterial[1].l[0].l.dir[2] = PukupukuMaterial[2].l[0].l.dir[2] =
            PukupukuMaterial[3].l[0].l.dir[2] = 0;
    }
}

void KwdisplayPukupuku(UNUSED s32 arg0) {
    AnmObject* object;
    s32 temp_a3;

    temp_a3 = Objallocptr2[0];
    object = &DynamicObjects[temp_a3];
    if (object->Anmptr >= 2) {
        if (KwanmBitcheckflag(temp_a3, 0x00000010) != 0) {
            KWCreateAffine3D(object->pos, object->ang, object->Scale);
            KwsetPukupukuLight(temp_a3);

            gSPDisplayList(DisplayListHead++, D_0D007828);
            gSPLight(DisplayListHead++, &PukupukuMaterial[0].l[0], LIGHT_1);
            gSPLight(DisplayListHead++, &PukupukuMaterial[0].a, LIGHT_2);
            gSPDisplayList(DisplayListHead++, d_course_banshee_boardwalk_dl_7B38);
            gSPLight(DisplayListHead++, &PukupukuMaterial[1].l[0], LIGHT_1);
            gSPLight(DisplayListHead++, &PukupukuMaterial[1].a, LIGHT_2);
            gSPDisplayList(DisplayListHead++, d_course_banshee_boardwalk_dl_7978);
            gSPLight(DisplayListHead++, &PukupukuMaterial[2].l[0], LIGHT_1);
            gSPLight(DisplayListHead++, &PukupukuMaterial[2].a, LIGHT_2);
            gSPDisplayList(DisplayListHead++, d_course_banshee_boardwalk_dl_78C0);
            gSPLight(DisplayListHead++, &PukupukuMaterial[3].l[0], LIGHT_1);
            gSPLight(DisplayListHead++, &PukupukuMaterial[3].a, LIGHT_2);
            gSPDisplayList(DisplayListHead++, d_course_banshee_boardwalk_dl_7650);
        }
    }
}

void KwdisplayTellesaSub(s32 ObjectIndex, s32 arg1, u32 Arg2) {
    UNUSED s32 pad[2];
    AnmObject* object;
    Camera* Camera = &Camera1[arg1];

    object = &DynamicObjects[ObjectIndex];
    object->Texang[1] = KWLookCamera(object->pos[0], object->pos[2], Camera->CameraPos);
    Kwtexture3DCI8XLUZBBl(object->pos, object->Texang, object->Scale, object->Alpha, (u8*) object->Texaddr,
                  object->Indexaddr, object->Vtxaddr, 0x00000030, 0x00000028, 0x00000030, 0x00000028);
    if ((KwanmBitcheckflag(ObjectIndex, 0x00000020) != 0) && (Arg2 < 0x15F91U)) {
        KwdrawKage3D(&D_8018C830, object->pos, 0.4f);
    }
}

void KwdisplayTellesa(s32 arg0) {
    u32 temp_s2;
    s32 someIndex;
    s32 ObjectIndex;

    for (someIndex = 0; someIndex < NUM_BOOS; someIndex++) {
        ObjectIndex = Objallocptr3[someIndex];
        if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
            temp_s2 = KwcheckVisArea(ObjectIndex, arg0, 0x4000U, 0x00000320);
            if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
                KwdisplayTellesaSub(ObjectIndex, arg0, temp_s2);
            }
        }
    }
}

void KwdisplayBat(s32 CameraId) {
    s32 var_s2;
    s32 ObjectIndex;
    Camera* temp_s7;

    ObjectIndex = Objallocptr[0];
    temp_s7 = &Camera1[CameraId];
    KwloadtextureCI8AAZBBlMr(DynamicObjects[ObjectIndex].Texaddr, DynamicObjects[ObjectIndex].Indexaddr, 0x00000020, 0x00000040,
                  5);
    D_80183E80[0] = DynamicObjects[ObjectIndex].Texang[0];
    D_80183E80[2] = DynamicObjects[ObjectIndex].Texang[2];
    if ((D_8018CFB0 != 0) || (D_8018CFC8 != 0)) {
        for (var_s2 = 0; var_s2 < 40; var_s2++) {
            ObjectIndex = EffectAllocArray2[var_s2];
            if (ObjectIndex == -1) {
                continue;
            }

            if ((DynamicObjects[ObjectIndex].Anmptr >= 2) && (Modelingnum < 0x2EF)) {
                D_80183E80[1] =
                    KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], temp_s7->CameraPos);
                KwsetTile(DynamicObjects[ObjectIndex].pos, D_80183E80, DynamicObjects[ObjectIndex].Scale,
                              D_0D0062B0);
            }
        }
    }
    if ((D_8018CFE8 != 0) || (D_8018D000 != 0)) {
        for (var_s2 = 0; var_s2 < 30; var_s2++) {
            ObjectIndex = EffectAllocArray3[var_s2];
            if (ObjectIndex == -1) {
                continue;
            }

            if ((DynamicObjects[ObjectIndex].Anmptr >= 2) && (Modelingnum < 0x2EF)) {
                D_80183E80[1] =
                    KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], temp_s7->CameraPos);
                KwsetTile(DynamicObjects[ObjectIndex].pos, D_80183E80, DynamicObjects[ObjectIndex].Scale,
                              D_0D0062B0);
            }
        }
    }
    gSPTexture(DisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}

void KwdisplayKanoke(s32 CameraId) {
    s32 ObjectIndex;
    AnmObject* object;

    ObjectIndex = Objallocptr[1];
    KwcheckVisArea(ObjectIndex, CameraId, 0x5555U, 0x00000320);
    if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
        object = &DynamicObjects[ObjectIndex];
        if (object->Anmptr >= 2) {
            Kwpolygon3D(object->pos, object->Texang, object->Scale, object->Shape);
        }
    }
}

void KwdisplayIceblockSub(s32 arg0) {
    Player* temp_v0;

    temp_v0 = &PlayerOne[arg0];
    D_80183E40[0] = temp_v0->Position[0];
    D_80183E40[1] = temp_v0->Position[1];
    D_80183E40[2] = temp_v0->Position[2];
    D_80183E80[0] = 0;
    D_80183E80[1] = 0;
    D_80183E80[2] = 0;
    Kwpolygon3DIceblock(D_80183E40, D_80183E80, 0.02f, IceKageGfx);
}

void KwdisplayIceblockKakera(s32 arg0) {
    s32 var_s3;
    s32 ObjectIndex;
    AnmObject* object;

    D_80183E80[0] = D_8016582C[0];
    D_80183E80[1] = D_8016582C[1];
    D_80183E80[2] = D_8016582C[2];
    gSPDisplayList(DisplayListHead++, D_0D007B00);
    gSPNumLights(DisplayListHead++, 1);
    gSPLight(DisplayListHead++, &Iceblock3Material.l[0], LIGHT_1);
    gSPLight(DisplayListHead++, &Iceblock3Material.a, LIGHT_2);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BOTH);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_LIGHTING | G_SHADING_SMOOTH);
    KwloadtextureblockIA16B(d_course_sherbet_land_ice, 0x00000020, 0x00000020);
    if (PlayerCount < 3) {
        for (var_s3 = 0; var_s3 < gObjectParticle2_SIZE; var_s3++) {
            ObjectIndex = EffectAllocArray2[var_s3];
            if (ObjectIndex != NULL_OBJECT_ID) {
                object = &DynamicObjects[ObjectIndex];
                if (object->Anmptr > 0) {
                    KWCreateAffine3D(object->pos, D_80183E80, object->Scale);
                    gSPVertex(DisplayListHead++, D_0D005BD0, 3, 0);
                    gSPDisplayList(DisplayListHead++, D_0D006930);
                }
            }
        }
    } else {
        for (var_s3 = 0; var_s3 < gObjectParticle2_SIZE; var_s3++) {
            ObjectIndex = EffectAllocArray2[var_s3];
            if (ObjectIndex != NULL_OBJECT_ID) {
                object = &DynamicObjects[ObjectIndex];
                if ((object->Anmptr > 0) && (arg0 == object->Stack[7]) && (Modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
                    KWCreateAffine3D(object->pos, D_80183E80, object->Scale);
                    gSPVertex(DisplayListHead++, D_0D005BD0, 3, 0);
                    gSPDisplayList(DisplayListHead++, D_0D006930);
                }
            }
        }
    }
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gSPTexture(DisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}

void KWDisplayIceBlock(s32 arg0) {
    s32 PlayerId;
    s32 ObjectIndex;

    Iceblock3Material.l[0].l.dir[0] = D_80165840[0];
    Iceblock3Material.l[0].l.dir[1] = D_80165840[1];
    Iceblock3Material.l[0].l.dir[2] = D_80165840[2];
    gSPLight(DisplayListHead++, &Iceblock3Material.l[0], LIGHT_1);
    gSPLight(DisplayListHead++, &Iceblock3Material.a, LIGHT_2);
    for (PlayerId = 0; PlayerId < PlayerCount; PlayerId++) {
        ObjectIndex = Jugemuallocptr[PlayerId];
        if (ObjectIndex) {}
        if (KwanmBitcheckevflag(ObjectIndex, 4) != false) {
            KwdisplayIceblockSub(PlayerId);
        }
        KwanmBitcheckevflag(ObjectIndex, 0x00000010);
    }
    KwdisplayIceblockKakera(arg0);
}

void KwdisplayIceblockKageSub(s32 PlayerId) {
    s32 test;
    Player* temp_v1;

    temp_v1 = &PlayerOne[PlayerId];
    test = Jugemuallocptr[PlayerId];
    if (KwanmBitcheckevflag(test, 8) != 0) {
        D_80183E40[0] = temp_v1->Position[0];
        D_80183E40[1] = temp_v1->Ground - 6.5;
        D_80183E40[2] = temp_v1->Position[2];
        Kwpolygon3DIcekage(D_80183E40, (u16*) D_80183E80, 0.02f, IceKageGfx, 0x000000FF);
    }
}

void KWDisplayIceBlockShadow(UNUSED s32 arg0) {
    s32 var_s0;

    Iceblock3Material.l[0].l.dir[0] = D_80165840[0];
    Iceblock3Material.l[0].l.dir[1] = D_80165840[1];
    Iceblock3Material.l[0].l.dir[2] = D_80165840[2];
    gSPLight(DisplayListHead++, &Iceblock3Material.l[0], LIGHT_1);
    gSPLight(DisplayListHead++, &Iceblock3Material.a, LIGHT_2);
    D_80183E80[0] = 0;
    D_80183E80[1] = 0;
    D_80183E80[2] = 0;
    if (MenuMultiplayerSelection == 1) {
        for (var_s0 = 0; var_s0 < PlayerCount; var_s0++) {
            KwdisplayIceblockKageSub(var_s0);
        }
    }
}

void KwdisplaySnowmanKakera(s32 CameraId) {
    UNUSED s32 stackPadding[2];
    Camera* sp44;
    s32 someIndex;
    s32 ObjectIndex;
    AnmObject* object;

    sp44 = &Camera1[CameraId];
    KwloadtextureCI8AAZBBl(d_course_frappe_snowland_snow_tlut, d_course_frappe_snowland_snow, 0x00000020, 0x00000020);
    for (someIndex = 0; someIndex < gObjectParticle2_SIZE; someIndex++) {
        ObjectIndex = EffectAllocArray2[someIndex];
        if (ObjectIndex != NULL_OBJECT_ID) {
            object = &DynamicObjects[ObjectIndex];
            if (object->Anmptr > 0) {
                KwcheckVisArea(ObjectIndex, CameraId, 0x2AABU, 0x000001F4);
                if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
                    object->Texang[1] = KWLookCamera(object->pos[0], object->pos[2], sp44->CameraPos);
                    KWCreateAffine3DAnm(ObjectIndex);
                    gSPDisplayList(DisplayListHead++, KWSetTile32x32);
                }
            }
        }
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KwdisplaySnowmanSub(s32 CameraId) {
    s32 var_s4;
    s32 ObjectIndex;
    Camera* Camera;

    Camera = &Camera1[CameraId];
    for (var_s4 = 0; var_s4 < NUM_SNOWMEN; var_s4++) {
        ObjectIndex = Objallocptr[var_s4];
        if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
            KwcheckVisArea(ObjectIndex, CameraId, 0x2AABU, 0x00000258);
            if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
                D_80183E80[0] = (s16) DynamicObjects[ObjectIndex].Texang[0];
                D_80183E80[1] =
                    KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], Camera->CameraPos);
                D_80183E80[2] = (u16) DynamicObjects[ObjectIndex].Texang[2];
                if (KwanmBitcheckflag(ObjectIndex, 0x00000010) != 0) {
                    Kwtexture3DCI8AAZBBl(DynamicObjects[ObjectIndex].pos, (u16*) D_80183E80,
                                       DynamicObjects[ObjectIndex].Scale, (u8*) DynamicObjects[ObjectIndex].Texaddr,
                                       DynamicObjects[ObjectIndex].Indexaddr, DynamicObjects[ObjectIndex].Vtxaddr,
                                       0x00000040, 0x00000040, 0x00000040, 0x00000020);
                }
                ObjectIndex = Objallocptr2[var_s4];
                D_80183E80[0] = (s16) DynamicObjects[ObjectIndex].Texang[0];
                D_80183E80[2] = (u16) DynamicObjects[ObjectIndex].Texang[2];
                Kwtexture3DCI8AAZBBl(DynamicObjects[ObjectIndex].pos, (u16*) D_80183E80,
                                   DynamicObjects[ObjectIndex].Scale, (u8*) DynamicObjects[ObjectIndex].Texaddr,
                                   DynamicObjects[ObjectIndex].Indexaddr, DynamicObjects[ObjectIndex].Vtxaddr, 0x00000040,
                                   0x00000040, 0x00000040, 0x00000020);
            }
        }
    }
}

void KwdisplaySnowman(s32 arg0) {
    KwdisplaySnowmanSub(arg0);
    KwdisplaySnowmanKakera(arg0);
}

void KWDisplayJugemu(s32 CameraId) {
    UNUSED s32 stackPadding;
    Camera* Camera;
    f32 var_f0;
    f32 var_f2;
    s32 ObjectIndex;
    AnmObject* object;

    ObjectIndex = Jugemuallocptr[CameraId];
    Camera = &Camera1[CameraId];
    if (KwanmBitcheckflag(ObjectIndex, 0x00000010) != 0) {
        object = &DynamicObjects[ObjectIndex];
        object->Texang[0] = 0;
        object->Texang[1] = KWLookCamera(object->pos[0], object->pos[2], Camera->CameraPos);
        object->Texang[2] = 0x8000;
        if (KwanmBitoffcheckevflag(ObjectIndex, 2) != 0) {
            Kwtexture3DCI8AAZBBl(object->pos, object->Texang, object->Scale, (u8*) object->Texaddr,
                               object->Indexaddr, object->Vtxaddr, (s32) object->Texsizey,
                               (s32) object->Fvalptr, (s32) object->Texsizey,
                               (s32) object->Fvalptr / 2);
        } else {
            Kwtexture3DCI8PclAAZBBl(object->pos, object->Texang, object->Scale, (s32) object->Alpha,
                          (u8*) object->Texaddr, object->Indexaddr, object->Vtxaddr, (s32) object->Texsizey,
                          (s32) object->Fvalptr, (s32) object->Texsizey, (s32) object->Fvalptr / 2);
        }
        if (ScreenSplitB == SCREEN_MODE_1P) {
            var_f0 = object->pos[0] - D_8018CF14->CameraPos[0];
            var_f2 = object->pos[2] - D_8018CF14->CameraPos[2];
            if (var_f0 < 0.0f) {
                var_f0 = -var_f0;
            }
            if (var_f2 < 0.0f) {
                var_f2 = -var_f2;
            }
            if ((var_f0 + var_f2) <= 200.0) {
                KwdrawKage3D(&D_8018C0B0[CameraId], object->pos, 0.35f);
            }
        }
    }
}

void KwsetDoshinLightvec(UNUSED s32 arg0) {
    KwcalcLightvector();
    DosunMaterial.l[0].l.dir[0] = D_80165840[0];
    DosunMaterial.l[0].l.dir[1] = D_80165840[1];
    DosunMaterial.l[0].l.dir[2] = D_80165840[2];
}

void KwsetDoshinLight(s32 ObjectIndex) {
    // Why these don't just use `gSPSetLights1` calls...
    switch (DynamicObjects[ObjectIndex].Output) { // hmm very strange 80165C18
        case 0:
            gSPLight(DisplayListHead++, &DosunMaterial.l[0], LIGHT_1);
            gSPLight(DisplayListHead++, &DosunMaterial.a, LIGHT_2);
            break;
        case 1:
            gSPLight(DisplayListHead++, &D_800E4650.l[0], LIGHT_1);
            gSPLight(DisplayListHead++, &D_800E4650.a, LIGHT_2);
            break;
        case 2:
            gSPLight(DisplayListHead++, &D_800E4668.l[0], LIGHT_1);
            gSPLight(DisplayListHead++, &D_800E4668.a, LIGHT_2);
            break;
        case 3:
            gSPLight(DisplayListHead++, &D_800E4680.l[0], LIGHT_1);
            gSPLight(DisplayListHead++, &D_800E4680.a, LIGHT_2);
            break;
        case 4:
            gSPLight(DisplayListHead++, &D_800E4698.l[0], LIGHT_1);
            gSPLight(DisplayListHead++, &D_800E4698.a, LIGHT_2);
            break;
        default:
            break;
    }
}

void KwdisplayDoshinSub(s32 ObjectIndex) {
    if ((DynamicObjects[ObjectIndex].Anmptr >= 2) && (KwanmBitoffcheckevflag(ObjectIndex, 0x00000040) != 0)) {
        KwdrawKage3DFlat(ObjectIndex, 1.75f);
        KWCreateAffine3D(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].Texang,
                                      DynamicObjects[ObjectIndex].Scale);
        KwsetDoshinLight(ObjectIndex);
        gSPDisplayList(DisplayListHead++, D_0D007828);
        gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
        gDPLoadTLUT_pal256(DisplayListHead++, d_course_bowsers_castle_thwomp_tlut);
        KwloadtextureblockCI8BMr(DynamicObjects[ObjectIndex].Indexaddr, 0x00000010, 0x00000040, 4);
        gSPDisplayList(DisplayListHead++, DynamicObjects[ObjectIndex].Shape);
    }
}

void KwdisplayDoshin(s32 CameraId) {
    s32 ObjectIndex;
    s32 i;
    UNUSED s32 stackPadding0;
    s16 minusone, plusone;
    Camera* Camera;
    AnmObject* object;

    Camera = &Camera1[CameraId];
    if (CameraId == PLAYER_ONE) {
        for (i = 0; i < NumActiveThwomps; i++) {
            ObjectIndex = Objallocptr[i];
            KwanmClrflag(ObjectIndex, 0x00070000);
            KwanmClrevflag(ObjectIndex, 0x00000110);
        }
    }

    KwsetDoshinLightvec(ObjectIndex);
    for (i = 0; i < NumActiveThwomps; i++) {
        ObjectIndex = Objallocptr[i];
        minusone = DynamicObjects[ObjectIndex].Unk0DF - 1;
        plusone = DynamicObjects[ObjectIndex].Unk0DF + 1;
        if (GameState != 9) {
            if ((D_8018CF68[CameraId] >= minusone) && (plusone >= D_8018CF68[CameraId]) &&
                (KwanmVischeckCameraXz(ObjectIndex, Camera, 0x8000U) != 0)) {
                KwdisplayDoshinSub(ObjectIndex);
            }
        } else {
            KwdisplayDoshinSub(ObjectIndex);
        }
    }
    gSPDisplayList(DisplayListHead++, KWTextureRGBAXLUAAZBBlInit);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gSPNumLights(DisplayListHead++, 1);
    gSPLight(DisplayListHead++, &D_800E4668.l[0], LIGHT_1);
    gSPLight(DisplayListHead++, &D_800E4668.a, LIGHT_2);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BOTH);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_LIGHTING | G_SHADING_SMOOTH);
    KWLoadTextureBlockRGBA16B((u16 *) d_course_bowsers_castle_thwomp_side, 0x00000020, 0x00000020);
    for (i = 0; i < gObjectParticle3_SIZE; i++) {
        ObjectIndex = EffectAllocArray3[i];
        if (ObjectIndex != NULL_OBJECT_ID) {
            object = &DynamicObjects[ObjectIndex];
            if ((object->Anmptr > 0) && (object->Chartptr == 3) && (Modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
                KWCreateAffine3D(object->pos, object->Texang, object->Scale);
                gSPVertex(DisplayListHead++, D_0D005C00, 3, 0);
                gSPDisplayList(DisplayListHead++, D_0D006930);
            }
        }
    }
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gSPTexture(DisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
    gSPDisplayList(DisplayListHead++, D_0D007AE0);
    KwloadtextureblockIA8B(D_8018D490, 0x00000020, 0x00000020);
    SmokeMode(0);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    for (i = 0; i < gObjectParticle2_SIZE; i++) {
        ObjectIndex = EffectAllocArray2[i];
        if (ObjectIndex != NULL_OBJECT_ID) {
            object = &DynamicObjects[ObjectIndex];
            if ((object->Anmptr >= 2) && (object->Chartptr == 2) && (Modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
                KWSetPrimColor(0x000000FF, 0x000000FF, 0x000000FF, (s32) object->Alpha);
                D_80183E80[1] = KWLookCamera(object->pos[0], object->pos[2], Camera->CameraPos);
                KwsetTile(object->pos, D_80183E80, object->Scale, VtxKTile32X32);
            }
        }
    }
}

void KwdisplayFuusenSub(s32 ObjectIndex, UNUSED s32 arg1, s32 vertexIndex) {
    AnmObject* object;

    if (Modelingnum <= MTX_HUD_POOL_SIZE_MAX) {
        object = &DynamicObjects[ObjectIndex];
        D_80183E80[2] = (s16) (object->Stack[6] + 0x8000);
        KWCreateAffine3D(object->pos, (u16*) D_80183E80, object->Scale);
        SetColorRender((s32) object->Stack[0], (s32) object->Stack[1], (s32) object->Stack[2],
                         (s32) object->Stack[3], (s32) object->Stack[4], (s32) object->Stack[5],
                         (s32) object->Alpha);
        gSPVertex(DisplayListHead++, &VtxKTile64X64BL[vertexIndex], 4, 0);
        gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
    }
}

void KwdisplayFuusen(s32 arg0) {
    s32 var_s1;
    s32 ObjectIndex;

    gSPDisplayList(DisplayListHead++, D_0D007E98);
    gDPLoadTLUT_pal256(DisplayListHead++, LetterPAL);
    IceMode(0, 0, 0, 0, 0, 0, 0);
    D_80183E80[0] = 0;
    D_80183E80[1] = 0x8000;
    KwloadtextureblockCI8B(Balloon1, 64, 32);
    for (var_s1 = 0; var_s1 < D_80165738; var_s1++) {
        ObjectIndex = EffectAllocArray3[var_s1];
        if ((ObjectIndex != NULL_OBJECT_ID) && (DynamicObjects[ObjectIndex].Anmptr >= 2)) {
            KwdisplayFuusenSub(ObjectIndex, arg0, 0);
        }
    }
    KwloadtextureblockCI8B(Balloon2, 64, 32);
    for (var_s1 = 0; var_s1 < D_80165738; var_s1++) {
        ObjectIndex = EffectAllocArray3[var_s1];
        if ((ObjectIndex != NULL_OBJECT_ID) && (DynamicObjects[ObjectIndex].Anmptr >= 2)) {
            KwdisplayFuusenSub(ObjectIndex, arg0, 4);
        }
    }
}

void KwdisplayTrainSmokeSub(s32 ObjectIndex, s32 CameraId) {
    Camera* Camera;

    Camera = &Camera1[CameraId];
    if (ObjectIndex != NULL_OBJECT_ID) {
        if ((DynamicObjects[ObjectIndex].Anmptr >= 2) && (DynamicObjects[ObjectIndex].Chartptr == 1) &&
            (Modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
            SetColorRender((s32) DynamicObjects[ObjectIndex].Output, (s32) DynamicObjects[ObjectIndex].Output,
                             (s32) DynamicObjects[ObjectIndex].Output, 0, 0, 0, (s32) DynamicObjects[ObjectIndex].Alpha);
            D_80183E80[1] =
                KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], Camera->CameraPos);
            KwsetTile(DynamicObjects[ObjectIndex].pos, D_80183E80, DynamicObjects[ObjectIndex].Scale, VtxKTile32X32);
        }
    }
}

// Trains smoke particles.
void KwdisplayTrainSmoke(s32 CameraId) {
    UNUSED s32 pad;
    UNUSED s32 j;
    Camera* Camera;
    s32 i;

    Camera = &Camera1[CameraId];
    gSPDisplayList(DisplayListHead++, D_0D007AE0);
    KwloadtextureblockA8B(D_0D029458, 32, 32);
    Color2Mode(255, 255, 255, 255, 255, 255, 255);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;

// Render smoke for any Number of trains. Don't know enough about these variables yet.
#ifdef AVOID_UB_WIP
    for (j = 0; j < NUM_TRAINS; j++) {
        if ((SL[j].SomeFlags != 0) &&
            (KwvischeckCameraXz(&SL[j].Locomotive.Position, Camera, 0x4000U) != 0)) {

            for (i = 0; i < 128; i++) {
                // Need to make a way to increase this array for each train.
                KwdisplayTrainSmokeSub(EffectAllocArray2[i], CameraId);
            }
        }
    }
#else

    if ((SL[0].SomeFlags != 0) &&
        (KwvischeckCameraXz(SL[0].Locomotive.Position, Camera, 0x4000U) != 0)) {

        for (i = 0; i < gObjectParticle2_SIZE; i++) {
            KwdisplayTrainSmokeSub(EffectAllocArray2[i], CameraId);
        }
    }
    if ((SL[1].SomeFlags != 0) &&
        (KwvischeckCameraXz(SL[1].Locomotive.Position, Camera, 0x4000U) != 0)) {
        for (i = 0; i < gObjectParticle3_SIZE; i++) {
            KwdisplayTrainSmokeSub(EffectAllocArray3[i], CameraId);
        }
    }
#endif
}

void KwdisplayShipSmokeSub(s32 ObjectIndex, s32 CameraId) {
    Camera* Camera;

    Camera = &Camera1[CameraId];
    if (ObjectIndex != NULL_OBJECT_ID) {
        if ((DynamicObjects[ObjectIndex].Anmptr >= 2) && (DynamicObjects[ObjectIndex].Chartptr == 6) &&
            (Modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
            SetColorRender((s32) DynamicObjects[ObjectIndex].Output, (s32) DynamicObjects[ObjectIndex].Output,
                             (s32) DynamicObjects[ObjectIndex].Output, DynamicObjects[ObjectIndex].Input,
                             DynamicObjects[ObjectIndex].Input, DynamicObjects[ObjectIndex].Input,
                             (s32) DynamicObjects[ObjectIndex].Alpha);
            D_80183E80[1] =
                KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], Camera->CameraPos);
            KwsetTile(DynamicObjects[ObjectIndex].pos, D_80183E80, DynamicObjects[ObjectIndex].Scale, VtxKTile32X32);
        }
    }
}

// Likely smoke related.
void KwdisplayShipSmoke(s32 CameraId) {
    UNUSED s32 pad[2];
    Camera* Camera;
    s32 i;

    Camera = &Camera1[CameraId];
    gSPDisplayList(DisplayListHead++, D_0D007AE0);

    KwloadtextureblockA8B(D_0D029458, 32, 32);
    Color2Mode(255, 255, 255, 255, 255, 255, 255);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    if ((Ship[0].SomeFlags != 0) && (KwvischeckCameraXz(Ship[0].Position, Camera, 0x4000U) != 0)) {
        for (i = 0; i < gObjectParticle2_SIZE; i++) {
            KwdisplayShipSmokeSub(EffectAllocArray2[i], CameraId);
        }
    }
    if ((Ship[1].SomeFlags != 0) && (KwvischeckCameraXz(Ship[1].Position, Camera, 0x4000U) != 0)) {
        for (i = 0; i < gObjectParticle3_SIZE; i++) {
            KwdisplayShipSmokeSub(EffectAllocArray3[i], CameraId);
        }
    }
}

void KwdisplayKoopaFireSub(s32 ObjectIndex, s32 CameraId) {
    Camera* Camera;
    AnmObject* object;

    Camera = &Camera1[CameraId];
    if (Modelingnum <= MTX_HUD_POOL_SIZE_MAX) {
        object = &DynamicObjects[ObjectIndex];
        if (object->Chartptr == 9) {
            Color2Mode(0xFF, (s32) object->Output, 0, (s32) object->Input, 0, 0, (s32) object->Alpha);
        } else {
            KWSetPrimColor(0xFF, (s32) object->Output, 0, (s32) object->Alpha);
        }
        D_80183E80[1] = KWLookCamera(object->pos[0], object->pos[2], Camera->CameraPos);
        KwsetTile(object->pos, D_80183E80, object->Scale, VtxKTile32X32);
    }
}

void KwdisplayFire(s32 CameraId) {
    s32 var_s0;
    s32 ObjectIndex;

    gSPDisplayList(DisplayListHead++, D_0D007AE0);
    KwloadtextureblockA8B(common_texture_particle_smoke[KW4GFTimer], 0x00000020, 0x00000020);
    SplashMode(0, 0, 0, 0x000000FF);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    for (var_s0 = 0; var_s0 < gObjectParticle1_SIZE; var_s0++) {
        ObjectIndex = EffectAllocArray1[var_s0];
        if ((ObjectIndex != NULL_OBJECT_ID) && (DynamicObjects[ObjectIndex].Anmptr >= 3)) {
            KwdisplayKoopaFireSub(ObjectIndex, CameraId);
        }
    }
}

extern char FireRGBA[8][4];

void KWDisplayFireParticleSub(s32 ObjectIndex, u8 arg1, Vec3f Arg2) {
    s32 realindex;

    if (Modelingnum <= MTX_HUD_POOL_SIZE_MAX) {
        switch (arg1) { /* irregular */
            case 0:
                SetColorRender(0xE6, 0xFF, 0xFF, 0x00, 0x00, 0xFF, (s32) DynamicObjects[ObjectIndex].Alpha);
                break;
            case 1:
                SetColorRender(0xFF, 0xFF, 0x96, 0xFF, 0x00, 0x00, (s32) DynamicObjects[ObjectIndex].Alpha);
                break;
            case 2:
                SetColorRender(0xFF, 0xE6, 0xFF, 0xFF, 0x00, 0x96, (s32) DynamicObjects[ObjectIndex].Alpha);
                break;
            case 3:
                if (HotSwapID != 0) {
                    realindex = ObjectIndex - FireParticleAllocArray[0];
                    SetColorRender(FireRGBA[realindex][0], FireRGBA[realindex][1], FireRGBA[realindex][2],
                                   FireRGBA[realindex][0] / 3, FireRGBA[realindex][1] / 3, FireRGBA[realindex][2] / 3,
                                   FireRGBA[realindex][3]);
                } else {
                    SetColorRender(0xFF, 0xFF, 0x1E, 0xFF, 0x00, 0x00, (s32) DynamicObjects[ObjectIndex].Alpha);
                }
                break;
            default:
                break;
        }
        D_80183E80[1] = KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], Arg2);
        KwsetTile(DynamicObjects[ObjectIndex].pos, D_80183E80, DynamicObjects[ObjectIndex].Scale, VtxKTile32X32);
    }
}

void KwdisplayKameFire(s32 CameraId) {
    UNUSED s32 stackPadding[2];
    Camera* sp54;
    s32 var_s0;
    s32 ObjectIndex;
    AnmObject* object;

    sp54 = &Camera1[CameraId];
    gSPDisplayList(DisplayListHead++, D_0D007AE0);
    KwloadtextureblockA8B(common_texture_particle_smoke[KW4GFTimer], 32, 32);
    Color2Mode(255, 255, 255, 255, 255, 255, 255);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    for (var_s0 = 0; var_s0 < gObjectParticle4_SIZE; var_s0++) {
        ObjectIndex = FireParticleAllocArray[var_s0];
        if (ObjectIndex != NULL_OBJECT_ID) {
            object = &DynamicObjects[ObjectIndex];
            if (object->Anmptr >= 2) {
                if (object->Texsizex == 3) {
                    KwcheckVisArea(ObjectIndex, CameraId, 0x4000U, 0x00000514);
                } else {
                    KwcheckVisArea(ObjectIndex, CameraId, 0x4000U, 0x000001F4);
                }
                if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
                    KWDisplayFireParticleSub(ObjectIndex, object->Texsizex, sp54->CameraPos);
                }
            }
        }
    }
}

void KwdisplayStarSub(s32 ObjectIndex, Vec3f arg1) {
    D_80183E80[0] = KWLookCameraPitch(DynamicObjects[ObjectIndex].pos[2], DynamicObjects[ObjectIndex].pos[1], arg1);
    D_80183E80[1] = KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], arg1);
    D_80183E80[2] = (u16) DynamicObjects[ObjectIndex].Texang[2];
    KWSetPrimColor((s32) DynamicObjects[ObjectIndex].Stack[0], (s32) DynamicObjects[ObjectIndex].Stack[1],
                  (s32) DynamicObjects[ObjectIndex].Stack[2], (s32) DynamicObjects[ObjectIndex].Alpha);
    KWCreateAffine3D(DynamicObjects[ObjectIndex].pos, (u16*) D_80183E80,
                                  DynamicObjects[ObjectIndex].Scale);
    gSPVertex(DisplayListHead++, VtxKTile32X32, 4, 0);
    gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
}

void KWDisplayStar(s32 CameraId) {
    s32 var_s0;
    s32 temp_a0;
    Camera* Camera;

    Camera = &Camera1[CameraId];
    gSPDisplayList(DisplayListHead++, D_0D007AE0);
    KwloadtextureblockIA8B(Hoshi, 0x00000020, 0x00000020);
    ToumeIMode(0x000000FF, 0x000000FF, 0, 0x000000FF);
    D_80183E80[0] = 0;
    for (var_s0 = 0; var_s0 < gObjectParticle3_SIZE; var_s0++) {
        temp_a0 = EffectAllocArray3[var_s0];
        if ((temp_a0 != -1) && (DynamicObjects[temp_a0].Anmptr >= 2)) {
            KwdisplayStarSub(temp_a0, Camera->CameraPos);
        }
    }
}

void KwdisplayChoropuSub(s32 ObjectIndex, s32 CameraId) {
    Camera* Camera;

    Camera = &Camera1[CameraId];
    if (DynamicObjects[ObjectIndex].Anmptr >= 3) {
        KwcheckVisArea(ObjectIndex, CameraId, 0x2AABU, 0x0000012C);
        if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
            D_80183E80[0] = (s16) DynamicObjects[ObjectIndex].Texang[0];
            D_80183E80[1] =
                KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], Camera->CameraPos);
            D_80183E80[2] = (u16) DynamicObjects[ObjectIndex].Texang[2];
            Kwtexture3DCI8AAZBBlMr(DynamicObjects[ObjectIndex].pos, (u16*) D_80183E80, DynamicObjects[ObjectIndex].Scale,
                          (u8*) DynamicObjects[ObjectIndex].Texaddr, DynamicObjects[ObjectIndex].Indexaddr, D_0D0062B0,
                          0x00000020, 0x00000040, 0x00000020, 0x00000040, 5);
        }
    }
}

void KwdisplayHoleSub(s32 ObjectIndex) {
    if (DynamicObjects[ObjectIndex].Anmptr > 0) {
        if (KwanmBitcheckflag(ObjectIndex, 0x00800000) != 0) {
            D_80183E50[0] = DynamicObjects[ObjectIndex].pos[0];
            D_80183E50[1] = DynamicObjects[ObjectIndex].Ground + 0.8;
            D_80183E50[2] = DynamicObjects[ObjectIndex].pos[2];
            D_80183E70[0] = DynamicObjects[ObjectIndex].Velocity[0];
            D_80183E70[1] = DynamicObjects[ObjectIndex].Velocity[1];
            D_80183E70[2] = DynamicObjects[ObjectIndex].Velocity[2];
            KwdrawHole3D(DynamicObjects[ObjectIndex].Scale);
        }
    }
}

// Almost certainly responsible for spawning/handling the moles on Moo Moo farm
void KwdisplayHole(UNUSED s32 Unused) {
    s32 someIndex;

    for (someIndex = 0; someIndex < NUM_TOTAL_MOLES; someIndex++) {
        KwdisplayHoleSub(EffectAllocArray1[someIndex]);
    }
}

void KwdisplayTuchi(s32 CameraId) {
    s32 var_s2;
    s32 ObjectIndex;
    Camera* sp44;
    AnmObject* object;

    sp44 = &Camera1[CameraId];
    gSPDisplayList(DisplayListHead++, KWTextureRGBAXLUAAZBBlInit);
    KWLoadTextureBlockRGBA16B((u16 *) d_course_moo_moo_farm_mole_dirt, 0x00000010, 0x00000010);
    for (var_s2 = 0; var_s2 < gObjectParticle2_SIZE; var_s2++) {
        ObjectIndex = EffectAllocArray2[var_s2];
        object = &DynamicObjects[ObjectIndex];
        if (object->Anmptr > 0) {
            KwcheckVisArea(ObjectIndex, CameraId, 0x2AABU, 0x000000C8);
            if ((KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) && (Modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
                object->Texang[1] = KWLookCamera(object->pos[0], object->pos[2], sp44->CameraPos);
                KWCreateAffine3DAnm(ObjectIndex);
                gSPDisplayList(DisplayListHead++, KWSetTile16x16);
            }
        }
    }
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KwdisplayChoropu(s32 CameraId) {
    s32 i;

    for (i = 0; i < NUM_GROUP1_MOLES; i++) {
        KwdisplayChoropuSub(Objallocptr[i], CameraId);
    }
    for (i = 0; i < NUM_GROUP2_MOLES; i++) {
        KwdisplayChoropuSub(Objallocptr2[i], CameraId);
    }
    for (i = 0; i < NUM_GROUP3_MOLES; i++) {
        KwdisplayChoropuSub(Objallocptr3[i], CameraId);
    }
    KwdisplayHole(CameraId);
    KwdisplayTuchi(CameraId);
}

void KwdisplayGoalflagSub(s32 ObjectIndex) {
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        gSPDisplayList(DisplayListHead++, D_0D0077A0);
        KWCreateAffine3D(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].ang,
                                      DynamicObjects[ObjectIndex].Scale);
        if (PauseFlag == 0) {
            DynamicObjects[ObjectIndex].Input = DrawLocalSkeletonShape((Armature*) DynamicObjects[ObjectIndex].Shape,
                                                                     (Animation**) DynamicObjects[ObjectIndex].Vtxaddr, 0,
                                                                     DynamicObjects[ObjectIndex].Input);
        } else {
            DrawLocalSkeletonShape((Armature*) DynamicObjects[ObjectIndex].Shape,
                                  (Animation**) DynamicObjects[ObjectIndex].Vtxaddr, 0, DynamicObjects[ObjectIndex].Input);
        }
    }
}

void KwdisplayGoalflag(s32 CameraId) {
    s32 var_s1;
    s32 temp_s0;

    for (var_s1 = 0; var_s1 < 4; var_s1++) {
        temp_s0 = Objallocptr[var_s1];
        KwcheckVisArea(temp_s0, CameraId, 0x4000U, 0x000005DC);
        if (KwanmBitcheckflag(temp_s0, VISIBLE) != 0) {
            KwdisplayGoalflagSub(temp_s0);
        }
    }
}

void KwdisplayKamomeSub(s32 ObjectIndex) {
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        KWCreateAffine3D(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].ang,
                                      DynamicObjects[ObjectIndex].Scale);
        gSPDisplayList(DisplayListHead++, D_0D0077D0);
        if (PauseFlag == 0) {
            DynamicObjects[ObjectIndex].Input = DrawLocalSkeletonShape((Armature*) DynamicObjects[ObjectIndex].Shape,
                                                                     (Animation**) DynamicObjects[ObjectIndex].Vtxaddr, 0,
                                                                     DynamicObjects[ObjectIndex].Input);
        } else {
            DrawLocalSkeletonShape((Armature*) DynamicObjects[ObjectIndex].Shape,
                                  (Animation**) DynamicObjects[ObjectIndex].Vtxaddr, 0, DynamicObjects[ObjectIndex].Input);
        }
    }
}

void KwdisplayKamome(s32 arg0) {
    s32 i;
    s32 var_s1;

    for (i = 0; i < NUM_SEAGULLS; i++) {
        var_s1 = Objallocptr2[i];
        if (KwcheckVisArea(var_s1, arg0, 0x5555U, 0x000005DC) < 0x9C401) {
            D_80165908 = 1;
            KwanmSetevflag(var_s1, 2);
        }
        if (KwanmBitcheckflag(var_s1, VISIBLE) != 0) {
            KwdisplayKamomeSub(var_s1);
        }
    }
}

void KwdisplayKaniSub(s32 ObjectIndex, s32 CameraId) {
    Camera* Camera;

    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        Camera = &Camera1[CameraId];
        KwdrawKage3DFast(ObjectIndex, 0.5f);
        DynamicObjects[ObjectIndex].Texang[1] =
            KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], Camera->CameraPos);
        Kwtexture3DCI8AAZBBl(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].Texang,
                           DynamicObjects[ObjectIndex].Scale, (u8*) DynamicObjects[ObjectIndex].Texaddr,
                           DynamicObjects[ObjectIndex].Indexaddr, VtxKTile64X64BL, 0x00000040, 0x00000040,
                           0x00000040, 0x00000020);
    }
}

void KwdisplayKani(s32 arg0) {
    s32 someIndex;
    s32 test;

    for (someIndex = 0; someIndex < NUM_CRABS; someIndex++) {
        test = Objallocptr[someIndex];
        KwcheckVisArea(test, arg0, 0x2AABU, 0x00000320);
        if (KwanmBitcheckflag(test, VISIBLE) != 0) {
            KwdisplayKaniSub(test, arg0);
        }
    }
}

void KwdisplayHarinezumiSub(s32 ObjectIndex, s32 CameraId) {
    Camera* Camera;

    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        Camera = &Camera1[CameraId];
        KwdrawKage3DBump(ObjectIndex, 0.7f);
        DynamicObjects[ObjectIndex].Texang[1] =
            KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], Camera->CameraPos);
        Kwtexture3DCI8AAZBBl(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].Texang,
                           DynamicObjects[ObjectIndex].Scale, (u8*) DynamicObjects[ObjectIndex].Texaddr,
                           DynamicObjects[ObjectIndex].Indexaddr, DynamicObjects[ObjectIndex].Vtxaddr, 64, 64, 64, 32);
    }
}

void KwdisplayHarinezumi(s32 arg0) {
    s32 test;
    u32 something;
    s32 someIndex;

    for (someIndex = 0; someIndex < NUM_HEDGEHOGS; someIndex++) {
        test = Objallocptr2[someIndex];
        something = KwcheckVisArea(test, arg0, 0x4000U, 0x000003E8);
        if (KwanmBitcheckflag(test, VISIBLE) != 0) {
            KwanmSetflag(test, 0x00200000);
            if (something < 0x2711U) {
                KwanmSetflag(test, 0x00000020);
            } else {
                KwanmClrflag(test, 0x00000020);
            }
            if (something < 0x57E41U) {
                KwanmSetflag(test, 0x00400000);
            }
            if (something < 0x52211U) {
                KwdisplayHarinezumiSub(test, arg0);
            }
        }
    }
}

UNUSED void func_800557AC() {
}

void KwdisplayPingSub(s32 ObjectIndex, u32 arg1, u32 Arg2) {
    Vec3f sp34;
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    if (object->Anmptr >= 2) {
        if (KwanmBitcheckflag(ObjectIndex, 0x00000020) != 0) {
            if (KwanmBitcheckevflag(ObjectIndex, 4) != 0) {
                if (Arg2 >= arg1) {
                    sp34[0] = object->pos[0];
                    sp34[1] = object->pos[1] - 1.0;
                    sp34[2] = object->pos[2];
                    KWCreateAffine3DIceKage(sp34, object->Texang,
                                                                           object->Scale);
                    gSPDisplayList(DisplayListHead++, D_0D0077D0);
                    DrawLocalSkeletonShape((Armature*) object->Shape, (Animation**) object->Vtxaddr,
                                          (s16) object->Texsizex, (s16) object->Anmnumbak);
                }
            } else if (arg1 < 0x15F91U) {
                KwdrawKage3DFlat(ObjectIndex, 1.5f);
            }
        }
        KWCreateAffine3D(object->pos, object->Texang, object->Scale);
        gSPDisplayList(DisplayListHead++, D_0D0077D0);
        DrawLocalSkeletonShape((Armature*) object->Shape, (Animation**) object->Vtxaddr, (s16) object->Texsizex,
                              (s16) object->Anmnumbak);
    }
}

void KwdisplayPing(s32 CameraId) {
    s32 i;
    s32 ObjectIndex;
    s32 temp_s1;
    s32 var_a3;
    u16 var_s1;
    u32 var_s3;

    if (PlayerCount == 1) {
        var_s3 = 0x0003D090;
    } else if (PlayerCount == 2) {
        var_s3 = 0x00027100;
    } else {
        var_s3 = 0x00015F90;
    }
    for (i = 0; i < NUM_PENGUINS; i++) {
        ObjectIndex = Objallocptr[i];
        if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
            if (PlayerCount == 1) {
                var_s1 = 0x4000;
                if (i == 0) {
                    var_a3 = 0x000005DC;
                } else if (KwanmBitcheckevflag(ObjectIndex, 8) != 0) {
                    var_a3 = 0x00000320;
                } else {
                    var_a3 = 0x000003E8;
                }
            } else {
                if (KwanmBitcheckevflag(ObjectIndex, 8) != 0) {
                    var_a3 = 0x000001F4;
                    var_s1 = 0x4000;
                } else {
                    var_a3 = 0x00000258;
                    var_s1 = 0x5555;
                }
            }
            temp_s1 = KwcheckVisArea(ObjectIndex, CameraId, var_s1, var_a3);
            if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
                KwdisplayPingSub(ObjectIndex, (u32) temp_s1, var_s3);
            }
        }
    }
}

void KwdisplayWanwanSub(s32 ObjectIndex, s32 CameraId) {
    Camera* Camera;

    Camera = &Camera1[CameraId];
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        if (KwanmBitcheckflag(ObjectIndex, 0x00100000) != 0) {
            D_80183E40[0] = DynamicObjects[ObjectIndex].pos[0];
            D_80183E40[1] = DynamicObjects[ObjectIndex].pos[1] + 16.0;
            D_80183E40[2] = DynamicObjects[ObjectIndex].pos[2];
            D_80183E80[0] = 0;
            D_80183E80[1] =
                KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], Camera->CameraPos);
            D_80183E80[2] = 0x8000;
            Kwtexture3DRGBAAAZBBlMr(D_80183E40, D_80183E80, 0.54f, (u16 *) d_course_rainbow_road_sphere, D_0D0062B0, 0x00000020,
                          0x00000040, 0x00000020, 0x00000040, 5);
        } else {
            KWCreateAffine3D(DynamicObjects[ObjectIndex].pos, DynamicObjects[ObjectIndex].ang,
                                          DynamicObjects[ObjectIndex].Scale);
            gSPDisplayList(DisplayListHead++, D_0D0077D0);
            DrawLocalSkeletonShape((Armature*) DynamicObjects[ObjectIndex].Shape,
                                  (Animation**) DynamicObjects[ObjectIndex].Vtxaddr, 0,
                                  (s16) DynamicObjects[ObjectIndex].Anmnumbak);
        }
    }
}

void KwdisplayWanwan(s32 CameraId) {
    s32 var_s1;
    s32 ObjectIndex;

    for (var_s1 = 0; var_s1 < NUM_CHAIN_CHOMPS; var_s1++) {
        ObjectIndex = Objallocptr2[var_s1];
        KwcheckLODTexture(ObjectIndex, CameraId, 0x000005DC, 0x000009C4);
        if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
            KwdisplayWanwanSub(ObjectIndex, CameraId);
        }
    }
}

void KwdisplayBalloonSub(s32 ObjectIndex, s32 CameraId) {
    UNUSED s32 pad;
    f32 test;
    Camera* Camera;

    Camera = &Camera1[CameraId];
    if (DynamicObjects[ObjectIndex].Anmptr >= 2) {
        KwcheckDisplayKage(ObjectIndex, CameraId, 0x0000012C);
        test = DynamicObjects[ObjectIndex].pos[1] - DynamicObjects[ObjectIndex].Ground;
        KwdrawKage3DFast(ObjectIndex, (20.0 / test) + 0.5);
        if (KwanmBitoffcheckflag(ObjectIndex, 0x00100000) != 0) {
            Kwpolygon3DSlight(DynamicObjects[ObjectIndex].pos, (u16*) DynamicObjects[ObjectIndex].ang,
                          DynamicObjects[ObjectIndex].Scale, d_course_luigi_raceway_dl_F960);
            gSPDisplayList(DisplayListHead++, d_course_luigi_raceway_dl_F650);
        } else {
            D_80183E80[0] = (s16) DynamicObjects[ObjectIndex].ang[0];
            D_80183E80[1] =
                (s16) (KWLookCamera(DynamicObjects[ObjectIndex].pos[0], DynamicObjects[ObjectIndex].pos[2], Camera->CameraPos) +
                       0x8000);
            D_80183E80[2] = (u16) DynamicObjects[ObjectIndex].ang[2];
            Kwpolygon3DSlight(DynamicObjects[ObjectIndex].pos, D_80183E80, DynamicObjects[ObjectIndex].Scale,
                          d_course_luigi_raceway_dl_FBE0);
            gSPDisplayList(DisplayListHead++, d_course_luigi_raceway_dl_FA20);
            if (PlayerCount == 1) {
                DynamicObjects[ObjectIndex].ang[1] = 0;
            }
        }
    }
}

void KwdisplayBalloon(s32 arg0) {
    s32 ObjectIndex;
    ObjectIndex = Objallocptr[0];
    if (GameState != 9) {
        KwcheckLODTexture(ObjectIndex, arg0, 0x000005DC, 0x00000BB8);
        if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
            KwdisplayBalloonSub(ObjectIndex, arg0);
        }
    } else {
        KwanmClrflag(ObjectIndex, 0x00100000);
        KwdisplayBalloonSub(ObjectIndex, arg0);
    }
}

void KwdisplayDaiSub(s32 ObjectIndex, UNUSED s32 arg1) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    if (object->Anmptr >= 2) {
        Kwpolygon3D(object->pos, object->ang, object->Scale, object->Shape);
    }
}

void KwdisplayDai(s32 arg0) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 3; someIndex++) {
        KwdisplayDaiSub(Objallocptr[someIndex], arg0);
    }
}

void KwdisplayCupSub(s32 ObjectIndex, UNUSED s32 arg1) {
    Mat4 someMatrix1;
    Mat4 someMatrix2;
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    if (object->Anmptr >= 2) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        KwcreateModelingMatrix(someMatrix1, object->pos, object->ang, object->Scale);
        Affine2Mtx(&DynamicP->MTXHud[Modelingnum], someMatrix1);
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPDisplayList(DisplayListHead++, D_0D0077A0);
        gSPDisplayList(DisplayListHead++, object->Shape);
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(someMatrix2);
        SetMatrix(someMatrix2, 0);
    }
}

void KwdisplayCup(s32 arg0) {
    KwdisplayCupSub(Objallocptr[3], arg0);
}

void KwdisplayNeon(s32 CameraId) {
    Camera* Camera;
    s32 var_s2;
    s32 ObjectIndex;
    AnmObject* object;

    Camera = &Camera1[CameraId];
    for (var_s2 = 0; var_s2 < 10; var_s2++) {
        ObjectIndex = Objallocptr[var_s2];
        if (ThunderFlg[CameraId] == 0) {
            object = &DynamicObjects[ObjectIndex];
            if ((object->Anmptr >= 2) && (KwanmBitoffcheckflag(ObjectIndex, 0x00080000) != 0) &&
                (KwanmVischeckCameraXz(ObjectIndex, Camera, 0x2AABU) != 0)) {
                object->Texang[1] = KwlookcameraXz(ObjectIndex, Camera);
                Kwtexture3DCI8AAZBBl(object->pos, object->Texang, object->Scale, (u8*) object->Texaddr,
                                   object->Indexaddr, VtxKTile64X64BL, 0x00000040, 0x00000040, 0x00000040,
                                   0x00000020);
            }
        }
    }
}

void KwdisplayBombsparkBt(s32 arg0, s32 arg1, s32 Arg2) {
    D_80165860 = sparkcoltbl[arg0][0];
    D_8016586C = sparkcoltbl[arg0][1];
    D_80165878 = sparkcoltbl[arg0][2];
    KWSetPrimColor(D_80165860, D_8016586C, D_80165878, Arg2);
    KWCreateAffine3D(D_80183E40, D_80183E80, 0.2f);
    KwloadtextureblockI8B(common_texture_particle_spark[arg1], 0x00000020, 0x00000020);
    gSPVertex(DisplayListHead++, VtxKTile32X32, 4, 0);
    gSPDisplayList(DisplayListHead++, CommonRectangleDisplay);
}

void KwdisplayBombbodyBt(s32 ObjectIndex, s32 CameraId, s32 Arg2) {
    s32 temp_s0;
    s32 temp_v0;
    s32 residue;
    Camera* Camera;
    AnmObject* object;

    Camera = &Camera1[CameraId];
    object = &DynamicObjects[ObjectIndex];
    residue = KW2GFCount % 4U;
    D_80183E40[0] = object->pos[0];
    D_80183E40[1] = object->pos[1] + 1.0;
    D_80183E40[2] = object->pos[2];
    D_80183E80[0] = 0;
    D_80183E80[1] = KWLookCamera(object->pos[0], object->pos[2], Camera->CameraPos);
    D_80183E80[2] = 0x8000;
    KWCreateAffine3D(D_80183E40, D_80183E80, 0.2f);
    gSPDisplayList(DisplayListHead++, D_0D007E98);
    ToumeIAMode(Arg2);
    KWTextureCI8SubBL((u8*) CommonTLUTBomb, common_texture_bomb[residue], VtxKTile32X32, 0x00000020,
                                   0x00000020, 0x00000020, 0x00000020);
    temp_s0 = Kwgameframetimer;
    gSPDisplayList(DisplayListHead++, D_0D007B00);
    SplashMode(0, 0, 0, Arg2);
    D_80183E40[1] = D_80183E40[1] + 4.0;
    D_80183E80[2] = 0;
    KwdisplayBombsparkBt(temp_s0 % 3, temp_s0 % 4, Arg2);
    temp_v0 = temp_s0 + 1;
    D_80183E80[2] = 0x6000;
    KwdisplayBombsparkBt(temp_v0 % 3, temp_v0 % 4, Arg2);
    temp_v0 = temp_s0 + 2;
    D_80183E80[2] = 0xA000;
    KwdisplayBombsparkBt(temp_v0 % 3, temp_v0 % 4, Arg2);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KwdisplayBombtireBt(s32 ObjectIndex, UNUSED s32 arg1, s32 Arg2) {
    gSPDisplayList(DisplayListHead++, D_0D0079E8);
    ToumeIAMode(Arg2);
    KWLoadTextureBlockRGBA16B(D_0D02AA58, 0x00000010, 0x00000010);
    D_80183E40[1] = DynamicObjects[ObjectIndex].pos[1] - 2.0;
    D_80183E40[0] = DynamicObjects[ObjectIndex].pos[0] + 2.0;
    D_80183E40[2] = DynamicObjects[ObjectIndex].pos[2] + 2.0;
    KwsetTile(D_80183E40, D_80183E80, 0.15f, VtxKTile16X16);
    D_80183E40[0] = DynamicObjects[ObjectIndex].pos[0] + 2.0;
    D_80183E40[2] = DynamicObjects[ObjectIndex].pos[2] - 2.0;
    KwsetTile(D_80183E40, D_80183E80, 0.15f, VtxKTile16X16);
    D_80183E40[0] = DynamicObjects[ObjectIndex].pos[0] - 2.0;
    D_80183E40[2] = DynamicObjects[ObjectIndex].pos[2] - 2.0;
    KwsetTile(D_80183E40, D_80183E80, 0.15f, VtxKTile16X16);
    D_80183E40[0] = DynamicObjects[ObjectIndex].pos[0] - 2.0;
    D_80183E40[2] = DynamicObjects[ObjectIndex].pos[2] + 2.0;
    KwsetTile(D_80183E40, D_80183E80, 0.15f, VtxKTile16X16);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KwdisplayBombshadowBt(s32 ObjectIndex, s32 PlayerId) {
    Mat4 sp30;
    Player* Player;

    Player = &PlayerOne[PlayerId];
    D_80183E50[0] = DynamicObjects[ObjectIndex].pos[0];
    D_80183E50[1] = DynamicObjects[ObjectIndex].Ground + 0.8;
    D_80183E50[2] = DynamicObjects[ObjectIndex].pos[2];
    KwcreateRotationMatrix(sp30, Player->Bump.BumpZX, D_80183E50, 0U, 0.5f);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], sp30);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(DisplayListHead++, ShadowModel2);
}

void KwstartBombkart(s32 playerIndex) {
    s32 ObjectIndex;

    ObjectIndex = Bomballocptr[playerIndex];
    KWAnmStart(ObjectIndex, 0);
    DynamicObjects[ObjectIndex].Alpha = 0;
}

void KwsetBombkartAlpha(s32 playerIndex, s32 primAlpha) {
    s32 ObjectIndex;

    ObjectIndex = Bomballocptr[playerIndex];
    KWAnmStart(ObjectIndex, 0);
    DynamicObjects[ObjectIndex].Alpha = (s16) primAlpha;
}

void KwfinishBombkart(s32 playerIndex) {
    KwanmFinish(Bomballocptr[playerIndex]);
}

void KWDisplayBombKartBT(s32 CameraId) {
    Player* temp_v0;
    s32 temp_s1;
    s32 temp_s0;
    s32 payerId;
    AnmObject* object;

    for (payerId = 0; payerId < NUM_BOMB_KARTS_BATTLE; payerId++) {
        temp_s0 = Bomballocptr[payerId];
        object = &DynamicObjects[temp_s0];
        if (object->Anmptr != 0) {
            temp_s1 = object->Alpha;
            temp_v0 = &PlayerOne[payerId];
            object->pos[0] = temp_v0->Position[0];
            object->pos[1] = temp_v0->Position[1] - 2.0;
            object->pos[2] = temp_v0->Position[2];
            object->Ground = temp_v0->Ground;
            KwdisplayBombbodyBt(temp_s0, CameraId, temp_s1);
            KwdisplayBombtireBt(temp_s0, CameraId, temp_s1);
            KwdisplayBombshadowBt(temp_s0, CameraId);
        }
    }
}

void KwdisplayBombkartSub(s32 bombIndex) {
    UNUSED s32 stackPadding;
    u8 thing;
    s32 temp_s0;
    s32 temp_v0;
    u8* bombFrame;
    BombKart sp40 = Bom[bombIndex];

    temp_v0 = KW2GFCount % 6U;
    thing = bombcoltbl[temp_v0];
    bombFrame = common_texture_bomb[thing];
    D_80183E40[0] = sp40.BombPos[0];
    D_80183E40[1] = sp40.BombPos[1] + 1.0;
    D_80183E40[2] = sp40.BombPos[2];
    Kwtexture3DCI8AAZBBl(D_80183E40, D_80183E80, 0.25f, (u8*) CommonTLUTBomb, bombFrame, VtxKTile32X32, 0x20, 0x20, 0x20,
                       0x20);
    temp_s0 = Kwgameframetimer;
    gSPDisplayList(DisplayListHead++, D_0D007B00);
    SplashMode(0, 0, 0, 0xFF);
    D_80183E40[1] = sp40.BombPos[1] + 5.0;
    D_80183E80[2] = 0;
    KwdisplayBombsparkBt((s32) temp_s0 % 3, temp_s0 % 4, 0xFFU);
    temp_v0 = temp_s0 + 1;
    D_80183E80[2] = 0x6000;
    KwdisplayBombsparkBt(temp_v0 % 3, temp_v0 % 4, 0xFFU);
    temp_v0 = temp_s0 + 2;
    D_80183E80[2] = 0xA000;
    KwdisplayBombsparkBt(temp_v0 % 3, temp_v0 % 4, 0xFFU);
}

void KwdisplayBombtire(s32 bombIndex, Vec3f arg1) {
    UNUSED s32 stackPadding[2];
    BombKart sp2C = Bom[bombIndex];

    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    gSPDisplayList(DisplayListHead++, KWTextureRGBAXLUAAZBBlInit);
    KWLoadTextureBlockRGBA16B(D_0D02AA58, 0x00000010, 0x00000010);
    D_80183E80[1] = KWLookCamera(sp2C.Wheel1Pos[0], sp2C.Wheel1Pos[2], arg1);
    KwsetTile(sp2C.Wheel1Pos, D_80183E80, 0.15f, VtxKTile16X16);
    D_80183E80[1] = KWLookCamera(sp2C.Wheel2Pos[0], sp2C.Wheel2Pos[2], arg1);
    KwsetTile(sp2C.Wheel2Pos, D_80183E80, 0.15f, VtxKTile16X16);
    D_80183E80[1] = KWLookCamera(sp2C.Wheel3Pos[0], sp2C.Wheel3Pos[2], arg1);
    KwsetTile(sp2C.Wheel3Pos, D_80183E80, 0.15f, VtxKTile16X16);
    D_80183E80[1] = KWLookCamera(sp2C.Wheel4Pos[0], sp2C.Wheel4Pos[2], arg1);
    KwsetTile(sp2C.Wheel4Pos, D_80183E80, 0.15f, VtxKTile16X16);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KwdisplayBombshadow(s32 bombIndex) {
    Mat4 mat;
    BombKart* temp_v0;

    temp_v0 = &Bom[bombIndex];
    D_80183E50[0] = temp_v0->BombPos[0];
    D_80183E50[1] = temp_v0->YPos + 1.0;
    D_80183E50[2] = temp_v0->BombPos[2];
    KwcreateRotationMatrix(mat, BomBump[bombIndex].BumpZX, D_80183E50, 0U, 0.5f);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], mat);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);
    gSPDisplayList(DisplayListHead++, ShadowModel2);
}

void KwdisplayBombkart(s32 CameraId) {
    Camera* Camera;
    s32 ObjectIndex;
    s32 temp_s4;
    s32 i;
    s32 State;
    BombKart* var_s1_2;

    if (GameState == 5) {
        CameraId = 0;
    }
    Camera = &Camera1[CameraId];
    if (CameraId == PLAYER_ONE) {
        for (i = 0; i < NUM_BOMB_KARTS_VERSUS; i++) {
            ObjectIndex = Bomballocptr[i];
            if (KwanmBitcheckflag(ObjectIndex, 0x00200000) != 0) {
                Bom[i].Unk4A = 0;
            } else if (GameState != 5) {
                Bom[i].Unk4A = 1;
            }
            KwanmClrflag(ObjectIndex, 0x00200000);
        }
    }

    for (i = 0; i < NUM_BOMB_KARTS_VERSUS; i++) {
        var_s1_2 = &Bom[i];
        // huh???
        State = var_s1_2->State;
        if (var_s1_2->State != BOMB_STATE_INACTIVE) {
            ObjectIndex = Bomballocptr[i];
            DynamicObjects[ObjectIndex].pos[0] = var_s1_2->BombPos[0];
            DynamicObjects[ObjectIndex].pos[1] = var_s1_2->BombPos[1];
            DynamicObjects[ObjectIndex].pos[2] = var_s1_2->BombPos[2];
            temp_s4 = KwcheckVisArea(ObjectIndex, CameraId, 0x31C4U, 0x000001F4);
            if (KwanmBitcheckflag(ObjectIndex, VISIBLE) != 0) {
                KwanmSetflag(ObjectIndex, 0x00200000);
                D_80183E80[0] = 0;
                D_80183E80[1] = KWLookCamera(var_s1_2->BombPos[0], var_s1_2->BombPos[2], Camera->CameraPos);
                D_80183E80[2] = 0x8000;
                KwdisplayBombbodyBt(ObjectIndex, CameraId, 0x000000FF);
                KwdisplayBombtire(i, Camera->CameraPos);
                if (((u32) temp_s4 < 0x4E21U) && (State != BOMB_STATE_EXPLODED)) {
                    KwdisplayBombshadow(i);
                }
            }
        }
    }
}

UNUSED void func_80057330(void) {
}

UNUSED void KwdisplayTyphoon(void) {

    gSPDisplayList(DisplayListHead++, KWTextureRGBAXLUAAZBBlInit);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BOTH);
    gSPDisplayList(DisplayListHead++, D_0D007AE0);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

UNUSED void func_800573BC(void) {
}

UNUSED void func_800573C4(void) {
}

UNUSED void func_800573CC(void) {
}
UNUSED void func_800573D4(void) {
}

UNUSED void func_800573DC(void) {
}

void Kwprint8X8(s32 x, s32 y, s8 str) {
    KWRectangle(x, y, 8, 8, (((str % 16) * 8) << 16) >> 16, (((unsigned short) (str / 16)) << 19) >> 16,
                             0);
}

void KaigyouCheck(s32* x, s32* y) {
    *x += 8;
    if (*x >= 296) {
        *x = 20;
        *y += 8;
    }
}

void KwprintSub(s32* x, s32* y, char* Arg2) {
    *x += 20;
    *y += 20;

    while (*Arg2 != '\0') {
        if (D_800E5628[(s32) *Arg2] >= 0) {
            Kwprint8X8(*x, *y, D_800E5628[(s32) *Arg2]);
        }
        KaigyouCheck(x, y);
        Arg2++;
    }
}

void PrintNumber(s32* x, s32* y, s32 Number, u32 numDigits) {
    s32 n;
    s8* ptr;
    s8 remainder;

    KaigyouCheck(x, y);
    n = Number;
    if (n < 0) {
        Kwprint8X8(*x, *y, D_800E5628[0x2D]);
        KaigyouCheck(x, y);
        n = -Number;
    }

    *D_801657B8 = -1;
    ptr = D_801657B8;
    if (n != 0) {
        while (n != 0) {
            remainder = n % numDigits;
            *++ptr = remainder;
            n = n / numDigits;
        }
    } else {
        *++ptr = 0;
    }

    do {
        Kwprint8X8(*x, *y, *ptr--);
        KaigyouCheck(x, y);
    } while (*ptr != -1);
}

/**
 * 801657B8[] does nothing? 0xFF a mask?
 * Index zero is a Null/0xFF Flag.
 * The other indexes increment 0-9
 * The final Index (10) increments the tenth digit.
 */
void KwprintnumberUnsignSub(s32* x, s32* y, s32 pathCount, u32 numDigits) {
    s8* ptr;
    s32 Count;
    s8 remainder;

    KaigyouCheck(x, y);
    *D_801657B8 = -1;
    ptr = D_801657B8;
    Count = pathCount;
    if (Count != 0) {
        while (Count != 0) {
            // Retrives ones digit (31 outputs 1).
            remainder = Count % numDigits;
            *++ptr = remainder;
            // Retrieves tens digit (31 outputs 3).
            Count = Count / numDigits;
        }
    } else {
        *++ptr = 0;
    }

    do {
        Kwprint8X8(*x, *y, *ptr--);
        KaigyouCheck(x, y);
    } while (*ptr != -1);
}

UNUSED void func_80057708() {
}

void LoadFont(void) {
    gSPDisplayList(DisplayListHead++, D_0D008108);
    gSPDisplayList(DisplayListHead++, D_0D008080);
    gDPSetAlphaCompare(DisplayListHead++, G_AC_THRESHOLD);
}

void KwprintmodeFinish(void) {
    gSPDisplayList(DisplayListHead++, D_0D007EB8);
}

void PrintString(s32 xPos, s32 YPos, char* str) {
    KwprintSub(&xPos, &YPos, str);
}

void PrintStringNumber(s32 arg0, s32 arg1, char* Arg2, s32 Arg3) {
    KwprintSub(&arg0, &arg1, Arg2);
    PrintNumber(&arg0, &arg1, Arg3, 10);
}

UNUSED void PrintStringUnsignedNumber(s32 arg0, s32 arg1, char* Arg2, u32 Arg3) {
    KwprintSub(&arg0, &arg1, Arg2);
    KwprintnumberUnsignSub(&arg0, &arg1, Arg3, 10);
}

UNUSED void PrintStringHex(s32 arg0, s32 arg1, char* Arg2, u32 Arg3) {
    KwprintSub(&arg0, &arg1, Arg2);
    PrintNumber(&arg0, &arg1, Arg3, 16);
    Kwprint8X8(arg0, arg1, D_800E5628[0x48]);
}

UNUSED void PrintStringUnsignedHex(s32 arg0, s32 arg1, char* Arg2, u32 Arg3) {
    KwprintSub(&arg0, &arg1, Arg2);
    KwprintnumberUnsignSub(&arg0, &arg1, Arg3, 16);
    Kwprint8X8(arg0, arg1, D_800E5628[0x48]);
}

UNUSED void PrintStringBinary(s32 arg0, s32 arg1, char* Arg2, u32 Arg3) {
    KwprintSub(&arg0, &arg1, Arg2);
    PrintNumber(&arg0, &arg1, Arg3, 2);
    Kwprint8X8(arg0, arg1, D_800E5628[0x42]);
}

UNUSED void PrintStringUnsignedBinary(s32 arg0, s32 arg1, char* Arg2, u32 Arg3) {
    KwprintSub(&arg0, &arg1, Arg2);
    KwprintnumberUnsignSub(&arg0, &arg1, Arg3, 2);
    Kwprint8X8(arg0, arg1, D_800E5628[0x42]);
}

UNUSED void Kwfprint(s32 arg0, s32 arg1, char* Arg2) {
    LoadFont();
    KwprintSub(&arg0, &arg1, Arg2);
    KwprintmodeFinish();
}

void Kwfprintd(s32 arg0, s32 arg1, char* Arg2, u32 Arg3) {
    LoadFont();
    KwprintSub(&arg0, &arg1, Arg2);
    PrintNumber(&arg0, &arg1, Arg3, 10);
    KwprintmodeFinish();
}

void Kwfprintud(s32 x, s32 y, char* str, u32 Arg3) {
    LoadFont();
    KwprintSub(&x, &y, str);
    KwprintnumberUnsignSub(&x, &y, Arg3, 10);
    KwprintmodeFinish();
}

UNUSED void Kwfprinth(s32 arg0, s32 arg1, char* Arg2, u32 Arg3) {
    LoadFont();
    KwprintSub(&arg0, &arg1, Arg2);
    PrintNumber(&arg0, &arg1, Arg3, 16);
    Kwprint8X8(arg0, arg1, D_800E5628[0x48]);
    KwprintmodeFinish();
}

UNUSED void Kwfprintuh(s32 arg0, s32 arg1, char* Arg2, u32 Arg3) {
    LoadFont();
    KwprintSub(&arg0, &arg1, Arg2);
    KwprintnumberUnsignSub(&arg0, &arg1, Arg3, 16);
    Kwprint8X8(arg0, arg1, D_800E5628[0x48]);
    KwprintmodeFinish();
}

UNUSED void Kwfprintb(s32 arg0, s32 arg1, char* Arg2, u32 Arg3) {
    LoadFont();
    KwprintSub(&arg0, &arg1, Arg2);
    PrintNumber(&arg0, &arg1, Arg3, 2);
    Kwprint8X8(arg0, arg1, D_800E5628[0x42]);
    KwprintmodeFinish();
}

UNUSED void Kwfprintub(s32 arg0, s32 arg1, char* Arg2, u32 Arg3) {
    LoadFont();
    KwprintSub(&arg0, &arg1, Arg2);
    KwprintnumberUnsignSub(&arg0, &arg1, Arg3, 2);
    Kwprint8X8(arg0, arg1, D_800E5628[0x42]);
    KwprintmodeFinish();
}

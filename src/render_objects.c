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
#include "OverKartHooks.h"

void kwset_tile(Vec3f pos, Vec3su orientation, f32 scale, Vtx* vtx) {
    KWCreateAffine3D(pos, orientation, scale);
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_rectangle_display);
}

void kwpolygon3D(Vec3f pos, Vec3su orientation, f32 scale, Gfx* gfx) {
    KWCreateAffine3D(pos, orientation, scale);
    gSPDisplayList(gDisplayListHead++, D_0D0077A0);
    gSPDisplayList(gDisplayListHead++, gfx);
}

UNUSED void kwpolygon3D_both(Vec3f pos, Vec3su orientation, f32 arg2, Gfx* gfx) {
    KWCreateAffine3D(pos, orientation, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D0077A0);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    gSPDisplayList(gDisplayListHead++, gfx);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void kwpolygon3D_slight(Vec3f arg0, Vec3su arg1, f32 arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D0077D0);
    gSPDisplayList(gDisplayListHead++, gfx);
}

UNUSED void kwpolygon3D_tex_flight(Vec3f arg0, Vec3su arg1, f32 arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D0077F8);
    gSPDisplayList(gDisplayListHead++, gfx);
}

UNUSED void kwpolygon3D_tex_slight(Vec3f arg0, Vec3su arg1, f32 arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007828);
    gSPDisplayList(gDisplayListHead++, gfx);
}

UNUSED void kwpolygon3D_tex_slight_both(Vec3f arg0, Vec3su arg1, f32 arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007828);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    gSPDisplayList(gDisplayListHead++, gfx);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void kwpolygon3D_iceblock(Vec3f arg0, Vec3su arg1, f32 arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007850);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    gSPDisplayList(gDisplayListHead++, gfx);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void kwpolygon3D_icekage(Vec3f arg0, Vec3su arg1, f32 arg2, Gfx* gfx, s32 arg4) {
    KWCreateAffine3D_IceKage(arg0, arg1, arg2);

    gSPDisplayList(gDisplayListHead++, D_0D007878);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, 0xFF, 0xFF, arg4);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    gSPDisplayList(gDisplayListHead++, gfx);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

UNUSED void kwpolygon3D_shade(Vec3f arg0, Vec3su arg1, f32 arg2, Gfx* gfx) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D0078A0);
    gSPDisplayList(gDisplayListHead++, gfx);
}

UNUSED void kwshade_triangle(s32 arg0, s32 arg1, u16 arg2, f32 arg3, Vtx* vtx) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D0078A0);
    gSPVertex(gDisplayListHead++, vtx, 3, 0);
    gSPDisplayList(gDisplayListHead++, D_0D006930);
}

UNUSED void kwshade_tile(s32 arg0, s32 arg1, u16 arg2, f32 arg3, Vtx* vtx) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D0078A0);
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_rectangle_display);
}

UNUSED void kwshade_xlu_tile(s32 arg0, s32 arg1, u16 arg2, f32 arg3, Vtx* vtx, s32 arg5) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D0078A0);

    gDPSetRenderMode(gDisplayListHead++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    vtx[0].v.cn[3] = arg5;
    vtx[1].v.cn[3] = arg5;
    vtx[2].v.cn[3] = arg5;
    vtx[3].v.cn[3] = arg5;
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_rectangle_display);
}

UNUSED void kw_shutter1(s32 arg0, s32 arg1, u16 arg2, f32 arg3, Vtx* vtx, s32 arg5) {
    vtx[1].v.ob[0] = arg5;
    vtx[2].v.ob[0] = arg5;
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D0078A0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_rectangle_display);
}

UNUSED void kw_shutter2(s32 arg0, s32 arg1, u16 arg2, f32 arg3, Vtx* vtx, s32 arg5) {
    vtx[0].v.ob[0] = arg5;
    vtx[3].v.ob[0] = arg5;
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D0078A0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gSPVertex(gDisplayListHead++, vtx, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_rectangle_display);
}

s32 kwget_maskval(s32 arg0) {
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

void kwloadtextureblock_rgba_32b(u8* texture, s32 width, s32 height) {
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, width, height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void kwloadtexturetile_rgba_32b(u8* texture, s32 width, s32 height) {
    gDPLoadTextureTile(gDisplayListHead++, texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, width, height, 0, 0, width - 1,
                       height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
}

void KWLoadTextureBlockRGBA16B(u16* texture, s32 width, s32 height) {
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, height, 0,
                        G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void kwloadtextureblock_rgba_16b_mr(u16* texture, s32 width, s32 height, s32 someMask) {
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, height, 0,
                        G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, someMask, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void kwloadtexturetile_rgba_16b(u16* texture, s32 width, s32 height) {
    gDPLoadTextureTile(gDisplayListHead++, texture, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, height, 0, 0, width - 1,
                       height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
}

void kwloadtextureblock_ia_16b(u8* texture, s32 width, s32 height) {
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_IA, G_IM_SIZ_16b, width, height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void kwloadtexturetile_ia_16b(u8* texture, s32 width, s32 height) {
    gDPLoadTextureTile(gDisplayListHead++, texture, G_IM_FMT_IA, G_IM_SIZ_16b, width, height, 0, 0, width - 1,
                       height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
}

void kwloadtextureblock_ia_8b(u8* texture, s32 width, s32 height) {
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_IA, G_IM_SIZ_8b, width, height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void kwloadtexturetile_ia_8b(u8* texture, s32 width, s32 height) {
    gDPLoadTextureTile(gDisplayListHead++, texture, G_IM_FMT_IA, G_IM_SIZ_8b, width, height, 0, 0, width - 1,
                       height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
}

void kwloadtextureblock_a_8b(u8* texture, s32 width, s32 height) {
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_I, G_IM_SIZ_8b, width, height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void kwloadtextureblock_ia_4b(u8* texture, s32 width, s32 height) {
    // This macro ought to be equivalent to the block of macros below but it doesn't match
    // See comment above the `gDPLoadBlock` macro
    // gDPLoadTextureBlock_4b(gDisplayListHead++, texture, G_IM_FMT_I, width, height, 0, G_TX_NOMIRROR | G_TX_CLAMP,
    // G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, texture);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 0, G_TX_RENDERTILE, G_TX_LOADTILE, 0,
               G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
               G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    // The last argument to this macro really should be `CALC_DXT_4b(width)` but that creates a massive diff
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, (((width * height) + 3) >> 2) - 1,
                 ((width / 16) + 2047) / (width / 16));
    gDPPipeSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_4b, (((width >> 1) + 7) >> 3), G_TX_RENDERTILE,
               G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP,
               G_TX_NOMASK, G_TX_NOLOD);
    gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, (width - 1) << G_TEXTURE_IMAGE_FRAC,
                   (height - 1) << G_TEXTURE_IMAGE_FRAC);
}

UNUSED void kwloadtexturetile_ia_4b(u8* texture, s32 width, s32 height) {
    gDPLoadTextureTile_4b(gDisplayListHead++, texture, G_IM_FMT_IA, width, height, 0, 0, width - 1, height - 1, 0,
                          G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                          G_TX_NOLOD);
}

void kwloadtextureblock_i_8b(u8* texture, s32 width, s32 height) {
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_I, G_IM_SIZ_8b, width, height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void KWLoadTextureBlockI4b(u8* image, s32 width, s32 height) {
    // This macro ought to be equivalent to the block of macros below but it doesn't match
    // See comment above the `gDPLoadBlock` macro
    // gDPLoadTextureBlock_4b(gDisplayListHead++, image, G_IM_FMT_I, width, height, 0, G_TX_NOMIRROR | G_TX_CLAMP,
    // G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 1, image);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 0, G_TX_RENDERTILE, G_TX_LOADTILE, 0,
               G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
               G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    // The last argument to this macro really should be `CALC_DXT_4b(width)` but that creates a massive diff
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, (((width * height) + 3) >> 2) - 1,
                 ((width / 16) + 2047) / (width / 16));
    gDPPipeSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_4b, (((width >> 1) + 7) >> 3), G_TX_RENDERTILE, G_TX_RENDERTILE,
               0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
               G_TX_NOLOD);
    gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, (width - 1) << G_TEXTURE_IMAGE_FRAC,
                   (height - 1) << G_TEXTURE_IMAGE_FRAC);
}

// Appears to be a complete copy of `kwloadtextureblock_a_4b`?
void kwloadtextureblock_a_4b(u8* image, s32 width, s32 height) {
    // This macro ought to be equivalent to the block of macros below but it doesn't match
    // See comment above the `gDPLoadBlock` macro
    // gDPLoadTextureBlock_4b(gDisplayListHead++, image, G_IM_FMT_I, width, height, 0, G_TX_NOMIRROR | G_TX_CLAMP,
    // G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 1, image);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 0, G_TX_RENDERTILE, G_TX_LOADTILE, 0,
               G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
               G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    // The last argument to this macro really should be `CALC_DXT_4b(width)` but that creates a massive diff
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, (((width * height) + 3) >> 2) - 1,
                 ((width / 16) + 2047) / (width / 16));
    gDPPipeSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_4b, (((width >> 1) + 7) >> 3), G_TX_RENDERTILE, G_TX_RENDERTILE,
               0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
               G_TX_NOLOD);
    gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, (width - 1) << G_TEXTURE_IMAGE_FRAC,
                   (height - 1) << G_TEXTURE_IMAGE_FRAC);
}

void kwloadtextureblock_a_4b_xflip(u8* image, s32 width, s32 height) {
    // This macro ought to be equivalent to the block of macros below but it doesn't match
    // See comment above the `gDPLoadBlock` macro
    // gDPLoadTextureBlock_4b(gDisplayListHead++, image, G_IM_FMT_I, width, height, 0, G_TX_NOMIRROR | G_TX_CLAMP,
    // G_TX_MIRROR | G_TX_WRAP, G_TX_NOMASK, masks, G_TX_NOLOD, G_TX_NOLOD);
    s32 masks = kwget_maskval(width);

    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 1, image);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_16b, 0, G_TX_RENDERTILE, G_TX_LOADTILE, 0,
               G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_MIRROR | G_TX_WRAP, masks, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    // The last argument to this macro really should be `CALC_DXT_4b(width)` but that creates a massive diff
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, (((width * height) + 3) >> 2) - 1,
                 ((width / 16) + 2047) / (width / 16));
    gDPPipeSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_4b, (((width >> 1) + 7) >> 3), G_TX_RENDERTILE, G_TX_RENDERTILE,
               0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_MIRROR | G_TX_WRAP, masks, G_TX_NOLOD);
    gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, (width - 1) << G_TEXTURE_IMAGE_FRAC,
                   (height - 1) << G_TEXTURE_IMAGE_FRAC);
}

void kwloadtextureblock_ci_8b(u8* texture, s32 width, s32 height) {
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_CI, G_IM_SIZ_8b, width, height, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

void kwloadtextureblock_ci_8b_mr(u8* texture, s32 width, s32 height, s32 someMask) {
    gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_CI, G_IM_SIZ_8b, width, height, 0,
                        G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, someMask, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
}

UNUSED void kwloadtexturetile_ci_8b(u8* texture, s32 width, s32 height) {
    gDPLoadTextureTile(gDisplayListHead++, texture, G_IM_FMT_CI, G_IM_SIZ_8b, width, height, 0, 0, width - 1,
                       height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                       G_TX_NOLOD, G_TX_NOLOD);
}

UNUSED void kwloadtextureblock_multi_16b(u8* image1, u8* image2, s32 width, s32 height) {
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL1, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);

    gDPLoadMultiBlock(gDisplayListHead++, image2, 0x100, G_TX_RENDERTILE, G_IM_FMT_I, G_IM_SIZ_8b, width, height, 0,
                      G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                      G_TX_NOLOD);
    gDPLoadTextureBlock(gDisplayListHead++, image1, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, height, 0,
                        G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    gDPSetTile(gDisplayListHead++, G_IM_FMT_I, G_IM_SIZ_8b, (width + 7) >> 3, 0x0100, 1, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    ;
    gDPSetTileSize(gDisplayListHead++, 1, 0, 0, (width - 1) << G_TEXTURE_IMAGE_FRAC,
                   (height - 1) << G_TEXTURE_IMAGE_FRAC);
}

void kwsettexturetile(Vtx* arg0) {
    gSPVertex(gDisplayListHead++, arg0, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_rectangle_display);
}

void kwsettexturetriangle(Vtx* arg0) {
    gSPVertex(gDisplayListHead++, arg0, 3, 0);
    gSPDisplayList(gDisplayListHead++, D_0D006930);
}

UNUSED void kwtile3D_shade(Vec3f arg0, Vec3su arg1, f32 arg2, Vtx* arg3) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D0078A0);
    gSPVertex(gDisplayListHead++, arg3, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_rectangle_display);
}

UNUSED void kwline3D_shade(Vec3f arg0, Vec3su arg1, f32 arg2, Vtx* arg3) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D0078D0);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    gSPVertex(gDisplayListHead++, arg3, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_rectangle_display);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void KWTextureRGBA_SubPT(u16* texture, Vtx* arg1, s32 width, s32 arg3, s32 height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u16* img = texture;

    for (heightIndex = 0; heightIndex < arg3 / height; heightIndex++) {
        KWLoadTextureBlockRGBA16B(img, width, height);
        kwsettexturetile(&arg1[vertexIndex]);
        img += width * height;
        vertexIndex += 4;
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureRGBA_SubBL(u16* texture, Vtx* arg1, s32 width, s32 arg3, s32 height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u16* img = texture;

    for (heightIndex = 0; heightIndex < arg3 / height; heightIndex++) {
        KWLoadTextureBlockRGBA16B(img, width, height);
        kwsettexturetile(&arg1[vertexIndex]);
        img += width * (height - 1);
        vertexIndex += 4;
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureRGBA_SubBLMR(u16* texture, Vtx* arg1, s32 width, s32 arg3, s32 height, s32 someMask) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u16* img = texture;

    for (heightIndex = 0; heightIndex < arg3 / height; heightIndex++) {
        kwloadtextureblock_rgba_16b_mr(img, width, height, someMask);
        kwsettexturetile(&arg1[vertexIndex]);
        img += width * (height - 1);
        vertexIndex += 4;
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

//! @todo tlut/texture unconfirmed. This could be texture1 and texture2
UNUSED void KWTextureRGBMA_SubBL(u8* tlut, u8* texture, Vtx* arg2, s32 width, s32 arg4, s32 height) {
    s32 var_s0 = 0;
    u8* img1 = tlut;
    u8* img2 = texture;
    s32 temp_lo_2;
    s32 var;
    s32 i;

    gSPDisplayList(gDisplayListHead++, D_0D008138);

    for (i = 0; i < arg4 / height; i++) {
        kwloadtextureblock_multi_16b(img1, img2, width, height);
        kwsettexturetile(&arg2[var_s0]);
        var = height - 1;
        temp_lo_2 = (width * var);
        img1 += temp_lo_2 * 2;
        img2 += temp_lo_2;
        var_s0 += 4;
    }
    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
    gSPDisplayList(gDisplayListHead++, D_0D008120);
}

void KWTextureRGBA32_SubPT(u8* texture, Vtx* arg1, s32 width, s32 arg3, s32 height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = texture;

    for (heightIndex = 0; heightIndex < arg3 / height; heightIndex++) {
        kwloadtextureblock_rgba_32b(img, width, height);
        kwsettexturetile(&arg1[vertexIndex]);
        img += width * height * 4;
        vertexIndex += 4;
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureRGBA32_SubBL(u8* texture, Vtx* arg1, s32 width, s32 arg3, s32 height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = texture;

    for (heightIndex = 0; heightIndex < arg3 / height; heightIndex++) {
        kwloadtextureblock_rgba_32b(img, width, height);
        kwsettexturetile(&arg1[vertexIndex]);
        img += width * (height - 1) * 4;
        vertexIndex += 4;
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTexture2DRGBA(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u16* texture, Vtx* arg5, s32 arg6, s32 arg7, UNUSED s32 arg8,
                   s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007928);
    KWTextureRGBA_SubPT(texture, arg5, arg6, arg7, arg9);
}

void kwtexture2D_flash_rgba_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u16* texture, Vtx* arg5, s32 arg6, s32 arg7, UNUSED s32 arg8,
                   s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007968);
    IceMode(kwflash_r, kwflash_g, kwflash_b, 128, 128, 128, 255);
    KWTextureRGBA_SubPT(texture, arg5, arg6, arg7, arg9);
}

UNUSED void kwtexture2D_rgba_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u16* texture, Vtx* arg5, s32 arg6, s32 arg7,
                          UNUSED s32 arg8, s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007948);
    KWTextureRGBA_SubBL(texture, arg5, arg6, arg7, arg9);
}

UNUSED void kwtexture2D_rgba_aa_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u16* texture, Vtx* arg5, s32 arg6, s32 arg7,
                          UNUSED s32 arg8, s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D0079A8);
    KWTextureRGBA_SubBL(texture, arg5, arg6, arg7, arg9);
}

void kwtexture2D_rgba_xlu(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 arg4, u16* texture, Vtx* arg6, s32 arg7, s32 arg8,
                   UNUSED s32 arg9, s32 argA) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D0079E8);

    SubAlphaMode(arg4);
    KWTextureRGBA_SubBL(texture, arg6, arg7, arg8, argA);
}

UNUSED void kwtexture2D_rgbma_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                          UNUSED s32 arg9, s32 argA) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007948);
    KWTextureRGBMA_SubBL(tlut, texture, arg6, arg7, arg8, argA);
}

void kwtexture2D_triangle_rgba(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u16* texture, Vtx* arg5, s32 arg6, s32 arg7) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007948);
    KWLoadTextureBlockRGBA16B(texture, arg6, arg7);
    kwsettexturetriangle(arg5);
}

UNUSED void KWTexture2DRGBA32PT(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7,
                          UNUSED s32 arg8, s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007968);
    KWTextureRGBA32_SubPT(texture, arg5, arg6, arg7, arg9);
}

UNUSED void KWTexture2DRGBA32BL(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7,
                          UNUSED s32 arg8, s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007948);
    KWTextureRGBA32_SubBL(texture, arg5, arg6, arg7, arg9);
}

void kwtexture3D_rgba_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u16* texture, Vtx* arg4, s32 arg5, s32 arg6, UNUSED s32 arg7,
                   s32 arg8) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007948);
    KWTextureRGBA_SubBL(texture, arg4, arg5, arg6, arg8);
}

UNUSED void kwtexture3D_rgba_aa_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u16* texture, Vtx* arg4, s32 arg5, s32 arg6,
                          UNUSED s32 arg7, s32 arg8) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D0079C8);
    KWTextureRGBA_SubBL(texture, arg4, arg5, arg6, arg8);
}

void kwtexture3D_rgba_aa_zb_bl_mr(Vec3f arg0, Vec3su arg1, f32 arg2, u16* texture, Vtx* arg4, s32 arg5, s32 arg6, UNUSED s32 arg7,
                   s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D0079C8);
    KWTextureRGBA_SubBLMR(texture, arg4, arg5, arg6, arg8, arg9);
}

UNUSED void kwtexture3D_rgba_aa_zb_bl_bt(Vec3f arg0, Vec3su arg1, f32 arg2, u16* texture, Vtx* arg4, s32 arg5, s32 arg6,
                          UNUSED s32 arg7, s32 arg8) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D0079C8);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    KWTextureRGBA_SubBL(texture, arg4, arg5, arg6, arg8);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void kwtexture3D_triangle_rgba(Vec3f arg0, Vec3su arg1, f32 arg2, u16* texture, Vtx* arg4, s32 arg5, s32 arg6) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007948);
    KWLoadTextureBlockRGBA16B(texture, arg5, arg6);
    kwsettexturetriangle(arg4);
}

UNUSED void KWTexture3DRGBA32AAZBBL(Vec3f arg0, Vec3su arg1, f32 arg2, u8* texture, Vtx* arg4, s32 arg5, s32 arg6,
                          UNUSED s32 arg7, s32 arg8) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D0079C8);
    KWTextureRGBA32_SubBL(texture, arg4, arg5, arg6, arg8);
}

UNUSED void kwtexture8x8_rgba_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u16* texture) {
    kwtexture2D_rgba_bl(arg0, arg1, arg2, arg3, texture, common_vtx_player_minimap_icon, 8, 8, 8, 8);
}

UNUSED void kwtexture2D_16x16_rgba_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u16* texture) {
    kwtexture2D_rgba_bl(arg0, arg1, arg2, arg3, texture, common_vtx_rectangle, 16, 16, 16, 16);
}

UNUSED void kwtexture3D_16x16_rgba_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u16* texture) {
    kwtexture3D_rgba_bl(arg0, arg1, arg2, texture, common_vtx_rectangle, 16, 16, 16, 16);
}

UNUSED void kwtexture_triangle_16x16_rgba(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u16* texture, Vtx* arg5) {
    kwtexture2D_triangle_rgba(arg0, arg1, arg2, arg3, texture, arg5, 16, 16);
}

UNUSED void kwtexture3D_triangle_16x16_rgba(Vec3f arg0, Vec3su arg1, f32 arg2, u16* texture, Vtx* arg4) {
    kwtexture3D_triangle_rgba(arg0, arg1, arg2, texture, arg4, 16, 16);
}

UNUSED void kwtexture2D_32x32_rgba(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u16* texture) {
    kwtexture2D_rgba_bl(arg0, arg1, arg2, arg3, texture, D_0D005AE0, 32, 32, 32, 32);
}

UNUSED void kwtexture2D_64x32_rgba_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u16* texture) {
    kwtexture2D_rgba_bl(arg0, arg1, arg2, arg3, texture, D_0D005FB0, 64, 32, 64, 32);
}

UNUSED void kwtexture3D_64x32_rgba_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u16* texture) {
    kwtexture3D_rgba_bl(arg0, arg1, arg2, texture, D_0D005FB0, 64, 32, 64, 32);
}

UNUSED void kwtexture2D_64x64_rgba_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u16* texture) {
    kwtexture2D_rgba_bl(arg0, arg1, arg2, arg3, texture, common_vtx_hedgehog, 64, 64, 64, 32);
}

UNUSED void kwtexture2D_64x64_rgba_xlu(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 arg4, u16* texture) {
    kwtexture2D_rgba_xlu(arg0, arg1, arg2, arg3, arg4, texture, common_vtx_hedgehog, 64, 64, 64, 32);
}

void kwloadtexture_ci8_aa_zb_bl(u8* tlut, u8* texture, s32 width, s32 height) {
    gSPDisplayList(gDisplayListHead++, D_0D007D78);
    gDPLoadTLUT_pal256(gDisplayListHead++, tlut);
    kwloadtextureblock_ci_8b(texture, width, height);
}

void kwloadtexture_ci8_aa_zb_bl_mr(u8* tlut, u8* arg1, s32 arg2, s32 arg3, s32 arg4) {
    gSPDisplayList(gDisplayListHead++, D_0D007D78);
    gDPLoadTLUT_pal256(gDisplayListHead++, tlut);
    kwloadtextureblock_ci_8b_mr(arg1, arg2, arg3, arg4);
}

void KWTextureCI8_SubPT(u8* tlut, u8* texture, Vtx* arg2, UNUSED s32 arg3, s32 arg4, s32 width, s32 height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = texture;

    gDPLoadTLUT_pal256(gDisplayListHead++, tlut);
    for (heightIndex = 0; heightIndex < arg4 / height; heightIndex++) {
        kwloadtextureblock_ci_8b(img, width, height);
        gSPVertex(gDisplayListHead++, &arg2[vertexIndex], 4, 0);
        gSPDisplayList(gDisplayListHead++, common_rectangle_display);
        img += width * height;
        vertexIndex += 4;
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureCI8_SubBL(u8* tlut, u8* texture, Vtx* arg2, UNUSED s32 arg3, s32 arg4, s32 width,
                                    s32 height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = texture;

    gDPLoadTLUT_pal256(gDisplayListHead++, tlut);
    for (heightIndex = 0; heightIndex < arg4 / height; heightIndex++) {
        kwloadtextureblock_ci_8b(img, width, height);
        gSPVertex(gDisplayListHead++, &arg2[vertexIndex], 4, 0);
        gSPDisplayList(gDisplayListHead++, common_rectangle_display);
        img += width * (height - 1);
        vertexIndex += 4;
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureCI8_SubBLMR(u8* tlut, u8* texture, Vtx* arg2, UNUSED s32 arg3, s32 arg4, s32 width, s32 height, s32 someMask) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = texture;

    gDPLoadTLUT_pal256(gDisplayListHead++, tlut);
    for (heightIndex = 0; heightIndex < arg4 / height; heightIndex++) {
        kwloadtextureblock_ci_8b_mr(img, width, height, someMask);
        gSPVertex(gDisplayListHead++, &arg2[vertexIndex], 4, 0);
        gSPDisplayList(gDisplayListHead++, common_rectangle_display);
        img += width * (height - 1);
        vertexIndex += 4;
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureCI8_SubBLCutX(u8* tlut, u8* texture, Vtx* arg2, s32 arg3, s32 width, s32 height) {
    s32 heightIndex;
    s32 vertexIndex = 0;
    u8* img = texture;

    gDPLoadTLUT_pal256(gDisplayListHead++, tlut);
    for (heightIndex = 0; heightIndex < arg3 / height; heightIndex++) {
        // Something seems off about arguments here, but if it matches it matches
        kwloadtextureblock_ci_8b(img, height, width);
        gSPVertex(gDisplayListHead++, &arg2[vertexIndex], 4, 0);
        gSPDisplayList(gDisplayListHead++, common_rectangle_display);
        img += height * width;
        vertexIndex += 4;
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void kwtexture2D_ci8_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007CB8);
    KWTextureCI8_SubPT(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

void KWTexture2DCI8BL(s32 x, s32 y, u16 angle, f32 size, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8, s32 arg9,
                   s32 argA) {
    kwcreateaffine2D(x, y, angle, size);
    gSPDisplayList(gDisplayListHead++, D_0D007CD8);
    KWTextureCI8_SubBL(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

void kwtexture2D_ci8_av(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007CF8);
    KWTextureCI8_SubBL(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

void KWTexture2DCI8AAPT(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007D18);
    KWTextureCI8_SubPT(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

void kwtexture2D_ci8_aa_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007D38);
    KWTextureCI8_SubBL(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

UNUSED void kwtexture2D_ci8_aa_bl_cutx(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                          s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007D38);
    KWTextureCI8_SubBLCutX(tlut, texture, arg6, arg7, arg8, arg9);
}

void KWTexture2DCI8XLUPT(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 arg4, u8* tlut, u8* texture, Vtx* arg7, s32 arg8,
                   s32 arg9, s32 argA, s32 argB) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007DB8);
    SubAlphaMode(arg4);
    KWTextureCI8_SubPT(tlut, texture, arg7, arg8, arg9, argA, argB);
}

UNUSED void KWTexture2DCI8XLUBL(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 arg4, u8* tlut, u8* texture, Vtx* arg7, s32 arg8,
                          s32 arg9, s32 argA, s32 argB) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007DD8);
    SubAlphaMode(arg4);
    KWTextureCI8_SubBL(tlut, texture, arg7, arg8, arg9, argA, argB);
}

void KWTexture2DCI8AAXLUPT(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 arg4, u8* tlut, u8* texture, Vtx* arg7, s32 arg8,
                   s32 arg9, s32 argA, s32 argB) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007E38);
    SubAlphaMode(arg4);
    KWTextureCI8_SubPT(tlut, texture, arg7, arg8, arg9, argA, argB);
}

UNUSED void kwtexture2D_ci8_aa_xlu_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 arg4, u8* tlut, u8* texture, Vtx* arg7, s32 arg8,
                          s32 arg9, s32 argA, s32 argB) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007E58);
    SubAlphaMode(arg4);
    KWTextureCI8_SubBL(tlut, texture, arg7, arg8, arg9, argA, argB);
}

UNUSED void kwtexture3D_ci8_pt(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5, s32 arg6, s32 arg7,
                          s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007CB8);
    KWTextureCI8_SubPT(tlut, texture, arg5, arg6, arg7, arg8, arg9);
}

void kwtexture3D_ci8_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007CD8);
    KWTextureCI8_SubBL(tlut, texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void kwtexture3D_ci8_aa_pt(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5, s32 arg6, s32 arg7,
                          s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007D18);
    KWTextureCI8_SubPT(tlut, texture, arg5, arg6, arg7, arg8, arg9);
}

void kwtexture3D_ci8_aa_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007D38);
    KWTextureCI8_SubBL(tlut, texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void kwtexture3D_ci8_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5, s32 arg6, s32 arg7,
                          s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007D58);
    KWTextureCI8_SubBL(tlut, texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void kwtexture3D_ci8_aa_zb_pt(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5, s32 arg6, s32 arg7,
                          s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007D98);
    KWTextureCI8_SubPT(tlut, texture, arg5, arg6, arg7, arg8, arg9);
}

void kwtexture3D_ci8_aa_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5, s32 arg6, s32 arg7,
                        s32 arg8, s32 arg9) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007D78);
    KWTextureCI8_SubBL(tlut, texture, arg5, arg6, arg7, arg8, arg9);
}

void kwtexture3D_ci8_aa_zb_bl_mr(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007D78);
    KWTextureCI8_SubBLMR(tlut, texture, arg5, arg6, arg7, arg8, arg9, argA);
}

UNUSED void kwtexture3D_ci8_aa_zb_bl_cutx(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5, s32 arg6, s32 arg7,
                          s32 arg8) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007D78);
    KWTextureCI8_SubBLCutX(tlut, texture, arg5, arg6, arg7, arg8);
}

UNUSED void kwtexture3D_ci8_xlu_pt(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7,
                          s32 arg8, s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007DB8);
    SubAlphaMode(arg3);
    KWTextureCI8_SubPT(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

void kwtexture3D_ci8_xlu_bl(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007DD8);
    SubAlphaMode(arg3);
    KWTextureCI8_SubBL(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

UNUSED void kwtexture3D_ci8_xlu_aa_pt(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7,
                          s32 arg8, s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007E38);
    SubAlphaMode(arg3);
    KWTextureCI8_SubPT(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

void kwtexture3D_ci8_xlu_aa_bl(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007E58);
    SubAlphaMode(arg3);
    KWTextureCI8_SubBL(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

void kwtexture3D_ci8_xlu_zb_pt(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007DF8);
    SubAlphaMode(arg3);
    KWTextureCI8_SubPT(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

void kwtexture3D_ci8_xlu_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007E18);
    SubAlphaMode(arg3);
    KWTextureCI8_SubBL(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

void kwtexture3D_ci8_xlu_aa_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007E98);
    SubAlphaMode(arg3);
    KWTextureCI8_SubBL(tlut, texture, arg6, arg7, arg8, arg9, argA);
}

void kwtexture3D_ci8_pcl_aa_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007E98);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);

    SubAlphaMode(arg3);
    KWTextureCI8_SubBL(tlut, texture, arg6, arg7, arg8, arg9, argA);

    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
}

UNUSED void kwtexture2D_24x48_ci8_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture2D_ci8_pt(arg0, arg1, arg2, arg3, tlut, texture, arg6, 24, 48, 24, 48);
}

UNUSED void kwtexture32x32_ci8_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture2D_ci8_pt(arg0, arg1, arg2, arg3, tlut, texture, arg6, 32, 32, 32, 32);
}

UNUSED void kwtexture3D_48x40_ci8_xlu_aa_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture3D_ci8_xlu_aa_zb_bl(arg0, arg1, arg2, arg3, tlut, texture, arg6, 48, 48, 48, 40);
}

UNUSED void kwtexture2D_48x48_ci8_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture2D_ci8_pt(arg0, arg1, arg2, arg3, tlut, texture, arg6, 48, 48, 48, 48);
}

UNUSED void kwtexture2D_64x32_ci8_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture2D_ci8_pt(arg0, arg1, arg2, arg3, tlut, texture, arg6, 64, 32, 64, 32);
}

UNUSED void kwtexture2D_64x64_ci8_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture2D_ci8_pt(arg0, arg1, arg2, arg3, tlut, texture, arg6, 64, 64, 64, 32);
}

UNUSED void kwtexture2D_64x64_ci8_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    KWTexture2DCI8BL(arg0, arg1, arg2, arg3, tlut, texture, arg6, 64, 64, 64, 32);
}

UNUSED void kwtexture2D_64x64_ci8_av(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture2D_ci8_av(arg0, arg1, arg2, arg3, tlut, texture, arg6, 64, 64, 64, 32);
}

UNUSED void kwtexture2D_64x64_ci8_aa_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    KWTexture2DCI8AAPT(arg0, arg1, arg2, arg3, tlut, texture, arg6, 64, 64, 64, 32);
}

UNUSED void kwtexture2D_64x64_ci8_aa_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture2D_ci8_aa_bl(arg0, arg1, arg2, arg3, tlut, texture, arg6, 64, 64, 64, 32);
}

UNUSED void kwtexture2D_64x64_ci8_xlu_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 arg4, u8* tlut, u8* texture, Vtx* arg7) {
    KWTexture2DCI8XLUPT(arg0, arg1, arg2, arg3, arg4, tlut, texture, arg7, 64, 64, 64, 32);
}

UNUSED void kwtexture2D_64x64_ci8_aa_xlu_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 arg4, u8* tlut, u8* texture, Vtx* arg7) {
    KWTexture2DCI8AAXLUPT(arg0, arg1, arg2, arg3, arg4, tlut, texture, arg7, 64, 64, 64, 32);
}

UNUSED void kwtexture3D_64x64_ci8_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5) {
    kwtexture3D_ci8_bl(arg0, arg1, arg2, tlut, texture, arg5, 64, 64, 64, 32);
}

UNUSED void kwtexture3D_64x64_ci8_aa_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5) {
    kwtexture3D_ci8_aa_bl(arg0, arg1, arg2, tlut, texture, arg5, 64, 64, 64, 32);
}

UNUSED void kwtexture3D_64x64_ci8_aa_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5) {
    kwtexture3D_ci8_aa_zb_bl(arg0, arg1, arg2, tlut, texture, arg5, 64, 64, 64, 32);
}

UNUSED void kwtexture3D_64x64_ci8_xlu_bl(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture3D_ci8_xlu_bl(arg0, arg1, arg2, arg3, tlut, texture, arg6, 64, 64, 64, 32);
}

UNUSED void kwtexture3D_64x64_ci8_xlu_aa_bl(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture3D_ci8_xlu_aa_bl(arg0, arg1, arg2, arg3, tlut, texture, arg6, 64, 64, 64, 32);
}

UNUSED void kwtexture3D_64x64_ci8_xlu_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture3D_ci8_xlu_zb_bl(arg0, arg1, arg2, arg3, tlut, texture, arg6, 64, 64, 64, 32);
}

UNUSED void kwtexture3D_64x64_ci8_xlu_aa_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, s32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture3D_ci8_xlu_aa_zb_bl(arg0, arg1, arg2, arg3, tlut, texture, arg6, 64, 64, 64, 32);
}

UNUSED void kwtexture2D_72x48_ci8_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    KWTexture2DCI8BL(arg0, arg1, arg2, arg3, tlut, texture, arg6, 72, 48, 72, 24);
}

UNUSED void kwtexture2D_72x48_ci8_aa_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    KWTexture2DCI8AAPT(arg0, arg1, arg2, arg3, tlut, texture, arg6, 72, 48, 72, 24);
}

UNUSED void kwtexture2D_72x48_ci8_aa_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* tlut, u8* texture, Vtx* arg6) {
    kwtexture2D_ci8_aa_bl(arg0, arg1, arg2, arg3, tlut, texture, arg6, 72, 48, 72, 24);
}

UNUSED void kwtexture3D_72x48_ci8_aa_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u8* tlut, u8* texture, Vtx* arg5) {
    kwtexture3D_ci8_aa_zb_bl(arg0, arg1, arg2, tlut, texture, arg5, 72, 48, 72, 24);
}

void KWTextureIA16_Sub(u16* texture, Vtx* arg1, s32 arg2, s32 arg3, s32 width, s32 height) {
    s32 heightIndex;
    s32 widthIndex;
    s32 vertexIndex = 0;
    u8* img = (u8 *) texture;

    for (heightIndex = 0; heightIndex < arg3 / height; heightIndex++) {
        for (widthIndex = 0; widthIndex < arg2 / width; widthIndex++) {
            kwloadtextureblock_ia_16b(img, width, height);
            gSPVertex(gDisplayListHead++, &arg1[vertexIndex], 4, 0);
            gSPDisplayList(gDisplayListHead++, common_rectangle_display);
            img += width * height * 2;
            vertexIndex += 4;
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void kwtextiledraw_ia16(u8* texture, Vtx* arg1, s32 arg2, s32 arg3, s32 width, s32 height) {
    s32 heightIndex;
    s32 widthIndex;
    s32 vertexIndex = 0;
    u8* img = texture;

    for (heightIndex = 0; heightIndex < arg3 / height; heightIndex++) {
        for (widthIndex = 0; widthIndex < arg2 / width; widthIndex++) {
            kwloadtexturetile_ia_16b(img, width, height);
            gSPVertex(gDisplayListHead++, &arg1[vertexIndex], 4, 0);
            gSPDisplayList(gDisplayListHead++, common_rectangle_display);
            img += width * height * 2;
            vertexIndex += 4;
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureIA8_Sub(u8* texture, Vtx* arg1, s32 arg2, s32 arg3, s32 width, s32 height) {
    s32 heightIndex;
    s32 widthIndex;
    s32 vertexIndex = 0;
    u8* img = texture;

    for (heightIndex = 0; heightIndex < arg3 / height; heightIndex++) {
        for (widthIndex = 0; widthIndex < arg2 / width; widthIndex++) {
            kwloadtextureblock_ia_8b(img, width, height);
            gSPVertex(gDisplayListHead++, &arg1[vertexIndex], 4, 0);
            gSPDisplayList(gDisplayListHead++, common_rectangle_display);
            img += width * height;
            vertexIndex += 4;
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void kwtextiledraw_ia8(u8* texture, Vtx* arg1, s32 arg2, s32 arg3, s32 width, s32 height) {
    s32 heightIndex;
    s32 widthIndex;
    s32 vertexIndex = 0;
    u8* img = texture;

    for (heightIndex = 0; heightIndex < arg3 / height; heightIndex++) {
        for (widthIndex = 0; widthIndex < arg2 / width; widthIndex++) {
            kwloadtexturetile_ia_8b(img, width, height);
            gSPVertex(gDisplayListHead++, &arg1[vertexIndex], 4, 0);
            gSPDisplayList(gDisplayListHead++, common_rectangle_display);
            img += width * height;
            vertexIndex += 4;
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureIA4_Sub(u8* texture, Vtx* arg1, s32 arg2, s32 arg3, s32 width, s32 height) {
    s32 i;
    s32 j;
    s32 var_s2 = 0;
    u8* img = texture;

    for (i = 0; i < arg3 / height; i++) {
        for (j = 0; j < arg2 / width; j++) {

            kwloadtextureblock_ia_4b(img, width, height);
            gSPVertex(gDisplayListHead++, &arg1[var_s2], 4, 0);
            gSPDisplayList(gDisplayListHead++, common_rectangle_display);
            img += (width * height) / 2;
            var_s2 += 4;
        }
    }

    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureA8_Sub(u8* texture, Vtx* arg1, s32 arg2, s32 arg3, s32 width, s32 height) {
    s32 heightIndex;
    s32 widthIndex;
    s32 vertexIndex = 0;
    u8* img = texture;

    for (heightIndex = 0; heightIndex < arg3 / height; heightIndex++) {
        for (widthIndex = 0; widthIndex < arg2 / width; widthIndex++) {
            kwloadtextureblock_i_8b(img, width, height);
            gSPVertex(gDisplayListHead++, &arg1[vertexIndex], 4, 0);
            gSPDisplayList(gDisplayListHead++, common_rectangle_display);
            img += width * height;
            vertexIndex += 4;
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void KWTextureA4_Sub(u8* texture, Vtx* arg1, s32 arg2, s32 arg3, s32 width, s32 height) {
    s32 i;
    s32 j;
    s32 var_s2 = 0;
    u8* img = texture;

    for (i = 0; i < arg3 / height; i++) {
        for (j = 0; j < arg2 / width; j++) {
            KWLoadTextureBlockI4b(img, width, height);
            gSPVertex(gDisplayListHead++, &arg1[var_s2], 4, 0);
            gSPDisplayList(gDisplayListHead++, common_rectangle_display);
            img += (width * height) / 2;
            var_s2 += 4;
        }
    }
    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}

void kwtexture2D_ia16_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A40);
    KWTextureIA16_Sub((u16 *) texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void kwtexture2D_ia16_tile_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                          s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A40);
    kwtextiledraw_ia16(texture, arg5, arg6, arg7, arg8, arg9);
}

void kwtexture2D_ia8_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A40);
    KWTextureIA8_Sub(texture, arg5, arg6, arg7, arg8, arg9);
}

void kwtexture2D_ia8_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A60);
    KWTextureIA8_Sub(texture, arg5, arg6, arg7, arg8, arg9);
}

void kwtexture2D_ia8_aa_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A80);
    KWTextureIA8_Sub(texture, arg5, arg6, arg7, arg8, arg9);
}

void kwtexture2D_ia8_aa_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007AA0);
    KWTextureIA8_Sub(texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void kwtexture2D_ia8_xlu_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 red, s32 green, s32 blue, s32 alpha, u8* texture,
                          Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A40);
    ToumeIMode(red, green, blue, alpha);
    KWTextureIA8_Sub(texture, arg9, argA, argB, argC, argD);
}

UNUSED void kwtexture2D_ia8_xlu_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 red, s32 green, s32 blue, s32 alpha, u8* texture,
                          Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A60);
    ToumeIMode(red, green, blue, alpha);
    KWTextureIA8_Sub(texture, arg9, argA, argB, argC, argD);
}

UNUSED void kwtexture2D_ia8_xlu_aa_zb_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 red, s32 green, s32 blue, s32 alpha, u8* texture,
                          Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007B00);
    ToumeIMode(red, green, blue, alpha);
    KWTextureIA8_Sub(texture, arg9, argA, argB, argC, argD);
}

UNUSED void kwtexture2D_ia8_tile_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                          s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A40);
    kwtextiledraw_ia8(texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void kwtexture2D_ia8_tile_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                          s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A60);
    kwtextiledraw_ia8(texture, arg5, arg6, arg7, arg8, arg9);
}

void kwtexture2D_ia4_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A40);
    KWTextureIA4_Sub(texture, arg5, arg6, arg7, arg8, arg9);
}

UNUSED void kwtexture2D_ia4_xlu_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 red, s32 green, s32 blue, s32 alpha, u8* texture,
                          Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A60);
    ToumeIMode(red, green, blue, alpha);
    KWTextureIA4_Sub(texture, arg9, argA, argB, argC, argD);
}
UNUSED void kwtexture2D_a4_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                          s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A40);
    gDPSetCombineLERP(gDisplayListHead++, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0);
    KWTextureA4_Sub(texture, arg5, arg6, arg7, arg8, arg9);
}

void kwtexture2D_a4_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A60);
    gDPSetCombineLERP(gDisplayListHead++, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0);
    KWTextureA4_Sub(texture, arg5, arg6, arg7, arg8, arg9);
}

void kwtexture2D_a4_xlu_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 red, s32 green, s32 blue, s32 alpha, u8* texture,
                   Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A40);
    SplashMode(red, green, blue, alpha);
    KWTextureA4_Sub(texture, arg9, argA, argB, argC, argD);
}

void kwtexture2D_a4_xlu_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, s32 red, s32 green, s32 blue, s32 alpha, u8* texture,
                   Vtx* arg9, s32 argA, s32 argB, s32 argC, s32 argD) {
    kwcreateaffine2D(arg0, arg1, arg2, arg3);
    gSPDisplayList(gDisplayListHead++, D_0D007A60);
    SplashMode(red, green, blue, alpha);
    KWTextureA4_Sub(texture, arg9, argA, argB, argC, argD);
}

void kwtexture3D_ia8_pt(Vec3f arg0, Vec3su arg1, f32 arg2, u8* texture, Vtx* arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007A40);
    KWTextureIA8_Sub(texture, arg4, arg5, arg6, arg7, arg8);
}

UNUSED void kwtexture3D_ia8_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u8* texture, Vtx* arg4, s32 arg5, s32 arg6, s32 arg7,
                          s32 arg8) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007A60);
    KWTextureIA8_Sub(texture, arg4, arg5, arg6, arg7, arg8);
}

UNUSED void kwtexture3D_ia8_zb_pt(Vec3f arg0, Vec3su arg1, f32 arg2, u8* texture, Vtx* arg4, s32 arg5, s32 arg6, s32 arg7,
                          s32 arg8) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007AC0);
    KWTextureIA8_Sub(texture, arg4, arg5, arg6, arg7, arg8);
}

UNUSED void kwtexture3D_ia8_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u8* texture, Vtx* arg4, s32 arg5, s32 arg6, s32 arg7,
                          s32 arg8) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007AE0);
    KWTextureIA8_Sub(texture, arg4, arg5, arg6, arg7, arg8);
}

UNUSED void kwtexture3D_16x16_ia8_pt(Vec3f arg0, Vec3su arg1, f32 arg2, u8* texture, Vtx* arg4) {
    kwtexture3D_ia8_pt(arg0, arg1, arg2, texture, arg4, 16, 16, 16, 16);
}

void kwdraw_kage3D(Collision* arg0, Vec3f arg1, f32 arg2) {
    if (kwcalc_bump(arg0, arg1) != 0) {
        D_80183E50[0] = arg1[0];
        D_80183E50[1] = CalcHeight(arg1[0], 0.0f, arg1[2], arg0->last_zx) + 0.8;
        D_80183E50[2] = arg1[2];
        KWCreateAffine3D_Hole(D_80183E50, arg0->bump_zx, arg2);
        gSPDisplayList(gDisplayListHead++, D_0D007B98);
    }
}

void kwdraw_kage3D_fast(s32 objectIndex, f32 scale) {
    AnmObject* object;

    if ((kwanm_bitcheckflag(objectIndex, 0x00000020) != 0) &&
        (kwanm_bitcheckflag(objectIndex, 0x00800000) != 0)) {
        object = &g_DynamicObjects[objectIndex];
        D_80183E50[0] = object->pos[0];
        D_80183E50[1] = object->ground + 0.8;
        D_80183E50[2] = object->pos[2];
        KWCreateAffine3D(D_80183E50, object->kageang, scale);
        gSPDisplayList(gDisplayListHead++, D_0D007B20);
    }
}

void kwdraw_kage3D_flat(s32 objectIndex, f32 arg1) {
    AnmObject* object;

    if (kwanm_bitcheckflag(objectIndex, 0x00000020) != 0) {
        object = &g_DynamicObjects[objectIndex];
        D_80183E50[0] = object->pos[0];
        D_80183E50[1] = object->ground + 0.8;
        D_80183E50[2] = object->pos[2];
        D_80183E98[0] = 0x4000;
        D_80183E98[1] = 0;
        D_80183E98[2] = 0;
        KWCreateAffine3D(D_80183E50, D_80183E98, arg1);
        gSPDisplayList(gDisplayListHead++, D_0D007B20);
    }
}

void kwdraw_kage_3D_bump(s32 objectIndex, f32 arg1) {
    Mat4 sp30;
    AnmObject* object;

    if ((kwanm_bitcheckflag(objectIndex, 0x00000020) != 0) &&
        (kwanm_bitcheckflag(objectIndex, 0x00800000) != 0)) {
        object = &g_DynamicObjects[objectIndex];
        D_80183E50[0] = object->pos[0];
        D_80183E50[1] = object->ground + 0.8;
        D_80183E50[2] = object->pos[2];
        kwcreate_rotation_matrix(sp30, object->pos_ende, D_80183E50, 0U, arg1);
        Affine2Mtx(&gDynamicP->mtxHud[modelingnum], sp30);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxHud[modelingnum++]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, D_0D007B98);
    }
}

void kwdraw_hole3D(f32 arg0) {
    KWCreateAffine3D_Hole(D_80183E50, D_80183E70, arg0);
    gSPDisplayList(gDisplayListHead++, D_0D007C10);
}

UNUSED void kwtexture3D_a8_zb_bl(Vec3f arg0, Vec3su arg1, f32 arg2, u8* texture, Vtx* arg4, s32 arg5, s32 arg6, s32 arg7,
                          s32 arg8) {
    KWCreateAffine3D(arg0, arg1, arg2);
    gSPDisplayList(gDisplayListHead++, D_0D007AE0);
    SparkMode(D_80165860, D_8016586C, D_80165878);
    KWTextureA8_Sub(texture, arg4, arg5, arg6, arg7, arg8);
}

UNUSED void kwtexture2D_16x16_ia16(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5) {
    kwtexture2D_ia16_pt(arg0, arg1, arg2, arg3, texture, arg5, 16, 16, 16, 16);
}

UNUSED void kwtexture2D_16x16_ia8(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5) {
    kwtexture2D_ia8_pt(arg0, arg1, arg2, arg3, texture, arg5, 16, 16, 16, 16);
}

UNUSED void kwtexture2D_16x16_ia4(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* arg4, Vtx* arg5) {
    kwtexture2D_ia4_pt(arg0, arg1, arg2, arg3, arg4, arg5, 16, 16, 16, 16);
}

UNUSED void kwtexture2D_32x32_ia16(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5) {
    kwtexture2D_ia16_pt(arg0, arg1, arg2, arg3, texture, arg5, 32, 32, 32, 32);
}

UNUSED void kwtexture2D_32x32_ia8(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* arg4, Vtx* arg5) {
    kwtexture2D_ia8_pt(arg0, arg1, arg2, arg3, arg4, arg5, 32, 32, 32, 32);
}

UNUSED void kwtexture2D_32x32_ia4(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* arg4, Vtx* arg5) {
    kwtexture2D_ia4_pt(arg0, arg1, arg2, arg3, arg4, arg5, 32, 32, 32, 32);
}

UNUSED void kwtexture3D_32x32_ia8(Vec3f arg0, Vec3su arg1, f32 arg2, u8* texture, Vtx* arg4) {
    kwtexture3D_ia8_pt(arg0, arg1, arg2, texture, arg4, 32, 32, 32, 32);
}

UNUSED void kwtexture2D_64x32_ia16(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5) {
    kwtexture2D_ia16_pt(arg0, arg1, arg2, arg3, texture, arg5, 64, 32, 64, 32);
}

UNUSED void kwtexture2D_64x32_ia8(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* arg4, Vtx* arg5) {
    kwtexture2D_ia8_pt(arg0, arg1, arg2, arg3, arg4, arg5, 64, 32, 64, 32);
}

UNUSED void kwtexture2D_64x32_ia8_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* arg4, Vtx* arg5) {
    kwtexture2D_ia8_bl(arg0, arg1, arg2, arg3, arg4, arg5, 64, 32, 64, 32);
}

UNUSED void kwtexture2D_64x32_ia8_aa_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5) {
    kwtexture2D_ia8_aa_pt(arg0, arg1, arg2, arg3, texture, arg5, 64, 32, 64, 32);
}

UNUSED void kwtexture2D_64x32_ia8_aa_bl(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5) {
    kwtexture2D_ia8_aa_bl(arg0, arg1, arg2, arg3, texture, arg5, 64, 32, 64, 32);
}

UNUSED void kwtexture2D_64x64_ia8(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5) {
    kwtexture2D_ia8_pt(arg0, arg1, arg2, arg3, texture, arg5, 64, 64, 64, 64);
}

UNUSED void kwtexture2D_64x64_ia16(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5) {
    kwtexture2D_ia16_pt(arg0, arg1, arg2, arg3, texture, arg5, 64, 64, 64, 32);
}

UNUSED void kwtexture2D_64x96_ia8_pt(s32 arg0, s32 arg1, u16 arg2, f32 arg3, u8* texture, Vtx* arg5) {
    kwtexture2D_ia8_pt(arg0, arg1, arg2, arg3, texture, arg5, 64, 96, 64, 48);
}

UNUSED void renderghost(void) {
    gDPSetRenderMode(gDisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
}

void initcolpal_ci8(u8* tlut) {
    gDPLoadTLUT_pal256(gDisplayListHead++, tlut);
    gDPLoadSync(gDisplayListHead++);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_NONE);
}

void KWSetPrimColor(s32 red, s32 green, s32 blue, s32 alpha) {
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, alpha);
}

UNUSED void KWSetEnvColor(s32 red, s32 green, s32 blue, s32 alpha) {
    gDPSetEnvColor(gDisplayListHead++, red, green, blue, alpha);
}

void set_color_render(s32 primRed, s32 primGreen, s32 primBlue, s32 envRed, s32 envGreen, s32 envBlue, s32 primAlpha) {
    gDPSetPrimColor(gDisplayListHead++, 0, 0, primRed, primGreen, primBlue, primAlpha);
    gDPSetEnvColor(gDisplayListHead++, envRed, envGreen, envBlue, 0xFF);
}

UNUSED void SubColorMode(s32 red, s32 green, s32 blue) {
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, 0xFF);
}

void SubAlphaMode(s32 alpha) {
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
}

void ToumeIAMode(s32 alpha) {
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, TEXEL0, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0,
                      PRIMITIVE, 0);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, alpha);
}

void ToumeIMode(u32 red, u32 green, u32 blue, u32 alpha) {
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, alpha);
}

void SmokeMode(s32 alpha) {
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, 1, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, 1, TEXEL0, 0, PRIMITIVE, 0);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, alpha);
}

void SplashMode(s32 red, s32 green, s32 blue, s32 alpha) {
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0,
                      PRIMITIVE, 0);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, alpha);
}

void RadarMode(s32 red, s32 green, s32 blue) {
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, 0xFF);
}

UNUSED void RankMode(s32 red, s32 green, s32 blue, s32 alpha) {
    gDPSetCombineLERP(gDisplayListHead++, 1, 0, SHADE, PRIMITIVE, 0, 0, 0, TEXEL0, 1, 0, SHADE, PRIMITIVE, 0, 0, 0,
                      TEXEL0);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, alpha);
}

UNUSED void WaterMode(s32 alpha) {
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, SHADE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, SHADE, TEXEL0, 0, PRIMITIVE,
                      0);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
}

UNUSED void ColAddMode(s32 red, s32 green, s32 blue, s32 alpha) {
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, alpha);
    gDPSetCombineLERP(gDisplayListHead++, 1, PRIMITIVE_ALPHA, TEXEL0, PRIMITIVE, 0, 0, 0, TEXEL0, 1, PRIMITIVE_ALPHA,
                      TEXEL0, PRIMITIVE, 0, 0, 0, TEXEL0);
}

void IceMode(s32 primRed, s32 primGreen, s32 primBlue, s32 envRed, s32 envGreen, s32 envBlue, s32 primAlpha) {
    gDPSetPrimColor(gDisplayListHead++, 0, 0, primRed, primGreen, primBlue, primAlpha);
    gDPSetEnvColor(gDisplayListHead++, envRed, envGreen, envBlue, 0xFF);
    gDPSetCombineLERP(gDisplayListHead++, 1, ENVIRONMENT, TEXEL0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0, 1, ENVIRONMENT,
                      TEXEL0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0);
}

void SparkMode(s32 red, s32 green, s32 blue) {
    gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, 0xFF);
}

void Color2Mode(u32 primRed, u32 primGreen, u32 primBlue, u32 envRed, u32 envGreen, u32 envBlue, u32 primAlpha) {
    gDPSetPrimColor(gDisplayListHead++, 0, 0, primRed, primGreen, primBlue, primAlpha);
    gDPSetEnvColor(gDisplayListHead++, envRed, envGreen, envBlue, 0xFF);
    gDPSetCombineLERP(gDisplayListHead++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                      PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
}

void KWRectangle(s32 x, s32 y, s32 width, s32 height, s32 s, s32 w, s32 mode) {

    s32 xh = (((x + width) - 1) << 2);
    s32 yh = (((y + height) - 1) << 2);
    s32 xl = ((x * 4));
    s32 yl = y * 4;

    s32 xh2 = (((x + width)) << 2);
    s32 yh2 = ((y + height) << 2);

    // If no cycle mode is set, render texture rectangle in copy mode
    if (mode == 0) {
        gSPTextureRectangle(gDisplayListHead++, xl, yl, xh, yh, G_TX_RENDERTILE, s << 5, (w << 5), 4 << 10, 1 << 10);
        return;
    }
    // Render texture rectangle in default cycle mode (1 cycle or 2 cycle)
    gSPTextureRectangle(gDisplayListHead++, xl, yl, xh2, yh2, G_TX_RENDERTILE, s << 5, (w << 5), 1 << 10, 1 << 10);
}

void spr_draw_noclip(s32 x, s32 y, s32 width, s32 height, s32 mode) {
    // (0, 0) means texture coordinates will be rendered from the top left corner
    KWRectangle(x, y, width, height, 0, 0, mode);
}

void SprDrawClip(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    UNUSED s32 pad[2];
    s32 sp2C;
    s32 var_a1;
    s32 var_v0;
    s32 var_v1;

    if ((-arg2 < arg0) && (-arg3 < arg1)) {
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
        KWRectangle(sp2C, var_a1, arg2 - var_v1, arg3 - var_v0, var_v1, var_v0, arg4);
    }
}

void spr_draw_clip_xflip(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    UNUSED s32 pad[2];
    s32 sp2C;
    s32 phi_a1;
    s32 phi_v1;
    s32 phi_v0;

    if ((-arg2 < arg0) && (-arg3 < arg1)) {
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
        KWRectangle(sp2C, phi_a1, arg2 - phi_v1, arg3 - phi_v0, phi_v1 + arg2, phi_v0, arg4);
    }
}

void SprDrawClipST(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    UNUSED s32 pad[2];
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 phi_a3;
    s32 phi_v0;
    s32 phi_v1;

    if ((-arg2 < arg0) && (-arg3 < arg1)) {
        sp34 = arg0;
        sp30 = arg1;
        phi_v0 = arg4;
        sp2C = arg2;
        phi_a3 = arg3;
        phi_v1 = arg5;
        if (arg0 < 0) {
            phi_v0 = arg4 - arg0;
            sp34 = 0;
            sp2C = arg2 + arg0;
        }
        if (arg1 < 0) {
            phi_v1 = arg5 - arg1;
            sp30 = 0;
            phi_a3 = arg3 + arg1;
        }
        KWRectangle(sp34, sp30, sp2C, phi_a3, phi_v0, phi_v1, arg6);
    }
}

UNUSED void func_8004BB34(void) {
}

void spr_draw_clip_scale(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    s16 t;
    s16 s;
    s16 temp_t9;
    s32 var_t0;
    s32 var_t1;
    s32 xl;
    s32 yl;
    UNUSED s32 thing0;
    UNUSED s32 thing1;

    var_t0 = (arg2 * 4 * arg4) + 0.5;
    var_t1 = (arg3 * 4 * arg4) + 0.5;
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
            gSPTextureRectangle(gDisplayListHead++, xl, yl, xl + var_t0, yl + var_t1, 0, s, t, temp_t9, temp_t9);
        }
    }
}

UNUSED void kwsprite_rgba_morph(s32 x, s32 y, u32 width, u32 height, s32 alpha, u16* texture1, u16* texture2) {
    gSPDisplayList(gDisplayListHead++, D_0D007F38);
    gSPDisplayList(gDisplayListHead++, D_0D008138);
    gDPSetTextureLOD(gDisplayListHead++, G_TL_TILE);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, alpha);
    gDPSetCombineLERP(gDisplayListHead++, TEXEL1, TEXEL0, PRIMITIVE_ALPHA, TEXEL0, TEXEL1, TEXEL0, PRIMITIVE, TEXEL0, 0,
                      0, 0, COMBINED, 0, 0, 0, COMBINED);
    gDPLoadMultiTile(gDisplayListHead++, texture1, 0, G_TX_RENDERTILE, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, height, 0, 0,
                     width - 1, height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
                     G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadMultiTile(gDisplayListHead++, texture2, 256, G_TX_RENDERTILE + 1, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, height,
                     0, 0, width - 1, height - 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP,
                     G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    spr_draw_noclip(x, y, width, height, 2);
    gSPDisplayList(gDisplayListHead++, D_0D008120);
}

void DrawLineHorizontal(s16 arg0, s16 arg1, s16 arg2, u16 red, u16 green, u16 blue, u16 alpha) {
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, alpha);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_NONE);
    gDPSetCombineMode(gDisplayListHead++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    SprDrawClip(arg0, arg1, arg2, 1, 1);
}

void DrawLineVertical(s16 arg0, s16 arg1, s16 arg2, u16 red, u16 green, u16 blue, u16 alpha) {
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, alpha);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_NONE);
    gDPSetCombineMode(gDisplayListHead++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    SprDrawClip(arg0, arg1, 1, arg2, 1);
}

void kwsprite_sub(u32 arg0, u32 arg1, u16* texture, u32 width, u32 arg4, u32 height, s32 arg6) {
    s32 i;
    u16* img2;

    arg0 -= (width / 2);
    arg1 -= (arg4 / 2);
    img2 = texture;

    for (i = 0; (u32) i < (arg4 / height); i++) {
        KWLoadTextureBlockRGBA16B(img2, width, height);
        SprDrawClip(arg0, arg1, width, height, arg6);
//! @todo fakematch?
#ifdef AVOID_UB
        img2 += (width * height);
#else
        img2 += (width * height) ^ ((arg4 / height) * 0);
#endif
        arg1 += height;
    }
}

UNUSED void func_8004C354() {
}

UNUSED void func_8004C35C() {
}

void KWSprite(s32 x, s32 y, u32 width, u32 height, u16* texture) {
    gSPDisplayList(gDisplayListHead++, D_0D008108);
    gSPDisplayList(gDisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_THRESHOLD);
    KWLoadTextureBlockRGBA16B(texture, width, height);
    SprDrawClip(x - (width >> 1), y - (height >> 1), width, height, 0);
    gSPDisplayList(gDisplayListHead++, D_0D007EB8);
}

void kwsprite_flash(s32 x, s32 y, u32 width, u32 height, u16* texture) {

    gSPDisplayList(gDisplayListHead++, D_0D007F38);
    IceMode(kwflash_r, kwflash_g, kwflash_b, 0x80, 0x80, 0x80, 0xFF);
    KWLoadTextureBlockRGBA16B(texture, width, height);
    SprDrawClip(x - (width >> 1), y - (height >> 1), width, height, 1);
    gSPDisplayList(gDisplayListHead++, D_0D007EB8);
}

UNUSED void kwsprite_t(s32 arg0, s32 arg1, u32 arg2, u32 arg3, u16* texture) {

    gSPDisplayList(gDisplayListHead++, D_0D008108);
    gSPDisplayList(gDisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_THRESHOLD);
    kwloadtexturetile_rgba_16b(texture, arg2, arg3);
    SprDrawClip(arg0 - (arg2 >> 1), arg1 - (arg3 >> 1), arg2, arg3, 0);
    gSPDisplayList(gDisplayListHead++, D_0D007EB8);
}

void kwsprite_32b(s32 arg0, s32 arg1, u32 arg2, u32 arg3, u8* texture) {

    gSPDisplayList(gDisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_THRESHOLD);
    kwloadtextureblock_rgba_32b(texture, arg2, arg3);
    SprDrawClip(arg0 - (arg2 >> 1), arg1 - (arg3 >> 1), arg2, arg3, 1);
    gSPDisplayList(gDisplayListHead++, D_0D007EB8);
}

void KWSpriteTile32B(s16 x, s16 y, u8* texture, u32 width, u32 height) {
    s32 texSizeLess;
    s32 i;
    s32 centerY;
    s32 centerX;
    s32 numTextureBlocks;
    u32 texSize;
    s32 heightDiv;
    s32 size;
    u8* textureCopy;

    centerX = x - (width / 2);
    centerY = y - (height / 2);
    textureCopy = texture;
    gSPDisplayList(gDisplayListHead++, D_0D007EF8);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    texSize = width * height * 4;
    numTextureBlocks = texSize / 4096;
    if (texSize % 4096) {
        numTextureBlocks++;
    }
    heightDiv = height / numTextureBlocks;
    size = numTextureBlocks;
    for (i = 0; i < size; i++) {
        kwloadtexturetile_rgba_32b(textureCopy, width, heightDiv);
        spr_draw_noclip(centerX, centerY, width, heightDiv, 1);
        textureCopy += (width * heightDiv * 4);
        texSizeLess = texSize - (width * heightDiv * 4);
        if (texSizeLess < 0) {
            heightDiv = texSize / width;
        } else {
            texSize -= (width * heightDiv * 4);
        }


        centerY += heightDiv;
    }

    gSPDisplayList(gDisplayListHead++, D_0D007EB8);
    // FAKE
    centerX++;
    centerX--;
}

void kwdisplay_titlelogo(s16 x, s16 y) {
    KWSpriteTile32B(x, y, logoptr, 256, 128);
}

UNUSED void KWSpriteDiv(s32 arg0, s32 arg1, u16* texture, s32 arg3, s32 arg4, s32 arg5) {
    gSPDisplayList(gDisplayListHead++, D_0D008108);
    gSPDisplayList(gDisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_THRESHOLD);
    kwsprite_sub(arg0, arg1, texture, arg3, arg4, arg5, 0);
    gSPDisplayList(gDisplayListHead++, D_0D007EB8);
}

void KWSpriteXLU(s32 arg0, s32 arg1, s32 arg2, u16* texture, s32 arg4, s32 arg5, UNUSED s32 arg6, s32 arg7) {
    gSPDisplayList(gDisplayListHead++, D_0D007F38);
    SubAlphaMode(arg2);
    kwsprite_sub(arg0, arg1, texture, arg4, arg5, arg7, 1);
}

void KWSpriteScale(s32 arg0, s32 arg1, f32 arg2, u16* texture, s32 arg4, s32 arg5) {
    gSPDisplayList(gDisplayListHead++, D_0D007F78);
    KWLoadTextureBlockRGBA16B(texture, arg4, arg5);
    spr_draw_clip_scale(arg0, arg1, arg4, arg5, arg2);
}

void KWSprite8x8(s32 x, s32 y, u16* texture) {
    KWSprite(x, y, 8, 8, texture);
}

UNUSED void kwsprite8x16(s32 x, s32 y, u16* texture) {
    KWSprite(x, y, 8, 16, texture);
}

UNUSED void kwsprite16x16(s32 x, s32 y, u16* texture) {
    KWSprite(x, y, 16, 16, texture);
}

void kwsprite32x8(s32 x, s32 y, u16* texture) {
    KWSprite(x, y, 32, 8, texture);
}

void kwsprite32x16(s32 x, s32 y, u16* texture) {
    KWSprite(x, y, 32, 16, texture);
}

UNUSED void kwsprite32x16_sc(s32 arg0, s32 arg1, f32 arg2, u16* texture) {
    KWSpriteScale(arg0, arg1, arg2, texture, 32, 16);
}

UNUSED void kwsprite32x32(s32 x, s32 y, u16* texture) {
    KWSprite(x, y, 32, 32, texture);
}

UNUSED void kwsprite32x32_32b(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_32b(arg0, arg1, 32, 32, texture);
}

UNUSED void kwsprite40x32(s32 x, s32 y, u16* texture) {
    KWSprite(x, y, 40, 32, texture);
}

UNUSED void kwsprite48x48(s32 x, s32 y, u16* texture) {
    KWSpriteDiv(x, y, texture, 48, 48, 24);
}

UNUSED void kwsprite64x32(s32 x, s32 y, u16* texture) {
    KWSprite(x, y, 64, 32, texture);
}

UNUSED void kwsprite64x32_sc(s32 arg0, s32 arg1, f32 arg2, u16* texture) {
    KWSpriteScale(arg0, arg1, arg2, texture, 64, 32);
}

UNUSED void kwsprite64x64(s32 arg0, s32 arg1, u16* texture) {
    KWSpriteDiv(arg0, arg1, texture, 64, 64, 32);
}

UNUSED void kwsprite_ia16(s32 arg0, s32 arg1, UNUSED u8* texture, s32 width, s32 arg4, s32 height) {
    UNUSED s32 pad;
    s32 var_s0;
    s32 i;
    u8* img;

    var_s0 = arg1 - (arg4 / 2);
    gSPDisplayList(gDisplayListHead++, D_0D007FE0);

    for (i = 0; i < arg4 / height; i++) {
        kwloadtextureblock_ia_16b(img, width, height);
        SprDrawClip(arg0 - (width / 2), var_s0, width, height, 1);
        img += width * height * 2;
        var_s0 += height;
    }
}

UNUSED void kwsprite_ia8_common(s32 arg0, s32 arg1, u8* texture, s32 width, s32 arg4, s32 height) {
    s32 var_s0 = arg1 - (arg4 / 2);
    s32 i;
    u8* img = texture;

    for (i = 0; i < arg4 / height; i++) {
        kwloadtextureblock_ia_8b(img, width, height);
        SprDrawClip(arg0 - (width / 2), var_s0, width, height, 1);
        img += width * height;
        var_s0 += height;
    }
}

UNUSED void kwsprite_ia8_pt(s32 arg0, s32 arg1, u8* texture, s32 arg3, s32 arg4, UNUSED s32 arg5, s32 arg6) {
    gSPDisplayList(gDisplayListHead++, D_0D007FE0);
    kwsprite_ia8_common(arg0, arg1, texture, arg3, arg4, arg6);
}

UNUSED void kwsprite_ia8_bl(s32 arg0, s32 arg1, u8* texture, s32 arg3, s32 arg4, UNUSED s32 arg5, s32 arg6) {
    gSPDisplayList(gDisplayListHead++, D_0D008000);
    kwsprite_ia8_common(arg0, arg1, texture, arg3, arg4, arg6);
}

UNUSED void kwsprite_ia8_xlu_pt(s32 arg0, s32 arg1, u8* texture, s32 red, s32 green, s32 blue, s32 alpha, s32 arg7, s32 arg8,
                          UNUSED s32 arg9, s32 argA) {
    gSPDisplayList(gDisplayListHead++, D_0D007FE0);
    ToumeIMode(red, green, blue, alpha);
    kwsprite_ia8_common(arg0, arg1, texture, arg7, arg8, argA);
}

UNUSED void func_8004D0CC(void) {
}

UNUSED void kwsprite_ia4(s32 arg0, s32 arg1, u8* texture, s32 width, s32 arg4, s32 height) {
    s32 var_s0;
    u8* img;
    s32 i;

    var_s0 = arg1 - (arg4 / 2);
    img = texture;
    gSPDisplayList(gDisplayListHead++, D_0D007FE0);

    for (i = 0; i < arg4 / height; i++) {
        kwloadtextureblock_ia_4b(img, width, height);
        SprDrawClip(arg0 - (width / 2), var_s0, width, height, 1);
        img += width * height;
        var_s0 += height;
    }
}

void kwsprite_ia4_xlu_pt(s32 arg0, s32 arg1, u8* texture, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 width, s32 arg8,
                   UNUSED s32 arg9, s32 height) {
    s32 var_s3;
    u8* img;
    s32 i;

    var_s3 = arg1 - (arg8 / 2);
    img = texture;
    gSPDisplayList(gDisplayListHead++, D_0D007FE0);
    ToumeIMode(arg3, arg4, arg5, arg6);

    for (i = 0; i < arg8 / height; i++) {
        kwloadtextureblock_ia_4b(img, width, height);
        SprDrawClip(arg0 - (width / 2), var_s3, width, height, 1);
        img += (width * height) / 2;
        var_s3 += height;
    }
}

void kwsprite_a4_xlu_pt(s32 arg0, s32 arg1, u8* texture, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 width, s32 arg8,
                   UNUSED s32 arg9, s32 height) {
    s32 var_s3;
    u8* img;
    s32 i;

    var_s3 = arg1 - (arg8 / 2);
    img = texture;
    gSPDisplayList(gDisplayListHead++, D_0D007FE0);
    SplashMode(arg3, arg4, arg5, arg6);

    for (i = 0; i < arg8 / height; i++) {
        kwloadtextureblock_a_4b(img, width, height);
        SprDrawClip(arg0 - (width / 2), var_s3, width, height, 1);
        img += (width * height) / 2;
        var_s3 += height;
    }
}

void kwsprite_a4_xlu_pt_xflip(s32 arg0, s32 arg1, u8* texture, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 width, s32 arg8,
                   UNUSED s32 arg9, s32 height) {
    s32 var_s3;
    u8* img;
    s32 i;

    var_s3 = arg1 - (arg8 / 2);
    img = texture;
    gSPDisplayList(gDisplayListHead++, D_0D007FE0);
    SplashMode(arg3, arg4, arg5, arg6);

    for (i = 0; i < arg8 / height; i++) {
        kwloadtextureblock_a_4b_xflip(img, width, height);
        spr_draw_clip_xflip(arg0 - (width / 2), var_s3, width, height, 1);
        img += (width * height) / 2;
        var_s3 += height;
    }
}

void kwsprite_scale_a4_xlu_bl(s32 arg0, s32 arg1, u8* texture, f32 arg3, s32 arg4, s32 arg5, s32 arg6, UNUSED s32 arg7, s32 width,
                   s32 arg9, UNUSED s32 argA, s32 height) {
    s32 i;
    s32 var_s3;
    u8* textureCopy;

    var_s3 = arg1 - (arg9 / 2);
    textureCopy = texture;
    gSPDisplayList(gDisplayListHead++, D_0D008000);
    RadarMode(arg4, arg5, arg6);
    for (i = 0; i < (arg9 / height); i++) {
        kwloadtextureblock_a_4b(textureCopy, width, height);
        spr_draw_clip_scale(arg0, arg1, width, arg9, arg3);
        textureCopy += (width * height) / 2;
        var_s3 += height;
    }
}

void kwsprite_draw_yra_ia16(s32 arg0, s32 arg1, u8* texture, s32 arg3, s32 arg4) {
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
    img = texture;
    var_s3 = arg1 - (arg4 / 2);
    gSPDisplayList(gDisplayListHead++, D_0D007FE0);

    sp5C = arg3 * 2;
    for (i = 0; i < arg4; i++) {
        temp_s0 = var_s1;
        temp_s5 = (s32) ((sinT(temp_s0) * temp_f20) + (f32) (arg0 - (arg3 / 2)));
        sinT(temp_s0);
        kwloadtextureblock_ia_16b(img, arg3, 1);
        SprDrawClip(temp_s5, var_s3, arg3, 1, 1);

        var_s1 += temp_s7;
        var_s3 += 1;
        img += sp5C;
    }
}

void kwsprite_draw_yra_ia8(s32 arg0, s32 arg1, u8* texture, s32 arg3, s32 arg4) {
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
    img = texture;
    var = arg3 / 2;
    var_s4 = arg1 - (arg4 / 2);

    gSPDisplayList(gDisplayListHead++, D_0D007FE0);

    for (i = 0; i < arg4; i++) {
        temp_s0 = var_s1;
        temp_s6 = (s32) ((sinT(temp_s0) * temp_f20) + (f32) (arg0 - (var)));
        sinT(temp_s0);
        kwloadtextureblock_ia_8b(img, arg3, 1);
        SprDrawClip(temp_s6, var_s4, arg3, 1, 1);
        var_s1 += temp_s7;
        img = &img[arg3];
        var_s4 += 1;
    }
}

UNUSED void kwsprite_draw_yra_ia4(s32 arg0, s32 arg1, u8* texture, s32 arg3, s32 arg4) {
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
    img = texture;
    var = arg3 / 2;
    var_s4 = arg1 - (arg4 / 2);

    gSPDisplayList(gDisplayListHead++, D_0D007FE0);
    for (i = 0; i < arg4; i++) {
        temp_s0 = var_s1;
        temp_s6 = (s32) ((sinT(temp_s0) * temp_f20) + (f32) (arg0 - (var)));
        sinT(temp_s0);
        kwloadtextureblock_ia_4b(img, arg3, 1);
        SprDrawClip(temp_s6, var_s4, arg3, 1, 1);
        var_s1 += temp_s7;
        img += arg3;
        var_s4 += 1;
    }
}

UNUSED void kwsprite8x160_ia8(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_ia8_pt(arg0, arg1, texture, 8, 160, 8, 160);
}

UNUSED void kwsprite12x160_ia8(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_ia8_pt(arg0, arg1, texture, 12, 160, 12, 160);
}

UNUSED void kwsprite12x192_ia8(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_ia8_pt(arg0, arg1, texture, 12, 192, 12, 192);
}

UNUSED void kwsprite16x16_ia(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_ia16(arg0, arg1, texture, 16, 16, 16);
}

UNUSED void kwsprite16x160_ia8(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_ia8_pt(arg0, arg1, texture, 16, 160, 16, 160);
}

UNUSED void kwsprite32x32_ia(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_ia16(arg0, arg1, texture, 32, 32, 32);
}

UNUSED void kwsprite32x32_ia8(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_ia8_pt(arg0, arg1, texture, 32, 32, 32, 32);
}

UNUSED void kwsprite32x32_ia4(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_ia4(arg0, arg1, texture, 32, 32, 32);
}

UNUSED void kwsprite32x32_yra_ia16(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_draw_yra_ia16(arg0, arg1, texture, 32, 32);
}

UNUSED void kwsprite32x32_yra_ia8(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_draw_yra_ia8(arg0, arg1, texture, 32, 32);
}

UNUSED void kwsprite32x32_yra_ia4(s32 arg0, s32 arg1, u8* arg2) {
    kwsprite_draw_yra_ia4(arg0, arg1, arg2, 32, 32);
}

UNUSED void kwsprite64x32_ia(s32 arg0, s32 arg1, u8* arg2) {
    kwsprite_ia16(arg0, arg1, arg2, 64, 32, 32);
}

UNUSED void kwsprite64x64_ia(s32 arg0, s32 arg1, u8* arg2) {
    kwsprite_ia16(arg0, arg1, arg2, 64, 64, 32);
}

UNUSED void kwsprite64x96_ia8(s32 arg0, s32 arg1, u8* texture) {
    kwsprite_ia8_pt(arg0, arg1, texture, 64, 96, 64, 48);
}

UNUSED void kwsprite112x32_ia8(s32 arg0, s32 arg1, u8* arg2) {
    kwsprite_ia8_pt(arg0, arg1, arg2, 112, 32, 112, 32);
}

UNUSED void kwsprite128x32_ia8(s32 arg0, s32 arg1, u8* arg2) {
    kwsprite_ia8_pt(arg0, arg1, arg2, 128, 32, 128, 32);
}

void spr_draw_ci8(s32 arg0, s32 arg1, u8* texture, s32 width, s32 arg4, s32 height) {
    s32 var_s0 = var_s0 = arg1 - (arg4 / 2);
    u8* img = texture;
    s32 i;

    for (i = 0; i < arg4 / height; i++) {
        kwloadtextureblock_ci_8b(img, width, height);
        SprDrawClip(arg0 - (width / 2), var_s0, width, height, 1);
        img += width * height;
        var_s0 += height;
    }
}

void spr_draw_ci8_yra(s32 arg0, s32 arg1, u8* texture, s32 arg3, s32 arg4) {
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
    img = texture;
    var = arg3 / 2;
    var_s4 = arg1 - (arg4 / 2);

    for (i = 0; i < arg4; i++) {
        temp_s0 = var_s1;
        temp_s6 = (u32) ((sinT(temp_s0) * temp_f20) + (f32) (arg0 - var));
        sinT(temp_s0);
        kwloadtextureblock_ci_8b(img, arg3, 1);
        SprDrawClip(temp_s6, var_s4, arg3, 1, 1);
        var_s1 += temp_s7;
        img += arg3;
        var_s4 += 1;
    }
}

UNUSED void func_8004E238(void) {
}

void SprDrawSubCI8(s32 arg0, s32 arg1, u8* tlut, u8* texture, s32 arg4, s32 arg5, s32 arg6) {
    gSPDisplayList(gDisplayListHead++, D_0D007CB8);
    initcolpal_ci8(tlut);
    spr_draw_ci8(arg0, arg1, texture, arg4, arg5, arg6);
}

void spr_draw_sub_ci8_xlu(s32 arg0, s32 arg1, s32 arg2, u8* tlut, u8* texture, s32 arg5, s32 arg6, s32 arg7) {
    gSPDisplayList(gDisplayListHead++, D_0D007DB8);
    SubAlphaMode(arg2);
    initcolpal_ci8(tlut);
    spr_draw_ci8(arg0, arg1, texture, arg5, arg6, arg7);
}

void spr_draw_sub_ci8_yra(s32 arg0, s32 arg1, u8* tlut, u8* texture, s32 arg4, s32 arg5) {
    gSPDisplayList(gDisplayListHead++, D_0D007DB8);
    SubAlphaMode(D_8016589C);
    initcolpal_ci8(tlut);
    spr_draw_ci8_yra(arg0, arg1, texture, arg4, arg5);
}

UNUSED void func_8004E3B8(void) {
}

UNUSED void kwsprite_ci8_pt(s32 arg0, s32 arg1, u8* tlut, u8* texture, s32 arg4, s32 arg5, UNUSED s32 arg6, s32 arg7) {
    SprDrawSubCI8(arg0, arg1, tlut, texture, arg4, arg5, arg7);
}

UNUSED void kwsprite_ci8_xlu_pt(s32 arg0, s32 arg1, s32 arg2, u8* tlut, u8* texture, s32 arg5, s32 arg6, UNUSED s32 arg7,
                          s32 arg8) {
    spr_draw_sub_ci8_xlu(arg0, arg1, arg2, tlut, texture, arg5, arg6, arg8);
}

UNUSED void kwsprite8x128_ci8(s32 arg0, s32 arg1, u8* tlut, u8* texture) {
    SprDrawSubCI8(arg0, arg1, tlut, texture, 8, 128, 128);
}

UNUSED void kwsprite32x32_ci8(s32 arg0, s32 arg1, u8* tlut, u8* texture) {
    SprDrawSubCI8(arg0, arg1, tlut, texture, 32, 32, 32);
}

UNUSED void kwsprite32x64_ci8(s32 arg0, s32 arg1, u8* tlut, u8* texture) {
    SprDrawSubCI8(arg0, arg1, tlut, texture, 32, 64, 64);
}

void kwsprite40x32_ci8(s32 arg0, s32 arg1, u8* tlut, u8* texture) {
    SprDrawSubCI8(arg0, arg1, tlut, texture, 40, 32, 32);
}

UNUSED void kwsprite48x48_ci8(s32 arg0, s32 arg1, u8* tlut, u8* texture) {
    SprDrawSubCI8(arg0, arg1, tlut, texture, 48, 48, 24);
}

UNUSED void kwsprite64x32_ci8(s32 arg0, s32 arg1, u8* tlut, u8* texture) {
    SprDrawSubCI8(arg0, arg1, tlut, texture, 64, 32, 32);
}

UNUSED void kwsprite64x64_ci8(s32 arg0, s32 arg1, u8* tlut, u8* texture) {
    SprDrawSubCI8(arg0, arg1, tlut, texture, 64, 64, 32);
}

UNUSED void kwsprite64x64_ci8_xlu(s32 arg0, s32 arg1, s32 arg2, u8* tlut, u8* texture) {
    spr_draw_sub_ci8_xlu(arg0, arg1, arg2, tlut, texture, 64, 64, 32);
}

UNUSED void kwsprite64x64_ci8_ra(s32 arg0, s32 arg1, u8* tlut, u8* arg3) {
    spr_draw_sub_ci8_yra(arg0, arg1, tlut, arg3, 64, 64);
}

UNUSED void kwsprite320x240_ci8(s32 arg0, s32 arg1, u8* tlut, u8* texture) {
    SprDrawSubCI8(arg0, arg1, tlut, texture, SCREEN_WIDTH, SCREEN_HEIGHT, 6);
}

void KWDisplayItembox(s32 playerId) {
    s32 objectIndex;
    AnmObject* object;
    Hud* temp_v0;

    objectIndex = ItemBoxAllocPtr[playerId];
    object = &g_DynamicObjects[objectIndex];
    if (object->anmptr >= 2) {
        temp_v0 = &playerHUD[playerId];
        kwsprite40x32_ci8(temp_v0->addix + temp_v0->ix, temp_v0->addiy + temp_v0->iy,
                      (u8*) object->texaddr, object->indexaddr);
    }
}

void KWDisplayItemboxs(s32 playerId) {
    s32 objectIndex;
    AnmObject* object;
    Hud* temp_v0;

    objectIndex = ItemBoxAllocPtr[playerId];
    object = &g_DynamicObjects[objectIndex];
    if (object->anmptr >= 2) {
        temp_v0 = &playerHUD[playerId];
        KWTexture2DCI8BL(temp_v0->addix + temp_v0->ix, temp_v0->addiy + temp_v0->iy, 0U,
                      temp_v0->is, (u8*) object->texaddr, (u8*) object->indexaddr, D_0D005C30,
                      0x00000028, 0x00000020, 0x00000028, 0x00000020);
    }
}

void kwdisplay_syuukai(s32 playerId) {
    kwsprite32x8((s32) playerHUD[playerId].cx, playerHUD[playerId].cy + 3,
                             common_texture_hud_lap);
    kwsprite32x16(playerHUD[playerId].cx + 0x1C, (s32) playerHUD[playerId].cy,
                              lap_list[playerHUD[playerId].lapcnt]);
}

void KWDisplayRank(s32 playerId) {
    if (playerHUD[playerId].ranksw != 0) {
        if (playerHUD[playerId].goal != 3) {
            kwtexture2D_a4_xlu_bl(playerHUD[playerId].rx + playerHUD[playerId].rgx,
                          playerHUD[playerId].ry + playerHUD[playerId].rgy, 0U,
                          playerHUD[playerId].rs, 0x000000FF, D_800E55F8[KWRank[playerId]], 0, 0x000000FF,
                          common_texture_hud_place[KWRank[playerId]], D_0D0068F0, 0x00000080, 0x00000040,
                          0x00000080, 0x00000040);
        } else {
            kwtexture2D_a4_xlu_bl(playerHUD[playerId].rx + playerHUD[playerId].rgx,
                          playerHUD[playerId].ry + playerHUD[playerId].rgy, 0U,
                          playerHUD[playerId].rs, 0x000000FF, D_800E55F8[KW8GFTimer], 0, 0x000000FF,
                          common_texture_hud_place[gGPCurrentRaceRankByPlayerId[playerId]], D_0D0068F0, 0x00000080,
                          0x00000040, 0x00000080, 0x00000040);
        }
    }
}

void kwdisplay_srank(s32 playerId) {
    if (playerHUD[playerId].ranksw != 0) {
        if (playerHUD[playerId].goal != 3) {
            kwtexture2D_a4_xlu_bl(playerHUD[playerId].rx + playerHUD[playerId].rgx,
                          playerHUD[playerId].ry + playerHUD[playerId].rgy, 0U,
                          playerHUD[playerId].rs, 0x000000FF,
                          D_800E5618[gGPCurrentRaceRankByPlayerId[playerId]], 0, 0x000000FF,
                          D_0D015258[gGPCurrentRaceRankByPlayerId[playerId]], D_0D006030, 0x00000040, 0x00000040,
                          0x00000040, 0x00000040);
        } else {
            kwtexture2D_a4_xlu_bl(playerHUD[playerId].rx + playerHUD[playerId].rgx,
                          playerHUD[playerId].ry + playerHUD[playerId].rgy, 0U,
                          playerHUD[playerId].rs, 0x000000FF, D_800E5618[KW4GFTimer], 0, 0x000000FF,
                          D_0D015258[gGPCurrentRaceRankByPlayerId[playerId]], D_0D006030, 0x00000040, 0x00000040,
                          0x00000040, 0x00000040);
        }
    }
}

void func_8004EB30(UNUSED s32 arg0) {
}

void kwdisplay_zanzou(s32 playerId) {
    Hud* temp_s0;

    temp_s0 = &playerHUD[playerId];
    if ((u8) temp_s0->tptr2 != 0) {
        KWSpriteXLU(temp_s0->lap1CompletionTimeX - 0x13, temp_s0->ty + 8, 0x00000080,
                      common_texture_hud_time, 0x00000020, 0x00000010, 0x00000020, 0x00000010);
        KWPrintLapTimeXLU((s32) temp_s0->lap1CompletionTimeX, (s32) temp_s0->ty, 0x00000080, (s32) temp_s0->totaltime);
    }
    if ((u8) temp_s0->tptr3 != 0) {
        KWSpriteXLU(temp_s0->lap2CompletionTimeX - 0x13, temp_s0->ty + 8, 0x00000050,
                      common_texture_hud_time, 0x00000020, 0x00000010, 0x00000020, 0x00000010);
        KWPrintLapTimeXLU((s32) temp_s0->lap2CompletionTimeX, (s32) temp_s0->ty, 0x00000050, (s32) temp_s0->totaltime);
    }
    if ((u8) temp_s0->cptr2 != 0) {
        KWSpriteXLU((s32) temp_s0->cx2, temp_s0->cy + 3, 0x00000080, common_texture_hud_lap,
                      0x00000020, 8, 0x00000020, 8);
        KWSpriteXLU(temp_s0->cx2 + 0x1C, (s32) temp_s0->cy, 0x00000080,
                      lap_list[temp_s0->lapcnt], 0x00000020, 0x00000010, 0x00000020, 0x00000010);
    }
    if ((u8) temp_s0->cptr3 != 0) {
        KWSpriteXLU((s32) temp_s0->cx3, temp_s0->cy + 3, 0x00000050, common_texture_hud_lap,
                      0x00000020, 8, 0x00000020, 8);
        KWSpriteXLU(temp_s0->cx3 + 0x1C, (s32) temp_s0->cy, 0x00000050,
                      lap_list[temp_s0->lapcnt], 0x00000020, 0x00000010, 0x00000020, 0x00000010);
    }
}

void kwdisplay_speedmeter(s32 arg0) {
    kwtexture2D_a4_xlu_pt(playerHUD[arg0].mx, playerHUD[arg0].my, 0U, 1.0f, g_mapR, g_mapG,
                  g_mapB, 0xFF, common_texture_speedometer, D_0D0064B0, 64, 96, 64, 48);
    kwtexture2D_a4_bl(D_8018CFEC, D_8018CFF4, D_8016579E, 1.0f, common_texture_speedometer_needle, D_0D005FF0, 0x40, 0x20,
                  0x40, 0x20);
}

void kwdisplay_radarmap(s32 arg0) {
    if (g_ScreenFlip != 0) {
        kwsprite_a4_xlu_pt_xflip(g_mapX[arg0] + rzerox, g_mapY[arg0] + rzeroy, (u8*) g_mapTexture,
                      (s32) g_mapR, (s32) g_mapG, (s32) g_mapB, 0x000000FF, (s32) g_mapWidth,
                      (s32) g_mapHeight, (s32) g_mapWidth, (s32) g_mapHeight);
    } else {
        kwsprite_a4_xlu_pt(g_mapX[arg0] + rzerox, g_mapY[arg0] + rzeroy, (u8*) g_mapTexture,
                      (s32) g_mapR, (s32) g_mapG, (s32) g_mapB, 0x000000FF, (s32) g_mapWidth,
                      (s32) g_mapHeight, (s32) g_mapWidth, (s32) g_mapHeight);
    }
}

void kwdisplay_kawanoradarmap(s32 arg0) {
    s16 temp_t0;
    s16 temp_v0;

    temp_v0 = D_800E5548[arg0 * 2];
    temp_t0 = D_800E5548[arg0 * 2 + 1];
    kwsprite_a4_xlu_pt(0x00000104, 0x0000003C, D_8018D248[arg0], 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, temp_v0,
                  temp_t0, temp_v0, temp_t0);
}

void kwdisplay_radarsline(s32 arg0) {
    f32 var_f0;
    f32 var_f2;

    var_f2 = ((g_mapX[arg0] + rzerox) - (g_mapWidth / 2)) + g_startX;
    var_f0 = ((g_mapY[arg0] + rzeroy) - (g_mapHeight / 2)) + g_startY;
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) { /* irregular */
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
    RadarFinishLine(var_f2, var_f0, common_texture_minimap_finish_line);
#else

#endif
}

void kwdisplay_radarplayer_sub(s32 arg0, s32 playerId, s32 characterId) {
    f32 thing0;
    f32 thing1;
    s16 x;
    s16 y;
    Player *player = &gPlayerOne[playerId];

    if (player->flag & (1 << 15)) {
        thing0 = HijackScaleMinimapX(player->position[0] * g_mapScale);
        thing1 = HijackScaleMinimapY(player->position[2] * g_mapScale);
        x = ((g_mapX[arg0] + rzerox) - (g_mapWidth / 2)) + g_startX + (s16)(thing0);
        y = ((g_mapY[arg0] + rzeroy) - (g_mapHeight / 2)) + g_startY + (s16)(thing1);
        // huh?
        if (characterId != 8) {
            if ((gGPCurrentRaceRankByPlayerId[playerId] == 0) && (g_gameMode != 3) && (g_gameMode != 1)) {
                kwtexture2D_flash_rgba_pt(x, y, player->direction[1] + DEGREES(180), 1.0f, &common_texture_minimap_kart_mario[characterId * 64], common_vtx_player_minimap_icon, 8, 8, 8, 8);
            } else {
                KWTexture2DRGBA(x, y, player->direction[1] + DEGREES(180), 1.0f, &common_texture_minimap_kart_mario[characterId * 64], common_vtx_player_minimap_icon, 8, 8, 8, 8);
            }
        } else {
            if (gGPCurrentRaceRankByPlayerId[playerId] == 0) {
                kwsprite_flash(x, y, 8, 8, common_texture_minimap_progress_dot);
            } else {
                KWSprite(x, y, 8, 8, common_texture_minimap_progress_dot);
            }
        }
    }
}

// WTF is up with the gPlayerOne access in this function?
void kwdisplay_radarplayer(s32 arg0) {
    UNUSED Player* player;
    s32 playerId;
    s32 idx;

    switch (g_gameMode) { /* irregular */
        case GRAND_PRIX:
            for (idx = kwkartnum - 1; idx >= 0; idx--) {
                playerId = rank_to_num[idx];
                if ((gPlayerOne + playerId)->flag & IS_CPU_PLAYER) {
                    kwdisplay_radarplayer_sub(arg0, playerId, 8);
                }
            }
            for (idx = kwkartnum - 1; idx >= 0; idx--) {
                playerId = rank_to_num[idx];
                if (((gPlayerOne + playerId)->flag & IS_CPU_PLAYER) != IS_CPU_PLAYER) {
                    kwdisplay_radarplayer_sub(arg0, playerId, (gPlayerOne + playerId)->kart);
                }
            }
            break;
        case TIME_TRIALS:
            for (idx = 0; idx < 8; idx++) {
                if (((gPlayerOne + idx)->flag & IS_GHOST) == IS_GHOST) {
                    kwdisplay_radarplayer_sub(arg0, idx, 8);
                }
            }
            kwdisplay_radarplayer_sub(arg0, 0, gPlayerOne->kart);
            break;
        case VERSUS:
            for (idx = g_playerCount - 1; idx >= 0; idx--) {
                playerId = rank_to_num[idx];
                kwdisplay_radarplayer_sub(arg0, playerId, (gPlayerOne + playerId)->kart);
            }
            break;
        case BATTLE:
            for (idx = 0; idx < g_playerCount; idx++) {
                if (!((gPlayerOne + idx)->flag & IS_BOMB)) {
                    kwdisplay_radarplayer_sub(arg0, idx, (gPlayerOne + idx)->kart);
                }
            }
            break;
    }
}

s32 kwdiv(s32* arg0, s32 arg1) {
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

void kwgettimer(s32 arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 sp24;

    sp24 = arg0;
    if (arg0 >= 599999) {
        sp24 = 599999;
    }
    D_801657D0[0] = kwdiv(&sp24, 60000);
    D_801657D0[1] = kwdiv(&sp24, 6000);
    D_801657D0[3] = kwdiv(&sp24, 1000);
    D_801657D0[4] = kwdiv(&sp24, 100);
    D_801657D0[6] = kwdiv(&sp24, 10);
    D_801657D0[7] = sp24;
    D_801657D0[2] = 10;
    D_801657D0[5] = 11;
}

void kwprintlaptime_sub(s32 arg0, s32 arg1) {
    s32 i;
    s32 phi_s1 = arg0;

    for (i = 0; i < 8; i++) {
        SprDrawClipST(phi_s1, arg1, 8, 16, D_801657D0[i] * 8, 0, 0);
        phi_s1 += 8;
    }
}

void kwprintlaptime(s32 arg0, s32 arg1, s32 arg2) {
    gSPDisplayList(gDisplayListHead++, D_0D008108);
    gSPDisplayList(gDisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_THRESHOLD);
    KWLoadTextureBlockRGBA16B(common_texture_hud_normal_digit, 104, 16);
    kwgettimer(arg2);
    kwprintlaptime_sub(arg0, arg1);
    gSPDisplayList(gDisplayListHead++, D_0D007EB8);
}

void kwprintlaptime_xlu_sub(s32 arg0, s32 arg1) {
    s32 phi_s1 = arg0;
    s32 i;

    for (i = 0; i < 8; i++) {
        SprDrawClipST(phi_s1, arg1, 8, 16, D_801657D0[i] * 8, 0, 1);
        phi_s1 += 8;
    }
}

void KWPrintLapTimeXLU(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    gSPDisplayList(gDisplayListHead++, D_0D007F38);
    SubAlphaMode(arg2);
    KWLoadTextureBlockRGBA16B(common_texture_hud_normal_digit, 104, 16);
    kwgettimer(arg3);
    kwprintlaptime_xlu_sub(arg0, arg1);
}

void kwprintlaptime_flash(s32 arg0, s32 arg1, s32 arg2) {
    gSPDisplayList(gDisplayListHead++, D_0D007F38);
    IceMode(kwflash_r, kwflash_g, kwflash_b, 128, 128, 128, 255);
    KWLoadTextureBlockRGBA16B(common_texture_hud_normal_digit, 104, 16);
    kwgettimer(arg2);
    kwprintlaptime_xlu_sub(arg0, arg1);
}

void KWDisplayTotalTime(s32 playerId) {
    s32 var_s0;

    if ((g_gameMode != 2) && (g_gameMode != 3)) {
        if (kwgoalcnt == playerHUD[playerId].goal) {
            if (replay_flag == 0) {
                for (var_s0 = 0; var_s0 < 3; var_s0++) {
                    if (D_80165658[var_s0] == 0) {
                        kwprintlaptime(playerHUD[playerId].lapCompletionTimeXs[var_s0],
                                    playerHUD[playerId].ty + (var_s0 * 0x10),
                                    playerHUD[playerId].laptimebuf[var_s0]);
                    } else {
                        kwprintlaptime_flash(playerHUD[playerId].lapCompletionTimeXs[var_s0],
                                            playerHUD[playerId].ty + (var_s0 * 0x10),
                                            playerHUD[playerId].laptimebuf[var_s0]);
                    }
                }
                kwsprite32x16(playerHUD[playerId].totalTimeX - 0x13, playerHUD[playerId].ty + 0x38,
                                          common_texture_hud_total_time);
                if (D_801657E5 != 0) {
                    kwprintlaptime_flash(playerHUD[playerId].totalTimeX, playerHUD[playerId].ty + 0x30,
                                        playerHUD[playerId].totaltime);
                } else {
                    kwprintlaptime(playerHUD[playerId].totalTimeX, playerHUD[playerId].ty + 0x30,
                                playerHUD[playerId].totaltime);
                }
            }
        } else {
            if (playerHUD[playerId].flashtm == 0) {
                kwsprite32x16(playerHUD[playerId].timerX - 0x13, playerHUD[playerId].ty + 8,
                                          common_texture_hud_time);
                kwprintlaptime(playerHUD[playerId].timerX, playerHUD[playerId].ty, playerHUD[playerId].totaltime);
            } else {
                kwsprite32x16(playerHUD[playerId].timerX - 0x13, playerHUD[playerId].ty + 8,
                                          common_texture_hud_lap_time);
                if (D_801657E3 != 0) {
                    kwprintlaptime_flash(playerHUD[playerId].timerX, playerHUD[playerId].ty,
                                        playerHUD[playerId].laptime);
                } else if (playerHUD[playerId].dispsw == 0) {
                    kwprintlaptime(playerHUD[playerId].timerX, playerHUD[playerId].ty, playerHUD[playerId].laptime);
                }
            }
        }
    }
}

void kwdisplay_lap88(s16 lapX, s16 lapY, s8 lap) {
    gSPDisplayList(gDisplayListHead++, D_0D008108);
    gSPDisplayList(gDisplayListHead++, D_0D007EF8);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_THRESHOLD);
    KWLoadTextureBlockRGBA16B(common_texture_hud_123, 32, 8);
    // Display current lap. Ex. 1/3

    SprDrawClipST(lapX, lapY, 8, 8, lap * 8, 0, 0); // display the digit
    SprDrawClipST(lapX + 8, lapY, 8, 8, 24, 0, 0);  // display the /
    SprDrawClipST(lapX + 16, lapY, 8, 8, 16, 0, 0); // display the 3
    gSPDisplayList(gDisplayListHead++, D_0D007EB8);
}

void kwdisplay_kaopanel_sub(f32 arg0, f32 arg1, s16 arg2, s16 arg3, s16 characterId, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    if ((g_courseID == COURSE_YOSHI_VALLEY) && (arg3 < 3) && (arg8 == 0)) {
        kwcreateaffine2D((s32) arg0, (s32) arg1, 0U, 1.0f);
        gSPDisplayList(gDisplayListHead++, D_0D007DB8);
        ToumeIMode(0x000000FF, 0x000000FF, 0x000000FF, kwa);
        gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_portrait_bomb_kart_and_question_mark);
        kwloadtextureblock_ci_8b(common_texture_portrait_question_mark, 0x00000020, 0x00000020);
        gSPDisplayList(gDisplayListHead++, D_0D0069E0);
    } else {
        kwcreateaffine2D(arg0, arg1, 0U, 1.0f);
        gSPDisplayList(gDisplayListHead++, D_0D007DB8);
        ToumeIMode(0x000000FF, 0x000000FF, 0x000000FF, arg5);
        gDPLoadTLUT_pal256(gDisplayListHead++, kao_colpal_list[characterId]);
        kwloadtextureblock_ci_8b(kao_index_list[characterId], 0x00000020, 0x00000020);
        if (arg7 != 0) {
            gSPDisplayList(gDisplayListHead++, D_0D0069F8);
        } else {
            gSPDisplayList(gDisplayListHead++, D_0D0069E0);
        }
        if (arg6 != 0) {
            kwcreateaffine2D(arg0, arg1, 0U, 1.0f);
            gSPDisplayList(gDisplayListHead++, D_0D007A60);
            ToumeIMode(wakur, wakug, wakub, 0x000000FF);
            kwloadtextureblock_ia_4b(common_texture_character_portrait_border, 0x20, 0x20);
            gSPDisplayList(gDisplayListHead++, D_0D0069E0);
        }
        gSPDisplayList(gDisplayListHead++, D_0D007DB8);
        ToumeIMode(0x000000FF, 0x000000FF, 0x000000FF, arg5);
        gSPDisplayList(gDisplayListHead++, D_0D007CB8);
        gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_hud_type_C_rank_font);
        kwloadtextureblock_ci_8b(common_texture_hud_type_C_rank_font[arg2], 0x00000010, 0x00000010);
        if (arg7 != 0) {
            kwcreateaffine2D((s32) (arg0 + 9.0f), (s32) (arg1 + 7.0f), 0U, 1.0f);
        } else {
            kwcreateaffine2D((s32) (arg0 - 9.0f), (s32) (arg1 + 7.0f), 0U, 1.0f);
        }
        gSPDisplayList(gDisplayListHead++, D_0D006980);
    }
}

void kwdisplay_kaopanel(void) {
    s16 temp_v0;
    s16 characterId;
    s32 var_s0;
    s32 lapCount;
    s32 var_a0;

    if (kwgoalpanelsw == 0) {
        for (var_s0 = 0; var_s0 < 4; var_s0++) {
            var_a0 = 0;
            if (g_hudCharpicRankY1[var_s0] >= 0.0f) {
                if (D_8018D078[var_s0] < 0.0) {
                    var_a0 = 1;
                }
                temp_v0 = rank_to_num[var_s0];
                characterId = gGPCurrentRaceCharacterIdByRank[var_s0];
                lapCount = gLapCountByPlayerId[temp_v0];
                if (characterId == gPlayerOne->kart) {
                    kwdisplay_kaopanel_sub(panelx[var_s0], g_hudCharpicRankY1[var_s0], var_s0, lapCount, characterId, 0x000000FF, 1,
                                  var_a0, 0);
                } else {
                    kwdisplay_kaopanel_sub(panelx[var_s0], g_hudCharpicRankY1[var_s0], var_s0, lapCount, characterId, kwa, 0,
                                  var_a0, 0);
                }
            }
        }
    } else {
        for (var_s0 = 0; var_s0 < 8; var_s0++) {
            var_a0 = 0;
            if (g_hudCharpicRankY1[var_s0] >= 0.0f) {
                if (D_8018D078[var_s0] <= 0.0) {
                    var_a0 = 1;
                }
                temp_v0 = rank_to_num[var_s0];
                // ????
                characterId = (gPlayerOne + temp_v0)->kart;
                lapCount = gLapCountByPlayerId[temp_v0];
                if (temp_v0 == 0) {
                    kwdisplay_kaopanel_sub(panelx[var_s0], g_hudCharpicRankY1[var_s0], var_s0, lapCount, characterId, 0x000000FF, 1,
                                  var_a0, 1);
                } else {
                    kwdisplay_kaopanel_sub(panelx[var_s0], g_hudCharpicRankY1[var_s0], var_s0, lapCount, characterId, 0x000000FF, 0,
                                  var_a0, 1);
                }
            }
        }
    }
    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}

s32 kwcalc_enemycar_zahyou(u16 arg0, s32* arg1, s32* arg2) {
    s32 var_v0 = 0;
    s32 thing = 0;
    s32 test = gLapCountByPlayerId[arg0];

    if (test < 3) {
        if (g_playerCount == 1) {
            if (test >= 0) {
                thing = (s32) (rate_count_F[arg0] * 928);
            }
            if (thing < 0x104) {
                *arg1 = thing;
                *arg2 = 0;
                var_v0 = 1;
            } else if (thing < 0x1D0) {
                *arg1 = 0x00000104;
                *arg2 = thing - 0x104;
                var_v0 = 2;
            } else if (thing < 0x2D4) {
                *arg1 = 0x2D4 - thing;
                *arg2 = 0x000000CC;
                var_v0 = 3;
            } else {
                *arg1 = 0;
                *arg2 = 0x3A0 - thing;
                var_v0 = 4;
            }
        } else {
            if (test >= 0) {
                thing = (s32) (rate_count_F[arg0] * 260);
            }
            *arg1 = thing;
            *arg2 = 0;
        }
    } else if (g_playerCount == 1) {
        *arg1 = 0x00000020;
        *arg2 = (gGPCurrentRaceRankByPlayerId[arg0] * 0x14) + 0x20;
    } else {
        thing = (s32) (rate_count_F[arg0] * 260);
        *arg1 = thing;
        *arg2 = 0;
    }
    return var_v0;
}

void kwcalc_bombkart_zahyou(u16 bombIndex, s32* arg1, s32* arg2) {
    s32 temp_v0 = bom[bombIndex].pathPointIndex;
    s32 var_v1 = 0;

    if (temp_v0 != 0) {
        var_v1 = (s32) (temp_v0 * 0x3A0) / (s32) center_line_max;
    }
    if (var_v1 < 0x104) {
        *arg1 = var_v1;
        *arg2 = 0;
    } else if (var_v1 < 0x1D0) {
        *arg1 = 0x00000104;
        *arg2 = var_v1 - 0x104;
    } else if (var_v1 < 0x2D4) {
        *arg1 = 0x2D4 - var_v1;
        *arg2 = 0x000000CC;
    } else {
        *arg1 = 0;
        *arg2 = 0x3A0 - var_v1;
    }
}

void kwdisplay_road(void) {
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 temp_v1;
    s16 var_s0;
    UNUSED s16 stackPadding;
    u16 var_s0_2;
    u16 var_s1;
    u16 var_s2;

    if (g_gameMode == TIME_TRIALS) {
        var_s0 = real_rank_to_num[0];
    } else {
        var_s0 = rank_to_num[0];
    }
    sp4C = kwcalc_enemycar_zahyou(var_s0, &sp54, &sp50);
    temp_v1 = gLapCountByPlayerId[var_s0];
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

void kwdisplay_bombpanel(void) {
    UNUSED s32 stackPadding0;
    s32 sp88;
    s32 sp84;
    UNUSED s32 stackPadding1;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < NUM_BOMB_KARTS_VERSUS; var_s1++) {
        if ((bom[var_s1].state != BOMB_STATE_EXPLODED) && (bom[var_s1].state != BOMB_STATE_INACTIVE)) {
            kwcalc_bombkart_zahyou(var_s1, &sp88, &sp84);
            gSPDisplayList(gDisplayListHead++, D_0D007DB8);
            gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_portrait_bomb_kart_and_question_mark);
            kwloadtextureblock_ci_8b(common_texture_portrait_bomb_kart, 0x00000020, 0x00000020);
            kwcreateaffine2D(sp88 + 0x20, sp84 + 0x12, 0U, 0.6f);
            gSPDisplayList(gDisplayListHead++, D_0D0069E0);
        }
    }
}

void kwdisplay_enemycar_sub(s32 playerId, s32 arg1) {
    s32 objectIndex;
    s32 spD0;
    s32 spCC;
    Player *dummy = &gPlayerOne[playerId];
    s32 spC4;
    s32 lapCount;
    s32 characterId;
    s32 spB8;
    s32 temp_v0_2;
    AnmObject* object;
    Player *player = &gPlayerOne[playerId];

    lapCount = gLapCountByPlayerId[playerId];
    characterId = player->kart;
    objectIndex = D_8018CE10[playerId].objectIndex;

    if (g_playerCount == 1) {
        spC4 = 0x00000012;
    } else {
        spC4 = 0x00000078;
    }

    temp_v0_2 = kwcalc_enemycar_zahyou(playerId, &spD0, &spCC);
    if ((temp_v0_2 == 2) || (temp_v0_2 == 3)) {
        spB8 = 1;
    } else {
        spB8 = 0;
    }

    if ((g_courseID == COURSE_YOSHI_VALLEY) && (lapCount < 3)) {
        gSPDisplayList(gDisplayListHead++, D_0D007DB8);
        gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_portrait_bomb_kart_and_question_mark);
        kwloadtextureblock_ci_8b(common_texture_portrait_question_mark, 0x00000020, 0x00000020);
        object = &g_DynamicObjects[objectIndex];
        object->pos[0] = object->pos_spline[0] + ((f32) (spD0 + 0x20));
        object->pos[1] = object->pos_spline[1] + ((f32) (spC4 + spCC));
        object->pos[2] = object->pos_spline[2];
        KWCreateAffine3D(object->pos, object->ang, object->scale);
        gSPDisplayList(gDisplayListHead++, D_0D0069E0);
    } else {
        gDPLoadTLUT_pal256(gDisplayListHead++, kao_colpal_list[characterId]);
        gSPDisplayList(gDisplayListHead++, D_0D007DB8);
        if (player->slip_flag & STAR) {
            IceMode((s32) kwflash_r, (s32) kwflash_g, (s32) kwflash_b, 0x00000080, 0x00000080, 0x00000080,
                          (s32) g_DynamicObjects[objectIndex].alpha);
        } else {
            SubAlphaMode((s32) g_DynamicObjects[objectIndex].alpha);
        }
        kwloadtextureblock_ci_8b(kao_index_list[characterId], 0x00000020, 0x00000020);
        object = &g_DynamicObjects[objectIndex];
        object->pos[0] = object->pos_spline[0] + ((f32) (spD0 + 0x20));
        object->pos[1] = object->pos_spline[1] + ((f32) (spC4 + spCC));
        object->pos[2] = object->pos_spline[2];
        KWCreateAffine3D(object->pos, object->ang, object->scale);
        if (spB8 != 0) {
            gSPDisplayList(gDisplayListHead++, D_0D0069F8);
        } else {
            gSPDisplayList(gDisplayListHead++, D_0D0069E0);
        }
        gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_hud_type_C_rank_tiny_font);
        kwloadtextureblock_ci_8b(common_texture_hud_type_C_rank_tiny_font[arg1 + 1], 8, 8);
        if (spB8 != 0) {
            kwcreateaffine2D(spD0 + 0x26, (spC4 + spCC) + 4, 0U, 1.0f);
        } else {
            kwcreateaffine2D(spD0 + 0x1B, (spC4 + spCC) + 4, 0U, 1.0f);
        }
        gSPDisplayList(gDisplayListHead++, D_0D006950);
        if ((player == gPlayerOne) && (g_ScreenSplitB == SCREEN_MODE_1P)) {
            gSPDisplayList(gDisplayListHead++, D_0D007A40);
            ToumeIMode(wakur, wakug, wakub, 0x000000FF);
            kwloadtextureblock_ia_4b(common_texture_character_portrait_border, 0x00000020, 0x00000020);
            KWCreateAffine3D(object->pos, object->ang, object->scale);
            gSPDisplayList(gDisplayListHead++, D_0D0069E0);
        }
    }
}

void kwdisplay_enemycar(void) {
    s32 temp_a0;
    s32 var_s0;
    s32 var_s1;
    s32 var_s3;
    Player* player;

    if (g_ScreenSplitB == 0) {
        kwdisplay_road();
    }
    var_s3 = 8;
    if ((g_playerCount == 2) && (g_ScreenSplitA == 2)) {
        var_s3 = 0;
    }
    for (var_s0 = var_s3 - 1, var_s1 = 0; var_s1 < var_s3; var_s1++, var_s0--) {
        temp_a0 = rank_to_num[var_s0];
        player = &gPlayerOne[temp_a0];
        if ((player->flag & EXISTS) && ((temp_a0 != 0) || (g_playerCount != 1))) {
            kwdisplay_enemycar_sub(temp_a0, var_s0);
        }
    }
    if (g_gameMode == 1) {
        kwdisplay_enemycar_sub(0, real_rank[0]);
    } else if (g_playerCount == 1) {
        kwdisplay_enemycar_sub(0, gGPCurrentRaceRankByPlayerId[0]);
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void kwdisplay_leaf(UNUSED s32 cameraId) {
    s32 someIndex;
    s32 leafIndex;
    AnmObject* object;

    gSPDisplayList(gDisplayListHead++, D_0D0079C8);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    KWLoadTextureBlockRGBA16B(common_texture_particle_leaf, 0x00000020, 0x00000010);
    for (someIndex = 0; someIndex < gLeafParticle_SIZE; someIndex++) {
        leafIndex = leafallocptr[someIndex];
        if (leafIndex != -1) {
            object = &g_DynamicObjects[leafIndex];
            if ((object->anmptr >= 2) && (object->chartptr == 7) && (modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
                KWCreateAffine3D_Anm(leafIndex);
                gSPDisplayList(gDisplayListHead++, D_0D0069C8);
            }
        }
    }
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void kwdisplay_snow(void) {
    s32 someIndex;
    s32 snowflakeIndex;

    gSPDisplayList(gDisplayListHead++, D_0D007AE0);
    gDPSetCombineLERP(gDisplayListHead++, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0);
    kwloadtextureblock_a_4b(D_0D0293D8, 0x10, 0x10);
    for (someIndex = 0; someIndex < NUM_SNOWFLAKES; someIndex++) {
        snowflakeIndex = EffectAllocArray1[someIndex];
        if (g_DynamicObjects[snowflakeIndex].anmptr >= 2) {
            KWCreateAffine3D_Anm(snowflakeIndex);
            gSPDisplayList(gDisplayListHead++, D_0D006980);
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void kwdisplay_kumo_draw(s32 objectIndex, s16 arg1, s16 arg2) {
    UNUSED s32 pad[1];
    if (g_DynamicObjects[objectIndex].flag & 0x10) {
        if (kumotype != g_DynamicObjects[objectIndex].chartptr) {
            kumotype = g_DynamicObjects[objectIndex].chartptr;
            KWLoadTextureBlockI4b(g_DynamicObjects[objectIndex].indexaddr, g_DynamicObjects[objectIndex].texsizey,
                          g_DynamicObjects[objectIndex].fvalptr);
        }
        kwcreateaffine2D(arg1, arg2, 0U, g_DynamicObjects[objectIndex].scale);
        gSPVertex(gDisplayListHead++, g_DynamicObjects[objectIndex].vtxaddr, 4, 0);
        gSPDisplayList(gDisplayListHead++, common_rectangle_display);
    }
}

void kwdisplay_star_draw(s32 objectIndex, s16 arg1, s16 arg2) {
    if (g_DynamicObjects[objectIndex].flag & 0x10) {
        if (kumotype != g_DynamicObjects[objectIndex].chartptr) {
            kumotype = g_DynamicObjects[objectIndex].chartptr;
            KWLoadTextureBlockI4b(g_DynamicObjects[objectIndex].indexaddr, g_DynamicObjects[objectIndex].texsizey,
                          g_DynamicObjects[objectIndex].fvalptr);
        }
        KWSetPrimColor(0x000000FF, 0x000000FF, 0x000000FF, g_DynamicObjects[objectIndex].alpha);
        kwcreateaffine2D(arg1, arg2, 0U, g_DynamicObjects[objectIndex].scale);
        gSPVertex(gDisplayListHead++, g_DynamicObjects[objectIndex].vtxaddr, 4, 0);
        gSPDisplayList(gDisplayListHead++, common_rectangle_display);
    }
}

void kwdisplay_kumo_sub(s16 arg0, s32 arg1) {
    s32 var_s0;
    s32 objectIndex;
    AnmObject* object;

    kumotype = 0xFF;
    gSPDisplayList(gDisplayListHead++, D_0D007A60);
    if ((u8) kwhoshisw != 0) {
        SplashMode(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
        for (var_s0 = 0; var_s0 < CloudCount1P; var_s0++) {
            objectIndex = g_CloudAllocate[arg1 + var_s0];
            object = &g_DynamicObjects[objectIndex];
            kwdisplay_star_draw(objectIndex, object->ix, arg0 - object->iy);
        }
    } else {
        KumoColorMode(0x000000FF, 0x000000FF, 0x000000FF);
        for (var_s0 = 0; var_s0 < CloudCount1P; var_s0++) {
            objectIndex = g_CloudAllocate[arg1 + var_s0];
            object = &g_DynamicObjects[objectIndex];
            kwdisplay_kumo_draw(objectIndex, object->ix, arg0 - object->iy);
        }
    }
}

void kwdisplay_kumo_2pud_sub(s16 arg0, s32 arg1) {
    s16 var_s5;
    s32 var_s0;
    s32 objectIndex;
    AnmObject* object;

    if (D_801658FE == 0) {
        if (g_courseID == COURSE_KOOPA_BEACH) {
            var_s5 = arg0;
        } else if (g_courseID == COURSE_MOO_MOO_FARM) {
            var_s5 = arg0 - 0x10;
        } else if (g_courseID == COURSE_YOSHI_VALLEY) {
            var_s5 = arg0 - 0x10;
        } else {
            var_s5 = arg0 + 0x10;
        }
    } else if (g_courseID == COURSE_KOOPA_BEACH) {
        var_s5 = arg0 * 2;
    } else {
        var_s5 = arg0 + 0x20;
    }
    kumotype = 0xFF;
    gSPDisplayList(gDisplayListHead++, D_0D007A60);
    if ((u8) kwhoshisw != 0) {
        SplashMode(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
        for (var_s0 = 0; var_s0 < CloudCount1P; var_s0++) {
            objectIndex = g_CloudAllocate[arg1 + var_s0];
            object = &g_DynamicObjects[objectIndex];
            kwdisplay_star_draw(objectIndex, object->ix, (var_s5 - object->iy) / 2);
        }
    } else {
        KumoColorMode(0x000000FF, 0x000000FF, 0x000000FF);
        for (var_s0 = 0; var_s0 < CloudCount1P; var_s0++) {
            objectIndex = g_CloudAllocate[arg1 + var_s0];
            object = &g_DynamicObjects[objectIndex];
            kwdisplay_kumo_draw(objectIndex, object->ix, (var_s5 - object->iy) / 2);
        }
    }
}

void kwdisplay_kumo_1p(void) {
    kwdisplay_kumo_sub(240 - D_800DC5EC->horizon, 0); // 28
}

void kwdisplay_kumo_2pl(void) {
    s16 temp_a0;

    temp_a0 = 0xF0 - D_800DC5EC->horizon;
    if (g_courseID == COURSE_KOOPA_BEACH) {
        temp_a0 = temp_a0 - 0x30;
    } else if (g_courseID == COURSE_MOO_MOO_FARM) {
        temp_a0 = temp_a0 - 0x40;
    } else if (g_courseID == COURSE_YOSHI_VALLEY) {
        temp_a0 = temp_a0 - 0x40;
    } else {
        temp_a0 = temp_a0 - 0x30;
    }
    kwdisplay_kumo_sub(temp_a0, 0);
}

void kwdisplay_kumo_2pr(void) {
    s16 temp_a0;

    temp_a0 = 0xF0 - D_800DC5F0->horizon;
    if (g_courseID == COURSE_KOOPA_BEACH) {
        temp_a0 = temp_a0 - 0x30;
    } else if (g_courseID == COURSE_MOO_MOO_FARM) {
        temp_a0 = temp_a0 - 0x40;
    } else if (g_courseID == COURSE_YOSHI_VALLEY) {
        temp_a0 = temp_a0 - 0x40;
    } else {
        temp_a0 = temp_a0 - 0x30;
    }
    kwdisplay_kumo_sub(temp_a0, CloudCount1P);
}

void kwdisplay_kumo_2pu(void) {
    kwdisplay_kumo_2pud_sub(240 - D_800DC5EC->horizon, 0);
}

void kwdisplay_kumo_2pd(void) {
    kwdisplay_kumo_2pud_sub(240 - D_800DC5F0->horizon, CloudCount1P);
}

void kwset_pukupuku_light(s32 arg0) {
    if (g_DynamicObjects[arg0].chartptr == 0) {
        pukupuku_material[0].l[0].l.dir[0] = pukupuku_material[1].l[0].l.dir[0] = pukupuku_material[2].l[0].l.dir[0] =
            pukupuku_material[3].l[0].l.dir[0] = 0;
        pukupuku_material[0].l[0].l.dir[1] = pukupuku_material[1].l[0].l.dir[1] = pukupuku_material[2].l[0].l.dir[1] =
            pukupuku_material[3].l[0].l.dir[1] = -0x78;
        pukupuku_material[0].l[0].l.dir[2] = pukupuku_material[1].l[0].l.dir[2] = pukupuku_material[2].l[0].l.dir[2] =
            pukupuku_material[3].l[0].l.dir[2] = 0;
    } else {
        pukupuku_material[0].l[0].l.dir[0] = pukupuku_material[1].l[0].l.dir[0] = pukupuku_material[2].l[0].l.dir[0] =
            pukupuku_material[3].l[0].l.dir[0] = 0x63;
        pukupuku_material[0].l[0].l.dir[1] = pukupuku_material[1].l[0].l.dir[1] = pukupuku_material[2].l[0].l.dir[1] =
            pukupuku_material[3].l[0].l.dir[1] = 0x42;
        pukupuku_material[0].l[0].l.dir[2] = pukupuku_material[1].l[0].l.dir[2] = pukupuku_material[2].l[0].l.dir[2] =
            pukupuku_material[3].l[0].l.dir[2] = 0;
    }
}

void kwdisplay_pukupuku(UNUSED s32 arg0) {
    AnmObject* object;
    s32 temp_a3;

    temp_a3 = objallocptr2[0];
    object = &g_DynamicObjects[temp_a3];
    if (object->anmptr >= 2) {
        if (kwanm_bitcheckflag(temp_a3, 0x00000010) != 0) {
            KWCreateAffine3D(object->pos, object->ang, object->scale);
            kwset_pukupuku_light(temp_a3);

            gSPDisplayList(gDisplayListHead++, D_0D007828);
            gSPLight(gDisplayListHead++, &pukupuku_material[0].l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &pukupuku_material[0].a, LIGHT_2);
            gSPDisplayList(gDisplayListHead++, d_course_banshee_boardwalk_dl_7B38);
            gSPLight(gDisplayListHead++, &pukupuku_material[1].l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &pukupuku_material[1].a, LIGHT_2);
            gSPDisplayList(gDisplayListHead++, d_course_banshee_boardwalk_dl_7978);
            gSPLight(gDisplayListHead++, &pukupuku_material[2].l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &pukupuku_material[2].a, LIGHT_2);
            gSPDisplayList(gDisplayListHead++, d_course_banshee_boardwalk_dl_78C0);
            gSPLight(gDisplayListHead++, &pukupuku_material[3].l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &pukupuku_material[3].a, LIGHT_2);
            gSPDisplayList(gDisplayListHead++, d_course_banshee_boardwalk_dl_7650);
        }
    }
}

void kwdisplay_tellesa_sub(s32 objectIndex, s32 arg1, u32 arg2) {
    UNUSED s32 pad[2];
    AnmObject* object;
    Camera* camera = &camera1[arg1];

    object = &g_DynamicObjects[objectIndex];
    object->texang[1] = KWLookCamera(object->pos[0], object->pos[2], camera->camera_pos);
    kwtexture3D_ci8_xlu_zb_bl(object->pos, object->texang, object->scale, object->alpha, (u8*) object->texaddr,
                  object->indexaddr, object->vtxaddr, 0x00000030, 0x00000028, 0x00000030, 0x00000028);
    if ((kwanm_bitcheckflag(objectIndex, 0x00000020) != 0) && (arg2 < 0x15F91U)) {
        kwdraw_kage3D(&D_8018C830, object->pos, 0.4f);
    }
}

void kwdisplay_tellesa(s32 arg0) {
    u32 temp_s2;
    s32 someIndex;
    s32 objectIndex;

    for (someIndex = 0; someIndex < NUM_BOOS; someIndex++) {
        objectIndex = objallocptr3[someIndex];
        if (g_DynamicObjects[objectIndex].anmptr >= 2) {
            temp_s2 = kwcheck_vis_area(objectIndex, arg0, 0x4000U, 0x00000320);
            if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
                kwdisplay_tellesa_sub(objectIndex, arg0, temp_s2);
            }
        }
    }
}

void kwdisplay_bat(s32 cameraId) {
    s32 var_s2;
    s32 objectIndex;
    Camera* temp_s7;

    objectIndex = objallocptr[0];
    temp_s7 = &camera1[cameraId];
    kwloadtexture_ci8_aa_zb_bl_mr(g_DynamicObjects[objectIndex].texaddr, g_DynamicObjects[objectIndex].indexaddr, 0x00000020, 0x00000040,
                  5);
    D_80183E80[0] = g_DynamicObjects[objectIndex].texang[0];
    D_80183E80[2] = g_DynamicObjects[objectIndex].texang[2];
    if ((D_8018CFB0 != 0) || (D_8018CFC8 != 0)) {
        for (var_s2 = 0; var_s2 < 40; var_s2++) {
            objectIndex = EffectAllocArray2[var_s2];
            if (objectIndex == -1) {
                continue;
            }

            if ((g_DynamicObjects[objectIndex].anmptr >= 2) && (modelingnum < 0x2EF)) {
                D_80183E80[1] =
                    KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], temp_s7->camera_pos);
                kwset_tile(g_DynamicObjects[objectIndex].pos, D_80183E80, g_DynamicObjects[objectIndex].scale,
                              D_0D0062B0);
            }
        }
    }
    if ((D_8018CFE8 != 0) || (D_8018D000 != 0)) {
        for (var_s2 = 0; var_s2 < 30; var_s2++) {
            objectIndex = EffectAllocArray3[var_s2];
            if (objectIndex == -1) {
                continue;
            }

            if ((g_DynamicObjects[objectIndex].anmptr >= 2) && (modelingnum < 0x2EF)) {
                D_80183E80[1] =
                    KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], temp_s7->camera_pos);
                kwset_tile(g_DynamicObjects[objectIndex].pos, D_80183E80, g_DynamicObjects[objectIndex].scale,
                              D_0D0062B0);
            }
        }
    }
    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}

void kwdisplay_kanoke(s32 cameraId) {
    s32 objectIndex;
    AnmObject* object;

    objectIndex = objallocptr[1];
    kwcheck_vis_area(objectIndex, cameraId, 0x5555U, 0x00000320);
    if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
        object = &g_DynamicObjects[objectIndex];
        if (object->anmptr >= 2) {
            kwpolygon3D(object->pos, object->texang, object->scale, object->shape);
        }
    }
}

void kwdisplay_iceblock_sub(s32 arg0) {
    Player* temp_v0;

    temp_v0 = &gPlayerOne[arg0];
    D_80183E40[0] = temp_v0->position[0];
    D_80183E40[1] = temp_v0->position[1];
    D_80183E40[2] = temp_v0->position[2];
    D_80183E80[0] = 0;
    D_80183E80[1] = 0;
    D_80183E80[2] = 0;
    kwpolygon3D_iceblock(D_80183E40, D_80183E80, 0.02f, IceKageGfx);
}

void kwdisplay_iceblock_kakera(s32 arg0) {
    s32 var_s3;
    s32 objectIndex;
    AnmObject* object;

    D_80183E80[0] = D_8016582C[0];
    D_80183E80[1] = D_8016582C[1];
    D_80183E80[2] = D_8016582C[2];
    gSPDisplayList(gDisplayListHead++, D_0D007B00);
    gSPNumLights(gDisplayListHead++, 1);
    gSPLight(gDisplayListHead++, &iceblock3_material.l[0], LIGHT_1);
    gSPLight(gDisplayListHead++, &iceblock3_material.a, LIGHT_2);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_LIGHTING | G_SHADING_SMOOTH);
    kwloadtextureblock_ia_16b(d_course_sherbet_land_ice, 0x00000020, 0x00000020);
    if (g_playerCount < 3) {
        for (var_s3 = 0; var_s3 < gObjectParticle2_SIZE; var_s3++) {
            objectIndex = EffectAllocArray2[var_s3];
            if (objectIndex != NULL_OBJECT_ID) {
                object = &g_DynamicObjects[objectIndex];
                if (object->anmptr > 0) {
                    KWCreateAffine3D(object->pos, D_80183E80, object->scale);
                    gSPVertex(gDisplayListHead++, D_0D005BD0, 3, 0);
                    gSPDisplayList(gDisplayListHead++, D_0D006930);
                }
            }
        }
    } else {
        for (var_s3 = 0; var_s3 < gObjectParticle2_SIZE; var_s3++) {
            objectIndex = EffectAllocArray2[var_s3];
            if (objectIndex != NULL_OBJECT_ID) {
                object = &g_DynamicObjects[objectIndex];
                if ((object->anmptr > 0) && (arg0 == object->stack[7]) && (modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
                    KWCreateAffine3D(object->pos, D_80183E80, object->scale);
                    gSPVertex(gDisplayListHead++, D_0D005BD0, 3, 0);
                    gSPDisplayList(gDisplayListHead++, D_0D006930);
                }
            }
        }
    }
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}

void KWDisplayIceBlock(s32 arg0) {
    s32 playerId;
    s32 objectIndex;

    iceblock3_material.l[0].l.dir[0] = D_80165840[0];
    iceblock3_material.l[0].l.dir[1] = D_80165840[1];
    iceblock3_material.l[0].l.dir[2] = D_80165840[2];
    gSPLight(gDisplayListHead++, &iceblock3_material.l[0], LIGHT_1);
    gSPLight(gDisplayListHead++, &iceblock3_material.a, LIGHT_2);
    for (playerId = 0; playerId < g_playerCount; playerId++) {
        objectIndex = jugemuallocptr[playerId];
        if (objectIndex) {}
        if (kwanm_bitcheckevflag(objectIndex, 4) != false) {
            kwdisplay_iceblock_sub(playerId);
        }
        kwanm_bitcheckevflag(objectIndex, 0x00000010);
    }
    kwdisplay_iceblock_kakera(arg0);
}

void kwdisplay_iceblock_kage_sub(s32 playerId) {
    s32 test;
    Player* temp_v1;

    temp_v1 = &gPlayerOne[playerId];
    test = jugemuallocptr[playerId];
    if (kwanm_bitcheckevflag(test, 8) != 0) {
        D_80183E40[0] = temp_v1->position[0];
        D_80183E40[1] = temp_v1->ground - 6.5;
        D_80183E40[2] = temp_v1->position[2];
        kwpolygon3D_icekage(D_80183E40, (u16*) D_80183E80, 0.02f, IceKageGfx, 0x000000FF);
    }
}

void KWDisplayIceBlockShadow(UNUSED s32 arg0) {
    s32 var_s0;

    iceblock3_material.l[0].l.dir[0] = D_80165840[0];
    iceblock3_material.l[0].l.dir[1] = D_80165840[1];
    iceblock3_material.l[0].l.dir[2] = D_80165840[2];
    gSPLight(gDisplayListHead++, &iceblock3_material.l[0], LIGHT_1);
    gSPLight(gDisplayListHead++, &iceblock3_material.a, LIGHT_2);
    D_80183E80[0] = 0;
    D_80183E80[1] = 0;
    D_80183E80[2] = 0;
    if (g_menuMultiplayerSelection == 1) {
        for (var_s0 = 0; var_s0 < g_playerCount; var_s0++) {
            kwdisplay_iceblock_kage_sub(var_s0);
        }
    }
}

void kwdisplay_snowman_kakera(s32 cameraId) {
    UNUSED s32 stackPadding[2];
    Camera* sp44;
    s32 someIndex;
    s32 objectIndex;
    AnmObject* object;

    sp44 = &camera1[cameraId];
    kwloadtexture_ci8_aa_zb_bl(d_course_frappe_snowland_snow_tlut, d_course_frappe_snowland_snow, 0x00000020, 0x00000020);
    for (someIndex = 0; someIndex < gObjectParticle2_SIZE; someIndex++) {
        objectIndex = EffectAllocArray2[someIndex];
        if (objectIndex != NULL_OBJECT_ID) {
            object = &g_DynamicObjects[objectIndex];
            if (object->anmptr > 0) {
                kwcheck_vis_area(objectIndex, cameraId, 0x2AABU, 0x000001F4);
                if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
                    object->texang[1] = KWLookCamera(object->pos[0], object->pos[2], sp44->camera_pos);
                    KWCreateAffine3D_Anm(objectIndex);
                    gSPDisplayList(gDisplayListHead++, D_0D0069E0);
                }
            }
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void kwdisplay_snowman_sub(s32 cameraId) {
    s32 var_s4;
    s32 objectIndex;
    Camera* camera;

    camera = &camera1[cameraId];
    for (var_s4 = 0; var_s4 < NUM_SNOWMEN; var_s4++) {
        objectIndex = objallocptr[var_s4];
        if (g_DynamicObjects[objectIndex].anmptr >= 2) {
            kwcheck_vis_area(objectIndex, cameraId, 0x2AABU, 0x00000258);
            if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
                D_80183E80[0] = (s16) g_DynamicObjects[objectIndex].texang[0];
                D_80183E80[1] =
                    KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], camera->camera_pos);
                D_80183E80[2] = (u16) g_DynamicObjects[objectIndex].texang[2];
                if (kwanm_bitcheckflag(objectIndex, 0x00000010) != 0) {
                    kwtexture3D_ci8_aa_zb_bl(g_DynamicObjects[objectIndex].pos, (u16*) D_80183E80,
                                       g_DynamicObjects[objectIndex].scale, (u8*) g_DynamicObjects[objectIndex].texaddr,
                                       g_DynamicObjects[objectIndex].indexaddr, g_DynamicObjects[objectIndex].vtxaddr,
                                       0x00000040, 0x00000040, 0x00000040, 0x00000020);
                }
                objectIndex = objallocptr2[var_s4];
                D_80183E80[0] = (s16) g_DynamicObjects[objectIndex].texang[0];
                D_80183E80[2] = (u16) g_DynamicObjects[objectIndex].texang[2];
                kwtexture3D_ci8_aa_zb_bl(g_DynamicObjects[objectIndex].pos, (u16*) D_80183E80,
                                   g_DynamicObjects[objectIndex].scale, (u8*) g_DynamicObjects[objectIndex].texaddr,
                                   g_DynamicObjects[objectIndex].indexaddr, g_DynamicObjects[objectIndex].vtxaddr, 0x00000040,
                                   0x00000040, 0x00000040, 0x00000020);
            }
        }
    }
}

void kwdisplay_snowman(s32 arg0) {
    kwdisplay_snowman_sub(arg0);
    kwdisplay_snowman_kakera(arg0);
}

void KWDisplayJugemu(s32 cameraId) {
    UNUSED s32 stackPadding;
    Camera* camera;
    f32 var_f0;
    f32 var_f2;
    s32 objectIndex;
    AnmObject* object;

    objectIndex = jugemuallocptr[cameraId];
    camera = &camera1[cameraId];
    if (kwanm_bitcheckflag(objectIndex, 0x00000010) != 0) {
        object = &g_DynamicObjects[objectIndex];
        object->texang[0] = 0;
        object->texang[1] = KWLookCamera(object->pos[0], object->pos[2], camera->camera_pos);
        object->texang[2] = 0x8000;
        if (kwanm_bitoffcheckevflag(objectIndex, 2) != 0) {
            kwtexture3D_ci8_aa_zb_bl(object->pos, object->texang, object->scale, (u8*) object->texaddr,
                               object->indexaddr, object->vtxaddr, (s32) object->texsizey,
                               (s32) object->fvalptr, (s32) object->texsizey,
                               (s32) object->fvalptr / 2);
        } else {
            kwtexture3D_ci8_pcl_aa_zb_bl(object->pos, object->texang, object->scale, (s32) object->alpha,
                          (u8*) object->texaddr, object->indexaddr, object->vtxaddr, (s32) object->texsizey,
                          (s32) object->fvalptr, (s32) object->texsizey, (s32) object->fvalptr / 2);
        }
        if (g_ScreenSplitB == SCREEN_MODE_1P) {
            var_f0 = object->pos[0] - D_8018CF14->camera_pos[0];
            var_f2 = object->pos[2] - D_8018CF14->camera_pos[2];
            if (var_f0 < 0.0f) {
                var_f0 = -var_f0;
            }
            if (var_f2 < 0.0f) {
                var_f2 = -var_f2;
            }
            if ((var_f0 + var_f2) <= 200.0) {
                kwdraw_kage3D(&D_8018C0B0[cameraId], object->pos, 0.35f);
            }
        }
    }
}

void kwset_doshin_lightvec(UNUSED s32 arg0) {
    kwcalc_lightvector();
    dosun_material.l[0].l.dir[0] = D_80165840[0];
    dosun_material.l[0].l.dir[1] = D_80165840[1];
    dosun_material.l[0].l.dir[2] = D_80165840[2];
}

void kwset_doshin_light(s32 objectIndex) {
    // Why these don't just use `gSPSetLights1` calls...
    switch (g_DynamicObjects[objectIndex].output) { // hmm very strange 80165C18
        case 0:
            gSPLight(gDisplayListHead++, &dosun_material.l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &dosun_material.a, LIGHT_2);
            break;
        case 1:
            gSPLight(gDisplayListHead++, &D_800E4650.l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E4650.a, LIGHT_2);
            break;
        case 2:
            gSPLight(gDisplayListHead++, &D_800E4668.l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E4668.a, LIGHT_2);
            break;
        case 3:
            gSPLight(gDisplayListHead++, &D_800E4680.l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E4680.a, LIGHT_2);
            break;
        case 4:
            gSPLight(gDisplayListHead++, &D_800E4698.l[0], LIGHT_1);
            gSPLight(gDisplayListHead++, &D_800E4698.a, LIGHT_2);
            break;
        default:
            break;
    }
}

void kwdisplay_doshin_sub(s32 objectIndex) {
    if ((g_DynamicObjects[objectIndex].anmptr >= 2) && (kwanm_bitoffcheckevflag(objectIndex, 0x00000040) != 0)) {
        kwdraw_kage3D_flat(objectIndex, 1.75f);
        KWCreateAffine3D(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].texang,
                                      g_DynamicObjects[objectIndex].scale);
        kwset_doshin_light(objectIndex);
        gSPDisplayList(gDisplayListHead++, D_0D007828);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        gDPLoadTLUT_pal256(gDisplayListHead++, d_course_bowsers_castle_thwomp_tlut);
        kwloadtextureblock_ci_8b_mr(g_DynamicObjects[objectIndex].indexaddr, 0x00000010, 0x00000040, 4);
        gSPDisplayList(gDisplayListHead++, g_DynamicObjects[objectIndex].shape);
    }
}

void kwdisplay_doshin(s32 cameraId) {
    s32 objectIndex;
    s32 i;
    UNUSED s32 stackPadding0;
    s16 minusone, plusone;
    Camera* camera;
    AnmObject* object;

    camera = &camera1[cameraId];
    if (cameraId == PLAYER_ONE) {
        for (i = 0; i < gNumActiveThwomps; i++) {
            objectIndex = objallocptr[i];
            kwanm_clrflag(objectIndex, 0x00070000);
            kwanm_clrevflag(objectIndex, 0x00000110);
        }
    }

    kwset_doshin_lightvec(objectIndex);
    for (i = 0; i < gNumActiveThwomps; i++) {
        objectIndex = objallocptr[i];
        minusone = g_DynamicObjects[objectIndex].unk_0DF - 1;
        plusone = g_DynamicObjects[objectIndex].unk_0DF + 1;
        if (gGamestate != 9) {
            if ((D_8018CF68[cameraId] >= minusone) && (plusone >= D_8018CF68[cameraId]) &&
                (kwanm_vischeck_camera_xz(objectIndex, camera, 0x8000U) != 0)) {
                kwdisplay_doshin_sub(objectIndex);
            }
        } else {
            kwdisplay_doshin_sub(objectIndex);
        }
    }
    gSPDisplayList(gDisplayListHead++, D_0D0079C8);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gSPNumLights(gDisplayListHead++, 1);
    gSPLight(gDisplayListHead++, &D_800E4668.l[0], LIGHT_1);
    gSPLight(gDisplayListHead++, &D_800E4668.a, LIGHT_2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_LIGHTING | G_SHADING_SMOOTH);
    KWLoadTextureBlockRGBA16B((u16 *) d_course_bowsers_castle_thwomp_side, 0x00000020, 0x00000020);
    for (i = 0; i < gObjectParticle3_SIZE; i++) {
        objectIndex = EffectAllocArray3[i];
        if (objectIndex != NULL_OBJECT_ID) {
            object = &g_DynamicObjects[objectIndex];
            if ((object->anmptr > 0) && (object->chartptr == 3) && (modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
                KWCreateAffine3D(object->pos, object->texang, object->scale);
                gSPVertex(gDisplayListHead++, D_0D005C00, 3, 0);
                gSPDisplayList(gDisplayListHead++, D_0D006930);
            }
        }
    }
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
    gSPDisplayList(gDisplayListHead++, D_0D007AE0);
    kwloadtextureblock_ia_8b(D_8018D490, 0x00000020, 0x00000020);
    SmokeMode(0);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    for (i = 0; i < gObjectParticle2_SIZE; i++) {
        objectIndex = EffectAllocArray2[i];
        if (objectIndex != NULL_OBJECT_ID) {
            object = &g_DynamicObjects[objectIndex];
            if ((object->anmptr >= 2) && (object->chartptr == 2) && (modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
                KWSetPrimColor(0x000000FF, 0x000000FF, 0x000000FF, (s32) object->alpha);
                D_80183E80[1] = KWLookCamera(object->pos[0], object->pos[2], camera->camera_pos);
                kwset_tile(object->pos, D_80183E80, object->scale, D_0D005AE0);
            }
        }
    }
}

void kwdisplay_fuusen_sub(s32 objectIndex, UNUSED s32 arg1, s32 vertexIndex) {
    AnmObject* object;

    if (modelingnum <= MTX_HUD_POOL_SIZE_MAX) {
        object = &g_DynamicObjects[objectIndex];
        D_80183E80[2] = (s16) (object->stack[6] + 0x8000);
        KWCreateAffine3D(object->pos, (u16*) D_80183E80, object->scale);
        set_color_render((s32) object->stack[0], (s32) object->stack[1], (s32) object->stack[2],
                         (s32) object->stack[3], (s32) object->stack[4], (s32) object->stack[5],
                         (s32) object->alpha);
        gSPVertex(gDisplayListHead++, &common_vtx_hedgehog[vertexIndex], 4, 0);
        gSPDisplayList(gDisplayListHead++, common_rectangle_display);
    }
}

void kwdisplay_fuusen(s32 arg0) {
    s32 var_s1;
    s32 objectIndex;

    gSPDisplayList(gDisplayListHead++, D_0D007E98);
    gDPLoadTLUT_pal256(gDisplayListHead++, letter_pal);
    IceMode(0, 0, 0, 0, 0, 0, 0);
    D_80183E80[0] = 0;
    D_80183E80[1] = 0x8000;
    kwloadtextureblock_ci_8b(balloon1, 64, 32);
    for (var_s1 = 0; var_s1 < D_80165738; var_s1++) {
        objectIndex = EffectAllocArray3[var_s1];
        if ((objectIndex != NULL_OBJECT_ID) && (g_DynamicObjects[objectIndex].anmptr >= 2)) {
            kwdisplay_fuusen_sub(objectIndex, arg0, 0);
        }
    }
    kwloadtextureblock_ci_8b(balloon2, 64, 32);
    for (var_s1 = 0; var_s1 < D_80165738; var_s1++) {
        objectIndex = EffectAllocArray3[var_s1];
        if ((objectIndex != NULL_OBJECT_ID) && (g_DynamicObjects[objectIndex].anmptr >= 2)) {
            kwdisplay_fuusen_sub(objectIndex, arg0, 4);
        }
    }
}

void kwdisplay_train_smoke_sub(s32 objectIndex, s32 cameraId) {
    Camera* camera;

    camera = &camera1[cameraId];
    if (objectIndex != NULL_OBJECT_ID) {
        if ((g_DynamicObjects[objectIndex].anmptr >= 2) && (g_DynamicObjects[objectIndex].chartptr == 1) &&
            (modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
            set_color_render((s32) g_DynamicObjects[objectIndex].output, (s32) g_DynamicObjects[objectIndex].output,
                             (s32) g_DynamicObjects[objectIndex].output, 0, 0, 0, (s32) g_DynamicObjects[objectIndex].alpha);
            D_80183E80[1] =
                KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], camera->camera_pos);
            kwset_tile(g_DynamicObjects[objectIndex].pos, D_80183E80, g_DynamicObjects[objectIndex].scale, D_0D005AE0);
        }
    }
}

// Trains smoke particles.
void kwdisplay_train_smoke(s32 cameraId) {
    UNUSED s32 pad;
    UNUSED s32 j;
    Camera* camera;
    s32 i;

    camera = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, D_0D007AE0);
    kwloadtextureblock_a_8b(D_0D029458, 32, 32);
    Color2Mode(255, 255, 255, 255, 255, 255, 255);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;

// Render smoke for any number of trains. Don't know enough about these variables yet.
#ifdef AVOID_UB_WIP
    for (j = 0; j < NUM_TRAINS; j++) {
        if ((SL[j].someFlags != 0) &&
            (kwvischeck_camera_xz(&SL[j].locomotive.position, camera, 0x4000U) != 0)) {

            for (i = 0; i < 128; i++) {
                // Need to make a way to increase this array for each train.
                kwdisplay_train_smoke_sub(EffectAllocArray2[i], cameraId);
            }
        }
    }
#else

    if ((SL[0].someFlags != 0) &&
        (kwvischeck_camera_xz(SL[0].locomotive.position, camera, 0x4000U) != 0)) {

        for (i = 0; i < gObjectParticle2_SIZE; i++) {
            kwdisplay_train_smoke_sub(EffectAllocArray2[i], cameraId);
        }
    }
    if ((SL[1].someFlags != 0) &&
        (kwvischeck_camera_xz(SL[1].locomotive.position, camera, 0x4000U) != 0)) {
        for (i = 0; i < gObjectParticle3_SIZE; i++) {
            kwdisplay_train_smoke_sub(EffectAllocArray3[i], cameraId);
        }
    }
#endif
}

void kwdisplay_ship_smoke_sub(s32 objectIndex, s32 cameraId) {
    Camera* camera;

    camera = &camera1[cameraId];
    if (objectIndex != NULL_OBJECT_ID) {
        if ((g_DynamicObjects[objectIndex].anmptr >= 2) && (g_DynamicObjects[objectIndex].chartptr == 6) &&
            (modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
            set_color_render((s32) g_DynamicObjects[objectIndex].output, (s32) g_DynamicObjects[objectIndex].output,
                             (s32) g_DynamicObjects[objectIndex].output, g_DynamicObjects[objectIndex].input,
                             g_DynamicObjects[objectIndex].input, g_DynamicObjects[objectIndex].input,
                             (s32) g_DynamicObjects[objectIndex].alpha);
            D_80183E80[1] =
                KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], camera->camera_pos);
            kwset_tile(g_DynamicObjects[objectIndex].pos, D_80183E80, g_DynamicObjects[objectIndex].scale, D_0D005AE0);
        }
    }
}

// Likely smoke related.
void kwdisplay_ship_smoke(s32 cameraId) {
    UNUSED s32 pad[2];
    Camera* camera;
    s32 i;

    camera = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, D_0D007AE0);

    kwloadtextureblock_a_8b(D_0D029458, 32, 32);
    Color2Mode(255, 255, 255, 255, 255, 255, 255);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    if ((ship[0].someFlags != 0) && (kwvischeck_camera_xz(ship[0].position, camera, 0x4000U) != 0)) {
        for (i = 0; i < gObjectParticle2_SIZE; i++) {
            kwdisplay_ship_smoke_sub(EffectAllocArray2[i], cameraId);
        }
    }
    if ((ship[1].someFlags != 0) && (kwvischeck_camera_xz(ship[1].position, camera, 0x4000U) != 0)) {
        for (i = 0; i < gObjectParticle3_SIZE; i++) {
            kwdisplay_ship_smoke_sub(EffectAllocArray3[i], cameraId);
        }
    }
}

void kwdisplay_koopa_fire_sub(s32 objectIndex, s32 cameraId) {
    Camera* camera;
    AnmObject* object;

    camera = &camera1[cameraId];
    if (modelingnum <= MTX_HUD_POOL_SIZE_MAX) {
        object = &g_DynamicObjects[objectIndex];
        if (object->chartptr == 9) {
            Color2Mode(0xFF, (s32) object->output, 0, (s32) object->input, 0, 0, (s32) object->alpha);
        } else {
            KWSetPrimColor(0xFF, (s32) object->output, 0, (s32) object->alpha);
        }
        D_80183E80[1] = KWLookCamera(object->pos[0], object->pos[2], camera->camera_pos);
        kwset_tile(object->pos, D_80183E80, object->scale, D_0D005AE0);
    }
}

void kwdisplay_fire(s32 cameraId) {
    s32 var_s0;
    s32 objectIndex;

    gSPDisplayList(gDisplayListHead++, D_0D007AE0);
    kwloadtextureblock_a_8b(common_texture_particle_smoke[KW4GFTimer], 0x00000020, 0x00000020);
    SplashMode(0, 0, 0, 0x000000FF);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    for (var_s0 = 0; var_s0 < gObjectParticle1_SIZE; var_s0++) {
        objectIndex = EffectAllocArray1[var_s0];
        if ((objectIndex != NULL_OBJECT_ID) && (g_DynamicObjects[objectIndex].anmptr >= 3)) {
            kwdisplay_koopa_fire_sub(objectIndex, cameraId);
        }
    }
}

void KWDisplayFireParticleSub(s32 objectIndex, u8 arg1, Vec3f arg2) {
    if (modelingnum <= MTX_HUD_POOL_SIZE_MAX) {
        switch (arg1) { /* irregular */
            case 0:
                set_color_render(0xE6, 0xFF, 0xFF, 0x00, 0x00, 0xFF, (s32) g_DynamicObjects[objectIndex].alpha);
                break;
            case 1:
                set_color_render(0xFF, 0xFF, 0x96, 0xFF, 0x00, 0x00, (s32) g_DynamicObjects[objectIndex].alpha);
                break;
            case 2:
                set_color_render(0xFF, 0xE6, 0xFF, 0xFF, 0x00, 0x96, (s32) g_DynamicObjects[objectIndex].alpha);
                break;
            case 3:
                set_color_render(0xFF, 0xFF, 0x1E, 0xFF, 0x00, 0x00, (s32) g_DynamicObjects[objectIndex].alpha);
                break;
            default:
                break;
        }
        D_80183E80[1] = KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], arg2);
        kwset_tile(g_DynamicObjects[objectIndex].pos, D_80183E80, g_DynamicObjects[objectIndex].scale, D_0D005AE0);
    }
}

void kwdisplay_kame_fire(s32 cameraId) {
    UNUSED s32 stackPadding[2];
    Camera* sp54;
    s32 var_s0;
    s32 objectIndex;
    AnmObject* object;

    sp54 = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, D_0D007AE0);
    kwloadtextureblock_a_8b(common_texture_particle_smoke[KW4GFTimer], 32, 32);
    Color2Mode(255, 255, 255, 255, 255, 255, 255);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    for (var_s0 = 0; var_s0 < gObjectParticle4_SIZE; var_s0++) {
        objectIndex = FireParticleAllocArray[var_s0];
        if (objectIndex != NULL_OBJECT_ID) {
            object = &g_DynamicObjects[objectIndex];
            if (object->anmptr >= 2) {
                if (object->texsizex == 3) {
                    kwcheck_vis_area(objectIndex, cameraId, 0x4000U, 0x00000514);
                } else {
                    kwcheck_vis_area(objectIndex, cameraId, 0x4000U, 0x000001F4);
                }
                if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
                    KWDisplayFireParticleSub(objectIndex, object->texsizex, sp54->camera_pos);
                }
            }
        }
    }
}

void kwdisplay_star_sub(s32 objectIndex, Vec3f arg1) {
    D_80183E80[0] = KWLookCameraPitch(g_DynamicObjects[objectIndex].pos[2], g_DynamicObjects[objectIndex].pos[1], arg1);
    D_80183E80[1] = KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], arg1);
    D_80183E80[2] = (u16) g_DynamicObjects[objectIndex].texang[2];
    KWSetPrimColor((s32) g_DynamicObjects[objectIndex].stack[0], (s32) g_DynamicObjects[objectIndex].stack[1],
                  (s32) g_DynamicObjects[objectIndex].stack[2], (s32) g_DynamicObjects[objectIndex].alpha);
    KWCreateAffine3D(g_DynamicObjects[objectIndex].pos, (u16*) D_80183E80,
                                  g_DynamicObjects[objectIndex].scale);
    gSPVertex(gDisplayListHead++, D_0D005AE0, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_rectangle_display);
}

void KWDisplayStar(s32 cameraId) {
    s32 var_s0;
    s32 temp_a0;
    Camera* camera;

    camera = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, D_0D007AE0);
    kwloadtextureblock_ia_8b(hoshi, 0x00000020, 0x00000020);
    ToumeIMode(0x000000FF, 0x000000FF, 0, 0x000000FF);
    D_80183E80[0] = 0;
    for (var_s0 = 0; var_s0 < gObjectParticle3_SIZE; var_s0++) {
        temp_a0 = EffectAllocArray3[var_s0];
        if ((temp_a0 != -1) && (g_DynamicObjects[temp_a0].anmptr >= 2)) {
            kwdisplay_star_sub(temp_a0, camera->camera_pos);
        }
    }
}

void kwdisplay_choropu_sub(s32 objectIndex, s32 cameraId) {
    Camera* camera;

    camera = &camera1[cameraId];
    if (g_DynamicObjects[objectIndex].anmptr >= 3) {
        kwcheck_vis_area(objectIndex, cameraId, 0x2AABU, 0x0000012C);
        if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
            D_80183E80[0] = (s16) g_DynamicObjects[objectIndex].texang[0];
            D_80183E80[1] =
                KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], camera->camera_pos);
            D_80183E80[2] = (u16) g_DynamicObjects[objectIndex].texang[2];
            kwtexture3D_ci8_aa_zb_bl_mr(g_DynamicObjects[objectIndex].pos, (u16*) D_80183E80, g_DynamicObjects[objectIndex].scale,
                          (u8*) g_DynamicObjects[objectIndex].texaddr, g_DynamicObjects[objectIndex].indexaddr, D_0D0062B0,
                          0x00000020, 0x00000040, 0x00000020, 0x00000040, 5);
        }
    }
}

void kwdisplay_hole_sub(s32 objectIndex) {
    if (g_DynamicObjects[objectIndex].anmptr > 0) {
        if (kwanm_bitcheckflag(objectIndex, 0x00800000) != 0) {
            D_80183E50[0] = g_DynamicObjects[objectIndex].pos[0];
            D_80183E50[1] = g_DynamicObjects[objectIndex].ground + 0.8;
            D_80183E50[2] = g_DynamicObjects[objectIndex].pos[2];
            D_80183E70[0] = g_DynamicObjects[objectIndex].velocity[0];
            D_80183E70[1] = g_DynamicObjects[objectIndex].velocity[1];
            D_80183E70[2] = g_DynamicObjects[objectIndex].velocity[2];
            kwdraw_hole3D(g_DynamicObjects[objectIndex].scale);
        }
    }
}

// Almost certainly responsible for spawning/handling the moles on Moo Moo farm
void kwdisplay_hole(UNUSED s32 unused) {
    s32 someIndex;

    for (someIndex = 0; someIndex < NUM_TOTAL_MOLES; someIndex++) {
        kwdisplay_hole_sub(EffectAllocArray1[someIndex]);
    }
}

void kwdisplay_tuchi(s32 cameraId) {
    s32 var_s2;
    s32 objectIndex;
    Camera* sp44;
    AnmObject* object;

    sp44 = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, D_0D0079C8);
    KWLoadTextureBlockRGBA16B((u16 *) d_course_moo_moo_farm_mole_dirt, 0x00000010, 0x00000010);
    for (var_s2 = 0; var_s2 < gObjectParticle2_SIZE; var_s2++) {
        objectIndex = EffectAllocArray2[var_s2];
        object = &g_DynamicObjects[objectIndex];
        if (object->anmptr > 0) {
            kwcheck_vis_area(objectIndex, cameraId, 0x2AABU, 0x000000C8);
            if ((kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) && (modelingnum <= MTX_HUD_POOL_SIZE_MAX)) {
                object->texang[1] = KWLookCamera(object->pos[0], object->pos[2], sp44->camera_pos);
                KWCreateAffine3D_Anm(objectIndex);
                gSPDisplayList(gDisplayListHead++, D_0D006980);
            }
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void kwdisplay_choropu(s32 cameraId) {
    s32 i;

    for (i = 0; i < NUM_GROUP1_MOLES; i++) {
        kwdisplay_choropu_sub(objallocptr[i], cameraId);
    }
    for (i = 0; i < NUM_GROUP2_MOLES; i++) {
        kwdisplay_choropu_sub(objallocptr2[i], cameraId);
    }
    for (i = 0; i < NUM_GROUP3_MOLES; i++) {
        kwdisplay_choropu_sub(objallocptr3[i], cameraId);
    }
    kwdisplay_hole(cameraId);
    kwdisplay_tuchi(cameraId);
}

void kwdisplay_goalflag_sub(s32 objectIndex) {
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        gSPDisplayList(gDisplayListHead++, D_0D0077A0);
        KWCreateAffine3D(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].ang,
                                      g_DynamicObjects[objectIndex].scale);
        if (pause_flag == 0) {
            g_DynamicObjects[objectIndex].input = DrawLocalSkeletonShape((Armature*) g_DynamicObjects[objectIndex].shape,
                                                                     (Animation**) g_DynamicObjects[objectIndex].vtxaddr, 0,
                                                                     g_DynamicObjects[objectIndex].input);
        } else {
            DrawLocalSkeletonShape((Armature*) g_DynamicObjects[objectIndex].shape,
                                  (Animation**) g_DynamicObjects[objectIndex].vtxaddr, 0, g_DynamicObjects[objectIndex].input);
        }
    }
}

void kwdisplay_goalflag(s32 cameraId) {
    s32 var_s1;
    s32 temp_s0;

    for (var_s1 = 0; var_s1 < 4; var_s1++) {
        temp_s0 = objallocptr[var_s1];
        kwcheck_vis_area(temp_s0, cameraId, 0x4000U, 0x000005DC);
        if (kwanm_bitcheckflag(temp_s0, VISIBLE) != 0) {
            kwdisplay_goalflag_sub(temp_s0);
        }
    }
}

void kwdisplay_kamome_sub(s32 objectIndex) {
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        KWCreateAffine3D(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].ang,
                                      g_DynamicObjects[objectIndex].scale);
        gSPDisplayList(gDisplayListHead++, D_0D0077D0);
        if (pause_flag == 0) {
            g_DynamicObjects[objectIndex].input = DrawLocalSkeletonShape((Armature*) g_DynamicObjects[objectIndex].shape,
                                                                     (Animation**) g_DynamicObjects[objectIndex].vtxaddr, 0,
                                                                     g_DynamicObjects[objectIndex].input);
        } else {
            DrawLocalSkeletonShape((Armature*) g_DynamicObjects[objectIndex].shape,
                                  (Animation**) g_DynamicObjects[objectIndex].vtxaddr, 0, g_DynamicObjects[objectIndex].input);
        }
    }
}

void kwdisplay_kamome(s32 arg0) {
    s32 i;
    s32 var_s1;

    for (i = 0; i < NUM_SEAGULLS; i++) {
        var_s1 = objallocptr2[i];
        if (kwcheck_vis_area(var_s1, arg0, 0x5555U, 0x000005DC) < 0x9C401) {
            D_80165908 = 1;
            kwanm_setevflag(var_s1, 2);
        }
        if (kwanm_bitcheckflag(var_s1, VISIBLE) != 0) {
            kwdisplay_kamome_sub(var_s1);
        }
    }
}

void kwdisplay_kani_sub(s32 objectIndex, s32 cameraId) {
    Camera* camera;

    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        camera = &camera1[cameraId];
        kwdraw_kage3D_fast(objectIndex, 0.5f);
        g_DynamicObjects[objectIndex].texang[1] =
            KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], camera->camera_pos);
        kwtexture3D_ci8_aa_zb_bl(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].texang,
                           g_DynamicObjects[objectIndex].scale, (u8*) g_DynamicObjects[objectIndex].texaddr,
                           g_DynamicObjects[objectIndex].indexaddr, common_vtx_hedgehog, 0x00000040, 0x00000040,
                           0x00000040, 0x00000020);
    }
}

void kwdisplay_kani(s32 arg0) {
    s32 someIndex;
    s32 test;

    for (someIndex = 0; someIndex < NUM_CRABS; someIndex++) {
        test = objallocptr[someIndex];
        kwcheck_vis_area(test, arg0, 0x2AABU, 0x00000320);
        if (kwanm_bitcheckflag(test, VISIBLE) != 0) {
            kwdisplay_kani_sub(test, arg0);
        }
    }
}

void kwdisplay_harinezumi_sub(s32 objectIndex, s32 cameraId) {
    Camera* camera;

    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        camera = &camera1[cameraId];
        kwdraw_kage_3D_bump(objectIndex, 0.7f);
        g_DynamicObjects[objectIndex].texang[1] =
            KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], camera->camera_pos);
        kwtexture3D_ci8_aa_zb_bl(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].texang,
                           g_DynamicObjects[objectIndex].scale, (u8*) g_DynamicObjects[objectIndex].texaddr,
                           g_DynamicObjects[objectIndex].indexaddr, g_DynamicObjects[objectIndex].vtxaddr, 64, 64, 64, 32);
    }
}

void kwdisplay_harinezumi(s32 arg0) {
    s32 test;
    u32 something;
    s32 someIndex;

    for (someIndex = 0; someIndex < NUM_HEDGEHOGS; someIndex++) {
        test = objallocptr2[someIndex];
        something = kwcheck_vis_area(test, arg0, 0x4000U, 0x000003E8);
        if (kwanm_bitcheckflag(test, VISIBLE) != 0) {
            kwanm_setflag(test, 0x00200000);
            if (something < 0x2711U) {
                kwanm_setflag(test, 0x00000020);
            } else {
                kwanm_clrflag(test, 0x00000020);
            }
            if (something < 0x57E41U) {
                kwanm_setflag(test, 0x00400000);
            }
            if (something < 0x52211U) {
                kwdisplay_harinezumi_sub(test, arg0);
            }
        }
    }
}

UNUSED void func_800557AC() {
}

void kwdisplay_ping_sub(s32 objectIndex, u32 arg1, u32 arg2) {
    Vec3f sp34;
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    if (object->anmptr >= 2) {
        if (kwanm_bitcheckflag(objectIndex, 0x00000020) != 0) {
            if (kwanm_bitcheckevflag(objectIndex, 4) != 0) {
                if (arg2 >= arg1) {
                    sp34[0] = object->pos[0];
                    sp34[1] = object->pos[1] - 1.0;
                    sp34[2] = object->pos[2];
                    KWCreateAffine3D_IceKage(sp34, object->texang,
                                                                           object->scale);
                    gSPDisplayList(gDisplayListHead++, D_0D0077D0);
                    DrawLocalSkeletonShape((Armature*) object->shape, (Animation**) object->vtxaddr,
                                          (s16) object->texsizex, (s16) object->anmnumbak);
                }
            } else if (arg1 < 0x15F91U) {
                kwdraw_kage3D_flat(objectIndex, 1.5f);
            }
        }
        KWCreateAffine3D(object->pos, object->texang, object->scale);
        gSPDisplayList(gDisplayListHead++, D_0D0077D0);
        DrawLocalSkeletonShape((Armature*) object->shape, (Animation**) object->vtxaddr, (s16) object->texsizex,
                              (s16) object->anmnumbak);
    }
}

void kwdisplay_ping(s32 cameraId) {
    s32 i;
    s32 objectIndex;
    s32 temp_s1;
    s32 var_a3;
    u16 var_s1;
    u32 var_s3;

    if (g_playerCount == 1) {
        var_s3 = 0x0003D090;
    } else if (g_playerCount == 2) {
        var_s3 = 0x00027100;
    } else {
        var_s3 = 0x00015F90;
    }
    for (i = 0; i < NUM_PENGUINS; i++) {
        objectIndex = objallocptr[i];
        if (g_DynamicObjects[objectIndex].anmptr >= 2) {
            if (g_playerCount == 1) {
                var_s1 = 0x4000;
                if (i == 0) {
                    var_a3 = 0x000005DC;
                } else if (kwanm_bitcheckevflag(objectIndex, 8) != 0) {
                    var_a3 = 0x00000320;
                } else {
                    var_a3 = 0x000003E8;
                }
            } else {
                if (kwanm_bitcheckevflag(objectIndex, 8) != 0) {
                    var_a3 = 0x000001F4;
                    var_s1 = 0x4000;
                } else {
                    var_a3 = 0x00000258;
                    var_s1 = 0x5555;
                }
            }
            temp_s1 = kwcheck_vis_area(objectIndex, cameraId, var_s1, var_a3);
            if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
                kwdisplay_ping_sub(objectIndex, (u32) temp_s1, var_s3);
            }
        }
    }
}

void kwdisplay_wanwan_sub(s32 objectIndex, s32 cameraId) {
    Camera* camera;

    camera = &camera1[cameraId];
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        if (kwanm_bitcheckflag(objectIndex, 0x00100000) != 0) {
            D_80183E40[0] = g_DynamicObjects[objectIndex].pos[0];
            D_80183E40[1] = g_DynamicObjects[objectIndex].pos[1] + 16.0;
            D_80183E40[2] = g_DynamicObjects[objectIndex].pos[2];
            D_80183E80[0] = 0;
            D_80183E80[1] =
                KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], camera->camera_pos);
            D_80183E80[2] = 0x8000;
            kwtexture3D_rgba_aa_zb_bl_mr(D_80183E40, D_80183E80, 0.54f, d_course_rainbow_road_sphere, D_0D0062B0, 0x00000020,
                          0x00000040, 0x00000020, 0x00000040, 5);
        } else {
            KWCreateAffine3D(g_DynamicObjects[objectIndex].pos, g_DynamicObjects[objectIndex].ang,
                                          g_DynamicObjects[objectIndex].scale);
            gSPDisplayList(gDisplayListHead++, D_0D0077D0);
            DrawLocalSkeletonShape((Armature*) g_DynamicObjects[objectIndex].shape,
                                  (Animation**) g_DynamicObjects[objectIndex].vtxaddr, 0,
                                  (s16) g_DynamicObjects[objectIndex].anmnumbak);
        }
    }
}

void kwdisplay_wanwan(s32 cameraId) {
    s32 var_s1;
    s32 objectIndex;

    for (var_s1 = 0; var_s1 < NUM_CHAIN_CHOMPS; var_s1++) {
        objectIndex = objallocptr2[var_s1];
        kwcheck_lod_texture(objectIndex, cameraId, 0x000005DC, 0x000009C4);
        if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
            kwdisplay_wanwan_sub(objectIndex, cameraId);
        }
    }
}

void kwdisplay_balloon_sub(s32 objectIndex, s32 cameraId) {
    UNUSED s32 pad;
    f32 test;
    Camera* camera;

    camera = &camera1[cameraId];
    if (g_DynamicObjects[objectIndex].anmptr >= 2) {
        kwcheck_display_kage(objectIndex, cameraId, 0x0000012C);
        test = g_DynamicObjects[objectIndex].pos[1] - g_DynamicObjects[objectIndex].ground;
        kwdraw_kage3D_fast(objectIndex, (20.0 / test) + 0.5);
        if (kwanm_bitoffcheckflag(objectIndex, 0x00100000) != 0) {
            kwpolygon3D_slight(g_DynamicObjects[objectIndex].pos, (u16*) g_DynamicObjects[objectIndex].ang,
                          g_DynamicObjects[objectIndex].scale, d_course_luigi_raceway_dl_F960);
            gSPDisplayList(gDisplayListHead++, d_course_luigi_raceway_dl_F650);
        } else {
            D_80183E80[0] = (s16) g_DynamicObjects[objectIndex].ang[0];
            D_80183E80[1] =
                (s16) (KWLookCamera(g_DynamicObjects[objectIndex].pos[0], g_DynamicObjects[objectIndex].pos[2], camera->camera_pos) +
                       0x8000);
            D_80183E80[2] = (u16) g_DynamicObjects[objectIndex].ang[2];
            kwpolygon3D_slight(g_DynamicObjects[objectIndex].pos, D_80183E80, g_DynamicObjects[objectIndex].scale,
                          d_course_luigi_raceway_dl_FBE0);
            gSPDisplayList(gDisplayListHead++, d_course_luigi_raceway_dl_FA20);
            if (g_playerCount == 1) {
                g_DynamicObjects[objectIndex].ang[1] = 0;
            }
        }
    }
}

void kwdisplay_balloon(s32 arg0) {
    s32 objectIndex;
    objectIndex = objallocptr[0];
    if (gGamestate != 9) {
        kwcheck_lod_texture(objectIndex, arg0, 0x000005DC, 0x00000BB8);
        if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
            kwdisplay_balloon_sub(objectIndex, arg0);
        }
    } else {
        kwanm_clrflag(objectIndex, 0x00100000);
        kwdisplay_balloon_sub(objectIndex, arg0);
    }
}

void kwdisplay_dai_sub(s32 objectIndex, UNUSED s32 arg1) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    if (object->anmptr >= 2) {
        kwpolygon3D(object->pos, object->ang, object->scale, object->shape);
    }
}

void kwdisplay_dai(s32 arg0) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 3; someIndex++) {
        kwdisplay_dai_sub(objallocptr[someIndex], arg0);
    }
}

void kwdisplay_cup_sub(s32 objectIndex, UNUSED s32 arg1) {
    Mat4 someMatrix1;
    Mat4 someMatrix2;
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    if (object->anmptr >= 2) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        kwcreate_modeling_matrix(someMatrix1, object->pos, object->ang, object->scale);
        Affine2Mtx(&gDynamicP->mtxHud[modelingnum], someMatrix1);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxHud[modelingnum++]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, D_0D0077A0);
        gSPDisplayList(gDisplayListHead++, object->shape);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(someMatrix2);
        SetMatrix(someMatrix2, 0);
    }
}

void kwdisplay_cup(s32 arg0) {
    kwdisplay_cup_sub(objallocptr[3], arg0);
}

void kwdisplay_neon(s32 cameraId) {
    Camera* camera;
    s32 var_s2;
    s32 objectIndex;
    AnmObject* object;

    camera = &camera1[cameraId];
    for (var_s2 = 0; var_s2 < 10; var_s2++) {
        objectIndex = objallocptr[var_s2];
        if (thunder_flg[cameraId] == 0) {
            object = &g_DynamicObjects[objectIndex];
            if ((object->anmptr >= 2) && (kwanm_bitoffcheckflag(objectIndex, 0x00080000) != 0) &&
                (kwanm_vischeck_camera_xz(objectIndex, camera, 0x2AABU) != 0)) {
                object->texang[1] = kwlookcamera_xz(objectIndex, camera);
                kwtexture3D_ci8_aa_zb_bl(object->pos, object->texang, object->scale, (u8*) object->texaddr,
                                   object->indexaddr, common_vtx_hedgehog, 0x00000040, 0x00000040, 0x00000040,
                                   0x00000020);
            }
        }
    }
}

void kwdisplay_bombspark_bt(s32 arg0, s32 arg1, s32 arg2) {
    D_80165860 = sparkcoltbl[arg0][0];
    D_8016586C = sparkcoltbl[arg0][1];
    D_80165878 = sparkcoltbl[arg0][2];
    KWSetPrimColor(D_80165860, D_8016586C, D_80165878, arg2);
    KWCreateAffine3D(D_80183E40, D_80183E80, 0.2f);
    kwloadtextureblock_i_8b(common_texture_particle_spark[arg1], 0x00000020, 0x00000020);
    gSPVertex(gDisplayListHead++, D_0D005AE0, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_rectangle_display);
}

void kwdisplay_bombbody_bt(s32 objectIndex, s32 cameraId, s32 arg2) {
    s32 temp_s0;
    s32 temp_v0;
    s32 residue;
    Camera* camera;
    AnmObject* object;

    camera = &camera1[cameraId];
    object = &g_DynamicObjects[objectIndex];
    residue = KW2GFCount % 4U;
    D_80183E40[0] = object->pos[0];
    D_80183E40[1] = object->pos[1] + 1.0;
    D_80183E40[2] = object->pos[2];
    D_80183E80[0] = 0;
    D_80183E80[1] = KWLookCamera(object->pos[0], object->pos[2], camera->camera_pos);
    D_80183E80[2] = 0x8000;
    KWCreateAffine3D(D_80183E40, D_80183E80, 0.2f);
    gSPDisplayList(gDisplayListHead++, D_0D007E98);
    ToumeIAMode(arg2);
    KWTextureCI8_SubBL((u8*) common_tlut_bomb, common_texture_bomb[residue], D_0D005AE0, 0x00000020,
                                   0x00000020, 0x00000020, 0x00000020);
    temp_s0 = kwgameframetimer;
    gSPDisplayList(gDisplayListHead++, D_0D007B00);
    SplashMode(0, 0, 0, arg2);
    D_80183E40[1] = D_80183E40[1] + 4.0;
    D_80183E80[2] = 0;
    kwdisplay_bombspark_bt(temp_s0 % 3, temp_s0 % 4, arg2);
    temp_v0 = temp_s0 + 1;
    D_80183E80[2] = 0x6000;
    kwdisplay_bombspark_bt(temp_v0 % 3, temp_v0 % 4, arg2);
    temp_v0 = temp_s0 + 2;
    D_80183E80[2] = 0xA000;
    kwdisplay_bombspark_bt(temp_v0 % 3, temp_v0 % 4, arg2);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void kwdisplay_bombtire_bt(s32 objectIndex, UNUSED s32 arg1, s32 arg2) {
    gSPDisplayList(gDisplayListHead++, D_0D0079E8);
    ToumeIAMode(arg2);
    KWLoadTextureBlockRGBA16B(D_0D02AA58, 0x00000010, 0x00000010);
    D_80183E40[1] = g_DynamicObjects[objectIndex].pos[1] - 2.0;
    D_80183E40[0] = g_DynamicObjects[objectIndex].pos[0] + 2.0;
    D_80183E40[2] = g_DynamicObjects[objectIndex].pos[2] + 2.0;
    kwset_tile(D_80183E40, D_80183E80, 0.15f, common_vtx_rectangle);
    D_80183E40[0] = g_DynamicObjects[objectIndex].pos[0] + 2.0;
    D_80183E40[2] = g_DynamicObjects[objectIndex].pos[2] - 2.0;
    kwset_tile(D_80183E40, D_80183E80, 0.15f, common_vtx_rectangle);
    D_80183E40[0] = g_DynamicObjects[objectIndex].pos[0] - 2.0;
    D_80183E40[2] = g_DynamicObjects[objectIndex].pos[2] - 2.0;
    kwset_tile(D_80183E40, D_80183E80, 0.15f, common_vtx_rectangle);
    D_80183E40[0] = g_DynamicObjects[objectIndex].pos[0] - 2.0;
    D_80183E40[2] = g_DynamicObjects[objectIndex].pos[2] + 2.0;
    kwset_tile(D_80183E40, D_80183E80, 0.15f, common_vtx_rectangle);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void kwdisplay_bombshadow_bt(s32 objectIndex, s32 playerId) {
    Mat4 sp30;
    Player* player;

    player = &gPlayerOne[playerId];
    D_80183E50[0] = g_DynamicObjects[objectIndex].pos[0];
    D_80183E50[1] = g_DynamicObjects[objectIndex].ground + 0.8;
    D_80183E50[2] = g_DynamicObjects[objectIndex].pos[2];
    kwcreate_rotation_matrix(sp30, player->bump.bump_zx, D_80183E50, 0U, 0.5f);
    Affine2Mtx(&gDynamicP->mtxHud[modelingnum], sp30);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxHud[modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, D_0D007B98);
}

void kwstart_bombkart(s32 playerIndex) {
    s32 objectIndex;

    objectIndex = bomballocptr[playerIndex];
    KWAnmStart(objectIndex, 0);
    g_DynamicObjects[objectIndex].alpha = 0;
}

void kwset_bombkart_alpha(s32 playerIndex, s32 primAlpha) {
    s32 objectIndex;

    objectIndex = bomballocptr[playerIndex];
    KWAnmStart(objectIndex, 0);
    g_DynamicObjects[objectIndex].alpha = (s16) primAlpha;
}

void kwfinish_bombkart(s32 playerIndex) {
    kwanm_finish(bomballocptr[playerIndex]);
}

void KWDisplayBombKartBT(s32 cameraId) {
    Player* temp_v0;
    s32 temp_s1;
    s32 temp_s0;
    s32 payerId;
    AnmObject* object;

    for (payerId = 0; payerId < NUM_BOMB_KARTS_BATTLE; payerId++) {
        temp_s0 = bomballocptr[payerId];
        object = &g_DynamicObjects[temp_s0];
        if (object->anmptr != 0) {
            temp_s1 = object->alpha;
            temp_v0 = &gPlayerOne[payerId];
            object->pos[0] = temp_v0->position[0];
            object->pos[1] = temp_v0->position[1] - 2.0;
            object->pos[2] = temp_v0->position[2];
            object->ground = temp_v0->ground;
            kwdisplay_bombbody_bt(temp_s0, cameraId, temp_s1);
            kwdisplay_bombtire_bt(temp_s0, cameraId, temp_s1);
            kwdisplay_bombshadow_bt(temp_s0, cameraId);
        }
    }
}

void kwdisplay_bombkart_sub(s32 bombIndex) {
    UNUSED s32 stackPadding;
    u8 thing;
    s32 temp_s0;
    s32 temp_v0;
    u8* bombFrame;
    BombKart sp40 = bom[bombIndex];

    temp_v0 = KW2GFCount % 6U;
    thing = bombcoltbl[temp_v0];
    bombFrame = common_texture_bomb[thing];
    D_80183E40[0] = sp40.bombPos[0];
    D_80183E40[1] = sp40.bombPos[1] + 1.0;
    D_80183E40[2] = sp40.bombPos[2];
    kwtexture3D_ci8_aa_zb_bl(D_80183E40, D_80183E80, 0.25f, (u8*) common_tlut_bomb, bombFrame, D_0D005AE0, 0x20, 0x20, 0x20,
                       0x20);
    temp_s0 = kwgameframetimer;
    gSPDisplayList(gDisplayListHead++, D_0D007B00);
    SplashMode(0, 0, 0, 0xFF);
    D_80183E40[1] = sp40.bombPos[1] + 5.0;
    D_80183E80[2] = 0;
    kwdisplay_bombspark_bt((s32) temp_s0 % 3, temp_s0 % 4, 0xFFU);
    temp_v0 = temp_s0 + 1;
    D_80183E80[2] = 0x6000;
    kwdisplay_bombspark_bt(temp_v0 % 3, temp_v0 % 4, 0xFFU);
    temp_v0 = temp_s0 + 2;
    D_80183E80[2] = 0xA000;
    kwdisplay_bombspark_bt(temp_v0 % 3, temp_v0 % 4, 0xFFU);
}

void kwdisplay_bombtire(s32 bombIndex, Vec3f arg1) {
    UNUSED s32 stackPadding[2];
    BombKart sp2C = bom[bombIndex];

    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    gSPDisplayList(gDisplayListHead++, D_0D0079C8);
    KWLoadTextureBlockRGBA16B(D_0D02AA58, 0x00000010, 0x00000010);
    D_80183E80[1] = KWLookCamera(sp2C.wheel1Pos[0], sp2C.wheel1Pos[2], arg1);
    kwset_tile(sp2C.wheel1Pos, D_80183E80, 0.15f, common_vtx_rectangle);
    D_80183E80[1] = KWLookCamera(sp2C.wheel2Pos[0], sp2C.wheel2Pos[2], arg1);
    kwset_tile(sp2C.wheel2Pos, D_80183E80, 0.15f, common_vtx_rectangle);
    D_80183E80[1] = KWLookCamera(sp2C.wheel3Pos[0], sp2C.wheel3Pos[2], arg1);
    kwset_tile(sp2C.wheel3Pos, D_80183E80, 0.15f, common_vtx_rectangle);
    D_80183E80[1] = KWLookCamera(sp2C.wheel4Pos[0], sp2C.wheel4Pos[2], arg1);
    kwset_tile(sp2C.wheel4Pos, D_80183E80, 0.15f, common_vtx_rectangle);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void kwdisplay_bombshadow(s32 bombIndex) {
    Mat4 mat;
    BombKart* temp_v0;

    temp_v0 = &bom[bombIndex];
    D_80183E50[0] = temp_v0->bombPos[0];
    D_80183E50[1] = temp_v0->yPos + 1.0;
    D_80183E50[2] = temp_v0->bombPos[2];
    kwcreate_rotation_matrix(mat, bom_bump[bombIndex].bump_zx, D_80183E50, 0U, 0.5f);
    Affine2Mtx(&gDynamicP->mtxHud[modelingnum], mat);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxHud[modelingnum++]),
              G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, D_0D007B98);
}

void kwdisplay_bombkart(s32 cameraId) {
    Camera* camera;
    s32 objectIndex;
    s32 temp_s4;
    s32 i;
    s32 state;
    BombKart* var_s1_2;

    if (gGamestate == 5) {
        cameraId = 0;
    }
    camera = &camera1[cameraId];
    if (cameraId == PLAYER_ONE) {
        for (i = 0; i < NUM_BOMB_KARTS_VERSUS; i++) {
            objectIndex = bomballocptr[i];
            if (kwanm_bitcheckflag(objectIndex, 0x00200000) != 0) {
                bom[i].unk_4A = 0;
            } else if (gGamestate != 5) {
                bom[i].unk_4A = 1;
            }
            kwanm_clrflag(objectIndex, 0x00200000);
        }
    }

    for (i = 0; i < NUM_BOMB_KARTS_VERSUS; i++) {
        var_s1_2 = &bom[i];
        // huh???
        state = var_s1_2->state;
        if (var_s1_2->state != BOMB_STATE_INACTIVE) {
            objectIndex = bomballocptr[i];
            g_DynamicObjects[objectIndex].pos[0] = var_s1_2->bombPos[0];
            g_DynamicObjects[objectIndex].pos[1] = var_s1_2->bombPos[1];
            g_DynamicObjects[objectIndex].pos[2] = var_s1_2->bombPos[2];
            temp_s4 = kwcheck_vis_area(objectIndex, cameraId, 0x31C4U, 0x000001F4);
            if (kwanm_bitcheckflag(objectIndex, VISIBLE) != 0) {
                kwanm_setflag(objectIndex, 0x00200000);
                D_80183E80[0] = 0;
                D_80183E80[1] = KWLookCamera(var_s1_2->bombPos[0], var_s1_2->bombPos[2], camera->camera_pos);
                D_80183E80[2] = 0x8000;
                kwdisplay_bombbody_bt(objectIndex, cameraId, 0x000000FF);
                kwdisplay_bombtire(i, camera->camera_pos);
                if (((u32) temp_s4 < 0x4E21U) && (state != BOMB_STATE_EXPLODED)) {
                    kwdisplay_bombshadow(i);
                }
            }
        }
    }
}

UNUSED void func_80057330(void) {
}

UNUSED void kwdisplay_typhoon(void) {

    gSPDisplayList(gDisplayListHead++, D_0D0079C8);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    gSPDisplayList(gDisplayListHead++, D_0D007AE0);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
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

void kwprint8x8(s32 x, s32 y, s8 str) {
    KWRectangle(x, y, 8, 8, (((str % 16) * 8) << 16) >> 16, (((unsigned short) (str / 16)) << 19) >> 16,
                             0);
}

void kaigyou_check(s32* x, s32* y) {
    *x += 8;
    if (*x >= 296) {
        *x = 20;
        *y += 8;
    }
}

void kwprint_sub(s32* x, s32* y, char* arg2) {
    *x += 20;
    *y += 20;

    while (*arg2 != '\0') {
        if (D_800E5628[(s32) *arg2] >= 0) {
            kwprint8x8(*x, *y, D_800E5628[(s32) *arg2]);
        }
        kaigyou_check(x, y);
        arg2++;
    }
}

void printNumber(s32* x, s32* y, s32 number, u32 numDigits) {
    s32 n;
    s8* ptr;
    s8 remainder;

    kaigyou_check(x, y);
    n = number;
    if (n < 0) {
        kwprint8x8(*x, *y, D_800E5628[0x2D]);
        kaigyou_check(x, y);
        n = -number;
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
        kwprint8x8(*x, *y, *ptr--);
        kaigyou_check(x, y);
    } while (*ptr != -1);
}

/**
 * 801657B8[] does nothing? 0xFF a mask?
 * Index zero is a null/0xFF flag.
 * The other indexes increment 0-9
 * The final index (10) increments the tenth digit.
 */
void kwprintnumber_unsign_sub(s32* x, s32* y, s32 pathCount, u32 numDigits) {
    s8* ptr;
    s32 count;
    s8 remainder;

    kaigyou_check(x, y);
    *D_801657B8 = -1;
    ptr = D_801657B8;
    count = pathCount;
    if (count != 0) {
        while (count != 0) {
            // Retrives ones digit (31 outputs 1).
            remainder = count % numDigits;
            *++ptr = remainder;
            // Retrieves tens digit (31 outputs 3).
            count = count / numDigits;
        }
    } else {
        *++ptr = 0;
    }

    do {
        kwprint8x8(*x, *y, *ptr--);
        kaigyou_check(x, y);
    } while (*ptr != -1);
}

UNUSED void func_80057708() {
}

void loadFont(void) {
    gSPDisplayList(gDisplayListHead++, D_0D008108);
    gSPDisplayList(gDisplayListHead++, D_0D008080);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_THRESHOLD);
}

void kwprintmode_finish(void) {
    gSPDisplayList(gDisplayListHead++, D_0D007EB8);
}

void printString(s32 xPos, s32 yPos, char* str) {
    kwprint_sub(&xPos, &yPos, str);
}

void printStringNumber(s32 arg0, s32 arg1, char* arg2, s32 arg3) {
    kwprint_sub(&arg0, &arg1, arg2);
    printNumber(&arg0, &arg1, arg3, 10);
}

UNUSED void printStringUnsignedNumber(s32 arg0, s32 arg1, char* arg2, u32 arg3) {
    kwprint_sub(&arg0, &arg1, arg2);
    kwprintnumber_unsign_sub(&arg0, &arg1, arg3, 10);
}

UNUSED void printStringHex(s32 arg0, s32 arg1, char* arg2, u32 arg3) {
    kwprint_sub(&arg0, &arg1, arg2);
    printNumber(&arg0, &arg1, arg3, 16);
    kwprint8x8(arg0, arg1, D_800E5628[0x48]);
}

UNUSED void printStringUnsignedHex(s32 arg0, s32 arg1, char* arg2, u32 arg3) {
    kwprint_sub(&arg0, &arg1, arg2);
    kwprintnumber_unsign_sub(&arg0, &arg1, arg3, 16);
    kwprint8x8(arg0, arg1, D_800E5628[0x48]);
}

UNUSED void printStringBinary(s32 arg0, s32 arg1, char* arg2, u32 arg3) {
    kwprint_sub(&arg0, &arg1, arg2);
    printNumber(&arg0, &arg1, arg3, 2);
    kwprint8x8(arg0, arg1, D_800E5628[0x42]);
}

UNUSED void printStringUnsignedBinary(s32 arg0, s32 arg1, char* arg2, u32 arg3) {
    kwprint_sub(&arg0, &arg1, arg2);
    kwprintnumber_unsign_sub(&arg0, &arg1, arg3, 2);
    kwprint8x8(arg0, arg1, D_800E5628[0x42]);
}

UNUSED void kwfprint(s32 arg0, s32 arg1, char* arg2) {
    loadFont();
    kwprint_sub(&arg0, &arg1, arg2);
    kwprintmode_finish();
}

void kwfprintd(s32 arg0, s32 arg1, char* arg2, u32 arg3) {
    loadFont();
    kwprint_sub(&arg0, &arg1, arg2);
    printNumber(&arg0, &arg1, arg3, 10);
    kwprintmode_finish();
}

void kwfprintud(s32 x, s32 y, char* str, u32 arg3) {
    loadFont();
    kwprint_sub(&x, &y, str);
    kwprintnumber_unsign_sub(&x, &y, arg3, 10);
    kwprintmode_finish();
}

UNUSED void kwfprinth(s32 arg0, s32 arg1, char* arg2, u32 arg3) {
    loadFont();
    kwprint_sub(&arg0, &arg1, arg2);
    printNumber(&arg0, &arg1, arg3, 16);
    kwprint8x8(arg0, arg1, D_800E5628[0x48]);
    kwprintmode_finish();
}

UNUSED void kwfprintuh(s32 arg0, s32 arg1, char* arg2, u32 arg3) {
    loadFont();
    kwprint_sub(&arg0, &arg1, arg2);
    kwprintnumber_unsign_sub(&arg0, &arg1, arg3, 16);
    kwprint8x8(arg0, arg1, D_800E5628[0x48]);
    kwprintmode_finish();
}

UNUSED void kwfprintb(s32 arg0, s32 arg1, char* arg2, u32 arg3) {
    loadFont();
    kwprint_sub(&arg0, &arg1, arg2);
    printNumber(&arg0, &arg1, arg3, 2);
    kwprint8x8(arg0, arg1, D_800E5628[0x42]);
    kwprintmode_finish();
}

UNUSED void kwfprintub(s32 arg0, s32 arg1, char* arg2, u32 arg3) {
    loadFont();
    kwprint_sub(&arg0, &arg1, arg2);
    kwprintnumber_unsign_sub(&arg0, &arg1, arg3, 2);
    kwprint8x8(arg0, arg1, D_800E5628[0x42]);
    kwprintmode_finish();
}

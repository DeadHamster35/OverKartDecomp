#include <ultra64.h>
#include <macros.h>
#include <PR/gbi.h>
#include <common_structs.h>
#include <mk64.h>
#include <actor_types.h>
#include <course.h>

#include "main.h"
#include "memory.h"
#include "collision.h"
#include "math_util.h"
#include "code_800029B0.h"
#include <defines.h>

#pragma intrinsic(sqrtf)

// Used to delete the choco mountain guard rail
void KillDisplayList(uintptr_t addr) {
    s32 segment = SEGMENT_NUMBER2(addr);
    s32 offset = SEGMENT_OFFSET(addr);

    Gfx* macro;

    macro = (Gfx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    macro->words.w0 = (G_ENDDL << 24);
    macro->words.w1 = 0;
}

void InitialBump(Collision* collision) {
    collision->last_xy = 5000;
    collision->last_yz = 5000;
    collision->last_zx = 5000;
    collision->flag_xy = 0;
    collision->flag_yz = 0;
    collision->flag_zx = 0;
    collision->distance_xy = 0;
    collision->distance_yz = 0;
    collision->distance_zx = 0;
    vec3f_set(collision->bump_xy, 0.0f, 0.0f, 1.0f);
    vec3f_set(collision->bump_yz, 1.0f, 0.0f, 0.0f);
    vec3f_set(collision->bump_zx, 0.0f, 1.0f, 0.0f);
}

f32 CheckWaterLevel(Player* player) {
    f32 playerX;
    f32 playerZ;
    s32 temp_v1;

    playerX = player->position[0];
    playerZ = player->position[2];
    switch (g_courseID) {
        case COURSE_BOWSER_CASTLE:
            if (playerX > 1859.0f) {
                return g_waterHeight;
            }
            if (playerX < 1549.0f) {
                return g_waterHeight;
            }
            if (playerZ > -1102.0f) {
                return g_waterHeight;
            }
            if (playerZ < -1402.0f) {
                return g_waterHeight;
            }
            return 20.0f;
        case COURSE_KOOPA_BEACH:
            if (playerX > 239.0f) {
                return g_waterHeight;
            }
            if (playerX < 67.0f) {
                return g_waterHeight;
            }
            if (playerZ > 2405.0f) {
                return g_waterHeight;
            }
            if (playerZ < 2233.0f) {
                return g_waterHeight;
            }
            return 0.8f;
        case COURSE_SHERBET_LAND:
            if ((check_status(player->bump.last_zx) & 0xFF) == SNOW) {
                return (f32) (minimum_y - 0xA);
            }
            return g_waterHeight;
        case COURSE_DK_JUNGLE:
            temp_v1 = CheckArea(player->bump.last_zx) & 0xFF;
            if (temp_v1 == 0xFF) {
                if ((check_status(player->bump.last_zx) & 0xFF) == CAVE) {
                    return -475.0f;
                }
                if (playerX > -478.0f) {
                    return -33.9f;
                }
                if (playerX < -838.0f) {
                    return -475.0f;
                }
                if (playerZ > -436.0f) {
                    return -475.0f;
                }
                if (playerZ < -993.0f) {
                    return -33.9f;
                }
                if (playerZ < playerX) {
                    return -475.0f;
                }
                return -33.9f;
            }
            if (temp_v1 >= 0x14) {
                return -475.0f;
            }
            return -33.9f;
        default:
            return g_waterHeight;
    }
}

s32 CheckTriangleZX(Collision* collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ, u16 index) {
    FaceStruct* triangle = &gFaceBuffer[index];
    UNUSED f32 pad;
    f32 x3;
    UNUSED f32 pad2;
    f32 z3;
    UNUSED f32 pad3[4];
    f32 x2;
    f32 z2;
    f32 x1;
    f32 z1;
    f32 distanceToSurface;
    f32 crossProductZX_1;
    f32 crossProductZX_2;
    f32 crossProductZX_3;
    s32 b = true;

    if (triangle->solb < -0.9f) {
        return 0;
    }
    if (triangle->minx > posX) {
        return 0;
    }
    if (triangle->minz > posZ) {
        return 0;
    }
    if (triangle->maxx < posX) {
        return 0;
    }
    if (triangle->maxz < posZ) {
        return 0;
    }
    if ((triangle->miny - boundingBoxSize * 3.0f) > posY) {
        return 0;
    }

    x1 = triangle->p1->v.ob[0];
    z1 = triangle->p1->v.ob[2];

    x2 = triangle->p2->v.ob[0];
    z2 = triangle->p2->v.ob[2];

    x3 = triangle->p3->v.ob[0];
    z3 = triangle->p3->v.ob[2];

    crossProductZX_1 = (z1 - posZ) * (x2 - posX) - (x1 - posX) * (z2 - posZ);

    if (!crossProductZX_1) {

        crossProductZX_2 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);

        crossProductZX_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);

        if ((crossProductZX_2 * crossProductZX_3) < 0.0f) {
            b = false;
        }
    } else {

        crossProductZX_2 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);

        if (!crossProductZX_2) {
            crossProductZX_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);

            if ((crossProductZX_1 * crossProductZX_3) < 0.0f) {
                b = false;
            }
        } else {
            if ((crossProductZX_1 * crossProductZX_2) < 0.0f) {
                b = false;
            } else {
                crossProductZX_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);
                if (crossProductZX_3 != 0) {
                    if ((crossProductZX_2 * crossProductZX_3) < 0.0f) {
                        b = false;
                    }
                }
            }
        }
    }
    if (!b) {
        return 0;
    }
    distanceToSurface =
        ((triangle->sola * posX) + (triangle->solb * posY) + (triangle->solc * posZ) + triangle->sold) -
        boundingBoxSize;
    if (distanceToSurface > 0.0f) {
        if (collision->distance_zx > distanceToSurface) {
            collision->flag_zx = 1;
            collision->last_zx = index;
            collision->distance_zx = distanceToSurface;
            collision->bump_zx[0] = triangle->sola;
            collision->bump_zx[1] = triangle->solb;
            collision->bump_zx[2] = triangle->solc;
        }
        return 0;
    }

    if (distanceToSurface > -16.0f) {
        collision->flag_zx = 1;
        collision->last_zx = index;
        collision->distance_zx = distanceToSurface;
        collision->bump_zx[0] = triangle->sola;
        collision->bump_zx[1] = triangle->solb;
        collision->bump_zx[2] = triangle->solc;
        return 1;
    }
    return 0;
}

s32 CheckTriangleXY(Collision* collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ, u16 index) {
    FaceStruct* triangle = &gFaceBuffer[index];
    UNUSED f32 pad[6];
    f32 x3;
    f32 y3;
    UNUSED f32 pad2[1];

    UNUSED f32 pad3[5];
    f32 x2;
    f32 y2;
    f32 x1;
    f32 y1;
    f32 distanceToSurface;
    f32 crossProductYX_1;
    f32 crossProductYX_2;
    f32 crossProductYX_3;
    s32 b = true;

    if (triangle->minx > posX) {
        return 0;
    }
    if (triangle->maxx < posX) {
        return 0;
    }
    if (triangle->maxy < posY) {
        return 0;
    }
    if (triangle->miny > posY) {
        return 0;
    }

    if ((triangle->minz - boundingBoxSize * 3.0f) > posZ) {
        return 0;
    }
    if ((triangle->maxz + boundingBoxSize * 3.0f) < posZ) {
        return 0;
    }

    x1 = triangle->p1->v.ob[0];
    y1 = triangle->p1->v.ob[1];

    x2 = triangle->p2->v.ob[0];
    y2 = triangle->p2->v.ob[1];

    x3 = triangle->p3->v.ob[0];
    y3 = triangle->p3->v.ob[1];

    crossProductYX_1 = (y1 - posY) * (x2 - posX) - (x1 - posX) * (y2 - posY);

    if (!crossProductYX_1) {

        crossProductYX_2 = (y2 - posY) * (x3 - posX) - (x2 - posX) * (y3 - posY);

        crossProductYX_3 = (y3 - posY) * (x1 - posX) - (x3 - posX) * (y1 - posY);

        if ((crossProductYX_2 * crossProductYX_3) < 0.0f) {
            b = false;
        }
    } else {
        crossProductYX_2 = (y2 - posY) * (x3 - posX) - (x2 - posX) * (y3 - posY);
        if (!crossProductYX_2) {
            crossProductYX_3 = (y3 - posY) * (x1 - posX) - (x3 - posX) * (y1 - posY);
            if (crossProductYX_1 * crossProductYX_3 < 0.0f) {
                b = false;
            }
        } else {
            if ((crossProductYX_1 * crossProductYX_2) < 0.0f) {
                b = false;
            } else {
                crossProductYX_3 = ((y3 - posY) * (x1 - posX)) - ((x3 - posX) * (y1 - posY));
                if (crossProductYX_3 != 0) {
                    if ((crossProductYX_2 * crossProductYX_3) < 0.0f) {
                        b = false;
                    }
                }
            }
        }
    }
    if (!b) {
        return 0;
    }
    distanceToSurface =
        ((triangle->sola * posX) + (triangle->solb * posY) + (triangle->solc * posZ) + triangle->sold) -
        boundingBoxSize;
    if (distanceToSurface > 0.0f) {
        if (distanceToSurface < collision->distance_xy) {
            collision->flag_xy = 1;
            collision->last_xy = index;
            collision->distance_xy = distanceToSurface;
            collision->bump_xy[0] = triangle->sola;
            collision->bump_xy[1] = triangle->solb;
            collision->bump_xy[2] = triangle->solc;
        }
        return 0;
    }

    if (distanceToSurface > -16.0f) {
        collision->flag_xy = 1;
        collision->last_xy = index;
        collision->distance_xy = distanceToSurface;
        collision->bump_xy[0] = triangle->sola;
        collision->bump_xy[1] = triangle->solb;
        collision->bump_xy[2] = triangle->solc;
        return 1;
    }
    return 0;
}

s32 CheckTriangleYZ(Collision* collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ, u16 index) {
    FaceStruct* triangle = &gFaceBuffer[index];
    s32 b = true;
    UNUSED f32 pad[7];
    f32 y3;
    f32 z3;
    UNUSED f32 pad2[5];
    f32 y2;
    f32 z2;
    f32 y1;
    f32 z1;
    f32 distanceToSurface;
    f32 crossProductYZ_1;
    f32 crossProductYZ_2;
    f32 crossProductYZ_3;
    if (triangle->minz > posZ) {
        return 0;
    }
    if (triangle->maxz < posZ) {
        return 0;
    }
    if (triangle->maxy < posY) {
        return 0;
    }
    if (triangle->miny > posY) {
        return 0;
    }
    if ((triangle->minx - (boundingBoxSize * 3.0f)) > posX) {
        return 0;
    }
    if ((triangle->maxx + (boundingBoxSize * 3.0f)) < posX) {
        return 0;
    }

    z1 = triangle->p1->v.ob[2];
    y1 = triangle->p1->v.ob[1];

    z2 = triangle->p2->v.ob[2];
    y2 = triangle->p2->v.ob[1];

    z3 = triangle->p3->v.ob[2];
    y3 = triangle->p3->v.ob[1];

    crossProductYZ_1 = (y1 - posY) * (z2 - posZ) - (z1 - posZ) * (y2 - posY);

    if (!crossProductYZ_1) {

        crossProductYZ_2 = ((y2 - posY) * (z3 - posZ)) - ((z2 - posZ) * (y3 - posY));

        crossProductYZ_3 = ((y3 - posY) * (z1 - posZ)) - ((z3 - posZ) * (y1 - posY));

        if ((crossProductYZ_2 * crossProductYZ_3) < 0.0f) {
            b = false;
        }
    } else {

        crossProductYZ_2 = ((y2 - posY) * (z3 - posZ)) - ((z2 - posZ) * (y3 - posY));

        if (crossProductYZ_2 == 0) {
            crossProductYZ_3 = ((y3 - posY) * (z1 - posZ)) - ((z3 - posZ) * (y1 - posY));

            if ((crossProductYZ_1 * crossProductYZ_3) < 0.0f) {
                b = false;
            }
        } else {

            if ((crossProductYZ_1 * crossProductYZ_2) < 0.0f) {
                b = false;
            } else {
                crossProductYZ_3 = ((y3 - posY) * (z1 - posZ)) - ((z3 - posZ) * (y1 - posY));
                if (crossProductYZ_3 != 0) {
                    if ((crossProductYZ_2 * crossProductYZ_3) < 0.0f) {
                        b = false;
                    }
                }
            }
        }
    }
    if (!b) {
        return 0;
    }

    distanceToSurface = ((((triangle->sola * posX) + (triangle->solb * posY)) + (triangle->solc * posZ)) +
                         triangle->sold) -
                        boundingBoxSize;

    if (distanceToSurface > 0.0f) {
        if (distanceToSurface < collision->distance_yz) {
            collision->flag_yz = 1;
            collision->last_yz = index;
            collision->distance_yz = distanceToSurface;
            collision->bump_yz[0] = triangle->sola;
            collision->bump_yz[1] = triangle->solb;
            collision->bump_yz[2] = triangle->solc;
        }
        return 0;
    }
    if (distanceToSurface > (-16.0f)) {
        collision->flag_yz = 1;
        collision->last_yz = index;
        collision->distance_yz = distanceToSurface;
        collision->bump_yz[0] = triangle->sola;
        collision->bump_yz[1] = triangle->solb;
        collision->bump_yz[2] = triangle->solc;
        return 1;
    }
    return 0;
}

s32 check_triangle(f32 posX, f32 posZ, u16 index) {
    FaceStruct* triangle = &gFaceBuffer[index];
    UNUSED f32 pad;
    f32 x3;
    UNUSED f32 pad2;
    f32 z3;
    f32 x2;
    UNUSED f32 pad3;
    f32 z2;
    f32 x1;
    f32 z1;
    UNUSED f32 pad4[4];
    f32 crossProductZX_1;
    f32 crossProductZX_3;
    f32 crossProductZX_2;
    s32 b = true;

    x1 = triangle->p1->v.ob[0];
    z1 = triangle->p1->v.ob[2];

    x2 = triangle->p2->v.ob[0];
    z2 = triangle->p2->v.ob[2];

    x3 = triangle->p3->v.ob[0];
    z3 = triangle->p3->v.ob[2];

    crossProductZX_1 = (z1 - posZ) * (x2 - posX) - (x1 - posX) * (z2 - posZ);

    if (!crossProductZX_1) {

        crossProductZX_2 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);

        crossProductZX_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);

        if ((crossProductZX_2 * crossProductZX_3) < 0.0f) {
            b = false;
        }
    } else {
        crossProductZX_2 = (z2 - posZ) * (x3 - posX) - (x2 - posX) * (z3 - posZ);
        if (!crossProductZX_2) {
            crossProductZX_3 = (z3 - posZ) * (x1 - posX) - (x3 - posX) * (z1 - posZ);
            if (crossProductZX_1 * crossProductZX_3 < 0.0f) {
                b = false;
            }
        } else {
            if ((crossProductZX_1 * crossProductZX_2) < 0.0f) {
                b = false;
            } else {
                crossProductZX_3 = ((z3 - posZ) * (x1 - posX)) - ((x3 - posX) * (z1 - posZ));
                if (crossProductZX_3 != 0) {
                    if ((crossProductZX_2 * crossProductZX_3) < 0.0f) {
                        b = false;
                    }
                }
            }
        }
    }
    return b;
}

s8 check_status(u16 index) {
    FaceStruct* triangle = &gFaceBuffer[index];
    return triangle->status;
}

s16 CheckArea(u16 index) {
    FaceStruct* triangle = &gFaceBuffer[index];
    return triangle->flag & 0xFF;
}

s16 check_shadow(u16 index) {
    FaceStruct* triangle = &gFaceBuffer[index];
    return triangle->flag & 0x1000;
}

s16 CheckSlope(u16 index) {
    FaceStruct* triangle = &gFaceBuffer[index];
    return triangle->flag & 0x400;
}

s16 OoBCheck(u16 index) {
    FaceStruct* triangle = &gFaceBuffer[index];
    return triangle->flag & 0x800;
}

f32 CalcHeight(f32 x, f32 y, f32 z, u16 index) {
    FaceStruct* triangle = &gFaceBuffer[index];
    if (triangle->solb == 0.0f) {
        return y;
    }
    return ((triangle->sola * x) + (triangle->solc * z) + triangle->sold) / -triangle->solb;
}

f32 get_hight(Collision* collision, Vec3f pos) {
    if (collision->flag_zx == 1) {
        return CalcHeight(pos[0], pos[1], pos[2], collision->last_zx);
    }
    if (collision->flag_xy == 1) {
        return CalcHeight(pos[0], pos[1], pos[2], collision->last_xy);
    }
    if (collision->flag_yz == 1) {
        return CalcHeight(pos[0], pos[1], pos[2], collision->last_yz);
    }
    return pos[1];
}

void BumpRoutin(Vec3f pos, UNUSED f32 boundingBoxSize, Vec3f velocity, UNUSED f32 unk) {
    f32 magnitude;
    f32 dotProduct;
    f32 x;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    f32 y;
    f32 z;
    UNUSED f32 x2;
    UNUSED f32 y2;
    f32 velocityMagnitude;
    UNUSED f32 z2;
    f32 scaleFactor;

    x = velocity[0];
    y = velocity[1];
    z = velocity[2];
    velocityMagnitude = sqrtf((x * x) + (y * y) + (z * z));

    if ((velocityMagnitude > 4.5) || (velocityMagnitude < 3.5)) {
        velocityMagnitude = 4.0f;
    }

    x2 = pos[0];
    y2 = pos[1];
    z2 = pos[2];

    dotProduct = (pos[0] * x) + (pos[1] * y) + (pos[2] * z);
    scaleX = x - dotProduct * pos[0];
    scaleY = y - dotProduct * pos[1];
    scaleZ = z - dotProduct * pos[2];

    x = scaleX - dotProduct * pos[0];
    y = scaleY - dotProduct * pos[1];
    z = scaleZ - dotProduct * pos[2];

    magnitude = sqrtf((x * x) + (y * y) + (z * z));

    scaleFactor = 1.0f / magnitude * velocityMagnitude;

    velocity[0] = x * scaleFactor;
    velocity[1] = y * scaleFactor;
    velocity[2] = z * scaleFactor;
}

void CalcBumpVelocity(Collision* collision, Vec3f velocity) {
    if (collision->distance_xy < 0.0f) {
        BumpRoutin(collision->bump_xy, collision->distance_xy, velocity, 2.0f);
    }

    if (collision->distance_yz < 0.0f) {
        BumpRoutin(collision->bump_yz, collision->distance_yz, velocity, 2.0f);
    }
}

/**
 * Adjusts the position of pos2 based on pos1 but in the orthogonal direction to pos2.
 */
void BumpVelocity(Vec3f pos1, f32 boundingBoxSize, Vec3f pos2, UNUSED f32 unk) {
    f32 x1;
    f32 y1;
    f32 z1;
    f32 x2;
    f32 y2;
    f32 z2;
    f32 dotProduct;
    f32 orthoX;
    f32 orthoY;
    f32 orthoZ;

    x2 = pos2[0];
    y2 = pos2[1];
    z2 = pos2[2];
    x1 = -pos1[0];
    y1 = -pos1[1];
    z1 = -pos1[2];

    dotProduct = (x1 * x2) + (y1 * y2) + (z1 * z2);

    orthoX = x2 - (dotProduct * x1);
    orthoY = y2 - (dotProduct * y1);
    orthoZ = z2 - (dotProduct * z1);

    if (boundingBoxSize < -3.5) {
        pos2[0] = orthoX - (dotProduct * x1 * 0.5f);
        pos2[1] = orthoY - (dotProduct * y1 * 0.5f);
        pos2[2] = orthoZ - (dotProduct * z1 * 0.5f);
    } else {
        pos2[0] = orthoX;
        pos2[1] = orthoY;
        pos2[2] = orthoZ;
    }
}

UNUSED s32 check_tire(KartTire* tire) {
    Collision collision;
    UNUSED s32 pad[12];
    s32 courseLengthX;
    s32 courseLengthZ;
    f32 tireX;
    f32 tireY;
    f32 tireZ;
    s16 sectionIndexX;
    s16 sectionIndexZ;
    u16 i;
    u16 numTriangles;
    u16 meshIndex;
    s16 gridIndex;
    u16 sectionIndex;

    collision.flag_xy = 0;
    collision.flag_yz = 0;
    collision.flag_zx = 0;
    collision.distance_xy = 1000.0f;
    collision.distance_yz = 1000.0f;
    collision.distance_zx = 1000.0f;
    tireX = tire->Position[0];
    tireY = tire->Position[1];
    tireZ = tire->Position[2];
    switch (tire->LastAxis) { /* irregular */
        case 0x80:
            if (CheckTriangleYZ(&collision, 5.0f, tireX, tireY, tireZ, (u16) (s32) tire->LastPointer) == 1) {
                tire->Height = CalcHeight(tireX, tireY, tireZ, tire->LastPointer);
                return 1;
            }
            break;
        case 0x40:
            if (CheckTriangleZX(&collision, 5.0f, tireX, tireY, tireZ, (u16) (s32) tire->LastPointer) == 1) {
                tire->Height = CalcHeight(tireX, tireY, tireZ, tire->LastPointer);
                return 1;
            }
            break;
        case 0x20:
            if (CheckTriangleXY(&collision, 5.0f, tireX, tireY, tireZ, (u16) (s32) tire->LastPointer) == 1) {
                tire->Height = CalcHeight(tireX, tireY, tireZ, tire->LastPointer);
                return 1;
            }
            break;
        default:
            break;
    }
    courseLengthX = maximum_x - minimum_x;
    courseLengthZ = maximum_z - minimum_z;
    sectionIndexX = (tireX - minimum_x) / (courseLengthX / GRID_SIZE);
    sectionIndexZ = (tireZ - minimum_z) / (courseLengthZ / GRID_SIZE);
    if (sectionIndexX < 0) {
        return 0;
    }
    if (sectionIndexZ < 0) {
        return 0;
    }
    if (sectionIndexX >= GRID_SIZE) {
        return 0;
    }
    if (sectionIndexZ >= GRID_SIZE) {
        return 0;
    }

    gridIndex = (sectionIndexX + sectionIndexZ * GRID_SIZE);
    numTriangles = gCollisionGrid[gridIndex].numTriangles;
    if (numTriangles == 0) {
        return 0;
    }
    sectionIndex = gCollisionGrid[gridIndex].triangle;
    for (i = 0; i < numTriangles; i++) {
        meshIndex = collision_table[sectionIndex];
        if (gFaceBuffer[meshIndex].flag & FACING_Y_AXIS) {
            if (meshIndex != tire->LastPointer) {
                if (CheckTriangleZX(&collision, 5.0f, tireX, tireY, tireZ, meshIndex) == 1) {
                    tire->Height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                    tire->Status = gFaceBuffer[meshIndex].status;
                    tire->LastAxis = 0x40;
                    tire->LastPointer = meshIndex;
                    if (gFaceBuffer[meshIndex].flag & 0x1000) {
                        tire->Dummy = 1;
                    } else {
                        tire->Dummy = 0;
                    }
                    return 1;
                }
            }
        } else if (gFaceBuffer[meshIndex].flag & FACING_X_AXIS) {
            if ((gFaceBuffer[meshIndex].sola != 1.0f) && (meshIndex != tire->LastPointer)) {
                if (CheckTriangleYZ(&collision, 5.0f, tireX, tireY, tireZ, meshIndex) == 1) {
                    tire->Height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                    tire->Status = gFaceBuffer[meshIndex].status;
                    tire->LastAxis = 0x80;
                    tire->LastPointer = meshIndex;
                    return 1;
                }
            }
        } else if ((gFaceBuffer[meshIndex].solc != 1.0f) && (meshIndex != tire->LastPointer)) {
            if (CheckTriangleXY(&collision, 5.0f, tireX, tireY, tireZ, meshIndex) == 1) {
                tire->Height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                tire->Status = gFaceBuffer[meshIndex].status;
                tire->LastAxis = 0x20;
                tire->LastPointer = meshIndex;
                return 1;
            }
        }
        sectionIndex++;
    }
    tire->Height = tireY;
    tire->Status = 0;
    //! @bug
    // Another function that has a return value but doesn't have an explicit return statement in one of its codepaths.
    // The return value at this point will be whatever was last returned by func_802AAE4C/func_802AB6C4/func_802AB288
    // depending on which (if any) if statements were entered on the loop's last cycle
}

s32 CheckTriangleZX_V(Collision* collision, f32 boundingBoxSize, f32 newX, f32 newY, f32 newZ,
                                       u16 index, f32 oldX, f32 oldY, f32 oldZ) {
    FaceStruct* triangle = &gFaceBuffer[index];
    UNUSED s32 pad;
    f32 x4;
    UNUSED f32 y4;
    f32 z4;
    f32 x3;
    UNUSED f32 y3;
    f32 z3;
    f32 x2;
    UNUSED f32 y2;
    f32 z2;
    f32 surfaceDistance;
    f32 temp;
    UNUSED s32 pad2[2];
    f32 area;
    f32 area2;
    f32 area3;
    s32 b = 1;

    if (triangle->minx > newX) {
        return 0;
    }
    if (triangle->minz > newZ) {
        return 0;
    }
    if (triangle->maxx < newX) {
        return 0;
    }
    if (triangle->maxz < newZ) {
        return 0;
    }
    if ((triangle->miny - boundingBoxSize * 3.0f) > newY) {
        return 0;
    }

    x2 = (f32) triangle->p1->v.ob[0];
    z2 = (f32) triangle->p1->v.ob[2];

    x3 = (f32) triangle->p2->v.ob[0];
    z3 = (f32) triangle->p2->v.ob[2];

    x4 = (f32) triangle->p3->v.ob[0];
    z4 = (f32) triangle->p3->v.ob[2];

    area = (z2 - newZ) * (x3 - newX) - (x2 - newX) * (z3 - newZ);

    if (area == 0) {
        area2 = (z3 - newZ) * (x4 - newX) - (x3 - newX) * (z4 - newZ);
        area3 = (z4 - newZ) * (x2 - newX) - (x4 - newX) * (z2 - newZ);
        if (area2 * area3 < 0.0f) {
            b = 0;
        }
    } else {

        area2 = (z3 - newZ) * (x4 - newX) - (x3 - newX) * (z4 - newZ);

        if (area2 == 0) {

            area3 = (z4 - newZ) * (x2 - newX) - (x4 - newX) * (z2 - newZ);

            if (area * area3 < 0.0f) {
                b = 0;
            }
        } else {
            if ((area * area2) < 0.0f) {
                b = 0;
            } else {
                area3 = (z4 - newZ) * (x2 - newX) - (x4 - newX) * (z2 - newZ);
                if (area3 != 0) {
                    if (area2 * area3 < 0.0f) {
                        b = 0;
                    }
                }
            }
        }
    }
    if (b == 0) {
        return 0;
    }

    surfaceDistance =
        (triangle->sola * newX) + (triangle->solb * newY) + (triangle->solc * newZ) + triangle->sold;

    if (surfaceDistance > boundingBoxSize) {
        if (collision->distance_zx > surfaceDistance) {
            collision->flag_zx = 1;
            collision->last_zx = index;
            collision->distance_zx = surfaceDistance - boundingBoxSize;
            collision->bump_zx[0] = triangle->sola;
            collision->bump_zx[1] = triangle->solb;
            collision->bump_zx[2] = triangle->solc;
        }
        return 0;
    }

    temp = (triangle->sola * oldX) + (triangle->solb * oldY) + (triangle->solc * oldZ) + triangle->sold;

    if (temp < 0.0f) {
        return 0;
    }

    collision->flag_zx = 1;
    collision->last_zx = index;
    collision->distance_zx = surfaceDistance - boundingBoxSize;
    collision->bump_zx[0] = triangle->sola;
    collision->bump_zx[1] = triangle->solb;
    collision->bump_zx[2] = triangle->solc;
    return 1;
}

/**
 * Wall collision
 */
s32 CheckTriangleXY_V(Collision* arg, f32 boundingBoxSize, f32 x1, f32 y1, f32 z1, u16 surfaceIndex, f32 posX,
                             f32 posY, f32 posZ) {
    FaceStruct* triangle = &gFaceBuffer[surfaceIndex];
    UNUSED s32 pad[6];
    f32 x4;
    f32 y4;
    f32 x3;
    f32 y3;
    f32 x2;
    f32 y2;
    UNUSED s32 pad2[3];
    f32 distanceToSurface;
    f32 distanceToSurface2;
    UNUSED s32 pad3[2];
    f32 area;
    f32 area2;
    f32 area3;
    s32 b = true;
    if (triangle->minx > x1) {
        return NO_COLLISION;
    }
    if (triangle->maxx < x1) {
        return NO_COLLISION;
    }
    if (triangle->maxy < y1) {
        return NO_COLLISION;
    }
    if (triangle->miny > y1) {
        return NO_COLLISION;
    }
    if ((triangle->minz - boundingBoxSize * 3.0f) > z1) {
        return NO_COLLISION;
    }
    if ((triangle->maxz + boundingBoxSize * 3.0f) < z1) {
        return NO_COLLISION;
    }

    x2 = (f32) triangle->p1->v.ob[0];
    y2 = (f32) triangle->p1->v.ob[1];

    x3 = (f32) triangle->p2->v.ob[0];
    y3 = (f32) triangle->p2->v.ob[1];

    x4 = (f32) triangle->p3->v.ob[0];
    y4 = (f32) triangle->p3->v.ob[1];

    area = (y2 - y1) * (x3 - x1) - (x2 - x1) * (y3 - y1);

    if (area == 0) {
        area2 = (y3 - y1) * (x4 - x1) - (x3 - x1) * (y4 - y1);
        area3 = (y4 - y1) * (x2 - x1) - (x4 - x1) * (y2 - y1);

        if (area2 * area3 < 0.0f) {
            b = false;
        }
    } else {

        area2 = (y3 - y1) * (x4 - x1) - (x3 - x1) * (y4 - y1);

        if (area2 == 0) {
            area3 = (y4 - y1) * (x2 - x1) - (x4 - x1) * (y2 - y1);

            if ((area * area3) < 0.0f) {
                b = false;
            }
        } else {
            if ((area * area2) < 0.0f) {
                b = false;
            } else {
                area3 = (y4 - y1) * (x2 - x1) - (x4 - x1) * (y2 - y1);

                if (area3 != 0) {
                    if ((area2 * area3) < 0.0f) {
                        b = false;
                    }
                }
            }
        }
    }

    if (!b) {
        return NO_COLLISION;
    }

    distanceToSurface =
        ((triangle->sola * x1) + (triangle->solb * y1) + (triangle->solc * z1)) + triangle->sold;
    if (triangle->flag & 0x200) {
        distanceToSurface2 =
            ((triangle->sola * posX) + (triangle->solb * posY) + (triangle->solc * posZ)) + triangle->sold;
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 > 0.0f)) {
            if (distanceToSurface < boundingBoxSize) {
                arg->flag_xy = 1;
                arg->last_xy = surfaceIndex;
                arg->distance_xy = distanceToSurface - boundingBoxSize;
                arg->bump_xy[0] = triangle->sola;
                arg->bump_xy[1] = triangle->solb;
                arg->bump_xy[2] = triangle->solc;
                return COLLISION;
            }
            return NO_COLLISION;
        }

        if ((distanceToSurface < 0.0f) && (distanceToSurface2 < 0.0f)) {
            distanceToSurface *= -1.0f;
            if (distanceToSurface < boundingBoxSize) {
                arg->flag_xy = 1;
                arg->last_xy = surfaceIndex;
                arg->distance_xy = distanceToSurface - boundingBoxSize;
                arg->bump_xy[0] = -triangle->sola;
                arg->bump_xy[1] = -triangle->solb;
                arg->bump_xy[2] = -triangle->solc;
                return 1;
            }
            return NO_COLLISION;
        }
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 < 0.0f)) {
            arg->flag_xy = 1;
            arg->last_xy = surfaceIndex;
            arg->distance_xy = -(distanceToSurface + boundingBoxSize);
            arg->bump_xy[0] = -triangle->sola;
            arg->bump_xy[1] = -triangle->solb;
            arg->bump_xy[2] = -triangle->solc;
            return COLLISION;
        }
        if ((distanceToSurface < 0.0f) && (distanceToSurface2 > 0.0f)) {
            arg->flag_xy = 1;
            arg->last_xy = surfaceIndex;
            arg->distance_xy = distanceToSurface + boundingBoxSize;
            arg->bump_xy[0] = triangle->sola;
            arg->bump_xy[1] = triangle->solb;
            arg->bump_xy[2] = triangle->solc;
            return COLLISION;
        }
        if (distanceToSurface == 0.0f) {
            if (distanceToSurface2 >= 0.0f) {
                arg->flag_xy = 1;
                arg->last_xy = surfaceIndex;
                arg->distance_xy = distanceToSurface2 + boundingBoxSize;
                arg->bump_xy[0] = triangle->sola;
                arg->bump_xy[1] = triangle->solb;
                arg->bump_xy[2] = triangle->solc;
                return COLLISION;
            }
            arg->flag_xy = 1;
            arg->last_xy = surfaceIndex;
            arg->distance_xy = -(distanceToSurface2 + boundingBoxSize);
            arg->bump_xy[0] = triangle->sola;
            arg->bump_xy[1] = triangle->solb;
            arg->bump_xy[2] = triangle->solc;
            return COLLISION;
        }
        return NO_COLLISION;
    }
    if (distanceToSurface > boundingBoxSize) {
        if (distanceToSurface < arg->distance_xy) {
            arg->flag_xy = 1;
            arg->last_xy = surfaceIndex;
            arg->distance_xy = distanceToSurface - boundingBoxSize;
            arg->bump_xy[0] = triangle->sola;
            arg->bump_xy[1] = triangle->solb;
            arg->bump_xy[2] = triangle->solc;
        }
        return NO_COLLISION;
    }

    distanceToSurface2 =
        (triangle->sola * posX) + (triangle->solb * posY) + (triangle->solc * posZ) + triangle->sold;
    if (distanceToSurface2 < 0.0f) {
        return NO_COLLISION;
    }
    arg->flag_xy = 1;
    arg->last_xy = surfaceIndex;
    arg->distance_xy = distanceToSurface - boundingBoxSize;
    arg->bump_xy[0] = triangle->sola;
    arg->bump_xy[1] = triangle->solb;
    arg->bump_xy[2] = triangle->solc;
    return COLLISION;
}

/**
 * This is actually more like colliding with face X/Y/Z
 */
s32 CheckTriangleYZ_V(Collision* arg, f32 boundingBoxSize, f32 x1, f32 y1, f32 z1, u16 surfaceIndex, f32 posX,
                             f32 posY, f32 posZ) {
    FaceStruct* triangle = &gFaceBuffer[surfaceIndex];
    s32 b = 1;
    UNUSED s32 pad[7];
    f32 y4;
    f32 z4;
    f32 y3;
    f32 z3;
    f32 y2;
    f32 z2;
    UNUSED s32 pad3[2];
    f32 distanceToSurface;
    f32 distanceToSurface2;
    UNUSED s32 pad4[2];
    f32 area;
    f32 area2;
    f32 area3;

    if (triangle->minz > z1) {
        return NO_COLLISION;
    }
    if (triangle->maxz < z1) {
        return NO_COLLISION;
    }
    if (triangle->maxy < y1) {
        return NO_COLLISION;
    }
    if (triangle->miny > y1) {
        return NO_COLLISION;
    }
    if ((triangle->minx - boundingBoxSize * 3.0f) > x1) {
        return NO_COLLISION;
    }
    if ((triangle->maxx + boundingBoxSize * 3.0f) < x1) {
        return NO_COLLISION;
    }

    z2 = (f32) triangle->p1->v.ob[2];
    y2 = (f32) triangle->p1->v.ob[1];

    z3 = (f32) triangle->p2->v.ob[2];
    y3 = (f32) triangle->p2->v.ob[1];

    z4 = (f32) triangle->p3->v.ob[2];
    y4 = (f32) triangle->p3->v.ob[1];

    area = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);

    if (area == 0) {
        area2 = (y3 - y1) * (z4 - z1) - (z3 - z1) * (y4 - y1);
        area3 = (y4 - y1) * (z2 - z1) - (z4 - z1) * (y2 - y1);

        if (area2 * area3 < 0.0f) {
            b = 0;
        }
    } else {

        area2 = (y3 - y1) * (z4 - z1) - (z3 - z1) * (y4 - y1);

        if (area2 == 0) {
            area3 = (y4 - y1) * (z2 - z1) - (z4 - z1) * (y2 - y1);

            if ((area * area3) < 0.0f) {
                b = 0;
            }
        } else {
            if ((area * area2) < 0.0f) {
                b = 0;
            } else {
                area3 = (y4 - y1) * (z2 - z1) - (z4 - z1) * (y2 - y1);

                if (area3 != 0) {
                    if ((area2 * area3) < 0.0f) {
                        b = 0;
                    }
                }
            }
        }
    }
    if (b == 0) {
        return NO_COLLISION;
    }

    distanceToSurface =
        ((triangle->sola * x1) + (triangle->solb * y1) + (triangle->solc * z1)) + triangle->sold;
    if (triangle->flag & 0x200) {
        distanceToSurface2 =
            ((triangle->sola * posX) + (triangle->solb * posY) + (triangle->solc * posZ)) + triangle->sold;
        // sp48 = temp_f4_2;
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 > 0.0f)) {
            if (distanceToSurface < boundingBoxSize) {
                arg->flag_yz = 1;
                arg->last_yz = surfaceIndex;
                arg->distance_yz = distanceToSurface - boundingBoxSize;
                arg->bump_yz[0] = triangle->sola;
                arg->bump_yz[1] = triangle->solb;
                arg->bump_yz[2] = triangle->solc;
                return COLLISION;
            }
            return NO_COLLISION;
        }

        if ((distanceToSurface < 0.0f) && (distanceToSurface2 < 0.0f)) {
            distanceToSurface *= -1.0f;
            if (distanceToSurface < boundingBoxSize) {
                arg->flag_yz = 1;
                arg->last_yz = surfaceIndex;
                arg->distance_yz = distanceToSurface - boundingBoxSize;
                arg->bump_yz[0] = -triangle->sola;
                arg->bump_yz[1] = -triangle->solb;
                arg->bump_yz[2] = -triangle->solc;
                return 1;
            }
            return NO_COLLISION;
        }
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 < 0.0f)) {
            arg->flag_yz = 1;
            arg->last_yz = surfaceIndex;
            arg->distance_yz = -(distanceToSurface + boundingBoxSize);
            arg->bump_yz[0] = -triangle->sola;
            arg->bump_yz[1] = -triangle->solb;
            arg->bump_yz[2] = -triangle->solc;
            return COLLISION;
        }
        if ((distanceToSurface < 0.0f) && (distanceToSurface2 > 0.0f)) {
            arg->flag_yz = 1;
            arg->last_yz = surfaceIndex;
            arg->distance_yz = distanceToSurface + boundingBoxSize;
            arg->bump_yz[0] = triangle->sola;
            arg->bump_yz[1] = triangle->solb;
            arg->bump_yz[2] = triangle->solc;
            return COLLISION;
        }
        if (distanceToSurface == 0.0f) {
            if (distanceToSurface2 >= 0.0f) {
                arg->flag_yz = 1;
                arg->last_yz = surfaceIndex;
                arg->distance_yz = distanceToSurface2 + boundingBoxSize;
                arg->bump_yz[0] = triangle->sola;
                arg->bump_yz[1] = triangle->solb;
                arg->bump_yz[2] = triangle->solc;
                return COLLISION;
            }
            arg->flag_yz = 1;
            arg->last_yz = surfaceIndex;
            arg->distance_yz = -(distanceToSurface2 + boundingBoxSize);
            arg->bump_yz[0] = triangle->sola;
            arg->bump_yz[1] = triangle->solb;
            arg->bump_yz[2] = triangle->solc;
            return COLLISION;
        }
        return NO_COLLISION;
    }
    if (distanceToSurface > boundingBoxSize) {
        if (arg->distance_yz > distanceToSurface) {
            arg->flag_yz = 1;
            arg->last_yz = surfaceIndex;
            arg->distance_yz = distanceToSurface - boundingBoxSize;
            arg->bump_yz[0] = triangle->sola;
            arg->bump_yz[1] = triangle->solb;
            arg->bump_yz[2] = triangle->solc;
        }
        return NO_COLLISION;
    }

    distanceToSurface2 =
        (triangle->sola * posX) + (triangle->solb * posY) + (triangle->solc * posZ) + triangle->sold;
    if (distanceToSurface2 < 0.0f) {
        return NO_COLLISION;
    }
    arg->flag_yz = 1;
    arg->last_yz = surfaceIndex;
    arg->distance_yz = distanceToSurface - boundingBoxSize;
    arg->bump_yz[0] = triangle->sola;
    arg->bump_yz[1] = triangle->solb;
    arg->bump_yz[2] = triangle->solc;
    return COLLISION;
}

u16 CheckBump2(Collision* collision, f32 boundingBoxSize, f32 newX, f32 newY, f32 newZ, f32 oldX, f32 oldY,
                            f32 oldZ) {
    s32 courseLengthX;
    s32 courseLengthZ;
    s16 sectionIndexX;
    s16 sectionIndexZ;
    u16 numTriangles;
    u16 collisionIndex;
    s16 gridIndex;

    u16 sectionIndex;

    u16 flags = 0;
    s32 sectionX;
    s32 sectionZ;

    u16 i;

    collision->flag_xy = 0;
    collision->flag_yz = 0;
    collision->flag_zx = 0;
    collision->distance_xy = 1000.0f;
    collision->distance_yz = 1000.0f;
    collision->distance_zx = 1000.0f;

    // Check if an intersection test has already been performed.
    if ((s32) collision->last_zx < (s32) g_courseFaceMaxIndex) {
        if (CheckTriangleZX_V(collision, boundingBoxSize, newX, newY, newZ, collision->last_zx,
                                               oldX, oldY, oldZ) == COLLISION) {
            flags |= FACING_Y_AXIS;
        }
    }

    if ((s32) collision->last_xy < (s32) g_courseFaceMaxIndex) {
        if (CheckTriangleXY_V(collision, boundingBoxSize, newX, newY, newZ, collision->last_xy, oldX, oldY,
                                     oldZ) == COLLISION) {
            flags |= FACING_Z_AXIS;
        }
    }

    if ((s32) collision->last_yz < (s32) g_courseFaceMaxIndex) {
        if (CheckTriangleYZ_V(collision, boundingBoxSize, newX, newY, newZ, collision->last_yz, oldX, oldY,
                                     oldZ) == COLLISION) {
            flags |= FACING_X_AXIS;
        }
    }

    if (flags == (FACING_Y_AXIS | FACING_Z_AXIS | FACING_X_AXIS)) {
        return flags;
    }

    courseLengthX = (s32) maximum_x - minimum_x;
    courseLengthZ = (s32) maximum_z - minimum_z;

    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (newX - minimum_x) / sectionX;
    sectionIndexZ = (newZ - minimum_z) / sectionZ;

    if (sectionIndexX < 0) {
        return 0;
    }
    if (sectionIndexZ < 0) {
        return 0;
    }
    if (sectionIndexX >= GRID_SIZE) {
        return 0;
    }
    if (sectionIndexZ >= GRID_SIZE) {
        return 0;
    }

    gridIndex = (sectionIndexX + sectionIndexZ * GRID_SIZE);
    numTriangles = gCollisionGrid[gridIndex].numTriangles;

    if (numTriangles == 0) {
        return flags;
    }

    sectionIndex = gCollisionGrid[gridIndex].triangle;

    for (i = 0; i < numTriangles; i++) {
        if (flags == (FACING_Y_AXIS | FACING_Z_AXIS | FACING_X_AXIS)) {
            return flags;
        }

        collisionIndex = collision_table[sectionIndex];

        if ((gFaceBuffer[collisionIndex].flag & FACING_Y_AXIS)) {
            if ((flags & FACING_Y_AXIS) == 0) {
                if (collisionIndex != collision->last_zx) {
                    if (CheckTriangleZX_V(collision, boundingBoxSize, newX, newY, newZ, collisionIndex,
                                                           oldX, oldY, oldZ) == COLLISION) {
                        flags |= FACING_Y_AXIS;
                    }
                }
            }
        } else if ((gFaceBuffer[collisionIndex].flag & FACING_X_AXIS) != 0) {
            if ((flags & FACING_X_AXIS) == 0) {
                if (collisionIndex != collision->last_yz) {
                    if (CheckTriangleYZ_V(collision, boundingBoxSize, newX, newY, newZ, collisionIndex, oldX,
                                                 oldY, oldZ) == COLLISION) {
                        flags |= FACING_X_AXIS;
                    }
                }
            }
        } else if ((flags & FACING_Z_AXIS) == 0) {
            if (collisionIndex != collision->last_xy) {
                if (CheckTriangleXY_V(collision, boundingBoxSize, newX, newY, newZ, collisionIndex, oldX, oldY,
                                             oldZ) == COLLISION) {
                    flags |= FACING_Z_AXIS;
                }
            }
        }
        sectionIndex++;
    }
    return flags;
}

u16 CheckBump(Collision* collision, f32 boundingBoxSize, f32 posX, f32 posY, f32 posZ) {
    u16 numTriangles;
    s32 courseLengthX;
    s32 courseLengthZ;
    u16 meshIndex;
    s32 sectionX;
    s32 sectionZ;
    s16 sectionIndexX;
    s16 sectionIndexZ;
    s16 gridIndex;
    u16 i;

    u16 sectionIndex;
    u16 flags;

    collision->flag_xy = 0;
    collision->flag_yz = 0;
    collision->flag_zx = 0;
    collision->distance_xy = 1000.0f;
    collision->distance_yz = 1000.0f;
    collision->distance_zx = 1000.0f;
    flags = 0;
    if (collision->last_zx < g_courseFaceMaxIndex) {
        if (CheckTriangleZX(collision, boundingBoxSize, posX, posY, posZ, collision->last_zx) == 1) {
            flags |= FACING_Y_AXIS;
        }
    }
    if (collision->last_xy < g_courseFaceMaxIndex) {
        if (CheckTriangleXY(collision, boundingBoxSize, posX, posY, posZ, collision->last_xy) == 1) {
            flags |= FACING_Z_AXIS;
        }
    }
    if (collision->last_yz < g_courseFaceMaxIndex) {
        if (CheckTriangleYZ(collision, boundingBoxSize, posX, posY, posZ, collision->last_yz) == 1) {
            flags |= FACING_X_AXIS;
        }
    }
    if (flags == (FACING_Y_AXIS | FACING_Z_AXIS | FACING_X_AXIS)) {
        return flags;
    }

    courseLengthX = (s32) maximum_x - minimum_x;
    courseLengthZ = (s32) maximum_z - minimum_z;

    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (posX - minimum_x) / sectionX;
    sectionIndexZ = (posZ - minimum_z) / sectionZ;

    if (sectionIndexX < 0) {
        return 0;
    }
    if (sectionIndexZ < 0) {
        return 0;
    }
    if (sectionIndexX >= GRID_SIZE) {
        return 0;
    }
    if (sectionIndexZ >= GRID_SIZE) {
        return 0;
    }

    gridIndex = sectionIndexX + sectionIndexZ * GRID_SIZE;
    numTriangles = gCollisionGrid[gridIndex].numTriangles;
    if (numTriangles == 0) {
        return flags;
    }
    sectionIndex = gCollisionGrid[gridIndex].triangle;

    for (i = 0; i < numTriangles; i++) {
        if (flags == (FACING_X_AXIS | FACING_Y_AXIS | FACING_Z_AXIS)) {
            return flags;
        }
        meshIndex = collision_table[sectionIndex];
        if (gFaceBuffer[meshIndex].flag & FACING_Y_AXIS) {
            if (!(flags & FACING_Y_AXIS)) {
                if (meshIndex != collision->last_zx) {
                    if (CheckTriangleZX(collision, boundingBoxSize, posX, posY, posZ, meshIndex) == 1) {
                        flags |= FACING_Y_AXIS;
                    }
                }
            }
        } else if (gFaceBuffer[meshIndex].flag & FACING_X_AXIS) {
            if (!(flags & FACING_X_AXIS)) {
                if (meshIndex != collision->last_yz) {
                    if (CheckTriangleYZ(collision, boundingBoxSize, posX, posY, posZ, meshIndex) == 1) {
                        flags |= FACING_X_AXIS;
                    }
                }
            }
        } else {
            if (!(flags & FACING_Z_AXIS)) {
                if (meshIndex != collision->last_xy) {
                    if (CheckTriangleXY(collision, boundingBoxSize, posX, posY, posZ, meshIndex) == 1) {
                        flags |= FACING_Z_AXIS;
                    }
                }
            }
        }
        sectionIndex++;
    }
    return flags;
}

extern u8 D_8014F1110;

/**
 * @brief Returns the height of the surface below the provided position. Used to set actors onto the course mesh.
 * If unable to spawn actor on the surface set to -3000.0f or
 * if outside the collision grid, spawn in the air (3000.0f).
 */
f32 CheckHight(f32 posX, f32 posY, f32 posZ) {
    f32 height;
    s16 sectionIndexX;
    s16 sectionIndexZ;
    s16 gridSection;

    u16 index;
    u16 numTriangles;
    u16 sectionIndex;
    f32 phi_f20 = -3000.0f;
    u16 i;

    s32 courseLengthX;
    s32 courseLengthZ;
    s32 sectionX;
    s32 sectionZ;

    courseLengthX = (maximum_x - minimum_x);
    courseLengthZ = (maximum_z - minimum_z);
    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (s16) ((posX - minimum_x) / sectionX);
    sectionIndexZ = (s16) ((posZ - minimum_z) / sectionZ);
    gridSection = sectionIndexX + (sectionIndexZ * GRID_SIZE);
    numTriangles = gCollisionGrid[gridSection].numTriangles;

    if (sectionIndexX < 0) {
        return 3000.0f;
    }
    if (sectionIndexZ < 0) {
        return 3000.0f;
    }
    if (sectionIndexX >= GRID_SIZE) {
        return 3000.0f;
    }
    if (sectionIndexZ >= GRID_SIZE) {
        return 3000.0f;
    }
    if (numTriangles == 0) {
        return 3000.0f;
    }

    sectionIndex = gCollisionGrid[gridSection].triangle;

    for (i = 0; i < numTriangles; i++) {

        index = collision_table[sectionIndex];

        if ((gFaceBuffer[index].flag & FACING_Y_AXIS) &&
            (check_triangle(posX, posZ, index) == 1)) {
            height = CalcHeight(posX, posY, posZ, index);
            if ((height <= posY) && (phi_f20 < height)) {
                phi_f20 = height;
            }
        }
        sectionIndex++;
    }
    return phi_f20;
}

#define MAX3(a, b, c, out) \
    if (a >= b) {          \
        if (a >= c) {      \
            out = a;       \
        } else             \
            out = c;       \
                           \
    } else if (b >= c) {   \
        out = b;           \
    } else                 \
        out = c;

#define MIN3(a, b, c, out) \
    if (a <= b) {          \
        if (a <= c) {      \
            out = a;       \
        } else             \
            out = c;       \
                           \
    } else if (b <= c) {   \
        out = b;           \
    } else                 \
        out = c;

void save_triangle(Vtx* vtx1, Vtx* vtx2, Vtx* vtx3, s8 surfaceType, u16 sectionId) {
    FaceStruct* triangle = &gFaceBuffer[g_courseFaceMaxIndex];
    s16 x2;
    s16 z2;
    u16 vtx1Flag;
    s16 x3;
    s16 x1;
    s16 y1;
    s16 z1;
    u16 vtx2Flag;
    s16 y2;
    u16 vtx3Flag;
    u16 flags;
    s16 y3;
    s16 z3;

    /* Unused variables placed around doubles for dramatic effect */
    UNUSED s32 pad2[7];

    f64 crossProductX;
    f64 crossProductY;
    f64 crossProductZ;
    f64 magnitude;

    UNUSED s32 pad3[3];

    f32 normalX;
    f32 normalY;
    f32 normalZ;
    f32 distance;

    s16 maxX;
    s16 maxZ;
    s16 minY;
    s16 minX;
    s16 maxY;
    s16 minZ;

    triangle->p1 = vtx1;
    triangle->p2 = vtx2;
    triangle->p3 = vtx3;
    if ((triangle->p1->v.flag == 4) && (triangle->p2->v.flag == 4) && (triangle->p3->v.flag == 4)) {

        return;
    }

    x1 = triangle->p1->v.ob[0];
    y1 = triangle->p1->v.ob[1];
    z1 = triangle->p1->v.ob[2];
    x2 = triangle->p2->v.ob[0];
    y2 = triangle->p2->v.ob[1];
    z2 = triangle->p2->v.ob[2];
    x3 = triangle->p3->v.ob[0];
    y3 = triangle->p3->v.ob[1];
    z3 = triangle->p3->v.ob[2];
    if ((x1 == x2) && (z1 == z2)) {
        triangle->p1 = vtx1;
        triangle->p3 = vtx2;
        triangle->p2 = vtx3;
        x1 = triangle->p1->v.ob[0];
        y1 = triangle->p1->v.ob[1];
        z1 = triangle->p1->v.ob[2];
        x2 = triangle->p3->v.ob[0];
        y2 = triangle->p3->v.ob[1];
        z2 = triangle->p3->v.ob[2];
        x3 = triangle->p2->v.ob[0];
        y3 = triangle->p2->v.ob[1];
        z3 = triangle->p2->v.ob[2];
    }
    MAX3(x1, x2, x3, maxX)

    MAX3(z1, z2, z3, maxZ)

    MAX3(y1, y2, y3, maxY)

    MIN3(x1, x2, x3, minX)

    MIN3(y1, y2, y3, minY)

    MIN3(z1, z2, z3, minZ)

    crossProductX = (((y2 - y1) * (z3 - z2)) - ((z2 - z1) * (y3 - y2)));
    crossProductY = (((z2 - z1) * (x3 - x2)) - ((x2 - x1) * (z3 - z2)));
    crossProductZ = (((x2 - x1) * (y3 - y2)) - ((y2 - y1) * (x3 - x2)));

    // length of the cross product
    magnitude =
        sqrtf((crossProductX * crossProductX) + (crossProductY * crossProductY) + (crossProductZ * crossProductZ));

    if (!magnitude) {
        return;
    }

    normalX = (f32) crossProductX / magnitude;
    normalY = (f32) crossProductY / magnitude;
    normalZ = (f32) crossProductZ / magnitude;

    // Distance from x to plane (cross product's normal).
    distance = -((normalX * x1) + (normalY * y1) + (normalZ * z1));

    // Return if normalY is not vertical.
    // Could be checking if the surface is a floor
    if (wall_flag) {
        if (normalY < -0.9f) {
            return;
        } else if (normalY > 0.9f) {
            return;
        }
    }

    // Return if normalY is not horizontal
    // Could be checking if the surface is a wall
    if (flat_flag) {
        if ((normalY < 0.1f) && (normalY > -0.1f)) {
            return;
        }
    }

    triangle->maxx = maxX;
    triangle->maxz = maxZ;
    triangle->minx = minX;
    triangle->minz = minZ;
    triangle->miny = minY;
    triangle->maxy = maxY;

    // Define the minimum and maximum dimensions of the course.
    if (minX < minimum_x) {
        minimum_x = minX;
    }
    if (minY < minimum_y) {
        minimum_y = minY;
    }
    if (minZ < minimum_z) {
        minimum_z = minZ;
    }
    if (maxX > maximum_x) {
        maximum_x = maxX;
    }
    if (maxY > maximum_y) {
        maximum_y = maxY;
    }
    if (maxZ > maximum_z) {
        maximum_z = maxZ;
    }

    triangle->sola = normalX;
    triangle->solb = normalY;
    triangle->solc = normalZ;
    triangle->sold = distance;

    triangle->status = (u16) surfaceType;

    // Square the crossProduct to produce a magnitude
    crossProductX = crossProductX * crossProductX;
    crossProductY = crossProductY * crossProductY;
    crossProductZ = crossProductZ * crossProductZ;

    shadowface = 0;
    outsideface = 0;

    vtx1Flag = triangle->p1->v.flag;
    vtx2Flag = triangle->p2->v.flag;
    vtx3Flag = triangle->p3->v.flag;

    flags = sectionId;

    if ((vtx1Flag == 1) && (vtx2Flag == 1) && (vtx3Flag == 1)) {
        flags |= 0x400;
    } else if ((vtx1Flag == 2) && (vtx2Flag == 2) && (vtx3Flag == 2)) {
        flags |= 0x800;
    } else if ((vtx1Flag == 3) && (vtx2Flag == 3) && (vtx3Flag == 3)) {
        flags |= 0x1000;
    } else if (D_8015F5A4 != 0) {
        flags |= 0x200;
    }

    triangle->flag = flags;

    // Find the axis with the highest magnitude.

    // Y is the significant axis
    if ((crossProductX <= crossProductY) && (crossProductY >= crossProductZ)) {
        triangle->flag |= FACING_Y_AXIS;
        // X is the significant axis
    } else if ((crossProductX > crossProductY) && (crossProductX >= crossProductZ)) {
        triangle->flag |= FACING_X_AXIS;
        // Z is the significant axis
    } else {
        triangle->flag |= FACING_Z_AXIS;
    }
    g_courseFaceMaxIndex++;
}

/**
 * Triangle contains three indices that are used to get the actual vertices.
 */
void read_triangle(u32 triangle, s8 surfaceType, u16 sectionId) {
    u32 vert1 = ((triangle & 0x00FF0000) >> 16) / 2;
    u32 vert2 = ((triangle & 0x0000FF00) >> 8) / 2;
    u32 vert3 = (triangle & 0x000000FF) / 2;

    Vtx* vtx1 = vertex_buffer2[vert1];
    Vtx* vtx2 = vertex_buffer2[vert2];
    Vtx* vtx3 = vertex_buffer2[vert3];

    save_triangle(vtx1, vtx2, vtx3, surfaceType, sectionId);
}

void read_triangle2(u32 triangle1, u32 triangle2, s8 surfaceType, u16 sectionId) {
    UNUSED s32 pad[2];
    u32 vert1 = ((triangle1 & 0x00FF0000) >> 16) / 2;
    u32 vert2 = ((triangle1 & 0x0000FF00) >> 8) / 2;
    u32 vert3 = (triangle1 & 0x000000FF) / 2;

    // This is actually triangle 2; vert 1,2,3.
    u32 vert4 = ((triangle2 & 0x00FF0000) >> 16) / 2;
    u32 vert5 = ((triangle2 & 0x0000FF00) >> 8) / 2;
    u32 vert6 = (triangle2 & 0x000000FF) / 2;

    Vtx* vtx1 = vertex_buffer2[vert1];
    Vtx* vtx2 = vertex_buffer2[vert2];
    Vtx* vtx3 = vertex_buffer2[vert3];

    Vtx* vtx4 = vertex_buffer2[vert4];
    Vtx* vtx5 = vertex_buffer2[vert5];
    Vtx* vtx6 = vertex_buffer2[vert6];

    // Triangle 1
    save_triangle(vtx1, vtx2, vtx3, surfaceType, sectionId);
    // Triangle 2
    save_triangle(vtx4, vtx5, vtx6, surfaceType, sectionId);
}

void read_quadrangle(u32 line, s8 surfaceType, u16 sectionId) {
    UNUSED s32 pad[6];
    Vtx* vtx1;
    Vtx* vtx2;
    Vtx* vtx3;
    Vtx* vtx4;

    u32 vert1 = ((line & 0x00FF0000) >> 16) / 2;
    u32 vert2 = ((line & 0x0000FF00) >> 8) / 2;
    u32 vert3 = (line & 0x000000FF) / 2;
    u32 vert4 = ((line & 0xFF000000) >> 24) / 2;

    vtx1 = vertex_buffer2[vert1];
    vtx2 = vertex_buffer2[vert2];
    vtx3 = vertex_buffer2[vert3];
    vtx4 = vertex_buffer2[vert4];

    // Triangle 1
    save_triangle(vtx1, vtx2, vtx3, surfaceType, sectionId);
    // Triangle 2
    save_triangle(vtx1, vtx3, vtx4, surfaceType, sectionId);
}

/**
 * Generates a list of pointers to course vtx.
 */
void read_vertex2(uintptr_t addr, u32 numVertices, u32 bufferIndex) {
    u32 i;
    u32 segment = SEGMENT_NUMBER2(addr);
    u32 offset = SEGMENT_OFFSET(addr);
    Vtx* vtx = (Vtx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    for (i = 0; i < numVertices; i++) {
        vertex_buffer2[bufferIndex] = vtx;
        vtx++;
        bufferIndex++;
    }
}
/**
 * @return 1 intersecting triangle, 0 not intersecting.
 */
s32 check_square_line(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 x1, s16 z1, s16 x2, s16 z2) {
    f32 xOffset;
    f32 projectedPoint;
    f32 zOffset;

    xOffset = x2 - x1;
    zOffset = z2 - z1;
    // Line is vertical
    if (xOffset == 0.0f) {
        if (x1 < minX) {
            return 0;
        }
        if (maxX < x1) {
            return 0;
        }
        if (zOffset > 0.0f) {
            if ((z1 < minZ) && (maxZ < z2)) {
                return 1;
            }
        } else if ((z2 < minZ) && (maxZ < z1)) {
            return 1;
        }
    } else {
        // Line is horizontal
        if (zOffset == 0.0f) {
            if (z1 < minZ) {
                return 0;
            }
            if (maxZ < z1) {
                return 0;
            }
            if (xOffset > 0.0f) {
                if ((x1 < minX) && (maxX < x2)) {
                    return 1;
                }
            } else if ((x2 < minX) && (maxX < x1)) {
                return 1;
            }
        } else {
            projectedPoint = ((xOffset / zOffset) * (minZ - z1)) + x1;
            if ((minX <= projectedPoint) && (projectedPoint <= maxX)) {
                return 1;
            }
            projectedPoint = ((xOffset / zOffset) * (maxZ - z1)) + x1;
            if ((minX <= projectedPoint) && (projectedPoint <= maxX)) {
                return 1;
            }
            projectedPoint = ((zOffset / xOffset) * (minX - x1)) + z1;
            if ((minZ <= projectedPoint) && (projectedPoint <= maxZ)) {
                return 1;
            }
            projectedPoint = ((zOffset / xOffset) * (maxX - x1)) + z1;
            if ((minZ <= projectedPoint) && (projectedPoint <= maxZ)) {
                return 1;
            }
        }
    }
    return 0;
}

s32 check_square_triangle(s16 minX, s16 maxX, s16 minZ, s16 maxZ, u16 index) {
    FaceStruct* triangle = &gFaceBuffer[index];
    s16 x1;
    s16 z1;
    s16 x2;
    s16 z2;
    s16 x3;
    s16 z3;

    x1 = triangle->p1->v.ob[0];
    z1 = triangle->p1->v.ob[2];
    x2 = triangle->p2->v.ob[0];
    z2 = triangle->p2->v.ob[2];
    x3 = triangle->p3->v.ob[0];
    z3 = triangle->p3->v.ob[2];
    if ((x1 >= minX) && (maxX >= x1) && (z1 >= minZ) && (maxZ >= z1)) {
        return 1;
    }
    if ((x2 >= minX) && (maxX >= x2) && (z2 >= minZ) && (maxZ >= z2)) {
        return 1;
    }
    if ((x3 >= minX) && (maxX >= x3) && (z3 >= minZ) && (maxZ >= z3)) {
        return 1;
    }
    if (check_square_line(minX, maxX, minZ, maxZ, x1, z1, x2, z2) == 1) {
        return 1;
    }
    if (check_square_line(minX, maxX, minZ, maxZ, x2, z2, x3, z3) == 1) {
        return 1;
    }
    if (check_square_line(minX, maxX, minZ, maxZ, x3, z3, x1, z1) == 1) {
        return 1;
    }
    return 0;
}

/**
 * Splits the collision mesh into 32x32 sections. This allows the game to check only
 * nearby geography for a collision rather than checking against the whole collision mesh.
 * (checking against the whole mesh for every actor would be expensive)
 */
void make_collision_table(void) {
    FaceStruct* triangle;
    s32 i, j, k;
    UNUSED s32 pad[5];
    s16 maxX;
    s16 maxZ;
    s16 minX;
    s16 minZ;
    s32 sectionZ;
    s32 sectionX;
    s32 courseLengthX;
    s32 courseLengthZ;
    s32 index;

    courseLengthX = (s32) maximum_x - minimum_x;
    courseLengthZ = (s32) maximum_z - minimum_z;

    // Separate the course into 32 sections
    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    // Reset the collision grid
    for (i = 0; i < 1024; i++) {
        gCollisionGrid[i].numTriangles = 0;
    }

    collision_pointer = 0;
    /**
     * @warning FreeMemoryPointer incremented just outside this function. Bad practice to hide memory allocation
     * like this.
     */
    collision_table = (u16*) FreeMemoryPointer;

    // 32x32 grid
    for (j = 0; j < GRID_SIZE; j++) {
        for (k = 0; k < GRID_SIZE; k++) {
            index = k + j * GRID_SIZE;

            // Select a section of the course using min/max akin to drawing a bounding-box
            minX = (minimum_x + (sectionX * k)) - 20;
            minZ = (minimum_z + (sectionZ * j)) - 20;

            maxX = minX + sectionX + 40;
            maxZ = minZ + sectionZ + 40;

            for (i = 0; i < g_courseFaceMaxIndex; i++) {
                triangle = gFaceBuffer + i;
                if (triangle->maxz < minZ) {
                    continue;
                }
                if (triangle->minz > maxZ) {
                    continue;
                }
                if (triangle->maxx < minX) {
                    continue;
                }
                if (triangle->minx > maxX) {
                    continue;
                }

                // Add the collision triangle to the list if it's inside the bounding-box
                if (check_square_triangle(minX, maxX, minZ, maxZ, (u16) i) == 1) {
                    // Point this grid section to the first triangle in the section
                    if (gCollisionGrid[index].numTriangles == 0) {
                        gCollisionGrid[index].triangle = collision_pointer;
                    }
                    gCollisionGrid[index].numTriangles++;
                    collision_table[collision_pointer] = (u16) i;
                    collision_pointer++;
                }
            }
        }
    }
}

/**
 * Recursive search for vtx and set surfaceTypes to -1 and sectionId's to 0xFF
 */
void SearchList(Gfx* gfx) {
    search_list3(gfx, SURFACE_DEFAULT, 0xFF);
}

/**
 * Recursive search for vtx and set sectionId's to 0xFF
 */
void SearchList2(Gfx* gfx, s8 surfaceType) {
    search_list3(gfx, surfaceType, 0xFF);
}

extern u32 facenumber;

/**
 * Generate via a recursive search and set for vertex data.
 */
void search_list3(Gfx* addr, s8 surfaceType, u16 sectionId) {
    s32 opcode;
    uintptr_t lo;
    uintptr_t hi;
    s32 i;

    s32 segment = SEGMENT_NUMBER2(addr);
    s32 offset = SEGMENT_OFFSET(addr);
    Gfx* gfx = (Gfx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    shadowface = 0;
    outsideface = 0;

    for (i = 0; i < 0x1FFF; i++) {
        lo = gfx->words.w0;
        hi = gfx->words.w1;
        opcode = GFX_GET_OPCODE(lo);

        if (opcode == (G_DL << 24)) {
            // G_DL's hi contains an addr to another DL.
            search_list3((Gfx*) hi, surfaceType, sectionId);

        } else if (opcode == (G_VTX << 24)) {
            read_vertex2(hi, (lo >> 10) & 0x3F, ((lo >> 16) & 0xFF) >> 1);

        } else if (opcode == (G_TRI1 << 24)) {
            facenumber += 1;
            read_triangle(hi, surfaceType, sectionId);

        } else if (opcode == (G_TRI2 << 24)) {
            facenumber += 2;
            read_triangle2(lo, hi, surfaceType, sectionId);

        } else if (opcode == (G_QUAD << 24)) {
            facenumber += 2;
            read_quadrangle(hi, surfaceType, sectionId);

        } else if (opcode == (G_ENDDL << 24)) {
            break;
        }

        gfx++;
    }
}

/**
 * Search for G_SETTILESIZE and set its args.
 */
void ScrollMapImage(uintptr_t addr, s32 uls, s32 ult) {
    u32 segment = SEGMENT_NUMBER2(addr);
    u32 offset = SEGMENT_OFFSET(addr);
    Gfx* gfx = (Gfx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    u32 opcode;

    uls = (uls << 12) & 0xFFF000;
    ult &= 0xFFF;

    while (true) {

        opcode = GFX_GET_OPCODE(gfx->words.w0);

        if (opcode == (u32) G_ENDDL << 24) {
            break;
        } else if (opcode == (u32) (G_SETTILESIZE << 24)) {
            gfx->words.w0 = (G_SETTILESIZE << 24) | uls | ult;

            break;
        }
        gfx++;
    }
}

void water_vertex(uintptr_t addr, u32 vertexCount, UNUSED s32 vert3, s8 alpha, u8 red, u8 green, u8 blue) {
    s32 segment = SEGMENT_NUMBER2(addr);
    s32 offset = SEGMENT_OFFSET(addr);
    s32 i;
    Vtx* vtx = (Vtx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);

    for (i = 0; (u32) i < vertexCount; i++) {
        if (red) {
            vtx->v.cn[0] = red;
            vtx->v.cn[1] = green;
            vtx->v.cn[2] = blue;
        }
        vtx->v.cn[3] = alpha;
        vtx++;
    }
}

/**
 * Recursive search for vertices and set their colour values.
 */
void MakeWaterVertex(uintptr_t displayList, s8 alpha, u8 red, u8 green, u8 blue) {
    s32 segment = SEGMENT_NUMBER2(displayList);
    s32 offset = SEGMENT_OFFSET(displayList);
    Gfx* gfx = (Gfx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    uintptr_t lo;
    uintptr_t hi;
    s32 opcode;

    while (true) {
        lo = gfx->words.w0;
        hi = gfx->words.w1;
        opcode = GFX_GET_OPCODE(lo);
        if (opcode == (G_ENDDL << 24)) {
            break;
        } else if (opcode == (G_DL << 24)) {
            MakeWaterVertex(hi, alpha, red, green, blue);
        } else if (opcode == (G_VTX << 24)) {
            // G_VTX contains an addr hi
            water_vertex(hi, (lo >> 10) & 0x3F, ((lo >> 16) & 0xFF) >> 1, alpha, red, green, blue);
        }
        gfx++;
    }
}

void collect_tire_position(Vec3f pos1, f32 boundingBoxSize, Vec3f pos2) {
    pos2[0] -= pos1[0] * boundingBoxSize;
    pos2[1] -= pos1[1] * boundingBoxSize;
    pos2[2] -= pos1[2] * boundingBoxSize;
}

u16 check_tire_2(Player* player, KartTire* tire, f32 tire2X, f32 tire2Y, f32 tire2Z) {
    Collision wtf;
    Collision* collision = &wtf;
    UNUSED s32 pad;
    u16 i;
    u16 meshIndex;
    u16 numTriangles;
    u16 sectionIndex;
    f32 tireX;
    f32 tireY;
    f32 tireZ;
    f32 boundingBoxSize;
    f32 height;

    s32 courseLengthX;
    s32 courseLengthZ;

    s16 sectionIndexX;
    s16 sectionIndexZ;
    s16 gridIndex;

    s32 sectionX;
    s32 sectionZ;
    UNUSED s32 pad2[9];

    collision->distance_xy = 1000.0f;
    collision->distance_yz = 1000.0f;
    collision->distance_zx = 1000.0f;
    boundingBoxSize = player->radius;
    collision->last_xy = 5000;
    collision->last_yz = 5000;
    collision->last_zx = 5000;
    collision->flag_xy = 0;
    collision->flag_yz = 0;
    collision->flag_zx = 0;
    tireX = tire->Position[0];
    tireY = tire->Position[1];
    tireZ = tire->Position[2];
    switch (tire->LastAxis) {
        case 0x80:
            if (CheckTriangleYZ_V(collision, boundingBoxSize, tireX, tireY, tireZ, tire->LastPointer,
                                         tire2X, tire2Y, tire2Z) == 1) {
                height = CalcHeight(tireX, tireY, tireZ, tire->LastPointer);
                if ((!(height > player->position[1])) && !((player->position[1] - height) > (2 * boundingBoxSize))) {
                    tire->Height = height;
                    collect_tire_position(collision->bump_yz, collision->distance_yz, tire->Position);
                    return 1;
                }
            }
            break;
        case 0x40:
            if (CheckTriangleZX_V(collision, boundingBoxSize, tireX, tireY, tireZ,
                                                   tire->LastPointer, tire2X, tire2Y, tire2Z) == 1) {
                height = CalcHeight(tireX, tireY, tireZ, tire->LastPointer);
                if (!(player->position[1] < height) && !((2 * boundingBoxSize) < (player->position[1] - height))) {
                    tire->Height = height;
                    collect_tire_position(collision->bump_zx, collision->distance_zx, tire->Position);
                    return 1;
                }
            }
            break;
        case 0x20:
            if (CheckTriangleXY_V(collision, boundingBoxSize, tireX, tireY, tireZ, tire->LastPointer,
                                         tire2X, tire2Y, tire2Z) == 1) {
                height = CalcHeight(tireX, tireY, tireZ, tire->LastPointer);
                if (!(player->position[1] < height) && !((2 * boundingBoxSize) < (player->position[1] - height))) {
                    tire->Height = height;
                    collect_tire_position(collision->bump_xy, collision->distance_xy, tire->Position);
                    return 1;
                }
            }
            break;
        case 0:
            break;
    }

    // If the surface flags are not set then try setting them.

    courseLengthX = (s32) maximum_x - minimum_x;
    courseLengthZ = (s32) maximum_z - minimum_z;

    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (tireX - minimum_x) / sectionX;
    sectionIndexZ = (tireZ - minimum_z) / sectionZ;

    if (sectionIndexX < 0) {
        return 0;
    }
    if (sectionIndexZ < 0) {
        return 0;
    }
    if (sectionIndexX >= GRID_SIZE) {
        return 0;
    }
    if (sectionIndexZ >= GRID_SIZE) {
        return 0;
    }

    gridIndex = sectionIndexX + sectionIndexZ * GRID_SIZE;
    numTriangles = gCollisionGrid[gridIndex].numTriangles;

    if (numTriangles == 0) {
        return 0;
    }

    sectionIndex = gCollisionGrid[gridIndex].triangle;

    for (i = 0; i < numTriangles; i++) {
        meshIndex = collision_table[sectionIndex];
        if (gFaceBuffer[meshIndex].flag & FACING_Y_AXIS) {
            if (meshIndex != tire->LastPointer) {
                if (CheckTriangleZX_V(collision, boundingBoxSize, tireX, tireY, tireZ, meshIndex,
                                                       tire2X, tire2Y, tire2Z) == 1) {
                    height = CalcHeight(tireX, tireY, tireZ, meshIndex);

                    if (!(player->position[1] < height) && !((2 * boundingBoxSize) < (player->position[1] - height))) {
                        collect_tire_position(collision->bump_zx, collision->distance_zx, tire->Position);
                        tire->Height = height;
                        tire->Status = (u8) gFaceBuffer[meshIndex].status;
                        tire->LastAxis = 0x40;
                        tire->LastPointer = meshIndex;
                        if (gFaceBuffer[meshIndex].flag & 0x1000) {
                            tire->Dummy = 1;
                        } else {
                            tire->Dummy = 0;
                        }
                        return 1;
                    }
                }
            }
        } else if (gFaceBuffer[meshIndex].flag & FACING_X_AXIS) {
            if (gFaceBuffer[meshIndex].solb != 0.0f) {
                if (meshIndex != tire->LastPointer) {
                    if (CheckTriangleYZ_V(collision, boundingBoxSize, tireX, tireY, tireZ, meshIndex, tire2X,
                                                 tire2Y, tire2Z) == 1) {
                        height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                        if (!(player->position[1] < height) && !((2 * boundingBoxSize) < (player->position[1] - height))) {
                            tire->Height = height;
                            collect_tire_position(collision->bump_yz, collision->distance_yz, tire->Position);
                            tire->Height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                            tire->Status = (u8) gFaceBuffer[meshIndex].status;
                            tire->LastAxis = 0x80;
                            tire->LastPointer = meshIndex;
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (gFaceBuffer[meshIndex].solb != 0.0f) {
                if (meshIndex != tire->LastPointer) {
                    if (CheckTriangleXY_V(collision, boundingBoxSize, tireX, tireY, tireZ, meshIndex, tire2X,
                                                 tire2Y, tire2Z) == 1) {
                        height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                        if (!(player->position[1] < height) && !((2 * boundingBoxSize) < (player->position[1] - height))) {
                            tire->Height = height;
                            collect_tire_position(collision->bump_xy, collision->distance_xy, tire->Position);
                            tire->Status = (u8) gFaceBuffer[meshIndex].status;
                            tire->LastAxis = 0x20;
                            tire->LastPointer = meshIndex;
                            return 1;
                        }
                    }
                }
            }
        }
        sectionIndex++;
    }
    tire->Height = tireY;
    tire->Status = 0;
    return 0;
}

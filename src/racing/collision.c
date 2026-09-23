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
void KillDisplayList(uintptr_t Addr) {
    s32 segment = SEGMENT_NUMBER2(Addr);
    s32 Offset = SEGMENT_OFFSET(Addr);

    Gfx* macro;

    macro = (Gfx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    macro->words.w0 = (G_ENDDL << 24);
    macro->words.w1 = 0;
}

void InitialBump(Collision* collision) {
    collision->LastXY = 5000;
    collision->LastYZ = 5000;
    collision->LastZX = 5000;
    collision->FlagXY = 0;
    collision->FlagYZ = 0;
    collision->FlagZX = 0;
    collision->DistanceXY = 0;
    collision->DistanceYZ = 0;
    collision->DistanceZX = 0;
    Vec3FSet(collision->BumpXY, 0.0f, 0.0f, 1.0f);
    Vec3FSet(collision->BumpYZ, 1.0f, 0.0f, 0.0f);
    Vec3FSet(collision->BumpZX, 0.0f, 1.0f, 0.0f);
}

f32 CheckWaterLevel(Player* Player) {
    f32 playerX;
    f32 playerZ;
    s32 temp_v1;

    playerX = Player->Position[0];
    playerZ = Player->Position[2];
    switch (CourseID) {
        case COURSE_BOWSER_CASTLE:
            if (playerX > 1859.0f) {
                return WaterHeight;
            }
            if (playerX < 1549.0f) {
                return WaterHeight;
            }
            if (playerZ > -1102.0f) {
                return WaterHeight;
            }
            if (playerZ < -1402.0f) {
                return WaterHeight;
            }
            return 20.0f;
        case COURSE_KOOPA_BEACH:
            if (playerX > 239.0f) {
                return WaterHeight;
            }
            if (playerX < 67.0f) {
                return WaterHeight;
            }
            if (playerZ > 2405.0f) {
                return WaterHeight;
            }
            if (playerZ < 2233.0f) {
                return WaterHeight;
            }
            return 0.8f;
        case COURSE_SHERBET_LAND:
            if ((CheckStatus(Player->Bump.LastZX) & 0xFF) == SNOW) {
                return (f32) (MinimumY - 0xA);
            }
            return WaterHeight;
        case COURSE_DK_JUNGLE:
            temp_v1 = CheckArea(Player->Bump.LastZX) & 0xFF;
            if (temp_v1 == 0xFF) {
                if ((CheckStatus(Player->Bump.LastZX) & 0xFF) == CAVE) {
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
            return WaterHeight;
    }
}

s32 CheckTriangleZX(Collision* collision, f32 boundingBoxSize, f32 PosX, f32 PosY, f32 PosZ, u16 Index) {
    FaceStruct* Triangle = &FaceBuffer[Index];
    UNUSED f32 pad;
    f32 x3;
    UNUSED f32 Pad2;
    f32 z3;
    UNUSED f32 Pad3[4];
    f32 x2;
    f32 z2;
    f32 x1;
    f32 z1;
    f32 distanceToSurface;
    f32 crossProductZX_1;
    f32 crossProductZX_2;
    f32 crossProductZX_3;
    s32 b = true;

    if (Triangle->Solb < -0.9f) {
        return 0;
    }
    if (Triangle->Minx > PosX) {
        return 0;
    }
    if (Triangle->Minz > PosZ) {
        return 0;
    }
    if (Triangle->Maxx < PosX) {
        return 0;
    }
    if (Triangle->Maxz < PosZ) {
        return 0;
    }
    if ((Triangle->Miny - boundingBoxSize * 3.0f) > PosY) {
        return 0;
    }

    x1 = Triangle->p1->v.ob[0];
    z1 = Triangle->p1->v.ob[2];

    x2 = Triangle->p2->v.ob[0];
    z2 = Triangle->p2->v.ob[2];

    x3 = Triangle->p3->v.ob[0];
    z3 = Triangle->p3->v.ob[2];

    crossProductZX_1 = (z1 - PosZ) * (x2 - PosX) - (x1 - PosX) * (z2 - PosZ);

    if (!crossProductZX_1) {

        crossProductZX_2 = (z2 - PosZ) * (x3 - PosX) - (x2 - PosX) * (z3 - PosZ);

        crossProductZX_3 = (z3 - PosZ) * (x1 - PosX) - (x3 - PosX) * (z1 - PosZ);

        if ((crossProductZX_2 * crossProductZX_3) < 0.0f) {
            b = false;
        }
    } else {

        crossProductZX_2 = (z2 - PosZ) * (x3 - PosX) - (x2 - PosX) * (z3 - PosZ);

        if (!crossProductZX_2) {
            crossProductZX_3 = (z3 - PosZ) * (x1 - PosX) - (x3 - PosX) * (z1 - PosZ);

            if ((crossProductZX_1 * crossProductZX_3) < 0.0f) {
                b = false;
            }
        } else {
            if ((crossProductZX_1 * crossProductZX_2) < 0.0f) {
                b = false;
            } else {
                crossProductZX_3 = (z3 - PosZ) * (x1 - PosX) - (x3 - PosX) * (z1 - PosZ);
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
        ((Triangle->Sola * PosX) + (Triangle->Solb * PosY) + (Triangle->Solc * PosZ) + Triangle->Sold) -
        boundingBoxSize;
    if (distanceToSurface > 0.0f) {
        if (collision->DistanceZX > distanceToSurface) {
            collision->FlagZX = 1;
            collision->LastZX = Index;
            collision->DistanceZX = distanceToSurface;
            collision->BumpZX[0] = Triangle->Sola;
            collision->BumpZX[1] = Triangle->Solb;
            collision->BumpZX[2] = Triangle->Solc;
        }
        return 0;
    }

    if (distanceToSurface > -16.0f) {
        collision->FlagZX = 1;
        collision->LastZX = Index;
        collision->DistanceZX = distanceToSurface;
        collision->BumpZX[0] = Triangle->Sola;
        collision->BumpZX[1] = Triangle->Solb;
        collision->BumpZX[2] = Triangle->Solc;
        return 1;
    }
    return 0;
}

s32 CheckTriangleXY(Collision* collision, f32 boundingBoxSize, f32 PosX, f32 PosY, f32 PosZ, u16 Index) {
    FaceStruct* Triangle = &FaceBuffer[Index];
    UNUSED f32 pad[6];
    f32 x3;
    f32 y3;
    UNUSED f32 Pad2[1];

    UNUSED f32 Pad3[5];
    f32 x2;
    f32 y2;
    f32 x1;
    f32 y1;
    f32 distanceToSurface;
    f32 crossProductYX_1;
    f32 crossProductYX_2;
    f32 crossProductYX_3;
    s32 b = true;

    if (Triangle->Minx > PosX) {
        return 0;
    }
    if (Triangle->Maxx < PosX) {
        return 0;
    }
    if (Triangle->Maxy < PosY) {
        return 0;
    }
    if (Triangle->Miny > PosY) {
        return 0;
    }

    if ((Triangle->Minz - boundingBoxSize * 3.0f) > PosZ) {
        return 0;
    }
    if ((Triangle->Maxz + boundingBoxSize * 3.0f) < PosZ) {
        return 0;
    }

    x1 = Triangle->p1->v.ob[0];
    y1 = Triangle->p1->v.ob[1];

    x2 = Triangle->p2->v.ob[0];
    y2 = Triangle->p2->v.ob[1];

    x3 = Triangle->p3->v.ob[0];
    y3 = Triangle->p3->v.ob[1];

    crossProductYX_1 = (y1 - PosY) * (x2 - PosX) - (x1 - PosX) * (y2 - PosY);

    if (!crossProductYX_1) {

        crossProductYX_2 = (y2 - PosY) * (x3 - PosX) - (x2 - PosX) * (y3 - PosY);

        crossProductYX_3 = (y3 - PosY) * (x1 - PosX) - (x3 - PosX) * (y1 - PosY);

        if ((crossProductYX_2 * crossProductYX_3) < 0.0f) {
            b = false;
        }
    } else {
        crossProductYX_2 = (y2 - PosY) * (x3 - PosX) - (x2 - PosX) * (y3 - PosY);
        if (!crossProductYX_2) {
            crossProductYX_3 = (y3 - PosY) * (x1 - PosX) - (x3 - PosX) * (y1 - PosY);
            if (crossProductYX_1 * crossProductYX_3 < 0.0f) {
                b = false;
            }
        } else {
            if ((crossProductYX_1 * crossProductYX_2) < 0.0f) {
                b = false;
            } else {
                crossProductYX_3 = ((y3 - PosY) * (x1 - PosX)) - ((x3 - PosX) * (y1 - PosY));
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
        ((Triangle->Sola * PosX) + (Triangle->Solb * PosY) + (Triangle->Solc * PosZ) + Triangle->Sold) -
        boundingBoxSize;
    if (distanceToSurface > 0.0f) {
        if (distanceToSurface < collision->DistanceXY) {
            collision->FlagXY = 1;
            collision->LastXY = Index;
            collision->DistanceXY = distanceToSurface;
            collision->BumpXY[0] = Triangle->Sola;
            collision->BumpXY[1] = Triangle->Solb;
            collision->BumpXY[2] = Triangle->Solc;
        }
        return 0;
    }

    if (distanceToSurface > -16.0f) {
        collision->FlagXY = 1;
        collision->LastXY = Index;
        collision->DistanceXY = distanceToSurface;
        collision->BumpXY[0] = Triangle->Sola;
        collision->BumpXY[1] = Triangle->Solb;
        collision->BumpXY[2] = Triangle->Solc;
        return 1;
    }
    return 0;
}

s32 CheckTriangleYZ(Collision* collision, f32 boundingBoxSize, f32 PosX, f32 PosY, f32 PosZ, u16 Index) {
    FaceStruct* Triangle = &FaceBuffer[Index];
    s32 b = true;
    UNUSED f32 pad[7];
    f32 y3;
    f32 z3;
    UNUSED f32 Pad2[5];
    f32 y2;
    f32 z2;
    f32 y1;
    f32 z1;
    f32 distanceToSurface;
    f32 crossProductYZ_1;
    f32 crossProductYZ_2;
    f32 crossProductYZ_3;
    if (Triangle->Minz > PosZ) {
        return 0;
    }
    if (Triangle->Maxz < PosZ) {
        return 0;
    }
    if (Triangle->Maxy < PosY) {
        return 0;
    }
    if (Triangle->Miny > PosY) {
        return 0;
    }
    if ((Triangle->Minx - (boundingBoxSize * 3.0f)) > PosX) {
        return 0;
    }
    if ((Triangle->Maxx + (boundingBoxSize * 3.0f)) < PosX) {
        return 0;
    }

    z1 = Triangle->p1->v.ob[2];
    y1 = Triangle->p1->v.ob[1];

    z2 = Triangle->p2->v.ob[2];
    y2 = Triangle->p2->v.ob[1];

    z3 = Triangle->p3->v.ob[2];
    y3 = Triangle->p3->v.ob[1];

    crossProductYZ_1 = (y1 - PosY) * (z2 - PosZ) - (z1 - PosZ) * (y2 - PosY);

    if (!crossProductYZ_1) {

        crossProductYZ_2 = ((y2 - PosY) * (z3 - PosZ)) - ((z2 - PosZ) * (y3 - PosY));

        crossProductYZ_3 = ((y3 - PosY) * (z1 - PosZ)) - ((z3 - PosZ) * (y1 - PosY));

        if ((crossProductYZ_2 * crossProductYZ_3) < 0.0f) {
            b = false;
        }
    } else {

        crossProductYZ_2 = ((y2 - PosY) * (z3 - PosZ)) - ((z2 - PosZ) * (y3 - PosY));

        if (crossProductYZ_2 == 0) {
            crossProductYZ_3 = ((y3 - PosY) * (z1 - PosZ)) - ((z3 - PosZ) * (y1 - PosY));

            if ((crossProductYZ_1 * crossProductYZ_3) < 0.0f) {
                b = false;
            }
        } else {

            if ((crossProductYZ_1 * crossProductYZ_2) < 0.0f) {
                b = false;
            } else {
                crossProductYZ_3 = ((y3 - PosY) * (z1 - PosZ)) - ((z3 - PosZ) * (y1 - PosY));
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

    distanceToSurface = ((((Triangle->Sola * PosX) + (Triangle->Solb * PosY)) + (Triangle->Solc * PosZ)) +
                         Triangle->Sold) -
                        boundingBoxSize;

    if (distanceToSurface > 0.0f) {
        if (distanceToSurface < collision->DistanceYZ) {
            collision->FlagYZ = 1;
            collision->LastYZ = Index;
            collision->DistanceYZ = distanceToSurface;
            collision->BumpYZ[0] = Triangle->Sola;
            collision->BumpYZ[1] = Triangle->Solb;
            collision->BumpYZ[2] = Triangle->Solc;
        }
        return 0;
    }
    if (distanceToSurface > (-16.0f)) {
        collision->FlagYZ = 1;
        collision->LastYZ = Index;
        collision->DistanceYZ = distanceToSurface;
        collision->BumpYZ[0] = Triangle->Sola;
        collision->BumpYZ[1] = Triangle->Solb;
        collision->BumpYZ[2] = Triangle->Solc;
        return 1;
    }
    return 0;
}

s32 CheckTriangle(f32 PosX, f32 PosZ, u16 Index) {
    FaceStruct* Triangle = &FaceBuffer[Index];
    UNUSED f32 pad;
    f32 x3;
    UNUSED f32 Pad2;
    f32 z3;
    f32 x2;
    UNUSED f32 Pad3;
    f32 z2;
    f32 x1;
    f32 z1;
    UNUSED f32 pad4[4];
    f32 crossProductZX_1;
    f32 crossProductZX_3;
    f32 crossProductZX_2;
    s32 b = true;

    x1 = Triangle->p1->v.ob[0];
    z1 = Triangle->p1->v.ob[2];

    x2 = Triangle->p2->v.ob[0];
    z2 = Triangle->p2->v.ob[2];

    x3 = Triangle->p3->v.ob[0];
    z3 = Triangle->p3->v.ob[2];

    crossProductZX_1 = (z1 - PosZ) * (x2 - PosX) - (x1 - PosX) * (z2 - PosZ);

    if (!crossProductZX_1) {

        crossProductZX_2 = (z2 - PosZ) * (x3 - PosX) - (x2 - PosX) * (z3 - PosZ);

        crossProductZX_3 = (z3 - PosZ) * (x1 - PosX) - (x3 - PosX) * (z1 - PosZ);

        if ((crossProductZX_2 * crossProductZX_3) < 0.0f) {
            b = false;
        }
    } else {
        crossProductZX_2 = (z2 - PosZ) * (x3 - PosX) - (x2 - PosX) * (z3 - PosZ);
        if (!crossProductZX_2) {
            crossProductZX_3 = (z3 - PosZ) * (x1 - PosX) - (x3 - PosX) * (z1 - PosZ);
            if (crossProductZX_1 * crossProductZX_3 < 0.0f) {
                b = false;
            }
        } else {
            if ((crossProductZX_1 * crossProductZX_2) < 0.0f) {
                b = false;
            } else {
                crossProductZX_3 = ((z3 - PosZ) * (x1 - PosX)) - ((x3 - PosX) * (z1 - PosZ));
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

s8 CheckStatus(u16 Index) {
    FaceStruct* Triangle = &FaceBuffer[Index];
    return Triangle->Status;
}

s16 CheckArea(u16 Index) {
    FaceStruct* Triangle = &FaceBuffer[Index];
    return Triangle->Flag & 0xFF;
}

s16 CheckShadow(u16 Index) {
    FaceStruct* Triangle = &FaceBuffer[Index];
    return Triangle->Flag & 0x1000;
}

s16 CheckSlope(u16 Index) {
    FaceStruct* Triangle = &FaceBuffer[Index];
    return Triangle->Flag & 0x400;
}

s16 OoBCheck(u16 Index) {
    FaceStruct* Triangle = &FaceBuffer[Index];
    return Triangle->Flag & 0x800;
}

f32 CalcHeight(f32 x, f32 y, f32 z, u16 Index) {
    FaceStruct* Triangle = &FaceBuffer[Index];
    if (Triangle->Solb == 0.0f) {
        return y;
    }
    return ((Triangle->Sola * x) + (Triangle->Solc * z) + Triangle->Sold) / -Triangle->Solb;
}

f32 GetHight(Collision* collision, Vec3f pos) {
    if (collision->FlagZX == 1) {
        return CalcHeight(pos[0], pos[1], pos[2], collision->LastZX);
    }
    if (collision->FlagXY == 1) {
        return CalcHeight(pos[0], pos[1], pos[2], collision->LastXY);
    }
    if (collision->FlagYZ == 1) {
        return CalcHeight(pos[0], pos[1], pos[2], collision->LastYZ);
    }
    return pos[1];
}

void BumpRoutin(Vec3f pos, UNUSED f32 boundingBoxSize, Vec3f Velocity, UNUSED f32 unk) {
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

    x = Velocity[0];
    y = Velocity[1];
    z = Velocity[2];
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

    Velocity[0] = x * scaleFactor;
    Velocity[1] = y * scaleFactor;
    Velocity[2] = z * scaleFactor;
}

void CalcBumpVelocity(Collision* collision, Vec3f Velocity) {
    if (collision->DistanceXY < 0.0f) {
        BumpRoutin(collision->BumpXY, collision->DistanceXY, Velocity, 2.0f);
    }

    if (collision->DistanceYZ < 0.0f) {
        BumpRoutin(collision->BumpYZ, collision->DistanceYZ, Velocity, 2.0f);
    }
}

/**
 * Adjusts the Position of pos2 based on pos1 but in the orthogonal Direction to pos2.
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

UNUSED s32 CheckTire(KartTire* tire) {
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
    u16 NumTriangles;
    u16 meshIndex;
    s16 gridIndex;
    u16 sectionIndex;

    collision.FlagXY = 0;
    collision.FlagYZ = 0;
    collision.FlagZX = 0;
    collision.DistanceXY = 1000.0f;
    collision.DistanceYZ = 1000.0f;
    collision.DistanceZX = 1000.0f;
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
    courseLengthX = MaximumX - MinimumX;
    courseLengthZ = MaximumZ - MinimumZ;
    sectionIndexX = (tireX - MinimumX) / (courseLengthX / GRID_SIZE);
    sectionIndexZ = (tireZ - MinimumZ) / (courseLengthZ / GRID_SIZE);
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
    NumTriangles = CollisionGridTable[gridIndex].NumTriangles;
    if (NumTriangles == 0) {
        return 0;
    }
    sectionIndex = CollisionGridTable[gridIndex].Triangle;
    for (i = 0; i < NumTriangles; i++) {
        meshIndex = CollisionTable[sectionIndex];
        if (FaceBuffer[meshIndex].Flag & FACING_Y_AXIS) {
            if (meshIndex != tire->LastPointer) {
                if (CheckTriangleZX(&collision, 5.0f, tireX, tireY, tireZ, meshIndex) == 1) {
                    tire->Height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                    tire->Status = FaceBuffer[meshIndex].Status;
                    tire->LastAxis = 0x40;
                    tire->LastPointer = meshIndex;
                    if (FaceBuffer[meshIndex].Flag & 0x1000) {
                        tire->Dummy = 1;
                    } else {
                        tire->Dummy = 0;
                    }
                    return 1;
                }
            }
        } else if (FaceBuffer[meshIndex].Flag & FACING_X_AXIS) {
            if ((FaceBuffer[meshIndex].Sola != 1.0f) && (meshIndex != tire->LastPointer)) {
                if (CheckTriangleYZ(&collision, 5.0f, tireX, tireY, tireZ, meshIndex) == 1) {
                    tire->Height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                    tire->Status = FaceBuffer[meshIndex].Status;
                    tire->LastAxis = 0x80;
                    tire->LastPointer = meshIndex;
                    return 1;
                }
            }
        } else if ((FaceBuffer[meshIndex].Solc != 1.0f) && (meshIndex != tire->LastPointer)) {
            if (CheckTriangleXY(&collision, 5.0f, tireX, tireY, tireZ, meshIndex) == 1) {
                tire->Height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                tire->Status = FaceBuffer[meshIndex].Status;
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
    // Another function that has a return Value but doesn't have an explicit return statement in one of its codepaths.
    // The return Value at this Point will be whatever was last returned by func_802AAE4C/func_802AB6C4/func_802AB288
    // depending on which (if any) if statements were entered on the Loop's last cycle
}

s32 CheckTriangleZXV(Collision* collision, f32 boundingBoxSize, f32 newX, f32 newY, f32 newZ,
                                       u16 Index, f32 oldX, f32 oldY, f32 oldZ) {
    FaceStruct* Triangle = &FaceBuffer[Index];
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
    UNUSED s32 Pad2[2];
    f32 Area;
    f32 area2;
    f32 area3;
    s32 b = 1;

    if (Triangle->Minx > newX) {
        return 0;
    }
    if (Triangle->Minz > newZ) {
        return 0;
    }
    if (Triangle->Maxx < newX) {
        return 0;
    }
    if (Triangle->Maxz < newZ) {
        return 0;
    }
    if ((Triangle->Miny - boundingBoxSize * 3.0f) > newY) {
        return 0;
    }

    x2 = (f32) Triangle->p1->v.ob[0];
    z2 = (f32) Triangle->p1->v.ob[2];

    x3 = (f32) Triangle->p2->v.ob[0];
    z3 = (f32) Triangle->p2->v.ob[2];

    x4 = (f32) Triangle->p3->v.ob[0];
    z4 = (f32) Triangle->p3->v.ob[2];

    Area = (z2 - newZ) * (x3 - newX) - (x2 - newX) * (z3 - newZ);

    if (Area == 0) {
        area2 = (z3 - newZ) * (x4 - newX) - (x3 - newX) * (z4 - newZ);
        area3 = (z4 - newZ) * (x2 - newX) - (x4 - newX) * (z2 - newZ);
        if (area2 * area3 < 0.0f) {
            b = 0;
        }
    } else {

        area2 = (z3 - newZ) * (x4 - newX) - (x3 - newX) * (z4 - newZ);

        if (area2 == 0) {

            area3 = (z4 - newZ) * (x2 - newX) - (x4 - newX) * (z2 - newZ);

            if (Area * area3 < 0.0f) {
                b = 0;
            }
        } else {
            if ((Area * area2) < 0.0f) {
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
        (Triangle->Sola * newX) + (Triangle->Solb * newY) + (Triangle->Solc * newZ) + Triangle->Sold;

    if (surfaceDistance > boundingBoxSize) {
        if (collision->DistanceZX > surfaceDistance) {
            collision->FlagZX = 1;
            collision->LastZX = Index;
            collision->DistanceZX = surfaceDistance - boundingBoxSize;
            collision->BumpZX[0] = Triangle->Sola;
            collision->BumpZX[1] = Triangle->Solb;
            collision->BumpZX[2] = Triangle->Solc;
        }
        return 0;
    }

    temp = (Triangle->Sola * oldX) + (Triangle->Solb * oldY) + (Triangle->Solc * oldZ) + Triangle->Sold;

    if (temp < 0.0f) {
        return 0;
    }

    collision->FlagZX = 1;
    collision->LastZX = Index;
    collision->DistanceZX = surfaceDistance - boundingBoxSize;
    collision->BumpZX[0] = Triangle->Sola;
    collision->BumpZX[1] = Triangle->Solb;
    collision->BumpZX[2] = Triangle->Solc;
    return 1;
}

/**
 * Wall collision
 */
s32 CheckTriangleXYV(Collision* arg, f32 boundingBoxSize, f32 x1, f32 y1, f32 z1, u16 surfaceIndex, f32 PosX,
                             f32 PosY, f32 PosZ) {
    FaceStruct* Triangle = &FaceBuffer[surfaceIndex];
    UNUSED s32 pad[6];
    f32 x4;
    f32 y4;
    f32 x3;
    f32 y3;
    f32 x2;
    f32 y2;
    UNUSED s32 Pad2[3];
    f32 distanceToSurface;
    f32 distanceToSurface2;
    UNUSED s32 Pad3[2];
    f32 Area;
    f32 area2;
    f32 area3;
    s32 b = true;
    if (Triangle->Minx > x1) {
        return NO_COLLISION;
    }
    if (Triangle->Maxx < x1) {
        return NO_COLLISION;
    }
    if (Triangle->Maxy < y1) {
        return NO_COLLISION;
    }
    if (Triangle->Miny > y1) {
        return NO_COLLISION;
    }
    if ((Triangle->Minz - boundingBoxSize * 3.0f) > z1) {
        return NO_COLLISION;
    }
    if ((Triangle->Maxz + boundingBoxSize * 3.0f) < z1) {
        return NO_COLLISION;
    }

    x2 = (f32) Triangle->p1->v.ob[0];
    y2 = (f32) Triangle->p1->v.ob[1];

    x3 = (f32) Triangle->p2->v.ob[0];
    y3 = (f32) Triangle->p2->v.ob[1];

    x4 = (f32) Triangle->p3->v.ob[0];
    y4 = (f32) Triangle->p3->v.ob[1];

    Area = (y2 - y1) * (x3 - x1) - (x2 - x1) * (y3 - y1);

    if (Area == 0) {
        area2 = (y3 - y1) * (x4 - x1) - (x3 - x1) * (y4 - y1);
        area3 = (y4 - y1) * (x2 - x1) - (x4 - x1) * (y2 - y1);

        if (area2 * area3 < 0.0f) {
            b = false;
        }
    } else {

        area2 = (y3 - y1) * (x4 - x1) - (x3 - x1) * (y4 - y1);

        if (area2 == 0) {
            area3 = (y4 - y1) * (x2 - x1) - (x4 - x1) * (y2 - y1);

            if ((Area * area3) < 0.0f) {
                b = false;
            }
        } else {
            if ((Area * area2) < 0.0f) {
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
        ((Triangle->Sola * x1) + (Triangle->Solb * y1) + (Triangle->Solc * z1)) + Triangle->Sold;
    if (Triangle->Flag & 0x200) {
        distanceToSurface2 =
            ((Triangle->Sola * PosX) + (Triangle->Solb * PosY) + (Triangle->Solc * PosZ)) + Triangle->Sold;
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 > 0.0f)) {
            if (distanceToSurface < boundingBoxSize) {
                arg->FlagXY = 1;
                arg->LastXY = surfaceIndex;
                arg->DistanceXY = distanceToSurface - boundingBoxSize;
                arg->BumpXY[0] = Triangle->Sola;
                arg->BumpXY[1] = Triangle->Solb;
                arg->BumpXY[2] = Triangle->Solc;
                return COLLISION;
            }
            return NO_COLLISION;
        }

        if ((distanceToSurface < 0.0f) && (distanceToSurface2 < 0.0f)) {
            distanceToSurface *= -1.0f;
            if (distanceToSurface < boundingBoxSize) {
                arg->FlagXY = 1;
                arg->LastXY = surfaceIndex;
                arg->DistanceXY = distanceToSurface - boundingBoxSize;
                arg->BumpXY[0] = -Triangle->Sola;
                arg->BumpXY[1] = -Triangle->Solb;
                arg->BumpXY[2] = -Triangle->Solc;
                return 1;
            }
            return NO_COLLISION;
        }
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 < 0.0f)) {
            arg->FlagXY = 1;
            arg->LastXY = surfaceIndex;
            arg->DistanceXY = -(distanceToSurface + boundingBoxSize);
            arg->BumpXY[0] = -Triangle->Sola;
            arg->BumpXY[1] = -Triangle->Solb;
            arg->BumpXY[2] = -Triangle->Solc;
            return COLLISION;
        }
        if ((distanceToSurface < 0.0f) && (distanceToSurface2 > 0.0f)) {
            arg->FlagXY = 1;
            arg->LastXY = surfaceIndex;
            arg->DistanceXY = distanceToSurface + boundingBoxSize;
            arg->BumpXY[0] = Triangle->Sola;
            arg->BumpXY[1] = Triangle->Solb;
            arg->BumpXY[2] = Triangle->Solc;
            return COLLISION;
        }
        if (distanceToSurface == 0.0f) {
            if (distanceToSurface2 >= 0.0f) {
                arg->FlagXY = 1;
                arg->LastXY = surfaceIndex;
                arg->DistanceXY = distanceToSurface2 + boundingBoxSize;
                arg->BumpXY[0] = Triangle->Sola;
                arg->BumpXY[1] = Triangle->Solb;
                arg->BumpXY[2] = Triangle->Solc;
                return COLLISION;
            }
            arg->FlagXY = 1;
            arg->LastXY = surfaceIndex;
            arg->DistanceXY = -(distanceToSurface2 + boundingBoxSize);
            arg->BumpXY[0] = Triangle->Sola;
            arg->BumpXY[1] = Triangle->Solb;
            arg->BumpXY[2] = Triangle->Solc;
            return COLLISION;
        }
        return NO_COLLISION;
    }
    if (distanceToSurface > boundingBoxSize) {
        if (distanceToSurface < arg->DistanceXY) {
            arg->FlagXY = 1;
            arg->LastXY = surfaceIndex;
            arg->DistanceXY = distanceToSurface - boundingBoxSize;
            arg->BumpXY[0] = Triangle->Sola;
            arg->BumpXY[1] = Triangle->Solb;
            arg->BumpXY[2] = Triangle->Solc;
        }
        return NO_COLLISION;
    }

    distanceToSurface2 =
        (Triangle->Sola * PosX) + (Triangle->Solb * PosY) + (Triangle->Solc * PosZ) + Triangle->Sold;
    if (distanceToSurface2 < 0.0f) {
        return NO_COLLISION;
    }
    arg->FlagXY = 1;
    arg->LastXY = surfaceIndex;
    arg->DistanceXY = distanceToSurface - boundingBoxSize;
    arg->BumpXY[0] = Triangle->Sola;
    arg->BumpXY[1] = Triangle->Solb;
    arg->BumpXY[2] = Triangle->Solc;
    return COLLISION;
}

/**
 * This is actually more like colliding with face X/Y/Z
 */
s32 CheckTriangleYZV(Collision* arg, f32 boundingBoxSize, f32 x1, f32 y1, f32 z1, u16 surfaceIndex, f32 PosX,
                             f32 PosY, f32 PosZ) {
    FaceStruct* Triangle = &FaceBuffer[surfaceIndex];
    s32 b = 1;
    UNUSED s32 pad[7];
    f32 y4;
    f32 z4;
    f32 y3;
    f32 z3;
    f32 y2;
    f32 z2;
    UNUSED s32 Pad3[2];
    f32 distanceToSurface;
    f32 distanceToSurface2;
    UNUSED s32 pad4[2];
    f32 Area;
    f32 area2;
    f32 area3;

    if (Triangle->Minz > z1) {
        return NO_COLLISION;
    }
    if (Triangle->Maxz < z1) {
        return NO_COLLISION;
    }
    if (Triangle->Maxy < y1) {
        return NO_COLLISION;
    }
    if (Triangle->Miny > y1) {
        return NO_COLLISION;
    }
    if ((Triangle->Minx - boundingBoxSize * 3.0f) > x1) {
        return NO_COLLISION;
    }
    if ((Triangle->Maxx + boundingBoxSize * 3.0f) < x1) {
        return NO_COLLISION;
    }

    z2 = (f32) Triangle->p1->v.ob[2];
    y2 = (f32) Triangle->p1->v.ob[1];

    z3 = (f32) Triangle->p2->v.ob[2];
    y3 = (f32) Triangle->p2->v.ob[1];

    z4 = (f32) Triangle->p3->v.ob[2];
    y4 = (f32) Triangle->p3->v.ob[1];

    Area = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);

    if (Area == 0) {
        area2 = (y3 - y1) * (z4 - z1) - (z3 - z1) * (y4 - y1);
        area3 = (y4 - y1) * (z2 - z1) - (z4 - z1) * (y2 - y1);

        if (area2 * area3 < 0.0f) {
            b = 0;
        }
    } else {

        area2 = (y3 - y1) * (z4 - z1) - (z3 - z1) * (y4 - y1);

        if (area2 == 0) {
            area3 = (y4 - y1) * (z2 - z1) - (z4 - z1) * (y2 - y1);

            if ((Area * area3) < 0.0f) {
                b = 0;
            }
        } else {
            if ((Area * area2) < 0.0f) {
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
        ((Triangle->Sola * x1) + (Triangle->Solb * y1) + (Triangle->Solc * z1)) + Triangle->Sold;
    if (Triangle->Flag & 0x200) {
        distanceToSurface2 =
            ((Triangle->Sola * PosX) + (Triangle->Solb * PosY) + (Triangle->Solc * PosZ)) + Triangle->Sold;
        // sp48 = temp_f4_2;
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 > 0.0f)) {
            if (distanceToSurface < boundingBoxSize) {
                arg->FlagYZ = 1;
                arg->LastYZ = surfaceIndex;
                arg->DistanceYZ = distanceToSurface - boundingBoxSize;
                arg->BumpYZ[0] = Triangle->Sola;
                arg->BumpYZ[1] = Triangle->Solb;
                arg->BumpYZ[2] = Triangle->Solc;
                return COLLISION;
            }
            return NO_COLLISION;
        }

        if ((distanceToSurface < 0.0f) && (distanceToSurface2 < 0.0f)) {
            distanceToSurface *= -1.0f;
            if (distanceToSurface < boundingBoxSize) {
                arg->FlagYZ = 1;
                arg->LastYZ = surfaceIndex;
                arg->DistanceYZ = distanceToSurface - boundingBoxSize;
                arg->BumpYZ[0] = -Triangle->Sola;
                arg->BumpYZ[1] = -Triangle->Solb;
                arg->BumpYZ[2] = -Triangle->Solc;
                return 1;
            }
            return NO_COLLISION;
        }
        if ((distanceToSurface > 0.0f) && (distanceToSurface2 < 0.0f)) {
            arg->FlagYZ = 1;
            arg->LastYZ = surfaceIndex;
            arg->DistanceYZ = -(distanceToSurface + boundingBoxSize);
            arg->BumpYZ[0] = -Triangle->Sola;
            arg->BumpYZ[1] = -Triangle->Solb;
            arg->BumpYZ[2] = -Triangle->Solc;
            return COLLISION;
        }
        if ((distanceToSurface < 0.0f) && (distanceToSurface2 > 0.0f)) {
            arg->FlagYZ = 1;
            arg->LastYZ = surfaceIndex;
            arg->DistanceYZ = distanceToSurface + boundingBoxSize;
            arg->BumpYZ[0] = Triangle->Sola;
            arg->BumpYZ[1] = Triangle->Solb;
            arg->BumpYZ[2] = Triangle->Solc;
            return COLLISION;
        }
        if (distanceToSurface == 0.0f) {
            if (distanceToSurface2 >= 0.0f) {
                arg->FlagYZ = 1;
                arg->LastYZ = surfaceIndex;
                arg->DistanceYZ = distanceToSurface2 + boundingBoxSize;
                arg->BumpYZ[0] = Triangle->Sola;
                arg->BumpYZ[1] = Triangle->Solb;
                arg->BumpYZ[2] = Triangle->Solc;
                return COLLISION;
            }
            arg->FlagYZ = 1;
            arg->LastYZ = surfaceIndex;
            arg->DistanceYZ = -(distanceToSurface2 + boundingBoxSize);
            arg->BumpYZ[0] = Triangle->Sola;
            arg->BumpYZ[1] = Triangle->Solb;
            arg->BumpYZ[2] = Triangle->Solc;
            return COLLISION;
        }
        return NO_COLLISION;
    }
    if (distanceToSurface > boundingBoxSize) {
        if (arg->DistanceYZ > distanceToSurface) {
            arg->FlagYZ = 1;
            arg->LastYZ = surfaceIndex;
            arg->DistanceYZ = distanceToSurface - boundingBoxSize;
            arg->BumpYZ[0] = Triangle->Sola;
            arg->BumpYZ[1] = Triangle->Solb;
            arg->BumpYZ[2] = Triangle->Solc;
        }
        return NO_COLLISION;
    }

    distanceToSurface2 =
        (Triangle->Sola * PosX) + (Triangle->Solb * PosY) + (Triangle->Solc * PosZ) + Triangle->Sold;
    if (distanceToSurface2 < 0.0f) {
        return NO_COLLISION;
    }
    arg->FlagYZ = 1;
    arg->LastYZ = surfaceIndex;
    arg->DistanceYZ = distanceToSurface - boundingBoxSize;
    arg->BumpYZ[0] = Triangle->Sola;
    arg->BumpYZ[1] = Triangle->Solb;
    arg->BumpYZ[2] = Triangle->Solc;
    return COLLISION;
}

u16 CheckBump2(Collision* collision, f32 boundingBoxSize, f32 newX, f32 newY, f32 newZ, f32 oldX, f32 oldY,
                            f32 oldZ) {
    s32 courseLengthX;
    s32 courseLengthZ;
    s16 sectionIndexX;
    s16 sectionIndexZ;
    u16 NumTriangles;
    u16 collisionIndex;
    s16 gridIndex;

    u16 sectionIndex;

    u16 flags = 0;
    s32 sectionX;
    s32 sectionZ;

    u16 i;

    collision->FlagXY = 0;
    collision->FlagYZ = 0;
    collision->FlagZX = 0;
    collision->DistanceXY = 1000.0f;
    collision->DistanceYZ = 1000.0f;
    collision->DistanceZX = 1000.0f;

    // Check if an intersection test has already been performed.
    if ((s32) collision->LastZX < (s32) CourseFaceMaxIndex) {
        if (CheckTriangleZXV(collision, boundingBoxSize, newX, newY, newZ, collision->LastZX,
                                               oldX, oldY, oldZ) == COLLISION) {
            flags |= FACING_Y_AXIS;
        }
    }

    if ((s32) collision->LastXY < (s32) CourseFaceMaxIndex) {
        if (CheckTriangleXYV(collision, boundingBoxSize, newX, newY, newZ, collision->LastXY, oldX, oldY,
                                     oldZ) == COLLISION) {
            flags |= FACING_Z_AXIS;
        }
    }

    if ((s32) collision->LastYZ < (s32) CourseFaceMaxIndex) {
        if (CheckTriangleYZV(collision, boundingBoxSize, newX, newY, newZ, collision->LastYZ, oldX, oldY,
                                     oldZ) == COLLISION) {
            flags |= FACING_X_AXIS;
        }
    }

    if (flags == (FACING_Y_AXIS | FACING_Z_AXIS | FACING_X_AXIS)) {
        return flags;
    }

    courseLengthX = (s32) MaximumX - MinimumX;
    courseLengthZ = (s32) MaximumZ - MinimumZ;

    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (newX - MinimumX) / sectionX;
    sectionIndexZ = (newZ - MinimumZ) / sectionZ;

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
    NumTriangles = CollisionGridTable[gridIndex].NumTriangles;

    if (NumTriangles == 0) {
        return flags;
    }

    sectionIndex = CollisionGridTable[gridIndex].Triangle;

    for (i = 0; i < NumTriangles; i++) {
        if (flags == (FACING_Y_AXIS | FACING_Z_AXIS | FACING_X_AXIS)) {
            return flags;
        }

        collisionIndex = CollisionTable[sectionIndex];

        if ((FaceBuffer[collisionIndex].Flag & FACING_Y_AXIS)) {
            if ((flags & FACING_Y_AXIS) == 0) {
                if (collisionIndex != collision->LastZX) {
                    if (CheckTriangleZXV(collision, boundingBoxSize, newX, newY, newZ, collisionIndex,
                                                           oldX, oldY, oldZ) == COLLISION) {
                        flags |= FACING_Y_AXIS;
                    }
                }
            }
        } else if ((FaceBuffer[collisionIndex].Flag & FACING_X_AXIS) != 0) {
            if ((flags & FACING_X_AXIS) == 0) {
                if (collisionIndex != collision->LastYZ) {
                    if (CheckTriangleYZV(collision, boundingBoxSize, newX, newY, newZ, collisionIndex, oldX,
                                                 oldY, oldZ) == COLLISION) {
                        flags |= FACING_X_AXIS;
                    }
                }
            }
        } else if ((flags & FACING_Z_AXIS) == 0) {
            if (collisionIndex != collision->LastXY) {
                if (CheckTriangleXYV(collision, boundingBoxSize, newX, newY, newZ, collisionIndex, oldX, oldY,
                                             oldZ) == COLLISION) {
                    flags |= FACING_Z_AXIS;
                }
            }
        }
        sectionIndex++;
    }
    return flags;
}

u16 CheckBump(Collision* collision, f32 boundingBoxSize, f32 PosX, f32 PosY, f32 PosZ) {
    u16 NumTriangles;
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

    collision->FlagXY = 0;
    collision->FlagYZ = 0;
    collision->FlagZX = 0;
    collision->DistanceXY = 1000.0f;
    collision->DistanceYZ = 1000.0f;
    collision->DistanceZX = 1000.0f;
    flags = 0;
    if (collision->LastZX < CourseFaceMaxIndex) {
        if (CheckTriangleZX(collision, boundingBoxSize, PosX, PosY, PosZ, collision->LastZX) == 1) {
            flags |= FACING_Y_AXIS;
        }
    }
    if (collision->LastXY < CourseFaceMaxIndex) {
        if (CheckTriangleXY(collision, boundingBoxSize, PosX, PosY, PosZ, collision->LastXY) == 1) {
            flags |= FACING_Z_AXIS;
        }
    }
    if (collision->LastYZ < CourseFaceMaxIndex) {
        if (CheckTriangleYZ(collision, boundingBoxSize, PosX, PosY, PosZ, collision->LastYZ) == 1) {
            flags |= FACING_X_AXIS;
        }
    }
    if (flags == (FACING_Y_AXIS | FACING_Z_AXIS | FACING_X_AXIS)) {
        return flags;
    }

    courseLengthX = (s32) MaximumX - MinimumX;
    courseLengthZ = (s32) MaximumZ - MinimumZ;

    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (PosX - MinimumX) / sectionX;
    sectionIndexZ = (PosZ - MinimumZ) / sectionZ;

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
    NumTriangles = CollisionGridTable[gridIndex].NumTriangles;
    if (NumTriangles == 0) {
        return flags;
    }
    sectionIndex = CollisionGridTable[gridIndex].Triangle;

    for (i = 0; i < NumTriangles; i++) {
        if (flags == (FACING_X_AXIS | FACING_Y_AXIS | FACING_Z_AXIS)) {
            return flags;
        }
        meshIndex = CollisionTable[sectionIndex];
        if (FaceBuffer[meshIndex].Flag & FACING_Y_AXIS) {
            if (!(flags & FACING_Y_AXIS)) {
                if (meshIndex != collision->LastZX) {
                    if (CheckTriangleZX(collision, boundingBoxSize, PosX, PosY, PosZ, meshIndex) == 1) {
                        flags |= FACING_Y_AXIS;
                    }
                }
            }
        } else if (FaceBuffer[meshIndex].Flag & FACING_X_AXIS) {
            if (!(flags & FACING_X_AXIS)) {
                if (meshIndex != collision->LastYZ) {
                    if (CheckTriangleYZ(collision, boundingBoxSize, PosX, PosY, PosZ, meshIndex) == 1) {
                        flags |= FACING_X_AXIS;
                    }
                }
            }
        } else {
            if (!(flags & FACING_Z_AXIS)) {
                if (meshIndex != collision->LastXY) {
                    if (CheckTriangleXY(collision, boundingBoxSize, PosX, PosY, PosZ, meshIndex) == 1) {
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
 * @brief Returns the Height of the surface below the provided Position. Used to set actors onto the course mesh.
 * If unable to spawn actor on the surface set to -3000.0f or
 * if outside the collision grid, spawn in the air (3000.0f).
 */
f32 CheckHight(f32 PosX, f32 PosY, f32 PosZ) {
    f32 Height;
    s16 sectionIndexX;
    s16 sectionIndexZ;
    s16 gridSection;

    u16 Index;
    u16 NumTriangles;
    u16 sectionIndex;
    f32 phi_f20 = -3000.0f;
    u16 i;

    s32 courseLengthX;
    s32 courseLengthZ;
    s32 sectionX;
    s32 sectionZ;

    courseLengthX = (MaximumX - MinimumX);
    courseLengthZ = (MaximumZ - MinimumZ);
    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (s16) ((PosX - MinimumX) / sectionX);
    sectionIndexZ = (s16) ((PosZ - MinimumZ) / sectionZ);
    gridSection = sectionIndexX + (sectionIndexZ * GRID_SIZE);
    NumTriangles = CollisionGridTable[gridSection].NumTriangles;

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
    if (NumTriangles == 0) {
        return 3000.0f;
    }

    sectionIndex = CollisionGridTable[gridSection].Triangle;

    for (i = 0; i < NumTriangles; i++) {

        Index = CollisionTable[sectionIndex];

        if ((FaceBuffer[Index].Flag & FACING_Y_AXIS) &&
            (CheckTriangle(PosX, PosZ, Index) == 1)) {
            Height = CalcHeight(PosX, PosY, PosZ, Index);
            if ((Height <= PosY) && (phi_f20 < Height)) {
                phi_f20 = Height;
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

void SaveTriangle(Vtx* vtx1, Vtx* vtx2, Vtx* vtx3, s8 surfaceType, u16 sectionId) {
    FaceStruct* Triangle = &FaceBuffer[CourseFaceMaxIndex];
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
    UNUSED s32 Pad2[7];

    f64 crossProductX;
    f64 crossProductY;
    f64 crossProductZ;
    f64 magnitude;

    UNUSED s32 Pad3[3];

    f32 normalX;
    f32 normalY;
    f32 normalZ;
    f32 Distance;

    s16 maxX;
    s16 maxZ;
    s16 minY;
    s16 minX;
    s16 maxY;
    s16 minZ;

    Triangle->p1 = vtx1;
    Triangle->p2 = vtx2;
    Triangle->p3 = vtx3;
    if ((Triangle->p1->v.flag == 4) && (Triangle->p2->v.flag == 4) && (Triangle->p3->v.flag == 4)) {

        return;
    }

    x1 = Triangle->p1->v.ob[0];
    y1 = Triangle->p1->v.ob[1];
    z1 = Triangle->p1->v.ob[2];
    x2 = Triangle->p2->v.ob[0];
    y2 = Triangle->p2->v.ob[1];
    z2 = Triangle->p2->v.ob[2];
    x3 = Triangle->p3->v.ob[0];
    y3 = Triangle->p3->v.ob[1];
    z3 = Triangle->p3->v.ob[2];
    if ((x1 == x2) && (z1 == z2)) {
        Triangle->p1 = vtx1;
        Triangle->p3 = vtx2;
        Triangle->p2 = vtx3;
        x1 = Triangle->p1->v.ob[0];
        y1 = Triangle->p1->v.ob[1];
        z1 = Triangle->p1->v.ob[2];
        x2 = Triangle->p3->v.ob[0];
        y2 = Triangle->p3->v.ob[1];
        z2 = Triangle->p3->v.ob[2];
        x3 = Triangle->p2->v.ob[0];
        y3 = Triangle->p2->v.ob[1];
        z3 = Triangle->p2->v.ob[2];
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

    // Length of the cross product
    magnitude =
        sqrtf((crossProductX * crossProductX) + (crossProductY * crossProductY) + (crossProductZ * crossProductZ));

    if (!magnitude) {
        return;
    }

    normalX = (f32) crossProductX / magnitude;
    normalY = (f32) crossProductY / magnitude;
    normalZ = (f32) crossProductZ / magnitude;

    // Distance from x to plane (cross product's normal).
    Distance = -((normalX * x1) + (normalY * y1) + (normalZ * z1));

    // Return if normalY is not vertical.
    // Could be checking if the surface is a floor
    if (WallFlag) {
        if (normalY < -0.9f) {
            return;
        } else if (normalY > 0.9f) {
            return;
        }
    }

    // Return if normalY is not horizontal
    // Could be checking if the surface is a wall
    if (FlatFlag) {
        if ((normalY < 0.1f) && (normalY > -0.1f)) {
            return;
        }
    }

    Triangle->Maxx = maxX;
    Triangle->Maxz = maxZ;
    Triangle->Minx = minX;
    Triangle->Minz = minZ;
    Triangle->Miny = minY;
    Triangle->Maxy = maxY;

    // Define the minimum and maximum dimensions of the course.
    if (minX < MinimumX) {
        MinimumX = minX;
    }
    if (minY < MinimumY) {
        MinimumY = minY;
    }
    if (minZ < MinimumZ) {
        MinimumZ = minZ;
    }
    if (maxX > MaximumX) {
        MaximumX = maxX;
    }
    if (maxY > MaximumY) {
        MaximumY = maxY;
    }
    if (maxZ > MaximumZ) {
        MaximumZ = maxZ;
    }

    Triangle->Sola = normalX;
    Triangle->Solb = normalY;
    Triangle->Solc = normalZ;
    Triangle->Sold = Distance;

    Triangle->Status = (u16) surfaceType;

    // Square the crossProduct to produce a magnitude
    crossProductX = crossProductX * crossProductX;
    crossProductY = crossProductY * crossProductY;
    crossProductZ = crossProductZ * crossProductZ;

    Shadowface = 0;
    Outsideface = 0;

    vtx1Flag = Triangle->p1->v.flag;
    vtx2Flag = Triangle->p2->v.flag;
    vtx3Flag = Triangle->p3->v.flag;

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

    Triangle->Flag = flags;

    // Find the axis with the highest magnitude.

    // Y is the significant axis
    if ((crossProductX <= crossProductY) && (crossProductY >= crossProductZ)) {
        Triangle->Flag |= FACING_Y_AXIS;
        // X is the significant axis
    } else if ((crossProductX > crossProductY) && (crossProductX >= crossProductZ)) {
        Triangle->Flag |= FACING_X_AXIS;
        // Z is the significant axis
    } else {
        Triangle->Flag |= FACING_Z_AXIS;
    }
    CourseFaceMaxIndex++;
}

/**
 * Triangle contains three indices that are Used to get the actual vertices.
 */
void ReadTriangle(u32 Triangle, s8 surfaceType, u16 sectionId) {
    u32 vert1 = ((Triangle & 0x00FF0000) >> 16) / 2;
    u32 vert2 = ((Triangle & 0x0000FF00) >> 8) / 2;
    u32 vert3 = (Triangle & 0x000000FF) / 2;

    Vtx* vtx1 = VertexBuffer2[vert1];
    Vtx* vtx2 = VertexBuffer2[vert2];
    Vtx* vtx3 = VertexBuffer2[vert3];

    SaveTriangle(vtx1, vtx2, vtx3, surfaceType, sectionId);
}

void ReadTriangle2(u32 triangle1, u32 triangle2, s8 surfaceType, u16 sectionId) {
    UNUSED s32 pad[2];
    u32 vert1 = ((triangle1 & 0x00FF0000) >> 16) / 2;
    u32 vert2 = ((triangle1 & 0x0000FF00) >> 8) / 2;
    u32 vert3 = (triangle1 & 0x000000FF) / 2;

    // This is actually Triangle 2; vert 1,2,3.
    u32 vert4 = ((triangle2 & 0x00FF0000) >> 16) / 2;
    u32 vert5 = ((triangle2 & 0x0000FF00) >> 8) / 2;
    u32 vert6 = (triangle2 & 0x000000FF) / 2;

    Vtx* vtx1 = VertexBuffer2[vert1];
    Vtx* vtx2 = VertexBuffer2[vert2];
    Vtx* vtx3 = VertexBuffer2[vert3];

    Vtx* vtx4 = VertexBuffer2[vert4];
    Vtx* vtx5 = VertexBuffer2[vert5];
    Vtx* vtx6 = VertexBuffer2[vert6];

    // Triangle 1
    SaveTriangle(vtx1, vtx2, vtx3, surfaceType, sectionId);
    // Triangle 2
    SaveTriangle(vtx4, vtx5, vtx6, surfaceType, sectionId);
}

void ReadQuadrangle(u32 line, s8 surfaceType, u16 sectionId) {
    UNUSED s32 pad[6];
    Vtx* vtx1;
    Vtx* vtx2;
    Vtx* vtx3;
    Vtx* vtx4;

    u32 vert1 = ((line & 0x00FF0000) >> 16) / 2;
    u32 vert2 = ((line & 0x0000FF00) >> 8) / 2;
    u32 vert3 = (line & 0x000000FF) / 2;
    u32 vert4 = ((line & 0xFF000000) >> 24) / 2;

    vtx1 = VertexBuffer2[vert1];
    vtx2 = VertexBuffer2[vert2];
    vtx3 = VertexBuffer2[vert3];
    vtx4 = VertexBuffer2[vert4];

    // Triangle 1
    SaveTriangle(vtx1, vtx2, vtx3, surfaceType, sectionId);
    // Triangle 2
    SaveTriangle(vtx1, vtx3, vtx4, surfaceType, sectionId);
}

/**
 * Generates a list of pointers to course vtx.
 */
void ReadVertex2(uintptr_t Addr, u32 numVertices, u32 bufferIndex) {
    u32 i;
    u32 segment = SEGMENT_NUMBER2(Addr);
    u32 Offset = SEGMENT_OFFSET(Addr);
    Vtx* vtx = (Vtx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    for (i = 0; i < numVertices; i++) {
        VertexBuffer2[bufferIndex] = vtx;
        vtx++;
        bufferIndex++;
    }
}
/**
 * @return 1 intersecting Triangle, 0 not intersecting.
 */
s32 CheckSquareLine(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 x1, s16 z1, s16 x2, s16 z2) {
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

s32 CheckSquareTriangle(s16 minX, s16 maxX, s16 minZ, s16 maxZ, u16 Index) {
    FaceStruct* Triangle = &FaceBuffer[Index];
    s16 x1;
    s16 z1;
    s16 x2;
    s16 z2;
    s16 x3;
    s16 z3;

    x1 = Triangle->p1->v.ob[0];
    z1 = Triangle->p1->v.ob[2];
    x2 = Triangle->p2->v.ob[0];
    z2 = Triangle->p2->v.ob[2];
    x3 = Triangle->p3->v.ob[0];
    z3 = Triangle->p3->v.ob[2];
    if ((x1 >= minX) && (maxX >= x1) && (z1 >= minZ) && (maxZ >= z1)) {
        return 1;
    }
    if ((x2 >= minX) && (maxX >= x2) && (z2 >= minZ) && (maxZ >= z2)) {
        return 1;
    }
    if ((x3 >= minX) && (maxX >= x3) && (z3 >= minZ) && (maxZ >= z3)) {
        return 1;
    }
    if (CheckSquareLine(minX, maxX, minZ, maxZ, x1, z1, x2, z2) == 1) {
        return 1;
    }
    if (CheckSquareLine(minX, maxX, minZ, maxZ, x2, z2, x3, z3) == 1) {
        return 1;
    }
    if (CheckSquareLine(minX, maxX, minZ, maxZ, x3, z3, x1, z1) == 1) {
        return 1;
    }
    return 0;
}

/**
 * Splits the collision mesh into 32x32 sections. This allows the game to check only
 * nearby geography for a collision rather than checking against the whole collision mesh.
 * (checking against the whole mesh for every actor would be expensive)
 */
void MakeCollisionTable(void) {
    FaceStruct* Triangle;
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
    s32 Index;

    courseLengthX = (s32) MaximumX - MinimumX;
    courseLengthZ = (s32) MaximumZ - MinimumZ;

    // Separate the course into 32 sections
    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    // Reset the collision grid
    for (i = 0; i < 1024; i++) {
        CollisionGridTable[i].NumTriangles = 0;
    }

    CollisionPointer = 0;
    /**
     * @warning FreeMemoryPointer incremented just outside this function. Bad practice to hide memory allocation
     * like this.
     */
    CollisionTable = (u16*) FreeMemoryPointer;

    // 32x32 grid
    for (j = 0; j < GRID_SIZE; j++) {
        for (k = 0; k < GRID_SIZE; k++) {
            Index = k + j * GRID_SIZE;

            // Select a section of the course using min/max akin to drawing a bounding-box
            minX = (MinimumX + (sectionX * k)) - 20;
            minZ = (MinimumZ + (sectionZ * j)) - 20;

            maxX = minX + sectionX + 40;
            maxZ = minZ + sectionZ + 40;

            for (i = 0; i < CourseFaceMaxIndex; i++) {
                Triangle = FaceBuffer + i;
                if (Triangle->Maxz < minZ) {
                    continue;
                }
                if (Triangle->Minz > maxZ) {
                    continue;
                }
                if (Triangle->Maxx < minX) {
                    continue;
                }
                if (Triangle->Minx > maxX) {
                    continue;
                }

                // Add the collision Triangle to the list if it's inside the bounding-box
                if (CheckSquareTriangle(minX, maxX, minZ, maxZ, (u16) i) == 1) {
                    // Point this grid section to the first Triangle in the section
                    if (CollisionGridTable[Index].NumTriangles == 0) {
                        CollisionGridTable[Index].Triangle = CollisionPointer;
                    }
                    CollisionGridTable[Index].NumTriangles++;
                    CollisionTable[CollisionPointer] = (u16) i;
                    CollisionPointer++;
                }
            }
        }
    }
}

/**
 * Recursive search for vtx and set surfaceTypes to -1 and sectionId's to 0xFF
 */
void SearchList(Gfx* gfx) {
    SearchList3(gfx, SURFACE_DEFAULT, 0xFF);
}

/**
 * Recursive search for vtx and set sectionId's to 0xFF
 */
void SearchList2(Gfx* gfx, s8 surfaceType) {
    SearchList3(gfx, surfaceType, 0xFF);
}

extern u32 Facenumber;

/**
 * Generate via a recursive search and set for vertex data.
 */
void SearchList3(Gfx* Addr, s8 surfaceType, u16 sectionId) {
    s32 opcode;
    uintptr_t lo;
    uintptr_t hi;
    s32 i;

    s32 segment = SEGMENT_NUMBER2(Addr);
    s32 Offset = SEGMENT_OFFSET(Addr);
    Gfx* gfx = (Gfx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    Shadowface = 0;
    Outsideface = 0;

    for (i = 0; i < 0x1FFF; i++) {
        lo = gfx->words.w0;
        hi = gfx->words.w1;
        opcode = GFX_GET_OPCODE(lo);

        if (opcode == (G_DL << 24)) {
            // G_DL's hi contains an Addr to another DL.
            SearchList3((Gfx*) hi, surfaceType, sectionId);

        } else if (opcode == (G_VTX << 24)) {
            ReadVertex2(hi, (lo >> 10) & 0x3F, ((lo >> 16) & 0xFF) >> 1);

        } else if (opcode == (G_TRI1 << 24)) {
            Facenumber += 1;
            ReadTriangle(hi, surfaceType, sectionId);

        } else if (opcode == (G_TRI2 << 24)) {
            Facenumber += 2;
            ReadTriangle2(lo, hi, surfaceType, sectionId);

        } else if (opcode == (G_QUAD << 24)) {
            Facenumber += 2;
            ReadQuadrangle(hi, surfaceType, sectionId);

        } else if (opcode == (G_ENDDL << 24)) {
            break;
        }

        gfx++;
    }
}

/**
 * Search for G_SETTILESIZE and set its args.
 */
void ScrollMapImage(uintptr_t Addr, s32 uls, s32 ult) {
    u32 segment = SEGMENT_NUMBER2(Addr);
    u32 Offset = SEGMENT_OFFSET(Addr);
    Gfx* gfx = (Gfx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
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

void WaterVertex(uintptr_t Addr, u32 vertexCount, UNUSED s32 vert3, s8 Alpha, u8 red, u8 Green, u8 Blue) {
    s32 segment = SEGMENT_NUMBER2(Addr);
    s32 Offset = SEGMENT_OFFSET(Addr);
    s32 i;
    Vtx* vtx = (Vtx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);

    for (i = 0; (u32) i < vertexCount; i++) {
        if (red) {
            vtx->v.cn[0] = red;
            vtx->v.cn[1] = Green;
            vtx->v.cn[2] = Blue;
        }
        vtx->v.cn[3] = Alpha;
        vtx++;
    }
}

/**
 * Recursive search for vertices and set their colour values.
 */
void MakeWaterVertex(uintptr_t DisplayList, s8 Alpha, u8 red, u8 Green, u8 Blue) {
    s32 segment = SEGMENT_NUMBER2(DisplayList);
    s32 Offset = SEGMENT_OFFSET(DisplayList);
    Gfx* gfx = (Gfx*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
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
            MakeWaterVertex(hi, Alpha, red, Green, Blue);
        } else if (opcode == (G_VTX << 24)) {
            // G_VTX contains an Addr hi
            WaterVertex(hi, (lo >> 10) & 0x3F, ((lo >> 16) & 0xFF) >> 1, Alpha, red, Green, Blue);
        }
        gfx++;
    }
}

void CollectTirePosition(Vec3f pos1, f32 boundingBoxSize, Vec3f pos2) {
    pos2[0] -= pos1[0] * boundingBoxSize;
    pos2[1] -= pos1[1] * boundingBoxSize;
    pos2[2] -= pos1[2] * boundingBoxSize;
}

u16 CheckTire2(Player* Player, KartTire* tire, f32 tire2X, f32 tire2Y, f32 tire2Z) {
    Collision wtf;
    Collision* collision = &wtf;
    UNUSED s32 pad;
    u16 i;
    u16 meshIndex;
    u16 NumTriangles;
    u16 sectionIndex;
    f32 tireX;
    f32 tireY;
    f32 tireZ;
    f32 boundingBoxSize;
    f32 Height;

    s32 courseLengthX;
    s32 courseLengthZ;

    s16 sectionIndexX;
    s16 sectionIndexZ;
    s16 gridIndex;

    s32 sectionX;
    s32 sectionZ;
    UNUSED s32 Pad2[9];

    collision->DistanceXY = 1000.0f;
    collision->DistanceYZ = 1000.0f;
    collision->DistanceZX = 1000.0f;
    boundingBoxSize = Player->Radius;
    collision->LastXY = 5000;
    collision->LastYZ = 5000;
    collision->LastZX = 5000;
    collision->FlagXY = 0;
    collision->FlagYZ = 0;
    collision->FlagZX = 0;
    tireX = tire->Position[0];
    tireY = tire->Position[1];
    tireZ = tire->Position[2];
    switch (tire->LastAxis) {
        case 0x80:
            if (CheckTriangleYZV(collision, boundingBoxSize, tireX, tireY, tireZ, tire->LastPointer,
                                         tire2X, tire2Y, tire2Z) == 1) {
                Height = CalcHeight(tireX, tireY, tireZ, tire->LastPointer);
                if ((!(Height > Player->Position[1])) && !((Player->Position[1] - Height) > (2 * boundingBoxSize))) {
                    tire->Height = Height;
                    CollectTirePosition(collision->BumpYZ, collision->DistanceYZ, tire->Position);
                    return 1;
                }
            }
            break;
        case 0x40:
            if (CheckTriangleZXV(collision, boundingBoxSize, tireX, tireY, tireZ,
                                                   tire->LastPointer, tire2X, tire2Y, tire2Z) == 1) {
                Height = CalcHeight(tireX, tireY, tireZ, tire->LastPointer);
                if (!(Player->Position[1] < Height) && !((2 * boundingBoxSize) < (Player->Position[1] - Height))) {
                    tire->Height = Height;
                    CollectTirePosition(collision->BumpZX, collision->DistanceZX, tire->Position);
                    return 1;
                }
            }
            break;
        case 0x20:
            if (CheckTriangleXYV(collision, boundingBoxSize, tireX, tireY, tireZ, tire->LastPointer,
                                         tire2X, tire2Y, tire2Z) == 1) {
                Height = CalcHeight(tireX, tireY, tireZ, tire->LastPointer);
                if (!(Player->Position[1] < Height) && !((2 * boundingBoxSize) < (Player->Position[1] - Height))) {
                    tire->Height = Height;
                    CollectTirePosition(collision->BumpXY, collision->DistanceXY, tire->Position);
                    return 1;
                }
            }
            break;
        case 0:
            break;
    }

    // If the surface flags are not set then try setting them.

    courseLengthX = (s32) MaximumX - MinimumX;
    courseLengthZ = (s32) MaximumZ - MinimumZ;

    sectionX = courseLengthX / GRID_SIZE;
    sectionZ = courseLengthZ / GRID_SIZE;

    sectionIndexX = (tireX - MinimumX) / sectionX;
    sectionIndexZ = (tireZ - MinimumZ) / sectionZ;

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
    NumTriangles = CollisionGridTable[gridIndex].NumTriangles;

    if (NumTriangles == 0) {
        return 0;
    }

    sectionIndex = CollisionGridTable[gridIndex].Triangle;

    for (i = 0; i < NumTriangles; i++) {
        meshIndex = CollisionTable[sectionIndex];
        if (FaceBuffer[meshIndex].Flag & FACING_Y_AXIS) {
            if (meshIndex != tire->LastPointer) {
                if (CheckTriangleZXV(collision, boundingBoxSize, tireX, tireY, tireZ, meshIndex,
                                                       tire2X, tire2Y, tire2Z) == 1) {
                    Height = CalcHeight(tireX, tireY, tireZ, meshIndex);

                    if (!(Player->Position[1] < Height) && !((2 * boundingBoxSize) < (Player->Position[1] - Height))) {
                        CollectTirePosition(collision->BumpZX, collision->DistanceZX, tire->Position);
                        tire->Height = Height;
                        tire->Status = (u8) FaceBuffer[meshIndex].Status;
                        tire->LastAxis = 0x40;
                        tire->LastPointer = meshIndex;
                        if (FaceBuffer[meshIndex].Flag & 0x1000) {
                            tire->Dummy = 1;
                        } else {
                            tire->Dummy = 0;
                        }
                        return 1;
                    }
                }
            }
        } else if (FaceBuffer[meshIndex].Flag & FACING_X_AXIS) {
            if (FaceBuffer[meshIndex].Solb != 0.0f) {
                if (meshIndex != tire->LastPointer) {
                    if (CheckTriangleYZV(collision, boundingBoxSize, tireX, tireY, tireZ, meshIndex, tire2X,
                                                 tire2Y, tire2Z) == 1) {
                        Height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                        if (!(Player->Position[1] < Height) && !((2 * boundingBoxSize) < (Player->Position[1] - Height))) {
                            tire->Height = Height;
                            CollectTirePosition(collision->BumpYZ, collision->DistanceYZ, tire->Position);
                            tire->Height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                            tire->Status = (u8) FaceBuffer[meshIndex].Status;
                            tire->LastAxis = 0x80;
                            tire->LastPointer = meshIndex;
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (FaceBuffer[meshIndex].Solb != 0.0f) {
                if (meshIndex != tire->LastPointer) {
                    if (CheckTriangleXYV(collision, boundingBoxSize, tireX, tireY, tireZ, meshIndex, tire2X,
                                                 tire2Y, tire2Z) == 1) {
                        Height = CalcHeight(tireX, tireY, tireZ, meshIndex);
                        if (!(Player->Position[1] < Height) && !((2 * boundingBoxSize) < (Player->Position[1] - Height))) {
                            tire->Height = Height;
                            CollectTirePosition(collision->BumpXY, collision->DistanceXY, tire->Position);
                            tire->Status = (u8) FaceBuffer[meshIndex].Status;
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

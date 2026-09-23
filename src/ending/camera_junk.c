#include <ultra64.h>
#include <macros.h>
#include <course.h>
#include <common_structs.h>

#include "camera_junk.h"
#include "camera.h"
#include "math_util.h"
#include "ceremony_and_credits.h"
#include "main.h"

void ResultCameraControl(void) {
    Camera* Camera;
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;

    Camera = &Cameras[0];
    GrouCamdemoMain(Camera);

    x_dist = Camera->LookatPos[0] - Camera->CameraPos[0];
    y_dist = Camera->LookatPos[1] - Camera->CameraPos[1];
    z_dist = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(x_dist, z_dist);
    Camera->CameraDirection[0] = Atan2t(sqrtf((x_dist * x_dist) + (z_dist * z_dist)), y_dist);
    Camera->CameraDirection[2] = 0;
}

// GP podium ceremony Camera settings?
void InitCameraPodiumCeremony(void) {
    Cameras[0].CameraPos[0] = -3133.0f;
    Cameras[0].CameraPos[1] = 19.0f;
    Cameras[0].CameraPos[2] = -467.0f;
    Cameras[0].LookatPos[0] = -3478.0f;
    Cameras[0].LookatPos[1] = 21.0f;
    Cameras[0].LookatPos[2] = -528.0f;
    Cameras[0].UpVector[0] = 0.0f;
    Cameras[0].UpVector[1] = 1.0f;
    Cameras[0].UpVector[2] = 0.0f;
    CameraZoom[0] = 40.0f;
    AspectRatio = 1.33333333f;
    ScreenNearPoint = 3.0f;
    FarClip = 6800.0f;
    GrouCamdemoInit();
}

#include <ultra64.h>
#include <macros.h>
#include <course.h>
#include <common_structs.h>

#include "camera_junk.h"
#include "camera.h"
#include "math_util.h"
#include "ceremony_and_credits.h"
#include "main.h"

void result_camera_control(void) {
    Camera* camera;
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;

    camera = &cameras[0];
    Grou_camdemo_main(camera);

    x_dist = camera->lookat_pos[0] - camera->camera_pos[0];
    y_dist = camera->lookat_pos[1] - camera->camera_pos[1];
    z_dist = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(x_dist, z_dist);
    camera->camera_direction[0] = Atan2t(sqrtf((x_dist * x_dist) + (z_dist * z_dist)), y_dist);
    camera->camera_direction[2] = 0;
}

// GP podium ceremony camera settings?
void init_camera_podium_ceremony(void) {
    cameras[0].camera_pos[0] = -3133.0f;
    cameras[0].camera_pos[1] = 19.0f;
    cameras[0].camera_pos[2] = -467.0f;
    cameras[0].lookat_pos[0] = -3478.0f;
    cameras[0].lookat_pos[1] = 21.0f;
    cameras[0].lookat_pos[2] = -528.0f;
    cameras[0].up_vector[0] = 0.0f;
    cameras[0].up_vector[1] = 1.0f;
    cameras[0].up_vector[2] = 0.0f;
    gCameraZoom[0] = 40.0f;
    g_aspectRatio = 1.33333333f;
    screen_near_point = 3.0f;
    g_farClip = 6800.0f;
    Grou_camdemo_init();
}

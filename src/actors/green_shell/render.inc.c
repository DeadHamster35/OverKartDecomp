#include <actors.h>
#include <PR/gbi.h>
#include <main.h>
#include <assets/common_data.h>

/**
 * @brief Renders the green shell actor.
 *
 * @param camera
 * @param matrix
 * @param shell
 */
void display_gshell(Camera* camera, Mat4 matrix, struct ShellActor* shell) {
    gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_green_shell); // set texture
    display_shell(camera, matrix, shell);
}

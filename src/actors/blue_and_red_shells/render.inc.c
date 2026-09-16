#include <actors.h>
#include <PR/gbi.h>

/**
 * @brief Render the red shell actor
 *
 * @param camera
 * @param matrix
 * @param shell
 */
void display_rshell(Camera* camera, Mat4 matrix, struct ShellActor* shell) {
    gDPLoadTLUT_pal256(gDisplayListHead++, &gTLUTRedShell); // set texture
    display_shell(camera, matrix, shell);
}

/**
 * @brief Render the blue shell actor
 *
 * @param camera
 * @param matrix
 * @param shell
 */
void display_tshell(Camera* camera, Mat4 matrix, struct ShellActor* shell) {
    gDPLoadTLUT_pal256(gDisplayListHead++, common_tlut_blue_shell); // set texture
    display_shell(camera, matrix, shell);
}

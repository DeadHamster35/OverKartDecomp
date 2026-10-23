#include <actors.h>
#include <PR/gbi.h>

/**
 * @brief Render the red shell actor
 *
 * @Param Camera
 * @Param Matrix
 * @Param shell
 */
void DisplayRshell(Camera* Camera, Mat4 Matrix, struct ShellActor* shell) {
    gDPLoadTLUT_pal256(DisplayListHead++, &TLUTRedShell); // set Texture
    DisplayShell(Camera, Matrix, shell);
}

/**
 * @brief Render the Blue shell actor
 *
 * @Param Camera
 * @Param Matrix
 * @Param shell
 */
void DisplayTshell(Camera* Camera, Mat4 Matrix, struct ShellActor* shell) {
    gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTBlueShell); // set Texture
    DisplayShell(Camera, Matrix, shell);
}

#include <actors.h>
#include <PR/gbi.h>
#include <Main.h>
#include <assets/common_data.h>

/**
 * @brief Renders the Green shell actor.
 *
 * @Param Camera
 * @Param Matrix
 * @Param shell
 */
void DisplayGshell(Camera* Camera, Mat4 Matrix, struct ShellActor* shell) {
    gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTGreenShell); // set Texture
    DisplayShell(Camera, Matrix, shell);
}

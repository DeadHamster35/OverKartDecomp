#include "Engine.h"
#include "rom.h"

#include <cstdlib>

#ifdef PC_LINK_GAME
extern "C" {
void GameprocInit(void);
void GameprocFrame(void);
}
#endif

#ifdef _WIN32
#include <locale.h>
#include <windows.h>
#endif

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

#ifdef _WIN32
    setlocale(LC_ALL, ".UTF8");
#endif

    GameEngine::Create();
    if (!PcRomLoad("baserom.us.z64")) {
        PcRomLoad("../baserom.us.z64");
    }

#ifdef PC_LINK_GAME
    /* main.c is linked. GameprocInit currently failfasts on leftover N64
     * hardware/IO; Fast3D fill is the visible frame. Set PC_TRY_GAMEPROC=1
     * to attempt the stock boot (SEH falls back to fill if it only AVs). */
    if (getenv("PC_TRY_GAMEPROC")) {
#ifdef _MSC_VER
        __try {
            GameprocInit();
            while (GameEngine::Instance->IsRunning()) {
                GameprocFrame();
            }
            GameEngine::Destroy();
            _Exit(0);
        } __except (EXCEPTION_EXECUTE_HANDLER) {
        }
#else
        GameprocInit();
        while (GameEngine::Instance->IsRunning()) {
            GameprocFrame();
        }
        GameEngine::Destroy();
        _Exit(0);
#endif
    }
#endif

    while (GameEngine::Instance->IsRunning()) {
        GameEngine::Instance->ProcessFrame();
    }

    GameEngine::Destroy();
    _Exit(0);
}

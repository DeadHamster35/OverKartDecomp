#pragma once

#ifdef __cplusplus
#include <fast/Fast3dWindow.h>
#include <fast/interpreter.h>
#include "ship/Context.h"

class GameEngine {
  public:
    static GameEngine* Instance;
    Ship::Context* context;

    GameEngine();
    static void Create();
    static void Destroy();
    bool IsRunning() const;
    void ProcessFrame();
    static void ProcessGfxCommands(Gfx* pool);
};

extern "C" {
#endif

void GameEngine_ProcessGfxCommands(Gfx* commands);

#ifdef __cplusplus
}
#endif

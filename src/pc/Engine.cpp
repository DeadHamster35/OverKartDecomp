#include "Engine.h"

#include <memory>
#include <unordered_map>
#include <fast/Fast3dWindow.h>
#include <fast/ucodehandlers.h>
#include "ship/Context.h"
#include "libultraship/controller/controldeck/ControlDeck.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define F3DEX_GBI 1
#define F3D_OLD 1
#define _LANGUAGE_C
extern "C" {
#include <libultraship/libultra/gbi.h>
}

#ifndef PC_WINDOW_NAME
#define PC_WINDOW_NAME "Amped Up"
#endif
#ifndef PC_WINDOW_ID
#define PC_WINDOW_ID "ampedup"
#endif

GameEngine* GameEngine::Instance;

GameEngine::GameEngine() {
    this->context = Ship::Context::CreateUninitializedInstance(PC_WINDOW_NAME, PC_WINDOW_ID, PC_WINDOW_ID ".cfg.json");

#ifdef _WIN32
    AllocConsole();
#endif

    this->context->InitConfiguration();
    this->context->InitConsoleVariables();
    this->context->InitResourceManager({}, {}, 3);
    this->context->InitConsole();

    auto controlDeck = std::make_shared<LUS::ControlDeck>();
    auto wnd = std::make_shared<Fast::Fast3dWindow>(std::vector<std::shared_ptr<Ship::GuiWindow>>{});

    this->context->Init({}, {}, 3, { 26800, 512, 1100 }, wnd, controlDeck);
    wnd->SetRendererUCode(ucode_f3dex);
}

void GameEngine::Create() {
    Instance = new GameEngine();
}

void GameEngine::Destroy() {
    delete Instance;
    Instance = nullptr;
}

bool GameEngine::IsRunning() const {
    auto wnd = std::dynamic_pointer_cast<Fast::Fast3dWindow>(this->context->GetWindow());
    return wnd != nullptr && wnd->IsRunning();
}

void GameEngine::ProcessGfxCommands(Gfx* pool) {
    auto wnd = std::dynamic_pointer_cast<Fast::Fast3dWindow>(Ship::Context::GetRawInstance()->GetWindow());
    if (wnd == nullptr) {
        return;
    }
    wnd->HandleEvents();
    std::unordered_map<Mtx*, MtxF> none;
    wnd->DrawAndRunGraphicsCommands(pool, none);
}

void GameEngine::ProcessFrame() {
    static Gfx sFillDl[] = {
        gsDPPipeSync(),
        gsDPSetCycleType(G_CYC_FILL),
        gsDPSetFillColor((GPACK_RGBA5551(40, 80, 160, 1) << 16) | GPACK_RGBA5551(40, 80, 160, 1)),
        gsDPFillRectangle(0, 0, 320 - 1, 240 - 1),
        gsDPFullSync(),
        gsSPEndDisplayList(),
    };
    ProcessGfxCommands(sFillDl);
}

extern "C" void GameEngine_ProcessGfxCommands(Gfx* commands) {
    GameEngine::ProcessGfxCommands(commands);
}

#pragma once

#include "SonicNexus.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{

struct Invincibility : GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : GameObject::Static {
        SpriteAnimation aniFrames;
        Animator animator;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    Player *parent;
    StateMachine<Invincibility> state;
    int32 timer;

    // ==============================
    // EVENTS
    // ==============================

    void Create(void *data);
    void Draw();
    void Update();
    void LateUpdate();

    static void StaticUpdate();
    static void StageLoad();
#if RETRO_REV0U
    static void StaticLoad(Static *sVars);
#endif
    static void Serialize();

#if GAME_INCLUDE_EDITOR
    static void EditorLoad();
    void EditorDraw();
#endif

    // ==============================
    // FUNCTIONS
    // ==============================

    void State_Spawner(void);
    void State_Child(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Invincibility);
};
} // namespace GameLogic

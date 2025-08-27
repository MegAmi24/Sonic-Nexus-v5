#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct Chompy : GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum ChompyJumpDirs {
        JUMPDIR_LEFT,
        JUMPDIR_RIGHT,
        JUMPDIR_RIGHT2, // not sure if or how this is different from JUMPDIR_RIGHT
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : GameObject::Static {
        SpriteAnimation aniFrames;
        Hitbox hitbox;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    ChompyJumpDirs jumpDir;
    StateMachine<Chompy> state;
    Animator animator;
    Vector2 oldPos;
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

    void State_Wait(void);
    void State_Jump(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Chompy);
};
} // namespace GameLogic

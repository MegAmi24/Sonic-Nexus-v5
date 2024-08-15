#pragma once

#include "All.hpp"

namespace GameLogic
{

struct Invincibility : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation aniFrames;
        RSDK::Animator animator;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    Player *parent;
    RSDK::StateMachine<Invincibility> state;
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

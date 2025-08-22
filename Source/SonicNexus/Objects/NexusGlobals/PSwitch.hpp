#pragma once

#include "SonicNexus.hpp"

namespace GameLogic
{

struct PSwitch : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum PSwitchTypes {
        PSWITCH_A,
        PSWITCH_B,
        PSWITCH_LOOP,
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation aniFrames;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    PSwitchTypes type;
    int32 height;
    RSDK::Hitbox hitbox;
    RSDK::Animator animator;

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

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(PSwitch);
};
} // namespace GameLogic

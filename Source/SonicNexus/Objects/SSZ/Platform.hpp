#pragma once

#include "SonicNexus.hpp"

namespace GameLogic
{

struct Platform : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum PlatformTypes {
        PLATFORM_FLOAT,
        PLATFORM_HORIZONTAL,
        PLATFORM_VERTICAL,
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation aniFrames;
        RSDK::Hitbox hitbox;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    PlatformTypes type;
    bool32 flippedOsc;
    RSDK::Animator animator;
    bool32 pressed;
    RSDK::Vector2 platformPos;
    RSDK::Vector2 changePos;

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

    RSDK_DECLARE(Platform);
};
} // namespace GameLogic

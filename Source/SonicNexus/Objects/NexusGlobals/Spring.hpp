#pragma once

#include "SonicNexus.hpp"

namespace GameLogic
{

struct Spring : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum SpringTypes {
        SPRING_YELLOW,
        SPRING_RED,
    };

    enum SpringDirections {
        SPRING_UP,
        SPRING_RIGHT,
        SPRING_LEFT,
        SPRING_DOWN,
        SPRING_UP_RIGHT,
        SPRING_UP_LEFT,
        SPRING_DOWN_RIGHT,
        SPRING_DOWN_LEFT,
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation aniFrames;
        RSDK::SoundFX sfxSpring;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    SpringTypes type;
    SpringDirections flipFlags;
    RSDK::Animator animator;
    RSDK::Animator baseAnim;
    RSDK::Hitbox boxHitbox;
    RSDK::Hitbox touchHitbox;
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

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Spring);
};
} // namespace GameLogic

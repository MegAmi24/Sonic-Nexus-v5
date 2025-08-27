#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct Spikes : GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum SpikesDirections {
        SPIKES_UP,
        SPIKES_RIGHT,
        SPIKES_LEFT,
        SPIKES_DOWN,
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : GameObject::Static {
        SpriteAnimation aniFrames;
        Hitbox boxHitbox;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    SpikesDirections flipFlags;
    Animator animator;
    Hitbox touchHitbox;

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

    RSDK_DECLARE(Spikes);
};
} // namespace GameLogic

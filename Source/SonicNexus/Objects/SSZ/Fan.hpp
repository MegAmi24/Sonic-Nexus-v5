#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct Fan : GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum FanTypes {
        Fan_TRACK1,
        Fan_TRACK2,
        Fan_TRACK3,
        Fan_TRACK4,
        Fan_TRACK5,
        Fan_TRACK6,
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
    Animator animator;

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

    RSDK_DECLARE(Fan);
};
} // namespace GameLogic

#pragma once

#include "All.hpp"

namespace GameLogic
{

struct RotatePalette : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    // ==============================
    // STRUCTS
    // ==============================

    struct TrackInfo {
        char fileName[0x40];
        uint32 trackLoop; // RSDKv2 doesn't support specifying a loop point, but that kinda sucks
    };

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation aniFrames;
        int32 timer;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
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

    RSDK_DECLARE(RotatePalette);
};
} // namespace GameLogic

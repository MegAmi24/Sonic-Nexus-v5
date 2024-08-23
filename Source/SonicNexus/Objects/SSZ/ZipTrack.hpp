#pragma once

#include "All.hpp"

namespace GameLogic
{

struct ZipTrack : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum ZipTrackTypes {
        ZIPTRACK_TRACK1,
        ZIPTRACK_TRACK2,
        ZIPTRACK_TRACK3,
        ZIPTRACK_TRACK4,
        ZIPTRACK_TRACK5,
        ZIPTRACK_TRACK6,
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
    ZipTrackTypes track;
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

    RSDK_DECLARE(ZipTrack);
};
} // namespace GameLogic

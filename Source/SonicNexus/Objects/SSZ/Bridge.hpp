#pragma once

#include "SonicNexus.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{

struct Bridge : GameObject::Entity {

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
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    int32 length;
    Animator animator;
    int32 walkedOn;
    int32 depress;
    int32 midPoint;
    int32 midHeight;
    int32 midYPos;
    Player *target;
    int32 left;
    int32 right;

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

    RSDK_DECLARE(Bridge);
};
} // namespace GameLogic

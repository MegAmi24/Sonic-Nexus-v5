#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

#define SCREEN_SCROLL_LEFT  (screenInfo->center.x - 8)
#define SCREEN_SCROLL_RIGHT (screenInfo->center.x + 8)
#define SCREEN_SCROLL_UP    (screenInfo->center.y - 16)
#define SCREEN_SCROLL_DOWN  (screenInfo->center.y + 16)

namespace GameLogic
{

struct Camera : GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum CameraStyles {
        CAMERASTYLE_FOLLOW,
        CAMERASTYLE_EXTENDED,
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : GameObject::Static {
        Vector2 boundary1;
        Vector2 newBoundary1;
        Vector2 boundary2;
        Vector2 newBoundary2;
        int32 waterLevel;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    Vector2 center;
    Entity *target;
    int32 style;
    int32 enabled;
    int32 adjustY;
    Vector2 scrollOffset;
    Vector2 scrollA;
    Vector2 scrollB;
    Vector2 scrollMove;
    Vector2 earthquake;
    int32 waterDrawPos;

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

    void SetPlayerScreenPosition(Entity *entity);
    void SetPlayerScreenPositionCDStyle(Entity *entity);
    void SetPlayerLockedScreenPosition(Entity *entity);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Camera);
};
} // namespace GameLogic

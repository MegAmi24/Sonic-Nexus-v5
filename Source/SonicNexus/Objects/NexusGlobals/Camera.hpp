#pragma once

#include "All.hpp"

#define SCREEN_SCROLL_LEFT  (screenInfo->center.x - 8)
#define SCREEN_SCROLL_RIGHT (screenInfo->center.x + 8)
#define SCREEN_SCROLL_UP    (screenInfo->center.y - 16)
#define SCREEN_SCROLL_DOWN  (screenInfo->center.y + 16)

namespace GameLogic
{

struct Camera : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum CameraStyles {
        CAMERASTYLE_NORMAL,
        CAMERASTYLE_CDSTYLE,
        CAMERASTYLE_HLOCKED,
        CAMERASTYLE_DISABLED,
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        RSDK::Vector2 boundary1;
        RSDK::Vector2 newBoundary1;
        RSDK::Vector2 boundary2;
        RSDK::Vector2 newBoundary2;
        int32 waterLevel;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    Entity *target;
    int32 style;
    int32 enabled;
    int32 adjustY;
    RSDK::Vector2 scrollOffset;
    RSDK::Vector2 scrollA;
    RSDK::Vector2 scrollB;
    int32 yScrollMove;
    RSDK::Vector2 earthquake;
    int32 lag;
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

    void SetPlayerScreenPosition(Player *player);
    void SetPlayerScreenPositionCDStyle(Player *player);
    void SetPlayerHLockedScreenPosition(Player *player);
    void SetPlayerLockedScreenPosition(Player *player);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Camera);
};
} // namespace GameLogic

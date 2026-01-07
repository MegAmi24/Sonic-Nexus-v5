#pragma once

#include "SonicNexus.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{

struct HUD : GameObject::Entity {

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
    StateMachine<HUD> state;
    Vector2 scorePos;
    Vector2 timePos;
    Vector2 ringPos;
    Vector2 lifePos;
    int32 flashingTimer;
    Player *parent;
    Animator textAnim;
    Animator numbersAnim;
    Animator lifeAnim;

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

    void State_Normal(void);
    void State_Enter(void);

    static void DrawNumbers(Animator *animator, Vector2 drawPos, int32 value, uint8 digitCount, bool32 allDigits);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(HUD);
};
} // namespace GameLogic

#pragma once

#include "SonicNexus.hpp"
#include "Players/Player.hpp"

namespace GameLogic
{

struct HUD : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

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
    RSDK::StateMachine<HUD> state;
    RSDK::Vector2 scorePos;
    RSDK::Vector2 timePos;
    RSDK::Vector2 ringPos;
    RSDK::Vector2 lifePos;
    int32 flashingTimer;
    Player *parent;
    RSDK::Animator textAnim;
    RSDK::Animator numbersAnim;
    RSDK::Animator lifeAnim;

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

    static void DrawNumbers(RSDK::Animator *animator, RSDK::Vector2 drawPos, int32 value, uint8 digitCount, bool32 allDigits);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(HUD);
};
} // namespace GameLogic

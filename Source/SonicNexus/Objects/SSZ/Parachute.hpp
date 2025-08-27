#pragma once

#include "SonicNexus.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{

struct Parachute : GameObject::Entity {

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
        Hitbox hitbox;
        SoundFX sfxCatch;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    StateMachine<Parachute> state;
    Animator animator;
    Player *target;
    Vector2 startPos;

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

    void State_Wait(void);
    void State_Glide(void);
    void State_Fall(void);
    void State_Reset(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Parachute);
};
} // namespace GameLogic

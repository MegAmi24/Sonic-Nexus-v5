#pragma once

#include "SonicNexus.hpp"
#include "Players/Player.hpp"

namespace GameLogic
{

struct Parachute : RSDK::GameObject::Entity {

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
        RSDK::Hitbox hitbox;
        RSDK::SoundFX sfxCatch;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    RSDK::StateMachine<Parachute> state;
    RSDK::Animator animator;
    Player *target;
    RSDK::Vector2 startPos;

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

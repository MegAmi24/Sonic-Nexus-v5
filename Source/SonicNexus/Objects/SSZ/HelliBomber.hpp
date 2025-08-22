#pragma once

#include "SonicNexus.hpp"
#include "Players/Player.hpp"

namespace GameLogic
{

struct HelliBomber : RSDK::GameObject::Entity {

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
        RSDK::Hitbox aggroHitbox;
        RSDK::Hitbox bulletHitbox;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    bool32 isBullet;
    RSDK::StateMachine<HelliBomber> state;
    RSDK::Animator animator;
    RSDK::Animator bladesAnim;
    int32 destX;
    int32 timer;
    Player *target;

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

    void State_Idle(void);
    void State_Move(void);
    void State_Wait(void);
    void State_Shoot(void);
    void State_Bullet(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(HelliBomber);
};
} // namespace GameLogic

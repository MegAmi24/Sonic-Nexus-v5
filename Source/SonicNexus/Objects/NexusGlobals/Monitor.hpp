#pragma once

#include "SonicNexus.hpp"
#include "Players/Player.hpp"

namespace GameLogic
{

struct Monitor : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum MonitorTypes {
        MONITOR_BLANK,
        MONITOR_RINGS,
        MONITOR_BLUESHIELD,
        MONITOR_INVINCIBILITY,
        MONITOR_SPEEDSHOES,
        MONITOR_EXTRALIFE,
        MONITOR_ROBOTNIK,
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation aniFrames;
        RSDK::Hitbox hitbox;
        RSDK::SoundFX sfxDestroy;
        RSDK::SoundFX sfxBlueShield;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    MonitorTypes type;
    RSDK::StateMachine<Monitor> state;
    RSDK::Vector2 iconPos;
    RSDK::Animator animator;
    RSDK::Animator iconAnim;
    bool32 falling;
    int32 timer;
    int32 pSpeed;
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
    void State_Powerup_Rise(void);
    void State_Powerup_Show(void);
    void State_Broken(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Monitor);
};
} // namespace GameLogic

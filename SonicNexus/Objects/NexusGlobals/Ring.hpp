#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct Ring : GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum RingTypes {
        RING_NORMAL,
        RING_LOSE,
        RING_SPARKLE,
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : GameObject::Static {
        SpriteAnimation aniFrames;
        Animator ringAnim;
        Hitbox hitbox;
        SoundFX sfxRing;
        bool32 pan;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    RingTypes type;
    StateMachine<Ring> state;
    Animator animator;
    int32 timer;

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
    void State_Lose(void);
    void State_Sparkle(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Ring);
};
} // namespace GameLogic

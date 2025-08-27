#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

#define LOGO_YPOS (108)

namespace GameLogic
{

struct Logo : GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum LogoTypes {
        LOGO_BOUNCING_DOWN,
        LOGO_BOUNCING_SCALE,
        LOGO_STATIC,
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : GameObject::Static {
        SpriteAnimation aniFrames;
        SoundFX sfxLand;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    Animator animator;
    Animator handAnim;
    Animator sonicBlinkAnim;
    Animator tailsBlinkAnim;
    StateMachine<Logo> state;
    StateMachine<Logo> stateDraw;
    int32 bounceCount;
    int32 scaleSpeed;
    int32 timer;
    int32 sonicBlink;
    int32 tailsBlink;

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
    
    void State_BouncingDown(void); // Unused
    void State_BouncingScale(void);
    void State_Static(void);

    void Draw_Normal(void);
    void Draw_BouncingScale(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Logo);
};
} // namespace GameLogic

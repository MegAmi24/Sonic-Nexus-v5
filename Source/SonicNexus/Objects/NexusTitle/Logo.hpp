#pragma once

#include "All.hpp"

#define LOGO_YPOS (108)

namespace GameLogic
{

struct Logo : RSDK::GameObject::Entity {

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

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation aniFrames;
        RSDK::SoundFX sfxLand;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    RSDK::Animator animator;
    RSDK::Animator handAnim;
    RSDK::Animator sonicBlinkAnim;
    RSDK::Animator tailsBlinkAnim;
    RSDK::StateMachine<Logo> state;
    RSDK::StateMachine<Logo> stateDraw;
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

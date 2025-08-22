#pragma once

#include "SonicNexus.hpp"

namespace GameLogic
{

struct StarPost : RSDK::GameObject::Entity {

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
        RSDK::SoundFX sfxLampPost;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    RSDK::StateMachine<StarPost> state;
    RSDK::StateMachine<StarPost> stateDraw;
    RSDK::Animator animator;
    RSDK::Animator starAnim;
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
    void State_Swinging(void);
    void State_Flashing(void);

    void Draw_Swinging(void);
    void Draw_Flashing(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(StarPost);
};
} // namespace GameLogic

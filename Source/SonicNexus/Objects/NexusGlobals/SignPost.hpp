#pragma once

#include "All.hpp"

namespace GameLogic
{

struct SignPost : RSDK::GameObject::Entity {

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
        RSDK::SoundFX sfxSignPost;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    RSDK::StateMachine<SignPost> state;
    RSDK::Animator poleAnim;
    RSDK::Animator signAnim;
    int32 noSpins;


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

    void State_Waiting(void);
    void State_Spinning(void);
    void State_Exit(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(SignPost);
};
} // namespace GameLogic

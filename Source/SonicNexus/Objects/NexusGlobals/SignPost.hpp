#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct SignPost : GameObject::Entity {

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
        SoundFX sfxSignPost;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    StateMachine<SignPost> state;
    Animator poleAnim;
    Animator signAnim;
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

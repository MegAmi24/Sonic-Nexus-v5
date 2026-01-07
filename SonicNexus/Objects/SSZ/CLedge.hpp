#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct CLedge : GameObject::Entity {

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
        SoundFX sfxCrumble;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    int32 length;
    StateMachine<CLedge> state;
    StateMachine<CLedge> stateDraw;
    Animator animator;
    int32 left;
    int32 right;
    int32 timer;
    int32 noBlocks;

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

    void State_Ledge(void);
    void State_Wait_Right(void);
    void State_Wait_Left(void);
    void State_Crumble_Left(void);
    void State_Crumble_Right(void);
    void State_Wait_Respawn(void);
    void State_FallingBlock(void);

    void Draw_Ledge(void);
    void Draw_Crumble_Left(void);
    void Draw_FallingBlock(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(CLedge);
};
} // namespace GameLogic

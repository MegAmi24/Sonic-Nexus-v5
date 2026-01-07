#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct MRally : GameObject::Entity {

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
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    StateMachine<MRally> state;
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

    void State_Move_Left(void);
    void State_Move_Right(void);
    void State_Move_Left_Fast(void);
    void State_Move_Right_Fast(void);
    void State_Wait_Left(void);
    void State_Wait_Right(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(MRally);
};
} // namespace GameLogic

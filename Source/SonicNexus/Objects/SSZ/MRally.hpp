#pragma once

#include "SonicNexus.hpp"

namespace GameLogic
{

struct MRally : RSDK::GameObject::Entity {

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
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    RSDK::StateMachine<MRally> state;
    RSDK::Animator animator;
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

#pragma once

#include "SonicNexus.hpp"
#include "Players/Player.hpp"

namespace GameLogic
{

struct ZipLine : RSDK::GameObject::Entity {

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
        RSDK::SoundFX sfxCatch;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    RSDK::StateMachine<ZipLine> state;
    RSDK::Animator animator;
    RSDK::Vector2 handlePos;
    int32 trackPos;
    RSDK::Vector2 originPos;
    Player *target;
    int32 lastY;

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

    void State_Stopped(void);
    void State_Moving(void);
    void State_Return(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(ZipLine);
};
} // namespace GameLogic

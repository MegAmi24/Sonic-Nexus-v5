#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct TubeSwitch : GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    // I have no idea what these should be called, I'm just guessing
    enum TubeSwitchTypes {
        TUBESWITCH_R_ENTRY,
        TUBESWITCH_L_ENTRY,
        TUBESWITCH_BOOST,
        TUBESWITCH_ROLL,
        TUBESWITCH_EXIT,
        TUBESWITCH_ENTRY,
    };

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
    TubeSwitchTypes type;
    Animator animator;

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

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(TubeSwitch);
};
} // namespace GameLogic

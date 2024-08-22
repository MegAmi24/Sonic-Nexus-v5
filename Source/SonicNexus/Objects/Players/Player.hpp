#pragma once

#include "All.hpp"

namespace GameLogic
{

struct Player : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum PlayerModes {
        PLAYERMODE_ACTIVE,
        PLAYERMODE_INACTIVE, // idk what this is for LOL
        PLAYERMODE_DEBUG,
    };

    enum ControlModes {
        CONTROLMODE_NONE = -1,
        CONTROLMODE_PLAYER1,
        CONTROLMODE_DELAY,
    };

    enum PlayerAni {
        ANI_STOPPED,
        ANI_WAITING,
        ANI_BORED,
        ANI_LOOKINGUP,
        ANI_LOOKINGDOWN,
        ANI_WALKING,
        ANI_RUNNING,
        ANI_SKIDDING,
        ANI_PEELOUT,
        ANI_SPINDASH,
        ANI_JUMPING,
        ANI_BOUNCING,
        ANI_HURT,
        ANI_DYING,
        ANI_DROWNING,
        ANI_LIFEICON,
        ANI_FANROTATE,
        ANI_BREATHING,
        ANI_PUSHING,
        ANI_FLAILINGLEFT,
        ANI_FLAILINGRIGHT,
        ANI_SLIDING,
        ANI_FINISHPOSE = 23,
        ANI_CORKSCREW  = 34,
        ANI_HANGING    = 43,
    };

    enum Shields {
        SHIELD_NONE,
        SHIELD_BLUE,
    };

    enum HurtTypes {
        HURT_NONE,
        HURT_HASSHIELD,
        HURT_RINGLOSS,
        HURT_DIE,
    };

    // ==============================
    // STRUCTS
    // ==============================

    struct PlayerMovementStats {
        int32 topSpeed;
        int32 acceleration;
        int32 deceleration;
        int32 airAcceleration;
        int32 airDeceleration;
        int32 gravityStrength;
        int32 jumpStrength;
        int32 rollingAcceleration;
        int32 rollingDeceleration;
    };

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        uint16 upBuffer;
        uint16 downBuffer;
        uint16 leftBuffer;
        uint16 rightBuffer;
        uint16 jumpPressBuffer;
        uint16 jumpHoldBuffer;
        RSDK::SpriteAnimation sonicFrames;
        RSDK::SoundFX sfxJump;
        RSDK::SoundFX sfxLoseRings;
        RSDK::SoundFX sfxHurt;
        RSDK::SoundFX sfxSpin;
        RSDK::SoundFX sfxCharge;
        RSDK::SoundFX sfxRelease;
        RSDK::SoundFX sfxSkidding;
        RSDK::SoundFX sfxDestroy;
        RSDK::SoundFX sfxBossHit;
        RSDK::SoundFX sfxYes;
        bool32 pauseEnabled;
        bool32 frameAdvance;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    uint8 propertyValue;
    Camera *camera;
    RSDK::Vector2 screenPos;
    int32 timer;
    RSDK::StateMachine<Player> state;
    RSDK::Animator animator;
    RSDK::SpriteAnimation aniFrames;
    RSDK::Hitbox *outerbox;
    RSDK::Hitbox *innerbox;
    uint8 skidding;
    uint8 pushing;
    int8 controlMode;
    uint8 frictionLoss;
    int32 lookPos;
    PlayerMovementStats stats;
    uint8 left;
    uint8 right;
    uint8 up;
    uint8 down;
    uint8 jumpPress;
    uint8 jumpHold;
    uint8 followPlayer1;
    uint8 trackScroll;
    uint8 gravity;
    uint8 water;
    uint8 flailing[3];
    int16 runningSpeed;
    int16 walkingSpeed;
    int16 jumpingSpeed;
    int32 rings;
    int32 spinDash;
    int32 aniJumpSpeed;
    int32 speedShoes;
    int32 invincibility;
    int32 flashing;
    int32 minRollSpeed;
    int32 ringExtraLife;
    RSDK::Hitbox *normalbox;
    RSDK::Hitbox *jumpbox;
    int32 animCheck;
    int16 jumpAnimSpeed;
    uint8 shield;

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

    // General Player Functions (ported from RSDKv2)
    static void ProcessPlayerControl(Player *player);
    static void SetMovementStats(PlayerMovementStats *stats);
    static void DefaultAirMovement(Player *player);
    static void DefaultGravityFalse(Player *player);
    static void DefaultGravityTrue(Player *player);
    static void DefaultGroundMovement(Player *player);
    static void DefaultJumpAction(Player *player);
    static void DefaultRollingMovement(Player *player);
    static void ProcessDebugMode(Player *player);
    static void ProcessPlayerAnimation(Player *player);

    // Script Subs (ported from Nexus)
    void Main(void);
    void State_Normal_Ground_Movement(void);
    void State_Air_Movement(void);
    void State_Rolling(void);
    void State_Rolling_Jump(void);
    void State_Looking_Up(void);
    void State_Looking_Down(void);
    void State_Spindash(void);
    void State_Peelout(void);
    void State_Getting_Hurt(void);
    void State_Hurt_Recoil(void);
    void State_Dying(void);
    void State_Drowning(void);
    void State_Hanging(void);
    void State_Corkscrew_Run(void);
    void State_Corkscrew_Roll(void);
    void State_Tube_Rolling(void);

    // Helper functions
    void HandleMovement(void);
    uint8 BoxCollision(Entity *thisEntity, RSDK::Hitbox *thisHitbox);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Player);
};
} // namespace GameLogic

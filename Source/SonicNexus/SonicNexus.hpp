#pragma once

#include "GameAPI/Game.hpp"

// Enums

enum ScreenSplit { FORCE_SPLIT };

enum PlaneFilterTypes {
    PLANEFILTER_NONE,
    PLANEFILTER_AL, // - Plane A, Low Layer
    PLANEFILTER_BL, // - Plane B, Low Layer
    PLANEFILTER_AH, // - Plane A, High Layer
    PLANEFILTER_BH, // - Plane B, High Layer
};

enum ActIDs {
    ACT_1,
    ACT_2,
    ACT_3,
};

enum ReservedEntities {
    SLOT_PLAYER1              = 0,
    SLOT_CAMERA1              = 1,
    SLOT_POWERUP1             = 2,
    SLOT_STAGESETUP           = 23,
    SLOT_TITLECARD            = 24,
    SLOT_TITLECARD_VERTTEXT1  = 25,
    SLOT_TITLECARD_VERTTEXT2  = 26,
    SLOT_TITLECARD_SIDEBAR    = 27,
    SLOT_TITLECARD_FIRSTWORD  = 28,
    SLOT_TITLECARD_SECONDWORD = 29,
    SLOT_TITLECARD_ZONE       = 30,
    SLOT_TITLECARD_ACTBALL    = 31,
    SLOT_ACTFINISH            = 30,
    SLOT_HUD                  = 31,
};

// Global Variables

// forward declare
struct GlobalVariables;

extern GlobalVariables *globals;

struct GlobalVariables {
    struct Constructor {
        Constructor()
        {
#if RETRO_REV0U
            RegisterGlobals((void **)&globals, sizeof(GlobalVariables), &GlobalVariables::Init);
#else
            RegisterGlobals((void **)&globals, sizeof(GlobalVariables));
#endif
        }
    };

    static Constructor c;

#if RETRO_REV0U
    static void Init(void *g);
#endif

    int32 score;
    int32 lives;
    int32 recMilliseconds;
    int32 recSeconds;
    int32 recMinutes;
};

// Game Helpers

namespace RSDK
{

template <typename R> struct Action {

    R (Action::*action)();

    inline void Init() { action = nullptr; }

    template <typename T> inline bool Set(R (T::*action)())
    {
        // converts from T:: -> Action:: without the compiler interfering :]
        union {
            R (T::*in)();
            R (Action::*out)();
        };
        in = action;

        this->action = out;
        return true;
    }

    inline bool Set(R (*action)())
    {
        // converts from T:: -> Action:: without the compiler interfering :]
        union {
            R (*in)();
            R (Action::*out)();
        };
        in = action;

        this->action = out;
        return true;
    }

    template <typename T> inline R SetAndRun(R (T::*action)(), void *self = nullptr)
    {
        bool applied = Set(action);

        if (applied)
            return Run(self);

        return R();
    }

    template <typename T> inline R SetAndRun(R (*action)(), void *self = nullptr)
    {
        bool applied = Set(action);

        if (applied)
            return Run(self);

        return R();
    }

    inline R Run(void *self)
    {
        if (action) {
            return (((Action *)self)->*action)();
        }

        return R();
    }

    template <typename T> inline bool Matches(void *other)
    {
        // converts from Action:: -> void (*)() without the compiler interfering :]
        union {
            R *in;
            R (Action::*out)();
        };
        in = other;

        return action == out;
    }

    template <typename T> inline bool Matches(R (T::*other)()) { return action == (R(Action::*)())other; }

    inline bool Matches(Action *other)
    {
        if (other == nullptr)
            return action == nullptr;
        else
            return action == other->action;
    }

    inline void Copy(Action *other)
    {
        if (other == nullptr)
            this->action = nullptr;
        else
            this->action = other->action;
    }

    // Equals
    inline void operator=(const Action &rhs) { this->Copy((Action *)&rhs); }

    // Conditionals
    inline bool operator==(const Action &rhs) { return this->Matches((Action *)&rhs); }
    inline bool operator!=(const Action &rhs) { return !(*this == rhs); }
};

} // namespace RSDK

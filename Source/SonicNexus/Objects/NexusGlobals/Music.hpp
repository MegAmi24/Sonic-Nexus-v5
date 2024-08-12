#pragma once

#include "All.hpp"

namespace GameLogic
{

struct Music : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum MusicTracks {
        TRACK_NONE = -1,
        TRACK_STAGE,
        TRACK_LEVELCOMPLETE,
        TRACK_INVINCIBILITY,
        TRACK_SPEEDSHOES,
        TRACK_SUPER, // Just a guess
        TRACK_BOSS,
        TRACK_1UP,
        TRACK_GAMEOVER,
    };

    // ==============================
    // STRUCTS
    // ==============================

    struct TrackInfo {
        char fileName[0x40];
        uint32 trackLoop; // RSDKv2 doesn't support specifying a loop point, but that kinda sucks
    };

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        TrackInfo musicTracks[CHANNEL_COUNT];
        int32 currentTrack;
        float volume;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================

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

    static void SetTrack(const char *path, uint8 track, uint32 loopPoint);
    static void Play(int32 track);
    static void Stop(void);
    static bool32 CurrentTrack(int32 track);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Music);
};
} // namespace GameLogic

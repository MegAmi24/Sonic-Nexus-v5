// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Music Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Music.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Music);

void Music::Update(void) {}
void Music::LateUpdate(void) {}
void Music::StaticUpdate(void) {}
void Music::Draw(void) {}
void Music::Create(void *data) {}

void Music::StageLoad(void)
{
    // Slot 0: stage music
    Music::SetTrack("Nexus/LevelComplete.ogg", TRACK_LEVELCOMPLETE, false);
    Music::SetTrack("Nexus/Invincibility.ogg", TRACK_INVINCIBILITY, false);
    Music::SetTrack("Nexus/SpeedShoes.ogg", TRACK_SPEEDSHOES, false);
    Music::SetTrack("Nexus/Invincibility.ogg", TRACK_SUPER, false);
    Music::SetTrack("Nexus/Boss.ogg", TRACK_BOSS, false);
    Music::SetTrack("Nexus/OneUp.ogg", TRACK_1UP, false);
    Music::SetTrack("Nexus/GameOver.ogg", TRACK_GAMEOVER, false);
    sVars->volume = 1.0f;
}

void Music::SetTrack(const char *filePath, uint8 trackID, uint32 loop)
{
    TrackInfo *track = &sVars->musicTracks[trackID];
    strcpy_s(track->fileName, filePath);
    track->trackLoop = loop;
}

void Music::Play(int32 track)
{
    Stop();
    sVars->currentTrack = track;
    TrackInfo *song     = &sVars->musicTracks[sVars->currentTrack];
    channels[sVars->currentTrack].PlayStream(song->fileName, 0, song->trackLoop, true);
    sVars->volume = 1.0f;
}

void Music::Stop() { channels[sVars->currentTrack].Stop(); }

void Music::Pause() { channels[sVars->currentTrack].Pause(); }

void Music::Resume() { channels[sVars->currentTrack].Resume(); }

int32 Music::CurrentTrack(void) { return sVars->currentTrack; }

void Music::SetVolume(float volume)
{
    sVars->volume = CLAMP(volume, 0.0f, 1.0f);
    channels[sVars->currentTrack].SetAttributes(sVars->volume, 0.0f, 1.0f);
}

#if GAME_INCLUDE_EDITOR
void Music::EditorDraw(void) {}

void Music::EditorLoad(void) {}
#endif

#if RETRO_REV0U
void Music::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Music); }
#endif

void Music::Serialize(void) {}

} // namespace GameLogic

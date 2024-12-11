#include "SonicNexus.hpp"
#include "Math.hpp"
#if RETRO_USE_MOD_LOADER
#include "PublicFunctions.hpp"
#endif

using namespace RSDK;

GlobalVariables::Constructor c;
GlobalVariables *globals = nullptr;

#if RETRO_REV0U
void GlobalVariables::Init(void *g)
{
    GlobalVariables *globals = (GlobalVariables *)g;
    memset(globals, 0, sizeof(GlobalVariables));
}
#endif

void LinkGameLogic(RSDK::EngineInfo *info)
{
    sceneInfo->debugMode = false;
    GameLogic::CalculateTrigAngles();
#if RETRO_USE_MOD_LOADER
    GameLogic::InitPublicFunctions();
#endif
}

#if RETRO_USE_MOD_LOADER
bool32 LinkModLogic(RSDK::EngineInfo *info, const char *modID)
{
#if RETRO_REV02
    LinkGameLogicDLL(info);
#else
    LinkGameLogicDLL(*info);
#endif

    RSDK::Mod::modID = modID;

    return true;
}
#endif

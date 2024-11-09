#include "SonicNexus.hpp"
#include "Math.hpp"

using namespace RSDK;

#if RETRO_USE_MOD_LOADER
extern "C" {
DLLExport bool32 LinkModLogic(EngineInfo *info, const char *id);
}

const char *modID = "SonicNexus";
#endif

GlobalVariables::Constructor c;
GlobalVariables *globals = nullptr;

void GlobalVariables::Init(void *g)
{
    GlobalVariables *globals = (GlobalVariables *)g;
    memset(globals, 0, sizeof(GlobalVariables));

    sceneInfo->debugMode = false;

    CalculateTrigAngles();
}

#if RETRO_USE_MOD_LOADER
void InitModAPI(void) {}

void InitModAPI(void);
#endif

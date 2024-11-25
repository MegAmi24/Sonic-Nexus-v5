// Although v5 has its own sin/cos lookup tables, it uses different formulas for setting the values, and thus some of those are different from v2
// This causes issues when using Math::SinXXX or Math::CosXXX with certain objects such as platforms
// As such, I decided to bring over v2's lookup tables and Sin/Cos functions

#include "Math.hpp"
#include <math.h>
#include <time.h>

#ifndef RSDK_PI
#define RSDK_PI 3.1415927f
#endif

namespace GameLogic
{

int32 SinValue512[512];
int32 CosValue512[512];

int32 SinValue256[256];
int32 CosValue256[256];

void CalculateTrigAngles()
{
    srand(time(NULL));

    for (int32 i = 0; i < 0x200; ++i) {
        float Val      = sinf(((float)i / 256) * RSDK_PI);
        SinValue512[i] = (int32)(Val * 512.0);
        Val            = cosf(((float)i / 256) * RSDK_PI);
        CosValue512[i] = (int32)(Val * 512.0);
    }

    CosValue512[0]   = 0x200;
    CosValue512[128] = 0;
    CosValue512[256] = -0x200;
    CosValue512[384] = 0;
    SinValue512[0]   = 0;
    SinValue512[128] = 0x200;
    SinValue512[256] = 0;
    SinValue512[384] = -0x200;

    for (int32 i = 0; i < 0x100; ++i) {
        SinValue256[i] = (SinValue512[i * 2] >> 1);
        CosValue256[i] = (CosValue512[i * 2] >> 1);
    }
}

} // namespace GameLogic

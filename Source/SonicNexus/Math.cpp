// Although v5 has its own sin/cos lookup tables, it uses different formulas for setting the values, and thus some of those are different from v2
// This causes issues when using Math::SinXXX or Math::CosXXX with certain objects such as platforms
// As such, I decided to bring over v2's lookup tables and sin/cos functions

#include "Math.hpp"
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

int32 sinVal512[512];
int32 cosVal512[512];

int32 sinVal256[256];
int32 cosVal256[256];

void CalculateTrigAngles()
{
    srand(time(NULL));

    for (int32 i = 0; i < 0x200; ++i) {
        float Val    = sinf(((float)i / 256) * M_PI);
        sinVal512[i] = (int32)(Val * 512.0);
        Val          = cosf(((float)i / 256) * M_PI);
        cosVal512[i] = (int32)(Val * 512.0);
    }

    cosVal512[0]   = 0x200;
    cosVal512[128] = 0;
    cosVal512[256] = -0x200;
    cosVal512[384] = 0;
    sinVal512[0]   = 0;
    sinVal512[128] = 0x200;
    sinVal512[256] = 0;
    sinVal512[384] = -0x200;

    for (int32 i = 0; i < 0x100; i++) {
        sinVal256[i] = (sinVal512[i * 2] >> 1);
        cosVal256[i] = (cosVal512[i * 2] >> 1);
    }
}
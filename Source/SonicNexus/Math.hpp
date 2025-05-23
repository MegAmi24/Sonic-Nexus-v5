#pragma once

#include "SonicNexus.hpp"

#define MEM_ZERO(x)  memset(&(x), 0, sizeof((x)))
#define MEM_ZEROP(x) memset((x), 0, sizeof(*(x)))

namespace GameLogic
{

extern int32 SinValue512[0x200];
extern int32 CosValue512[0x200];

extern int32 SinValue256[0x100];
extern int32 CosValue256[0x100];

// Setup Angles
void CalculateTrigAngles();

inline int32 Sin512(int32 angle)
{
    if (angle < 0)
        angle = 0x200 - angle;
    angle &= 0x1FF;
    return SinValue512[angle];
}

inline int32 Cos512(int32 angle)
{
    if (angle < 0)
        angle = 0x200 - angle;
    angle &= 0x1FF;
    return CosValue512[angle];
}

inline int32 Sin256(int32 angle)
{
    if (angle < 0)
        angle = 0x100 - angle;
    angle &= 0xFF;
    return SinValue256[angle];
}

inline int32 Cos256(int32 angle)
{
    if (angle < 0)
        angle = 0x100 - angle;
    angle &= 0xFF;
    return CosValue256[angle];
}

} // namespace GameLogic

#pragma once

#include "SonicNexus.hpp"

//#define M_PI (3.1415927)

#define MEM_ZERO(x)  memset(&(x), 0, sizeof((x)))
#define MEM_ZEROP(x) memset((x), 0, sizeof(*(x)))

extern int32 sinVal512[0x200];
extern int32 cosVal512[0x200];

extern int32 sinVal256[0x100];
extern int32 cosVal256[0x100];

// Setup Angles
void CalculateTrigAngles();

inline int32 sin512(int32 angle)
{
    if (angle < 0)
        angle = 0x200 - angle;
    angle &= 0x1FF;
    return sinVal512[angle];
}

inline int32 cos512(int32 angle)
{
    if (angle < 0)
        angle = 0x200 - angle;
    angle &= 0x1FF;
    return cosVal512[angle];
}

inline int32 sin256(int32 angle)
{
    if (angle < 0)
        angle = 0x100 - angle;
    angle &= 0xFF;
    return sinVal256[angle];
}

inline int32 cos256(int32 angle)
{
    if (angle < 0)
        angle = 0x100 - angle;
    angle &= 0xFF;
    return cosVal256[angle];
}

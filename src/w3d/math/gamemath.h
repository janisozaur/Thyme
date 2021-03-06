////////////////////////////////////////////////////////////////////////////////
//                               --  THYME  --                                //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Thyme
//
//          File:: GAMEMATH.H
//
//        Author:: Tiberian Technologies
//
//  Contributors:: OmniBlade
//
//   Description:: Floating point math functions. Based on Tiberian Technologies
//                 Renegade Scripts wwmath class.
//
//       License:: Thyme is free software: you can redistribute it and/or 
//                 modify it under the terms of the GNU General Public License 
//                 as published by the Free Software Foundation, either version 
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//
////////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma once
#endif

#ifndef GAMEMATH_H
#define GAMEMATH_H

#include "always.h"
#include "gamedebug.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <xmmintrin.h>

#define GAMEMATH_EPSILON    0.0001f
#define GAMEMATH_EPSILON2   GAMEMATH_EPSILON * GAMEMATH_EPSILON
#define GAMEMATH_PI         3.141592654f
#define GAMEMATH_FLOAT_MAX  (FLT_MAX)
#define GAMEMATH_FLOAT_MIN  (FLT_MIN)
#define GAMEMATH_FLOAT_TINY (1.0e-37f)
#define GAMEMATH_SQRT2      1.414213562f
#define GAMEMATH_SQRT3      1.732050808f
#define GAMEMATH_OOSQRT2    0.707106781f
#define GAMEMATH_OOSQRT3    0.577350269f

#ifndef RAD_TO_DEG
#define RAD_TO_DEG(x) (((double)x)*180.0/GAMEMATH_PI)
#endif
#ifndef DEG_TO_RAD
#define DEG_TO_RAD(x) (((double)x)*GAMEMATH_PI/180.0)
#endif
#ifndef RAD_TO_DEGF
#define RAD_TO_DEGF(x) (((float)x)*180.0f/GAMEMATH_PI)
#endif
#ifndef DEG_TO_RADF
#define DEG_TO_RADF(x) (((float)x)*GAMEMATH_PI/180.0f)
#endif

const int ARC_TABLE_SIZE = 1024;
const int SIN_TABLE_SIZE = 1024;

extern float _FastAcosTable[ARC_TABLE_SIZE];
extern float _FastAsinTable[ARC_TABLE_SIZE];
extern float _FastSinTable[SIN_TABLE_SIZE];
extern float _FastInvSinTable[SIN_TABLE_SIZE];

namespace GameMath {

void Init();
void Shutdown();

inline float Fabs(float val)
{
    float tmp = val;
    uint32_t value = *reinterpret_cast<uint32_a*>(&tmp);
    value &= 0x7fffffff;
    return float(*reinterpret_cast<float_a*>(&value));
}

inline int Float_To_Int_Chop(const float &f)
{
    int32_t a = *reinterpret_cast<const int32_a*>(&f);
    int32_t sign = (a >> 31);
    int32_t mantissa = (a & ((1 << 23) - 1)) | (1 << 23);
    int32_t exponent = ((a & 0x7fffffff) >> 23) - 127;
    int32_t r = ((uint32_t)(mantissa) << 8) >> (31 - exponent);

    return ((r ^ (sign)) - sign) & ~(exponent >> 31);
}

inline int Float_To_Int_Floor(const float &f)
{
    int32_t a = *reinterpret_cast<const int32_a*>(&f);
    int32_t sign = (a >> 31);
    a &= 0x7fffffff;
    int32_t exponent = (a >> 23) - 127;
    int32_t expsign = ~(exponent >> 31);
    int32_t imask = ((1 << (31 - (exponent)))) - 1;
    int32_t mantissa = (a & ((1 << 23) - 1));
    int32_t r = ((uint32_t)(mantissa | (1 << 23)) << 8) >> (31 - exponent);
#pragma warning(suppress: 6290) //warning C6290: Bitwise operation on logical result: ! has higher precedence than &. Use && or (!(x & y)) instead
    r = ((r & expsign) ^ (sign)) + ((!((mantissa << 8) & imask) & (expsign ^ ((a - 1) >> 31))) & sign);

    return r;
}

inline float Cos(float val)
{
    return float(cosf(val));   // TODO reimplement based on fdlibm for cross platform reproducibility.
}

inline float Sin(float val)
{
    return float(sinf(val));   // TODO reimplement based on fdlibm for cross platform reproducibility.
}

inline float Sqrt(float val)
{
    return float(sqrtf(val));   // IEEE standard says this is predictable for all conforming implementations.
}

inline float Inv_Sqrt(float val)
{
    return float(1.0f / sqrtf(val));
}

inline float Acos(float val)
{
    return float(acosf(val));
}

inline float Asin(float val)
{
    return float(asinf(val));
}

inline float Atan(float val)
{
    return float(atanf(val));
}

inline float Atan2(float y, float x)
{
    return float(atan2f(y, x));
}

inline float Tan(float val)
{
    return float(tanf(val));
}

inline float Fast_Sin(float val)
{
    val *= float(SIN_TABLE_SIZE) / (2.0f * GAMEMATH_PI);
    int idx0 = Float_To_Int_Floor(val);
    int idx1 = idx0 + 1;
    float frac = val - float(idx0);
    idx0 = (unsigned(idx0)) & (SIN_TABLE_SIZE - 1);
    idx1 = (unsigned(idx1)) & (SIN_TABLE_SIZE - 1);

    return (1.0f - frac) * _FastSinTable[idx0] + frac * _FastSinTable[idx1];
}

inline float Fast_Inv_Sin(float val)
{
    return float(1.0f / Fast_Sin(val));
}

inline float Fast_Cos(float val)
{
    val += (GAMEMATH_PI * 0.5f);
    val *= float(SIN_TABLE_SIZE) / (2.0f * GAMEMATH_PI);
    int idx0 = Float_To_Int_Floor(val);
    int idx1 = idx0 + 1;
    float frac = val - float(idx0);
    idx0 = (unsigned(idx0)) & (SIN_TABLE_SIZE - 1);
    idx1 = (unsigned(idx1)) & (SIN_TABLE_SIZE - 1);

    return (1.0f - frac) * _FastSinTable[idx0] + frac * _FastSinTable[idx1];
}

inline float Fast_Inv_Cos(float val)
{
    return float(1.0f / Fast_Cos(val));
}

inline float Fast_Acos(float val)
{
    // Presumably the table method isn't accurate enough in this range.
    if ( Fabs(val) > 0.975f ) {
        return Acos(val);
    }

    val *= float(ARC_TABLE_SIZE / 2);
    int idx0 = Float_To_Int_Floor(val);
    int idx1 = idx0 + 1;
    float frac = val - float(idx0);
    idx0 += ARC_TABLE_SIZE / 2;
    idx1 += ARC_TABLE_SIZE / 2;

    ASSERT_PRINT((idx0 >= 0) && (idx0 < ARC_TABLE_SIZE), "Index out of table range");
    ASSERT_PRINT((idx1 >= 0) && (idx1 < ARC_TABLE_SIZE), "Index out of table range");

    return (1.0f - frac) * _FastAcosTable[idx0] + frac * _FastAcosTable[idx1];
}

inline float Fast_Asin(float val)
{
    // Presumably the table method isn't accurate enough in this range.
    if ( Fabs(val) > 0.975f ) {
        return Asin(val);
    }

    val *= float(ARC_TABLE_SIZE / 2);
    int idx0 = Float_To_Int_Floor(val);
    int idx1 = idx0 + 1;
    float frac = val - float(idx0);
    idx0 += ARC_TABLE_SIZE / 2;
    idx1 += ARC_TABLE_SIZE / 2;

    ASSERT_PRINT((idx0 >= 0) && (idx0 < ARC_TABLE_SIZE), "Index out of table range");
    ASSERT_PRINT((idx1 >= 0) && (idx1 < ARC_TABLE_SIZE), "Index out of table range");

    return (1.0f - frac) * _FastAsinTable[idx0] + frac * _FastAsinTable[idx1];
}

inline float Sign(float val)
{
    if ( val > 0.0f ) {
        return +1.0f;
    }

    if ( val < 0.0f ) {
        return -1.0f;
    }

    return 0.0f;
}

inline float Ceil(float val)
{
    return ceilf(val);
}

inline float Floor(float val) {
    return floorf(val);
}

inline bool Fast_Is_Float_Positive(const float &val)
{
    return ((*reinterpret_cast<uint32_a const *>(&val)) & 0x80000000) == 0;
}

inline float Random_Float()
{
    return float((float(rand() & 0xFFF)) / float(0xFFF));
}

inline float Random_Float(float min, float max)
{
    return float(Random_Float() * (max - min) + min);
}

inline float Wrap(float val, float min = 0.0f, float max = 1.0f)
{
    if ( val >= max ) {
        val -= (max - min);
    }

    if ( val < min ) {
        val += (max - min);
    }

    if ( val < min ) {
        val = min;
    }

    if ( val > max ) {
        val = max;
    }

    return float(val);
}

//inline double Wrap(double val, double min = 0.0f, double max = 1.0f)
//{
//    if ( val >= max ) {
//        val -= (max - min);
//    }
//
//    if ( val < min ) {
//        val += (max - min);
//    }
//
//    if ( val < min ) {
//        val = min;
//    }
//
//    if ( val > max ) {
//        val = max;
//    }
//
//    return static_cast<double>(val);
//}

inline float Lerp(float a, float b, float lerp)
{
    return float(a + (b - a) * lerp);
}

// Do we want any double math?
//inline double Lerp(double a, double b, float lerp)
//{
//    return (a + (b - a) * lerp);
//}

inline int Lerp(int a, int b, float lerp)
{
    return (a + int((b - a) * lerp));
}


inline int Float_To_Long(float f)
{
#ifdef PROCESSOR_X86
    return _mm_cvtt_ss2si(_mm_load_ss(&f));
#else
    return int(f);
#endif
}

// Do we want any double math?
//inline long Float_To_Long(double f)
//{
//#ifdef PROCESSOR_X86
//    return _mm_cvttsd_si32(_mm_load_pd(&f));
//#else
//    return (int)(f);
//#endif
//}

inline uint8_t Unit_Float_To_Byte(float f)
{
    return uint8_t(f * 255.0f);
}

inline float Byte_To_Unit_Float(unsigned char byte)
{ 
    return float(byte) / 255.0f;
}

inline bool Is_Valid_Float(float x)
{
    uint32_a *plong = reinterpret_cast<uint32_a*>(&x);
    uint32_t exponent = ((*plong) & 0x7F800000) >> (32 - 9);

    if ( exponent == 0xFF ) {
        return false;
    }

    return true;
}

//inline bool Is_Valid_Double(double x)
//{
//    uint32_a *plong = reinterpret_cast<uint32_a*>(&x) + 1;
//    uint32_t exponent = ((*plong) & 0x7FF00000) >> (32 - 12);
//
//    if ( exponent == 0x7FF ) {
//        return false;
//    }
//
//    return true;
//}

} // namespace GameMath

#endif

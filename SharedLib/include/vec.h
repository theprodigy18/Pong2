#pragma once

#include <smmintrin.h>

typedef __m128 float4;

typedef union _Vec2
{
    struct
    {
        f32 x, y;
    };
    f32 v[2];
} Vec2;

typedef union _Vec3
{
    struct
    {
        f32 x, y, z;
    };
    f32 v[3];
} Vec3;

typedef union ALIGN(16) _Vec4
{
    float4 _data; // Don't ever use this manually.
    struct
    {
        f32 x, y, z, w;
    };
    f32 v[4];
} Vec4;

// Recommended to use this factory function for Vec4 instead of manual creation.
static inline Vec4
Vec4_Create(f32 x, f32 y, f32 z, f32 w)
{
    return (Vec4) {._data = _mm_set_ps(w, z, y, x)};
}

_Static_assert(sizeof(Vec4) == 16, "Vec4 must be 16 bytes.");
_Static_assert(_Alignof(Vec4) == 16, "Vec4 must be 16 byte aligned.");
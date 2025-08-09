#pragma once

#include "vec.h"

typedef struct ALIGN(16) _Mat4
{
    Vec4 m[4];
} Mat4;

static inline Mat4 Mat4_Identity()
{
    Mat4 result;
    result.m[0] = Vec4_Create(1.0f, 0.0f, 0.0f, 0.0f);
    result.m[1] = Vec4_Create(0.0f, 1.0f, 0.0f, 0.0f);
    result.m[2] = Vec4_Create(0.0f, 0.0f, 1.0f, 0.0f);
    result.m[3] = Vec4_Create(0.0f, 0.0f, 0.0f, 1.0f);
    return result;
}

static inline Mat4 Mat4_Translate(const Vec3* pVec)
{
    Mat4 result;
    result.m[0] = Vec4_Create(1.0f, 0.0f, 0.0f, 0.0f);
    result.m[1] = Vec4_Create(0.0f, 1.0f, 0.0f, 0.0f);
    result.m[2] = Vec4_Create(0.0f, 0.0f, 1.0f, 0.0f);
    result.m[3] = Vec4_Create(pVec->x, pVec->y, pVec->z, 1.0f);
    return result;
}

static inline Mat4 Mat4_Scale(const Vec3* pVec)
{
    Mat4 result;
    result.m[0] = Vec4_Create(pVec->x, 0.0f, 0.0f, 0.0f);
    result.m[1] = Vec4_Create(0.0f, pVec->y, 0.0f, 0.0f);
    result.m[2] = Vec4_Create(0.0f, 0.0f, pVec->z, 0.0f);
    result.m[3] = Vec4_Create(0.0f, 0.0f, 0.0f, 1.0f);
    return result;
}

static inline Mat4 Mat4_ScaleUniform(f32 scale)
{
    Mat4 result;
    result.m[0] = Vec4_Create(scale, 0.0f, 0.0f, 0.0f);
    result.m[1] = Vec4_Create(0.0f, scale, 0.0f, 0.0f);
    result.m[2] = Vec4_Create(0.0f, 0.0f, scale, 0.0f);
    result.m[3] = Vec4_Create(0.0f, 0.0f, 0.0f, 1.0f);
    return result;
}

static inline Mat4 Mat4_Transpose(const Mat4* pMat)
{
    float4 row0 = pMat->m[0]._data;
    float4 row1 = pMat->m[1]._data;
    float4 row2 = pMat->m[2]._data;
    float4 row3 = pMat->m[3]._data;

    // Unpack pairs of rows.
    float4 tmp0 = _mm_unpacklo_ps(row0, row1); // [x0 x1 y0 y1]
    float4 tmp1 = _mm_unpackhi_ps(row0, row1); // [z0 z1 w0 w1]
    float4 tmp2 = _mm_unpacklo_ps(row2, row3); // [x2 x3 y2 y3]
    float4 tmp3 = _mm_unpackhi_ps(row2, row3); // [z2 z3 w2 w3]

    Mat4 result;

    // Final assembly.
    result.m[0]._data = _mm_movelh_ps(tmp0, tmp2); // [x0 x1 x2 x3]
    result.m[1]._data = _mm_movehl_ps(tmp2, tmp0); // [y0 y1 y2 y3]
    result.m[2]._data = _mm_movelh_ps(tmp1, tmp3); // [z0 z1 z2 z3]
    result.m[3]._data = _mm_movehl_ps(tmp3, tmp1); // [w0 w1 w2 w3]

    return result;
}

static inline Vec4 Mat4_MultiplyVec4(const Mat4* pMat, const Vec4* pVec)
{
    return Vec4_Create(
        _mm_cvtss_f32(_mm_dp_ps(pMat->m[0]._data, pVec->_data, 0b11110001)),
        _mm_cvtss_f32(_mm_dp_ps(pMat->m[1]._data, pVec->_data, 0b11110001)),
        _mm_cvtss_f32(_mm_dp_ps(pMat->m[2]._data, pVec->_data, 0b11110001)),
        _mm_cvtss_f32(_mm_dp_ps(pMat->m[3]._data, pVec->_data, 0b11110001)));
}

static inline Mat4 Mat4_MultiplyMat4(const Mat4* pMatA, const Mat4* pMatB)
{
    Mat4 t = Mat4_Transpose(pMatB);

    Mat4 result;
    for (i32 i = 0; i < 4; ++i)
    {
        result.m[i]._data = _mm_set_ps(
            _mm_cvtss_f32(_mm_dp_ps(pMatA->m[i]._data, t.m[3]._data, 0b11110001)),
            _mm_cvtss_f32(_mm_dp_ps(pMatA->m[i]._data, t.m[2]._data, 0b11110001)),
            _mm_cvtss_f32(_mm_dp_ps(pMatA->m[i]._data, t.m[1]._data, 0b11110001)),
            _mm_cvtss_f32(_mm_dp_ps(pMatA->m[i]._data, t.m[0]._data, 0b11110001)));
    }

    return result;
}
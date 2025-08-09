#include "pch.h"
#include "Resources/MeshBuffer.h"

#include "Graphics/Graphics.h"

#pragma region INTERNAL
static u32 SizeofGLType(GLenum type)
{
    switch (type)
    {
    case GL_FLOAT:
    case GL_INT:
    case GL_UNSIGNED_INT:
        return 4;
    default:
        return 0;
    }
}
#pragma endregion

bool DROP_PackVertex(
    const void* pVertices, u32 vertexStride, u32 vertexCount,
    const VertexAttribLayout* pLayout, u32 layoutCount,
    PackData* pData, VertexAttribLayout** ppLayout)
{
    ASSERT_MSG(pVertices, "Vertex data is null.");
    ASSERT_MSG(pLayout, "Vertex layout is null.");
    ASSERT_MSG(pData, "PackData output is null.");
    ASSERT_MSG(ppLayout, "Layout output is null.");

    *pData    = NULL;
    *ppLayout = NULL;

    // Create new layout.
    VertexAttribLayout* pNewLayout = ALLOC(VertexAttribLayout, layoutCount);
    ASSERT(pNewLayout);

    u32 packedStride = 0;
    for (u32 i = 0; i < layoutCount; ++i)
    {
        pNewLayout[i]        = pLayout[i];
        pNewLayout[i].offset = packedStride;
        packedStride += pLayout[i].count * SizeofGLType(pLayout[i].type);
    }

    // Allocation for raw buffer.
    u8* pPackedBuffer = ALLOC(u8, vertexCount * packedStride);
    ASSERT(pPackedBuffer);

    const u8* src = (const u8*) pVertices;
    for (u32 v = 0; v < vertexCount; ++v)
    {
        for (u32 a = 0; a < layoutCount; ++a)
        {
            const VertexAttribLayout* pAttrib = &pLayout[a];

            // Calculate the address of src and dst.
            const u8* pSrcAttrib = src + pAttrib->offset;
            u8*       pDstAttrib = pPackedBuffer + (v * packedStride) + pNewLayout[a].offset;

            // Copy the value to dst.
            u32 bytesToCopy = pAttrib->count * SizeofGLType(pAttrib->type);
            MEM_COPY(pDstAttrib, pSrcAttrib, bytesToCopy);
        }

        src += vertexStride;
    }

    // Alloc packed data.
    PackData data = ALLOC(_PackData, 1);
    ASSERT(data);
    data->pData  = pPackedBuffer;
    data->count  = vertexCount;
    data->stride = packedStride;

    *pData    = data;
    *ppLayout = pNewLayout;

    return true;
}

bool DROP_CreateVertexBuffer(const PackData data, GLuint* pVBO)
{
    ASSERT_MSG(data && data->pData, "Packed data is null.");
    ASSERT_MSG(pVBO, "VBO output is null.");

    glGenBuffers(1, pVBO);
    glBindBuffer(GL_ARRAY_BUFFER, *pVBO);
    glBufferData(GL_ARRAY_BUFFER, data->count * data->stride, data->pData, GL_STATIC_DRAW);

    // if (glGetError() != GL_NO_ERROR)
    // {
    //     ASSERT_MSG(false, "Failed to create VBO.");
    //     glDeleteBuffers(1, pVBO);
    //     *pVBO = 0;
    //     return false;
    // }

    return true;
}

bool DROP_CreateVertexArray(GLuint vbo, u32 stride, const VertexAttribLayout* pLayout, u32 layoutCount, GLuint* pVAO)
{
    ASSERT_MSG(pVAO, "VAO output is null.");

    glGenVertexArrays(1, pVAO);
    glBindVertexArray(*pVAO);

    if (pLayout && vbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        for (u32 i = 0; i < layoutCount; ++i)
        {
            glEnableVertexAttribArray(pLayout[i].location);
            glVertexAttribPointer(
                pLayout[i].location,
                pLayout[i].count,
                pLayout[i].type,
                pLayout[i].normalized,
                stride,
                (const void*) (u64) pLayout[i].offset);
        }
    }

    glBindVertexArray(0);
    // if (glGetError() != GL_NO_ERROR)
    // {
    //     ASSERT_MSG(false, "Failed to create VAO.");
    //     glDeleteVertexArrays(1, pVAO);
    //     *pVAO = 0;
    //     return false;
    // }

    return true;
}

bool DROP_CreateIndexBuffer(GLuint vao, const u32* pIndices, u32 count, GLuint* pIBO)
{
    ASSERT_MSG(vao, "VAO is null.");
    ASSERT_MSG(pIndices, "Index data is null.");
    ASSERT_MSG(pIBO, "IBO output is null.");

    glBindVertexArray(vao);

    glGenBuffers(1, pIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), pIndices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    // if (glGetError() != GL_NO_ERROR)
    // {
    //     ASSERT_MSG(false, "Failed to create IBO.");
    //     glDeleteBuffers(1, pIBO);
    //     *pIBO = 0;
    //     return false;
    // }

    return true;
}

void DROP_DestroyPackData(PackData* pData)
{
    ASSERT_MSG(pData && *pData, "Packed data is null.");
    PackData data = *pData;

    if (data)
    {
        FREE((void*) data->pData);

        data->pData  = NULL;
        data->count  = 0;
        data->stride = 0;

        FREE(data);
    }

    *pData = NULL;
}

void DROP_DestroyLayout(VertexAttribLayout** ppLayout)
{
    ASSERT_MSG(ppLayout && *ppLayout, "Layout is null.");
    VertexAttribLayout* pLayout = *ppLayout;

    if (pLayout)
    {
        FREE(pLayout);
    }

    *ppLayout = NULL;
}
#include "pch.h"
#include "EntryPoint.h"
#include "Platform/Window.h"
#include "Graphics/Graphics.h"
#include "Resources/Shader.h"
#include "Resources/MeshBuffer.h"

#include <math.h>

// Get current time.
static f64 GetCurrentTimeMs()
{
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (f64) (counter.QuadPart * 1000.0) / frequency.QuadPart;
}

// Memory debugging global variables.
#ifdef DEBUG
Allocation* g_allocations = NULL;
u64         g_totalAllocs = 0;
i32         g_allocCount  = 0;
#endif // DEBUG

// Main loop control.
static bool g_running = true;

// Window callbacks.
static bool OnClose()
{
    PostQuitMessage(0);
    g_running = false;
    return true;
}

// Constant var.
static const f32 TWO_PI = 6.28318531f;

// Global var.
static f32 g_phase = 0.0f;

// Shaders.
static const char* BASIC_SHADERS[]       = {"assets/shaders/basic.vert", "assets/shaders/basic.frag"};
static const char* TONEMAP_SHADERS[]     = {"assets/shaders/tonemap.vert", "assets/shaders/tonemap.frag"};
static const char* BRIGHT_PASS_SHADERS[] = {"assets/shaders/bright_pass.vert", "assets/shaders/bright_pass.frag"};
static const char* BLUR_SHADERS[]        = {"assets/shaders/blur.vert", "assets/shaders/blur.frag"};
static GLint       g_basicProgram        = -1;
static GLint       g_tonemapProgram      = -1;
static GLint       g_brightPassProgram   = -1;
static GLint       g_blurProgram         = -1;
static Mesh        g_rectMesh;
static Mesh        g_fullscreenQuadMesh;

static bool CreateAllShaderPrograms()
{
    // Basic shader.
    g_basicProgram = DROP_CreateShaderProgram(BASIC_SHADERS[0], BASIC_SHADERS[1]);
    if (g_basicProgram == -1)
    {
        ASSERT_MSG(false, "Failed to load basic shader.");
        return false;
    }

    g_tonemapProgram = DROP_CreateShaderProgram(TONEMAP_SHADERS[0], TONEMAP_SHADERS[1]);
    if (g_tonemapProgram == -1)
    {
        ASSERT_MSG(false, "Failed to load tonemap shader.");
        DROP_DestroyShaderProgram(g_basicProgram);
        g_basicProgram = -1;
        return false;
    }

    g_brightPassProgram = DROP_CreateShaderProgram(BRIGHT_PASS_SHADERS[0], BRIGHT_PASS_SHADERS[1]);
    if (g_brightPassProgram == -1)
    {
        ASSERT_MSG(false, "Failed to load bright pass shader.");
        DROP_DestroyShaderProgram(g_basicProgram);
        g_basicProgram = -1;
        DROP_DestroyShaderProgram(g_tonemapProgram);
        g_tonemapProgram = -1;
        return false;
    }

    g_blurProgram = DROP_CreateShaderProgram(BLUR_SHADERS[0], BLUR_SHADERS[1]);
    if (g_blurProgram == -1)
    {
        ASSERT_MSG(false, "Failed to load blur shader.");
        DROP_DestroyShaderProgram(g_basicProgram);
        g_basicProgram = -1;
        DROP_DestroyShaderProgram(g_tonemapProgram);
        g_tonemapProgram = -1;
        DROP_DestroyShaderProgram(g_brightPassProgram);
        g_brightPassProgram = -1;
        return false;
    }

    return true;
}

static void DestroyAllShaderPrograms()
{
    DROP_DestroyShaderProgram(g_basicProgram);
    DROP_DestroyShaderProgram(g_tonemapProgram);
    DROP_DestroyShaderProgram(g_brightPassProgram);
    DROP_DestroyShaderProgram(g_blurProgram);
}

static bool CreateAllMeshBuffers()
{
    // Create rect mesh.
    Vertex rectVertices[] = {
        {.pos = {-0.5f, 0.5f}},
        {.pos = {-0.5f, -0.5f}},
        {.pos = {0.5f, -0.5f}},
        {.pos = {0.5f, 0.5f}}};

    VertexAttribLayout rectLayout[] = {
        {.location = 0, .count = 2, .type = GL_FLOAT, .normalized = false, .offset = 0}};

    PackData            rectPackedData    = NULL;
    VertexAttribLayout* pRectPackedLayout = NULL;
    if (!DROP_PackVertex(
            (const void*) rectVertices,
            sizeof(Vertex), ARRAY_COUNT(rectVertices),
            rectLayout, ARRAY_COUNT(rectLayout),
            &rectPackedData, &pRectPackedLayout))
    {
        ASSERT_MSG(false, "Failed to pack rect mesh.");
        return false;
    }

    GLuint vbo = 0;
    if (!DROP_CreateVertexBuffer(rectPackedData, &vbo))
    {
        ASSERT_MSG(false, "Failed to create rect mesh VBO.");
        DROP_DestroyPackData(&rectPackedData);
        DROP_DestroyLayout(&pRectPackedLayout);
        return false;
    }

    GLuint vao = 0;
    if (!DROP_CreateVertexArray(
            vbo, rectPackedData->stride,
            pRectPackedLayout, ARRAY_COUNT(rectLayout),
            &vao))
    {
        ASSERT_MSG(false, "Failed to create rect mesh VAO.");
        DROP_DestroyPackData(&rectPackedData);
        DROP_DestroyLayout(&pRectPackedLayout);
        glDeleteBuffers(1, &vbo);
        return false;
    }

    DROP_DestroyPackData(&rectPackedData);
    DROP_DestroyLayout(&pRectPackedLayout);

    const u32 indices[] = {0, 1, 2, 2, 3, 0};

    GLuint ibo = 0;
    if (!DROP_CreateIndexBuffer(
            vao, indices,
            ARRAY_COUNT(indices), &ibo))
    {
        ASSERT_MSG(false, "Failed to create rect mesh IBO.");
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        return false;
    }

    // Create fullscreen quad mesh.
    Vertex fullscreenQuadVertices[] = {
        {.pos = {-1.0f, 1.0f}, .uv = {0.0f, 1.0f}},
        {.pos = {-1.0f, -1.0f}, .uv = {0.0f, 0.0f}},
        {.pos = {1.0f, -1.0f}, .uv = {1.0f, 0.0f}},
        {.pos = {1.0f, 1.0f}, .uv = {1.0f, 1.0f}}};

    VertexAttribLayout fullscreenQuadLayout[] = {
        {.location = 0, .count = 2, .type = GL_FLOAT, .normalized = false, .offset = 0},
        {.location = 1, .count = 2, .type = GL_FLOAT, .normalized = false, .offset = 12}};

    PackData            fullscreenQuadPackedData    = NULL;
    VertexAttribLayout* pFullscreenQuadPackedLayout = NULL;
    if (!DROP_PackVertex(
            (const void*) fullscreenQuadVertices,
            sizeof(Vertex), ARRAY_COUNT(fullscreenQuadVertices),
            fullscreenQuadLayout, ARRAY_COUNT(fullscreenQuadLayout),
            &fullscreenQuadPackedData, &pFullscreenQuadPackedLayout))
    {
        ASSERT_MSG(false, "Failed to pack fullscreen quad mesh.");
        return false;
    }

    GLuint fullscreenQuadVbo = 0;
    if (!DROP_CreateVertexBuffer(
            fullscreenQuadPackedData, &fullscreenQuadVbo))
    {
        ASSERT_MSG(false, "Failed to create fullscreen quad mesh VBO.");
        DROP_DestroyPackData(&fullscreenQuadPackedData);
        DROP_DestroyLayout(&pFullscreenQuadPackedLayout);
        return false;
    }

    GLuint fullscreenQuadVao = 0;
    if (!DROP_CreateVertexArray(
            fullscreenQuadVbo, fullscreenQuadPackedData->stride,
            pFullscreenQuadPackedLayout, ARRAY_COUNT(fullscreenQuadLayout),
            &fullscreenQuadVao))
    {
        ASSERT_MSG(false, "Failed to create fullscreen quad mesh VAO.");
        DROP_DestroyPackData(&fullscreenQuadPackedData);
        DROP_DestroyLayout(&pFullscreenQuadPackedLayout);
        glDeleteBuffers(1, &fullscreenQuadVbo);
        return false;
    }

    DROP_DestroyPackData(&fullscreenQuadPackedData);
    DROP_DestroyLayout(&pFullscreenQuadPackedLayout);

    GLuint fullscreenQuadIbo = 0;
    if (!DROP_CreateIndexBuffer(
            fullscreenQuadVao, indices,
            ARRAY_COUNT(indices), &fullscreenQuadIbo))
    {
        ASSERT_MSG(false, "Failed to create fullscreen quad mesh IBO.");
        glDeleteVertexArrays(1, &fullscreenQuadVao);
        glDeleteBuffers(1, &fullscreenQuadVbo);
        return false;
    }

    // Rect mesh.
    g_rectMesh.vao = vao;
    g_rectMesh.vbo = vbo;
    g_rectMesh.ibo = ibo;

    // Fullscreen quad mesh.
    g_fullscreenQuadMesh.vao = fullscreenQuadVao;
    g_fullscreenQuadMesh.vbo = fullscreenQuadVbo;
    g_fullscreenQuadMesh.ibo = fullscreenQuadIbo;

    return true;
}

static void DestroyAllMeshBuffers()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &g_rectMesh.vao);
    glDeleteBuffers(1, &g_rectMesh.ibo);
    glDeleteBuffers(1, &g_rectMesh.vbo);

    glDeleteVertexArrays(1, &g_fullscreenQuadMesh.vao);
    glDeleteBuffers(1, &g_fullscreenQuadMesh.ibo);
    glDeleteBuffers(1, &g_fullscreenQuadMesh.vbo);
}

int EntryPoint()
{
    WndInitProps wndProps = {
        .title    = L"Pong Game",
        .width    = 1280,
        .height   = 720,
        .callback = {
            .OnClose = OnClose}};

    WndHandle wndHandle = NULL;
    if (!DROP_CreateWindow(&wndProps, &wndHandle) || !wndHandle)
    {
        ASSERT_MSG(false, "Failed to create window.");
        return 1;
    }

    GfxInitProps gfxProps = {.wndHandle = wndHandle};

    GfxHandle gfxHandle = NULL;
    if (!DROP_CreateGraphics(&gfxProps, &gfxHandle) || !gfxHandle)
    {
        ASSERT_MSG(false, "Failed to create graphics.");
        DROP_DestroyWindow(&wndHandle);
        return 1;
    }

    wglMakeCurrent(gfxHandle->hdc, gfxHandle->context);
    GfxFramebuffer framebuffer;
    if (!DROP_CreateHDRFramebuffer(gfxHandle, wndProps.width, wndProps.height, &framebuffer))
    {
        ASSERT_MSG(false, "Failed to create framebuffer.");
        DROP_DestroyGraphics(&gfxHandle);
        DROP_DestroyWindow(&wndHandle);
        return 1;
    }
    GfxFramebuffer brightPassFB;
    if (!DROP_CreateHDRFramebuffer(gfxHandle, wndProps.width, wndProps.height, &brightPassFB))
    {
        ASSERT_MSG(false, "Failed to create framebuffer.");
        DROP_DestroyFramebuffer(&framebuffer);
        DROP_DestroyGraphics(&gfxHandle);
        DROP_DestroyWindow(&wndHandle);
        return 1;
    }
    GfxFramebuffer blurFB;
    if (!DROP_CreateHDRFramebuffer(gfxHandle, wndProps.width, wndProps.height, &blurFB))
    {
        ASSERT_MSG(false, "Failed to create framebuffer.");
        DROP_DestroyFramebuffer(&framebuffer);
        DROP_DestroyFramebuffer(&brightPassFB);
        DROP_DestroyGraphics(&gfxHandle);
        DROP_DestroyWindow(&wndHandle);
        return 1;
    }

    GLuint hdrLoc   = glGetUniformLocation(g_tonemapProgram, "hdr");
    GLuint bloomLoc = glGetUniformLocation(g_tonemapProgram, "bloom");

    CreateAllShaderPrograms();
    CreateAllMeshBuffers();

    ShowWindow(wndHandle->hwnd, SW_SHOW);

    static f64 lastTime = 0.0f;
    f64        currTime = GetCurrentTimeMs();
    lastTime            = currTime;

    while (g_running)
    {
        // Calculate delta time.
        currTime      = GetCurrentTimeMs();
        f32 deltaTime = (f32) (currTime - lastTime) / 1000.0f;

        Drop_PollEvents();

        g_phase += 1.0f * deltaTime;
        if (g_phase > TWO_PI)
            g_phase -= TWO_PI;

        f32 intensity = (sinf(g_phase) * 0.5f + 0.5f) * 0.02f;
        intensity     = 0.0f;

        wglMakeCurrent(gfxHandle->hdc, gfxHandle->context);

        // Step 1: Render to HDR framebuffer.
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.framebuffer);
        glViewport(0, 0, wndProps.width, wndProps.height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(g_basicProgram);
        glBindVertexArray(g_rectMesh.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        // Step 2: Bright pass.
        glBindFramebuffer(GL_FRAMEBUFFER, brightPassFB.framebuffer);
        glViewport(0, 0, wndProps.width, wndProps.height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(g_brightPassProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, framebuffer.colorTexture);
        glUniform1i(glGetUniformLocation(g_brightPassProgram, "hdr"), 0);
        glBindVertexArray(g_fullscreenQuadMesh.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        // Blit bright pass to the screen.
        glBindFramebuffer(GL_READ_FRAMEBUFFER, brightPassFB.framebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, wndProps.width, wndProps.height,
                          0, 0, wndProps.width, wndProps.height,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);

        SwapBuffers(gfxHandle->hdc);
        lastTime = currTime;
    }

    ShowWindow(wndHandle->hwnd, SW_HIDE);

    DestroyAllMeshBuffers();
    DestroyAllShaderPrograms();

    DROP_DestroyFramebuffer(&framebuffer);
    DROP_DestroyGraphics(&gfxHandle);
    DROP_DestroyWindow(&wndHandle);

    PRINT_LEAKS();
    DEBUG_CLEANUP();
    return 0;
}
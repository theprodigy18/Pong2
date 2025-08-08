#include "pch.h"
#include "EntryPoint.h"
#include "Platform/Window.h"
#include "Graphics/Graphics.h"

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
static const f32 PI     = 3.14159265f;
static const f32 TWO_PI = 6.28318531f;

// Global var.
static f32 g_phase = 0.0f;

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

        glClearColor(intensity, intensity, intensity, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SwapBuffers(gfxHandle->hdc);
        lastTime = currTime;
    }

    DROP_DestroyGraphics(&gfxHandle);
    DROP_DestroyWindow(&wndHandle);

    PRINT_LEAKS();
    DEBUG_CLEANUP();
    return 0;
}
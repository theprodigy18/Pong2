#pragma once

typedef struct _WndHandle
{
    HWND hwnd;
    HDC  hdc;
} _WndHandle;

typedef _WndHandle* WndHandle;

typedef struct _WndCallback
{
    bool (*OnClose)();
} WndCallback;

typedef struct _WndInitProps
{
    const wchar_t* title;
    i32            width;
    i32            height;
    WndCallback    callback;
} WndInitProps;

bool DROP_CreateWindow(const WndInitProps* pProps, WndHandle* pHandle);
void DROP_DestroyWindow(WndHandle* pHandle);

void Drop_PollEvents();
#include "pch.h"
#include "Platform/Window.h"

#pragma region INTERNAL
static LRESULT CALLBACK InternalWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    WndCallback* callback = (WndCallback*) GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    if (!callback)
    {
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    bool result = false;

    switch (msg)
    {
    case WM_CLOSE:
    {
        result = callback->OnClose();
        break;
    }
    default:
        break;
    }

    return result ? 0 : DefWindowProcW(hwnd, msg, wParam, lParam);
}

static LRESULT CALLBACK TempWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR) InternalWndProc);

        CREATESTRUCTW* cs       = (CREATESTRUCTW*) lParam;
        WndCallback*   callback = (WndCallback*) cs->lpCreateParams;

        if (!callback)
        {
            LOG_ERROR("Window callback is null.");
            return DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR) callback);
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

static bool      s_isInitialized = false;
static HINSTANCE s_hInstance     = NULL;
static wchar_t*  s_szClassName   = L"PONG_GAME_WINDOW_CLASS";
static i32       s_wndCount      = 0;
#pragma endregion

bool DROP_CreateWindow(const WndInitProps* pProps, WndHandle* pHandle)
{
    ASSERT_MSG(pProps, "Window properties are null.");
    *pHandle = NULL;

    if (!s_isInitialized)
    {
        SetProcessDPIAware();

        s_hInstance = GetModuleHandleW(NULL);
        if (!s_hInstance)
        {
            ASSERT_MSG(false, "Failed to get module handle.");
            return false;
        }

        WNDCLASSEXW wcex = {
            .cbSize        = sizeof(WNDCLASSEXW),
            .style         = CS_OWNDC,
            .hInstance     = s_hInstance,
            .cbClsExtra    = 0,
            .cbWndExtra    = 0,
            .hCursor       = LoadCursorW(NULL, IDC_ARROW),
            .hIcon         = LoadIconW(NULL, IDI_APPLICATION),
            .hIconSm       = LoadIconW(NULL, IDI_APPLICATION),
            .hbrBackground = NULL,
            .lpszMenuName  = NULL,
            .lpszClassName = s_szClassName,
            .lpfnWndProc   = TempWndProc};

        if (!RegisterClassExW(&wcex))
        {
            ASSERT_MSG(false, "Failed to register window class.");
            return false;
        }

        s_isInitialized = true;
    }

    WndCallback* wndCallback = ALLOC(WndCallback, 1);
    ASSERT(wndCallback);
    *wndCallback = pProps->callback;

    DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT  rc      = {0, 0, pProps->width, pProps->height};
    AdjustWindowRect(&rc, dwStyle, FALSE);

    HWND hwnd = CreateWindowExW(
        0, s_szClassName, pProps->title, dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        NULL, NULL, s_hInstance, (void*) wndCallback);
    if (!hwnd)
    {
        ASSERT_MSG(false, "Failed to create window.");
        return false;
    }

    WndHandle handle = ALLOC(_WndHandle, 1);
    ASSERT(handle);

    handle->hwnd = hwnd;
    handle->hdc  = GetDC(hwnd);
    *pHandle     = handle;

    ++s_wndCount;
    return true;
}

void DROP_DestroyWindow(WndHandle* pHandle)
{
    ASSERT_MSG(pHandle && *pHandle, "Window handle is null.");
    WndHandle handle = *pHandle;

    if (handle)
    {
        if (handle->hwnd)
        {
            if (handle->hdc)
            {
                ReleaseDC(handle->hwnd, handle->hdc);
            }

            WndCallback* callback = (WndCallback*) GetWindowLongPtrW(handle->hwnd, GWLP_USERDATA);
            if (callback)
            {
                FREE(callback);
            }

            DestroyWindow(handle->hwnd);
        }

        handle->hwnd = NULL;
        handle->hdc  = NULL;
        FREE(handle);

        --s_wndCount;
    }

    *pHandle = NULL;

    if (s_wndCount <= 0)
    {
        UnregisterClassW(s_szClassName, s_hInstance);

        s_hInstance     = NULL;
        s_szClassName   = NULL;
        s_wndCount      = 0;
        s_isInitialized = false;
    }
}

void Drop_PollEvents()
{
    MSG msg;
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            break;
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}
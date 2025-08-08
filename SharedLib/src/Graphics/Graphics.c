#include "pch.h"
#include "Graphics/Graphics.h"

#include <string.h>

#pragma region INTERNAL
static HMODULE s_openglDLL = NULL;

#define LOAD_GL_FUNCTION(type, name)                      \
    name = (type) wglGetProcAddress(#name);               \
    if (!name)                                            \
    {                                                     \
        name = (type) GetProcAddress(s_openglDLL, #name); \
    }                                                     \
    ASSERT(name);

#pragma region OpenGL Functions Pointers
PFNGLCREATEPROGRAMPROC           glCreateProgram           = NULL;
PFNGLCREATESHADERPROC            glCreateShader            = NULL;
PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation      = NULL;
PFNGLUNIFORM1FPROC               glUniform1f               = NULL;
PFNGLUNIFORM2FVPROC              glUniform2fv              = NULL;
PFNGLUNIFORM3FVPROC              glUniform3fv              = NULL;
PFNGLUNIFORM1IPROC               glUniform1i               = NULL;
PFNGLUNIFORMMATRIX4FVPROC        glUniformMatrix4fv        = NULL;
PFNGLVERTEXATTRIBDIVISORPROC     glVertexAttribDivisor     = NULL;
PFNGLACTIVETEXTUREPROC           glActiveTexture           = NULL;
PFNGLBUFFERSUBDATAPROC           glBufferSubData           = NULL;
PFNGLDRAWARRAYSINSTANCEDPROC     glDrawArraysInstanced     = NULL;
PFNGLBINDFRAMEBUFFERPROC         glBindFramebuffer         = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC  glCheckFramebufferStatus  = NULL;
PFNGLGENFRAMEBUFFERSPROC         glGenFramebuffers         = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC    glFramebufferTexture2D    = NULL;
PFNGLDRAWBUFFERSPROC             glDrawBuffers             = NULL;
PFNGLDELETEFRAMEBUFFERSPROC      glDeleteFramebuffers      = NULL;
PFNGLBLENDFUNCIPROC              glBlendFunci              = NULL;
PFNGLBLENDEQUATIONPROC           glBlendEquation           = NULL;
PFNGLCLEARBUFFERFVPROC           glClearBufferfv           = NULL;
PFNGLSHADERSOURCEPROC            glShaderSource            = NULL;
PFNGLCOMPILESHADERPROC           glCompileShader           = NULL;
PFNGLGETSHADERIVPROC             glGetShaderiv             = NULL;
PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog        = NULL;
PFNGLATTACHSHADERPROC            glAttachShader            = NULL;
PFNGLLINKPROGRAMPROC             glLinkProgram             = NULL;
PFNGLVALIDATEPROGRAMPROC         glValidateProgram         = NULL;
PFNGLGETPROGRAMIVPROC            glGetProgramiv            = NULL;
PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog       = NULL;
PFNGLGENBUFFERSPROC              glGenBuffers              = NULL;
PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays         = NULL;
PFNGLGETATTRIBLOCATIONPROC       glGetAttribLocation       = NULL;
PFNGLBINDVERTEXARRAYPROC         glBindVertexArray         = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer     = NULL;
PFNGLBINDBUFFERPROC              glBindBuffer              = NULL;
PFNGLBINDBUFFERBASEPROC          glBindBufferBase          = NULL;
PFNGLBUFFERDATAPROC              glBufferData              = NULL;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv = NULL;
PFNGLUSEPROGRAMPROC              glUseProgram              = NULL;
PFNGLDELETEVERTEXARRAYSPROC      glDeleteVertexArrays      = NULL;
PFNGLDELETEBUFFERSPROC           glDeleteBuffers           = NULL;
PFNGLDELETEPROGRAMPROC           glDeleteProgram           = NULL;
PFNGLDETACHSHADERPROC            glDetachShader            = NULL;
PFNGLDELETESHADERPROC            glDeleteShader            = NULL;
PFNGLDRAWELEMENTSINSTANCEDPROC   glDrawElementsInstanced   = NULL;
PFNGLGENERATEMIPMAPPROC          glGenerateMipmap          = NULL;
PFNGLDEBUGMESSAGECALLBACKPROC    glDebugMessageCallback    = NULL;
#pragma endregion

void LoadOpenGLFunctions()
{
    s_openglDLL = LoadLibraryA("opengl32.dll");
    ASSERT(s_openglDLL);

    LOAD_GL_FUNCTION(PFNGLCREATEPROGRAMPROC, glCreateProgram);
    LOAD_GL_FUNCTION(PFNGLCREATESHADERPROC, glCreateShader);
    LOAD_GL_FUNCTION(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
    LOAD_GL_FUNCTION(PFNGLUNIFORM1FPROC, glUniform1f);
    LOAD_GL_FUNCTION(PFNGLUNIFORM2FVPROC, glUniform2fv);
    LOAD_GL_FUNCTION(PFNGLUNIFORM3FVPROC, glUniform3fv);
    LOAD_GL_FUNCTION(PFNGLUNIFORM1IPROC, glUniform1i);
    LOAD_GL_FUNCTION(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
    LOAD_GL_FUNCTION(PFNGLVERTEXATTRIBDIVISORPROC, glVertexAttribDivisor);
    LOAD_GL_FUNCTION(PFNGLACTIVETEXTUREPROC, glActiveTexture);
    LOAD_GL_FUNCTION(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
    LOAD_GL_FUNCTION(PFNGLDRAWARRAYSINSTANCEDPROC, glDrawArraysInstanced);
    LOAD_GL_FUNCTION(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
    LOAD_GL_FUNCTION(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus);
    LOAD_GL_FUNCTION(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
    LOAD_GL_FUNCTION(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
    LOAD_GL_FUNCTION(PFNGLDRAWBUFFERSPROC, glDrawBuffers);
    LOAD_GL_FUNCTION(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers);
    LOAD_GL_FUNCTION(PFNGLBLENDFUNCIPROC, glBlendFunci);
    LOAD_GL_FUNCTION(PFNGLBLENDEQUATIONPROC, glBlendEquation);
    LOAD_GL_FUNCTION(PFNGLCLEARBUFFERFVPROC, glClearBufferfv);
    LOAD_GL_FUNCTION(PFNGLSHADERSOURCEPROC, glShaderSource);
    LOAD_GL_FUNCTION(PFNGLCOMPILESHADERPROC, glCompileShader);
    LOAD_GL_FUNCTION(PFNGLGETSHADERIVPROC, glGetShaderiv);
    LOAD_GL_FUNCTION(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
    LOAD_GL_FUNCTION(PFNGLATTACHSHADERPROC, glAttachShader);
    LOAD_GL_FUNCTION(PFNGLLINKPROGRAMPROC, glLinkProgram);
    LOAD_GL_FUNCTION(PFNGLVALIDATEPROGRAMPROC, glValidateProgram);
    LOAD_GL_FUNCTION(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
    LOAD_GL_FUNCTION(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
    LOAD_GL_FUNCTION(PFNGLGENBUFFERSPROC, glGenBuffers);
    LOAD_GL_FUNCTION(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
    LOAD_GL_FUNCTION(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
    LOAD_GL_FUNCTION(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
    LOAD_GL_FUNCTION(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
    LOAD_GL_FUNCTION(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
    LOAD_GL_FUNCTION(PFNGLBINDBUFFERPROC, glBindBuffer);
    LOAD_GL_FUNCTION(PFNGLBINDBUFFERBASEPROC, glBindBufferBase);
    LOAD_GL_FUNCTION(PFNGLBUFFERDATAPROC, glBufferData);
    LOAD_GL_FUNCTION(PFNGLGETVERTEXATTRIBPOINTERVPROC, glGetVertexAttribPointerv);
    LOAD_GL_FUNCTION(PFNGLUSEPROGRAMPROC, glUseProgram);
    LOAD_GL_FUNCTION(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
    LOAD_GL_FUNCTION(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
    LOAD_GL_FUNCTION(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
    LOAD_GL_FUNCTION(PFNGLDETACHSHADERPROC, glDetachShader);
    LOAD_GL_FUNCTION(PFNGLDELETESHADERPROC, glDeleteShader);
    LOAD_GL_FUNCTION(PFNGLDRAWELEMENTSINSTANCEDPROC, glDrawElementsInstanced);
    LOAD_GL_FUNCTION(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap);
    LOAD_GL_FUNCTION(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback);
    LOAD_GL_FUNCTION(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
}

void CALLBACK GLDebugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length, const GLchar* message,
                              const void* userParam)
{
    if (severity == GL_DEBUG_SEVERITY_LOW ||
        severity == GL_DEBUG_SEVERITY_MEDIUM ||
        severity == GL_DEBUG_SEVERITY_HIGH)
    {
        ASSERT_MSG(false, "OpenGL error: %s", message);
    }
    else
    {
        LOG_TRACE((char*) message);
    }
}

PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC    wglChoosePixelFormatARB    = NULL;
PFNWGLGETEXTENSIONSSTRINGARBPROC  wglGetExtensionsStringARB  = NULL;

static bool s_isInitialized = false;
static i32  s_gfxCount      = 0;
#pragma endregion

bool DROP_CreateGraphics(const GfxInitProps* pProps, GfxHandle* pHandle)
{
    ASSERT_MSG(pProps && pProps->wndHandle, "Graphics properties are null.");
    *pHandle = NULL;

    if (!s_isInitialized)
    {
        WndInitProps dummyWndProps = {
            .title    = L"Pong Game Dummy Window",
            .width    = 1280,
            .height   = 720,
            .callback = {.OnClose = NULL}};

        WndHandle dummyWndHandle = NULL;

        if (!DROP_CreateWindow(&dummyWndProps, &dummyWndHandle) || !dummyWndHandle)
        {
            ASSERT_MSG(false, "Failed to create dummy window.");
            return false;
        }

        PIXELFORMATDESCRIPTOR pfd = {
            .nSize        = sizeof(PIXELFORMATDESCRIPTOR),
            .nVersion     = 1,
            .dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            .iPixelType   = PFD_TYPE_RGBA,
            .cColorBits   = 32,
            .cBlueBits    = 8,
            .cGreenBits   = 8,
            .cRedBits     = 8,
            .cAlphaBits   = 8,
            .cDepthBits   = 24,
            .cStencilBits = 8};

        i32 pixelFormat = ChoosePixelFormat(dummyWndHandle->hdc, &pfd);
        if (!pixelFormat)
        {
            ASSERT_MSG(false, "Failed to choose pixel format.");
            DROP_DestroyWindow(&dummyWndHandle);
            return false;
        }

        if (!SetPixelFormat(dummyWndHandle->hdc, pixelFormat, &pfd))
        {
            ASSERT_MSG(false, "Failed to set pixel format.");
            DROP_DestroyWindow(&dummyWndHandle);
            return false;
        }

        HGLRC dummyContext = wglCreateContext(dummyWndHandle->hdc);
        if (!dummyContext)
        {
            ASSERT_MSG(false, "Failed to create dummy context.");
            DROP_DestroyWindow(&dummyWndHandle);
            return false;
        }

        if (!wglMakeCurrent(dummyWndHandle->hdc, dummyContext))
        {
            ASSERT_MSG(false, "Failed to make dummy context current.");
            wglDeleteContext(dummyContext);
            DROP_DestroyWindow(&dummyWndHandle);
            return false;
        }

        LOG_TRACE("OpenGL version: %s", glGetString(GL_VERSION));
        LOG_TRACE("OpenGL vendor: %s", glGetString(GL_VENDOR));
        LOG_TRACE("OpenGL renderer: %s", glGetString(GL_RENDERER));

        // Load OpenGL functions.
        LoadOpenGLFunctions();

        LOAD_GL_FUNCTION(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);
        LOAD_GL_FUNCTION(PFNWGLCHOOSEPIXELFORMATARBPROC, wglChoosePixelFormatARB);
        LOAD_GL_FUNCTION(PFNWGLGETEXTENSIONSSTRINGARBPROC, wglGetExtensionsStringARB);

        if (!wglCreateContextAttribsARB || !wglChoosePixelFormatARB || !wglGetExtensionsStringARB)
        {
            ASSERT_MSG(false, "Failed to load OpenGL functions.");
            wglDeleteContext(dummyContext);
            DROP_DestroyWindow(&dummyWndHandle);
            return false;
        }

        const char* ext = wglGetExtensionsStringARB(dummyWndHandle->hdc);
        if (!ext || !strstr(ext, "WGL_EXT_framebuffer_sRGB"))
        {
            ASSERT_MSG(false, "No support for sRGB.");
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(dummyContext);
            DROP_DestroyWindow(&dummyWndHandle);
            return false;
        }

        glDebugMessageCallback(GLDebugCallback, NULL);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glEnable(GL_DEBUG_OUTPUT);

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(dummyContext);
        DROP_DestroyWindow(&dummyWndHandle);

        s_isInitialized = true;
    }

    WndHandle wndHandle = pProps->wndHandle;

    const GLint pixelAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_SWAP_METHOD_ARB, WGL_SWAP_COPY_ARB,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_RED_BITS_ARB, 8,
        WGL_GREEN_BITS_ARB, 8,
        WGL_BLUE_BITS_ARB, 8,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT, GL_TRUE,
        0};

    i32 pixelFormat = 0;
    u32 numFormats  = 0;
    if (!wglChoosePixelFormatARB(
            wndHandle->hdc, pixelAttribs,
            NULL, 1,
            &pixelFormat, &numFormats) ||
        !numFormats)
    {
        ASSERT_MSG(false, "Failed to choose pixel format.");
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd;
    if (!DescribePixelFormat(wndHandle->hdc, pixelFormat,
                             sizeof(PIXELFORMATDESCRIPTOR), &pfd))
    {
        ASSERT_MSG(false, "Failed to describe pixel format.");
        return false;
    }

    if (!SetPixelFormat(wndHandle->hdc, pixelFormat, &pfd))
    {
        ASSERT_MSG(false, "Failed to set pixel format.");
        return false;
    }

    const GLint contextAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
        0};

    HGLRC context = wglCreateContextAttribsARB(wndHandle->hdc, NULL, contextAttribs);
    if (!context)
    {
        ASSERT_MSG(false, "Failed to create context.");
        return false;
    }

    if (!wglMakeCurrent(wndHandle->hdc, context))
    {
        ASSERT_MSG(false, "Failed to make context current.");
        wglDeleteContext(context);
        return false;
    }

    glEnable(GL_FRAMEBUFFER_SRGB);

    GfxHandle handle = ALLOC(_GfxHandle, 1);
    ASSERT(handle);

    handle->hdc     = wndHandle->hdc;
    handle->context = context;
    *pHandle        = handle;

    ++s_gfxCount;
    return true;
}

void DROP_DestroyGraphics(GfxHandle* pHandle)
{
    ASSERT_MSG(pHandle && *pHandle, "Graphics handle is null.");
    GfxHandle handle = *pHandle;

    if (handle)
    {
        wglMakeCurrent(NULL, NULL);

        if (handle->context)
        {
            wglDeleteContext(handle->context);
        }

        handle->hdc     = NULL;
        handle->context = NULL;

        FREE(handle);
        --s_gfxCount;
    }

    *pHandle = NULL;

    if (s_gfxCount <= 0)
    {
        FreeLibrary(s_openglDLL);

        s_gfxCount      = 0;
        s_isInitialized = false;
    }
}
#pragma once

#include "Platform/Window.h"

typedef struct _GfxHandle
{
    HDC   hdc;
    HGLRC context;
} _GfxHandle;

typedef _GfxHandle* GfxHandle;

typedef struct _GfxInitProps
{
    WndHandle wndHandle;
} GfxInitProps;

typedef struct _GfxFramebuffer
{
    GLuint colorTexture;
    GLuint depthBuffer;
    GLuint framebuffer;
} GfxFramebuffer;

bool DROP_CreateGraphics(const GfxInitProps* pProps, GfxHandle* pHandle);
void DROP_DestroyGraphics(GfxHandle* pHandle);

bool DROP_CreateHDRFramebuffer(const GfxHandle gfxHandle, i32 width, i32 height, GfxFramebuffer* pFramebuffer);
void DROP_DestroyFramebuffer(GfxFramebuffer* pFramebuffer);

extern PFNGLCREATEPROGRAMPROC           glCreateProgram;
extern PFNGLCREATESHADERPROC            glCreateShader;
extern PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation;
extern PFNGLUNIFORM1FPROC               glUniform1f;
extern PFNGLUNIFORM2FVPROC              glUniform2fv;
extern PFNGLUNIFORM3FVPROC              glUniform3fv;
extern PFNGLUNIFORM1IPROC               glUniform1i;
extern PFNGLUNIFORMMATRIX4FVPROC        glUniformMatrix4fv;
extern PFNGLVERTEXATTRIBDIVISORPROC     glVertexAttribDivisor;
extern PFNGLACTIVETEXTUREPROC           glActiveTexture;
extern PFNGLBUFFERSUBDATAPROC           glBufferSubData;
extern PFNGLDRAWARRAYSINSTANCEDPROC     glDrawArraysInstanced;
extern PFNGLBINDFRAMEBUFFERPROC         glBindFramebuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC  glCheckFramebufferStatus;
extern PFNGLGENFRAMEBUFFERSPROC         glGenFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC    glFramebufferTexture2D;
extern PFNGLDRAWBUFFERSPROC             glDrawBuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC      glDeleteFramebuffers;
extern PFNGLBLENDFUNCIPROC              glBlendFunci;
extern PFNGLBLENDEQUATIONPROC           glBlendEquation;
extern PFNGLCLEARBUFFERFVPROC           glClearBufferfv;
extern PFNGLSHADERSOURCEPROC            glShaderSource;
extern PFNGLCOMPILESHADERPROC           glCompileShader;
extern PFNGLGETSHADERIVPROC             glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog;
extern PFNGLATTACHSHADERPROC            glAttachShader;
extern PFNGLLINKPROGRAMPROC             glLinkProgram;
extern PFNGLVALIDATEPROGRAMPROC         glValidateProgram;
extern PFNGLGETPROGRAMIVPROC            glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog;
extern PFNGLGENBUFFERSPROC              glGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays;
extern PFNGLGETATTRIBLOCATIONPROC       glGetAttribLocation;
extern PFNGLBINDVERTEXARRAYPROC         glBindVertexArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer;
extern PFNGLBINDBUFFERPROC              glBindBuffer;
extern PFNGLBINDBUFFERBASEPROC          glBindBufferBase;
extern PFNGLBUFFERDATAPROC              glBufferData;
extern PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv;
extern PFNGLUSEPROGRAMPROC              glUseProgram;
extern PFNGLDELETEVERTEXARRAYSPROC      glDeleteVertexArrays;
extern PFNGLDELETEBUFFERSPROC           glDeleteBuffers;
extern PFNGLDELETEPROGRAMPROC           glDeleteProgram;
extern PFNGLDETACHSHADERPROC            glDetachShader;
extern PFNGLDELETESHADERPROC            glDeleteShader;
extern PFNGLDRAWELEMENTSINSTANCEDPROC   glDrawElementsInstanced;
extern PFNGLGENERATEMIPMAPPROC          glGenerateMipmap;
extern PFNGLDEBUGMESSAGECALLBACKPROC    glDebugMessageCallback;
extern PFNGLGENRENDERBUFFERSPROC        glGenRenderbuffers;
extern PFNGLBINDRENDERBUFFERPROC        glBindRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEPROC     glRenderbufferStorage;
extern PFNGLDELETERENDERBUFFERSPROC     glDeleteRenderbuffers;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
extern PFNGLBLITFRAMEBUFFERPROC         glBlitFramebuffer;

#pragma once

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef signed char      i8;
typedef signed short     i16;
typedef signed int       i32;
typedef signed long long i64;

typedef float  f32;
typedef double f64;

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <opengl/glcorearb.h>
#include <opengl/wglext.h>

#include <GL/gl.h>
#else
#error "Only Windows is supported at the moment."
#endif // PLATFORM_WINDOWS

#include <stdlib.h>
#include <stdbool.h>

#ifdef DEBUG
#include <stdio.h>
#include <stdarg.h>
#include "log.h"
#include "debug_memory.h"
#endif // DEBUG

#include "API_Props.h"

#ifdef DEBUG
#define ALLOC(type, count) (type*) _DebugMalloc(sizeof(type) * (count), __FILE__, __LINE__)
#define FREE(ptr) _DebugFree(ptr, __FILE__, __LINE__)
#define PRINT_LEAKS() _DebugPrintLeaks()
#define DEBUG_CLEANUP() _DebugCleanup()
#else
#define ALLOC(type, count) (type*) malloc(sizeof(type) * (count))
#define FREE(ptr) free(ptr)
#define PRINT_LEAKS()
#define DEBUG_CLEANUP()
#endif // DEBUG

#define ZERO_MEMORY(ptr, count) memset(ptr, 0, sizeof(*ptr) * (count))

#ifdef DEBUG
#ifdef _MSC_VER
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define DEBUG_BREAK() __builtin_trap()
#else
#error "Unsupported compiler."
#endif // _MSC_VER

#define DEBUG_OP(x) x

#define LOG_TRACE(...) _Log("[TRACE]", __FILE__, __LINE__, TEXT_COLOR_GREEN, __VA_ARGS__)
#define LOG_WARN(...) _Log("[WARN]", __FILE__, __LINE__, TEXT_COLOR_YELLOW, __VA_ARGS__)
#define LOG_ERROR(...) _Log("[ERROR]", __FILE__, __LINE__, TEXT_COLOR_RED, __VA_ARGS__)

#define ASSERT(x) \
    if (!(x)) DEBUG_BREAK()

#define ASSERT_MSG(x, ...)      \
    if (!(x))                   \
    {                           \
        LOG_ERROR(__VA_ARGS__); \
        DEBUG_BREAK();          \
    }

#else
#define DEBUG_BREAK()

#define DEBUG_OP(x)

#define LOG_TRACE(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)

#define ASSERT(x)
#define ASSERT_MSG(x, ...)
#endif // DEBUG
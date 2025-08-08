#pragma once

#ifdef PLATFORM_WINDOWS
#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif // DLL_EXPORTS
#else
#error "Only Windows is supported at the moment."
#endif // PLATFORM_WINDOWS
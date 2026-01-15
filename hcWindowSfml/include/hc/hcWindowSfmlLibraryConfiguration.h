#pragma once

// Core export macros
#if HC_PLATFORM == HC_PLATFORM_WIN32
# if HC_COMPILER == HC_COMPILER_MSVC || HC_COMPILER == HC_COMPILER_GNUC || HC_COMPILER == HC_COMPILER_INTEL
#   if defined(HC_STATIC_LIB)
#     define HC_WINDOW_SFML_EXPORT
#   else
#     if defined(HC_WINDOW_SFML_EXPORTS)
#       define HC_WINDOW_SFML_EXPORT __declspec(dllexport)
#     else
#       define HC_WINDOW_SFML_EXPORT __declspec(dllimport)
#     endif
#   endif
# endif
# define HC_WINDOW_SFML_HIDDEN
#else // Linux / Mac
# define HC_WINDOW_SFML_EXPORT __attribute__ ((visibility("default")))
# define HC_WINDOW_SFML_HIDDEN __attribute__ ((visibility("hidden")))
#endif

// DLL export for plug-ins
#if HC_PLATFORM == HC_PLATFORM_WIN32
# if HC_COMPILER == HC_COMPILER_MSVC || HC_COMPILER == HC_COMPILER_GNUC || HC_COMPILER == HC_COMPILER_INTEL
#   define HC_PLUGIN_EXPORT __declspec(dllexport)
# endif
# define HC_PLUGIN_HIDDEN
#else // Linux / Mac
# define HC_PLUGIN_EXPORT __attribute__ ((visibility("default")))
# define HC_PLUGIN_HIDDEN __attribute__ ((visibility("hidden")))
#endif


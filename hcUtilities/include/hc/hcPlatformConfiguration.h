#pragma once

#define HC_PLATFORM_WIN32 1
#define HC_PLATFORM_OSX 2
#define HC_PLATFORM_LINUX 3

#define HC_COMPILER_MSVC 1
#define HC_COMPILER_INTEL 2
#define HC_COMPILER_GNUC 3

#define HC_ARCHITECTURE_x86_64 1
#define HC_ARCHITECTURE_x86_32 2

#define HC_ENDIAN_LITTLE 1
#define HC_ENDIAN_BIG 2

#define HC_ENDIAN_ HC_ENDIAN_LITTLE

#if defined(__clang__)

# define HC_COMPILER HC_COMPILER_MSVC // CLANG
# define HC_COMP_VER __clang_version__
# define HC_THREADLOCAL thread_local
# define HC_STDCALL __attribute__((stdcall))
# define HC_CDECL __attribute__ ((cdecl))

#elif defined(__GNUC__)

# define HC_COMPILER HC_COMPILER_GNUC //GNUC
# define HC_COMP_VER (((__GNUC__)*100) + (__GNUC_MINOR__*10) + __GNUC_PATCHLEVEL__)
# define HC_THREADLOCAL thread_local
# define HC_STDCALL __attribute__((stdcall))
# define HC_CDECL __attribute__ ((cdecl))

#elif defined (__INTEL_COMPILER)

# define HC_COMPILER HC_COMPILER_INTEL // INTEL
# define HC_COMP_VER __INTEL_COMPILER
# define HC_THREADLOCAL thread_local
# define HC_STDCALL __stdcall
# define HC_CDECL __cdecl

#elif defined(_MSC_VER)

# define HC_COMPILER HC_COMPILER_MSVC
# define HC_COMP_VER _MSC_VER
# define HC_THREADLOCAL __declspec(thread)
# define HC_STDCALL __stdcall
# define HC_CDECL __cdecl
# undef __PRETTY_FUNCTION__
# define __PRETTY_FUNCTION__ __FUNCSIG__ 

#else

# error "No known compiler."

#endif

#if HC_COMPILER == HC_COMPILER_MSVC
# if HC_COMP_VER >= 1200
#   define FORCEINLINE __forceinline
#   ifndef RESTRICT
#     define RESTRICT __restrict
#   endif
# endif
#elif defined(__MINGW32__)
# if !defined (FORCEINLINE)
#   define FORCEINLINE __inline
#   ifndef RESTRICT
#     define RESTRICT
#   endif
# endif
#else
# define  FORCEINLINE __inline
#   ifndef RESTRICT
#     define RESTRICT __restrict
#   endif
#endif

#if defined(__WIN32__) || defined(_WIN32)
#define HC_PLATFORM HC_PLATFORM_WIN32
#elif defined( __APPLE_CC__)
#define HC_PLATFORM HC_PLATFORM_OSX
#else
#define HC_PLATFORM HC_PLATFORM_LINUX
#endif

#if defined(__x86_64__) || defined(_M_X64)
# define HC_ARCH_TYPE HC_ARCHITECTURE_x86_64
#else
# define HC_ARCH_TYPE HC_ARCHITECTURE_x86_32
#endif

#if HC_COMPILER == HC_COMPILER_MSVC
# define MS_ALIGN(n)  __declspec(align(n))
# ifdef GCC_PACK
#   define GCC_PACK(n)
# endif
# ifndef GCC_ALIGN
#   define GCC_ALIGN(n)
# endif
#else
# define MS_ALIGN(n)  
# define GCC_PACK(n)   __attribute__((packed, aligned(n)))
# define GCC_ALIGN(n) __attribute__((aligned(n)))
#endif

#if HC_COMPILER == HC_COMPILER_MSVC || HC_COMPILER == HC_COMPILER_INTEL || HC_COMPILER == HC_COMPILER_GNUC
# define _NOEXCEPT noexcept
#else
# define _NOEXCEPT
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
# if defined(_DEBUG) || defined(DEBUG)
#   define HC_DEBUG_MODE 1
# else
#   define HC_DEBUG_MODE 0
# endif
# if HC_COMPILER == HC_COMPILER_INTEL
#   define HC_THREADLOCAL __declspec(thread)
# endif
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_OSX
# define stripcmp strcasecmp
# if defined(_DEBUG) || defined(DEBUG)
#   define HC_DEBUG_MODE 1
# else
#   define HC_DEBUG_MODE 0
# endif
# if HC_COMPILER == HC_COMPILER_INTEL 
#   define HC_THREADLOCAL __thread
# endif
#endif

#include <cassert>
#if HC_DEBUG_MODE
# define HC_DEBUG_ONLY(x) x
# define HC_ASSERT(x) assert(x)
#else
# define HC_DEBUG_ONLY(x)
# define HC_ASSERT(x)
#endif

#if HC_COMPILER == HC_COMPILER_MSVC
# define _CRT_SECURE_NO_WARNINGS
# pragma warning(disable : 4251)
# pragma warning(disable: 4201)
#endif

#if HC_DEBUG_MODE
#define  HC_DYN_LIB_SUFIX "-d.dll"
#else
#define  HC_DYN_LIB_SUFIX ".dll"
#endif

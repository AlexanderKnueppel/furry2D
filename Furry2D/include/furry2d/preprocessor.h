#ifndef __FURRY_PREPROCESSOR_H__
#define __FURRY_PREPROCESSOR_H__  

/*
* ****************************************
* Copyright (c) 2014 Furry Graphics/Game Engine. All rights reserved.
*
* For the full copyright and license information, please view the LICENSE.txt
* file that was distributed with this source code.
*
* \Author Alexander Knueppel         
*
* ****************************************
*/

#if defined(__WIN32__) || defined(_MSC_VER)
#   define FURRY_PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__unix__)
#   define FURRY_PLATFORM_LINUX
#elif defined(__APPLE__) || defined(MACOSX)
#   define FURRY_PLATFORM_MACOSX // unsupported yet
#   error Furry2D is not supported under "Apple MacOSX" yet!
#elif defined(__FreeBSD__)
#   define FURRY_PLATFORM_FREEBSD // unsupported yet
#   error Furry2D is not supported under "FreeBSD" yet!
#elif defined(__sparc__) || defined(__sun__)
#   define FURRY__PLATFORM_SOLARIS // unsupported yet
#   error Furry2D is not supported under "Sun Solaris" yet!
#else
#   error Unknown platform, no support!
#endif

#if defined(_DEBUG) 
#	define FURRY_DEBUG
#else
#	define FURRY_RELEASE
#endif

#ifdef FURRY2D_EXPORTS
#   if defined(FURRY_PLATFORM_WINDOWS)
#       define FURRY_API __declspec(dllexport)
#   elif defined(FURRY_PLATFORM_LINUX)
#       define FURRY_API
#   endif
#else
#   if defined(FURRY_PLATFORM_WINDOWS)
#		define FURRY_API __declspec(dllimport)
#	else
#		define FURRY_API
#   endif
#endif

#if defined(__GNUC__)
#   define FURRY_COMPILER_GCC  // GNU Compiler Collection
#elif defined(_MSC_VER)
#   define FURRY_COMPILER_VC   // VisualC++
#endif

#if defined(FURRY_COMPILER_GCC)
// optimization here
#elif defined(FURRY_COMPILER_VC)
//Only on windows
#	pragma warning( disable: 4251 ) //Warning: needs to have dll-interface to be used by clients, it's a nuisance (template issues...)
// optimization here
#endif

//definition types
#define FURRY_VERSION     0.1

#include <cstdint>
#include <exception>

namespace furry {
}

#endif

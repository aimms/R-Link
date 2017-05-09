/*  This file is part of R-Link.

    R-Link is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    R-Link is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with R-Link.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _INCLUDE_Common_GlobalInclude
#define _INCLUDE_Common_GlobalInclude

// This Common/GlobalInclude.h file is to be included in every file
// it determines and specifies compiler specific settings and general
// purpose macros

// The organization of this file is:
//   I.   Compiler determination (except for adding a new compiler/target you shouldn't touch this)
//   II.  Compiler specific settings/default overrides
//   III. General purpose macros with a compiler specific implementation
//   IV.  General purpose macros

// --
//   SECTION I: Determine compiler
// --
// the Microsoft compiler
#if defined(_MSC_VER) 
	#define _HOST_COMPILER_MICROSOFT
	#define _HOST_COMPILER_DEFINED
	#define _TARGET_PLATFORM_WINDOWS
	#define _TARGET_PLATFORM_DEFINED
	#if defined(_M_X64)
		#define _TARGET_ARCHITECTURE_X64
		#define _TARGET_ARCHITECTURE_DEFINED
	#endif
	#if defined(_M_IX86)
		#define _TARGET_ARCHITECTURE_X86
		#define _TARGET_ARCHITECTURE_DEFINED
	#endif
#endif

// the gnu compiler
#if defined(__GNUC__)
	#define _HOST_COMPILER_GCC
	#define _HOST_COMPILER_DEFINED
	#define _TARGET_PLATFORM_LINUX
	#define _TARGET_PLATFORM_DEFINED
	#if defined(__x86_64__)
		#define _TARGET_ARCHITECTURE_X64
		#define _TARGET_ARCHITECTURE_DEFINED
	#endif
	#if defined(__i386__)
		#define _TARGET_ARCHITECTURE_X86
		#define _TARGET_ARCHITECTURE_DEFINED
	#endif
#endif


#if !defined(_TARGET_PLATFORM_DEFINED)
#error Unable to determine platform.
#endif

#if !defined(_TARGET_ARCHITECTURE_DEFINED)
#error Unable to determine architecture.
#endif 

// --
//   SECTION II: compiler specific settings
// --
#ifdef _HOST_COMPILER_MICROSOFT
    #ifndef WINVER
        #define WINVER 0x0602		// XP and beyond functionality available
    #endif
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0602 // XP and beyond functionality available
    #endif
	#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN // does stuff, specifically stops including winsock.h in windows.h
                                // see http://msdn.microsoft.com/en-us/library/ms737629(VS.85).aspx
	#endif
							
    #define NOMINMAX                // this prohibits definition of min/max in windef.h 
	#ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
	#endif
    #ifndef _CRT_SECURE_NO_DEPRECATE
        #define _CRT_SECURE_NO_DEPRECATE
    #endif
    #ifndef _SCL_SECURE_NO_WARNINGS
        #define _SCL_SECURE_NO_WARNINGS
    #endif
#endif

// --
//   SECTION II: general purpose but compiler specific macro definitions
// --

#ifdef _HOST_COMPILER_GCC
	#define DLLEXPORT     __attribute__ ((visibility("default")))
	#define DLLIMPORT     __attribute__ ((visibility("default")))
	#define DLLINTERFACE  __attribute__ ((visibility("default")))
    #define DLLLOCAL      __attribute__ ((visibility("hidden")))
 	#define FORCEINLINE   __inline__
	#define DEPRECATED(txt) __attribute__((deprecated))
	// note: deprecated must be placed after the function name up to GCC 3.3  
#endif
#ifdef _HOST_COMPILER_MICROSOFT
	#define DLLEXPORT __declspec(dllexport)
	//#define DLLIMPORT __declspec(dllimport)
	#define DLLINTERFACE  
	#define DLLLOCAL
	#define FORCEINLINE __forceinline
	#define DEPRECATED(txt) __declspec(deprecated(txt))
#endif



// --
//   SECTION IV: general purpose macros for any compiler/platform/host
// --

#ifdef _HOST_COMPILER_MICROSOFT
#define GG_LONGLONG(x) x##I64
#define GG_ULONGLONG(x) x##UI64
#else
#define GG_LONGLONG(x) x##LL
#define GG_ULONGLONG(x) x##ULL
#endif

#define GG_INT8_C(x)    (x)
#define GG_INT16_C(x)   (x)
#define GG_INT32_C(x)   (x)
#define GG_INT64_C(x)   GG_LONGLONG(x)

#define GG_UINT8_C(x)   (x ## U)
#define GG_UINT16_C(x)  (x ## U)
#define GG_UINT32_C(x)  (x ## U)
#define GG_UINT64_C(x)  GG_ULONGLONG(x)


#ifdef __cplusplus
template<class T> void _ignore_arg( const T& ) { }
#endif

#ifdef _DEBUG 
    #ifndef DEBUG
	    #define DEBUG
    #endif
#endif

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)


#include <assert.h>

#endif // #ifndef _INCLUDE_Common_GlobalInclude

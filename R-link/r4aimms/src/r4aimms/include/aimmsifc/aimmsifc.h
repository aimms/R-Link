#ifndef _INCLUDED_aimmsifc_H
#define _INCLUDED_aimmsifc_H

#if defined(__GNUC__)
	#define DLLEXPORT     __attribute__ ((visibility("default")))
	#define DLLIMPORT     __attribute__ ((visibility("default")))
	#define DLLINTERFACE  __attribute__ ((visibility("default")))
    #define DLLLOCAL      __attribute__ ((visibility("hidden")))
 	#define FORCEINLINE   __inline__
	#define DEPRECATED(txt) __attribute__((deprecated))
	// note: deprecated must be placed after the function name up to GCC 3.3  
#endif
#if defined(_MSC_VER) 
	#define DLLEXPORT __declspec(dllexport)
	#define DLLIMPORT __declspec(dllimport)
	#define DLLINTERFACE  
	#define DLLLOCAL
	#define FORCEINLINE __forceinline
	#define DEPRECATED(txt) __declspec(deprecated(txt))
#endif

#include "iAimms.h"

namespace aimmsifc
{

    aimmsifc::iAimmsW* getUnicodeInterface(bool usedInExternalAimmsDLL, const char *dllNameHint);

    aimmsifc::iAimmsA* getUTF8Interface(bool usedInExternalAimmsDLL, const char *dllNameHint);

    aimmsifc::iAimmsA* getCCPInterface(bool usedInExternalAimmsDLL, const char *dllNameHint);

}

#endif //_INCLUDED_aimmsifc_H

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

#ifndef _INCLUDE_common_IntegerTypes
#define _INCLUDE_common_IntegerTypes

#include "Common/GlobalInclude.h"

// prevent including Linux header files during AIMMS 3 compilation on 
// Windows when determining dependencies using the GCC compiler
#ifndef __DO_NOT_DEPEND__ 
#if defined(_HOST_COMPILER_GCC)
    #include <stdint.h>
	#include <unistd.h>
#endif
#if defined(_HOST_COMPILER_MICROSOFT)
	typedef signed __int8		int8_t;
	typedef signed __int16		int16_t;
	typedef signed __int32		int32_t;
	typedef signed __int64		int64_t;

	typedef unsigned __int8		uint8_t;
	typedef unsigned __int16	uint16_t;
	typedef unsigned __int32	uint32_t;
	typedef unsigned __int64	uint64_t;
#endif
#endif

#endif // #ifndef _INCLUDE_common_IntegerTypes

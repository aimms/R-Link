// dll_main.cpp : Defines the entry point for the dll main application
// this file is intended to be included only, not compiled on itself
// it is skeleton-code

#include "iAimmsFactory.h"
//#include <aimmsifc/aimmsifc.h>
#include <Common/GlobalInclude.h>


namespace aimmsifc
{
    // implementation in main.cpp
    aimmsifc::iAimmsW* getUnicodeInterface(bool usedInExternalAimmsDLL, const char *dllSearchPath);
    aimmsifc::iAimmsA* getUTF8Interface(bool usedInExternalAimmsDLL, const char *dllSearchPath);
    aimmsifc::iAimmsA* getCCPInterface(bool usedInExternalAimmsDLL, const char *dllSearchPath);

}
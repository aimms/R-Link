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
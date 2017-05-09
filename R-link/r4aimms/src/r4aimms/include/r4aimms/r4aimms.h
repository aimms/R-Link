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
#pragma once
#include "Common/GlobalInclude.h"

#ifdef R4AIMMS_EXPORTS
#define R4AIMMS_API DLLEXPORT
#else
#define R4AIMMS_API DLLIMPORT
#endif

#define STDCALL 
#ifdef _TARGET_PLATFORM_WINDOWS
#ifdef _TARGET_ARCHITECTURE_X86
#undef STDCALL
#define STDCALL __stdcall
#endif
#endif

extern "C"{
    R4AIMMS_API wchar_t* STDCALL R_home();
    R4AIMMS_API void STDCALL R_initialize(const wchar_t* szHomeFolderR);
    R4AIMMS_API void STDCALL R_executeScript(const wchar_t* szScript);
    R4AIMMS_API void STDCALL R_terminate();
};


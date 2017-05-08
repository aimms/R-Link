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


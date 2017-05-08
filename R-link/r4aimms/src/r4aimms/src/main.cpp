// dll_main.cpp : Defines the entry point for the dll main application
// this file is intended to be included only, not compiled on itself
// it is skeleton-code

#include "Common/GlobalInclude.h"
#include "aimmsifc/aimmsifcFactory.h"

void InitializeDynamicLibrary();
void FinalizeDynamicLibrary();

#ifdef _HOST_COMPILER_MICROSOFT
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE /*hModule*/,
                       DWORD  ul_reason_for_call,
                       LPVOID /*lpReserved*/
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitializeDynamicLibrary();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		FinalizeDynamicLibrary();
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#endif

#ifdef _HOST_COMPILER_GCC
extern "C"  void __attribute__ ((constructor)) _server_core_lib_init()
{
	InitializeDynamicLibrary();
}

extern "C"  void __attribute__ ((destructor)) _server_core_lib_fini()
{
	FinalizeDynamicLibrary();
}
#endif

#include "r4aimms/r4aimms.h"
#include "REngine.h"

#include "aimmsifc/aimmsifc.h"

#include <memory>
#include <vector>


aimmsifc::iAimms* getAimms()
{
    static aimmsifc::iAimms* g_Aimms;
    if (g_Aimms == 0){
        g_Aimms = aimmsifc::getUnicodeInterface(true, 0);
    }
    return g_Aimms;
}

inline std::wstring a2w(std::string mbString){
    std::wstring wideString;
    if (mbString.length() > 0){
        size_t nLenRequired = mbstowcs(0, mbString.c_str(), 0);
        if (nLenRequired == ((size_t)-1)){
            throw std::runtime_error("mbstowcs: conversion failed");
        }
        wideString.resize(nLenRequired);
        size_t result = mbstowcs(&wideString[0], mbString.c_str(), nLenRequired + 1);
        if (result != nLenRequired){
            throw std::runtime_error("mbstowcs: conversion failed");
        }
    }
    return wideString;
}
void handleException(const wchar_t* szContext, const std::exception& ex){
    std::wstring errorMsg = a2w(ex.what());
    getAimms()->ErrorRaise(aimmsifc::SeverityCode::Error, errorMsg.c_str(), szContext);
}

void handleUnknownException(const wchar_t* szContext){
    getAimms()->ErrorRaise(aimmsifc::SeverityCode::Error, L"unknown exception occurred", szContext);
}



// use a vector of shared pointers to be able to initialize 
// and terminate R from various libraries.
// first initialize is only executed
// last terminate will actually terminate it
std::vector<std::shared_ptr<r4aimms::REngine>> g_REngines;

R4AIMMS_API void STDCALL R_initialize(const wchar_t* szHomeFolderR)
{
    if (g_REngines.empty()){
        try{
            std::shared_ptr<r4aimms::REngine> spEngine = std::make_shared<r4aimms::REngine>();
            if (szHomeFolderR && szHomeFolderR[0] != 0){
                spEngine->initialize(szHomeFolderR);
            }
            else{
                spEngine->initialize(R_home());
            }

            g_REngines.push_back(spEngine);
        }
        catch (std::exception& ex){
            if (getAimms()) handleException(L"R_initialize", ex);
        }
        catch (...){
            if (getAimms()) handleUnknownException(L"R_initialize");
        }
    }
    else{
        g_REngines.push_back(g_REngines.back());
    }
}

R4AIMMS_API void STDCALL R_executeScript(const wchar_t* szScript)
{
    if (!g_REngines.empty()){
        try{
            g_REngines.back()->executeScript(szScript);
        }
        catch (std::exception& ex){
            if (getAimms()) handleException(L"R_executeScript", ex);
        }
        catch (...){
            if (getAimms()) handleUnknownException(L"R_executeScript");
        }
    }
    else{

    }

}

R4AIMMS_API void STDCALL R_terminate()
{
    if (!g_REngines.empty()){
        try{
            std::shared_ptr<r4aimms::REngine> spEngine = g_REngines.back();
            g_REngines.pop_back();
            if (g_REngines.empty()){
                spEngine->terminate();
            }
        }
        catch (std::exception& ex){
            if (getAimms()) handleException(L"R_terminate", ex);
        }
        catch (...){
            if (getAimms()) handleUnknownException(L"R_terminate");
        }
    }

}

// shamelessly copied from the R-source
R4AIMMS_API wchar_t* STDCALL R_home()
{
#ifdef _HOST_COMPILER_MICROSOFT
    static wchar_t rhomebuf[MAX_PATH];

    char rhomebufA[MAX_PATH];
    LONG rc;
    HKEY hkey;
    DWORD keytype = REG_SZ, cbData = sizeof(rhomebuf);

    /* First try the C environment space */
    if (getenv("R_HOME")){
        strncpy(rhomebufA, getenv("R_HOME"), MAX_PATH);
        mbstowcs(&rhomebuf[0], &rhomebufA[0], MAX_PATH);
        return (rhomebuf);
    }

    /* Then the Windows API environment space */
    if (GetEnvironmentVariableW(L"R_HOME", rhomebuf, sizeof(rhomebuf)) > 0){
        return (rhomebuf);
    }

    /* And then the registry */
    rc = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\R-core\\R", 0, KEY_READ, &hkey);
    if (rc == ERROR_SUCCESS) {
        rc = RegQueryValueExW(hkey, L"InstallPath", 0, &keytype,(LPBYTE)rhomebuf, &cbData);
        RegCloseKey(hkey);
    }
    else{
        return NULL;
    }

    if (rc != ERROR_SUCCESS){
        return NULL;
    }
    return rhomebuf;
#endif
    return 0;
}

namespace aimmsifc
{

    aimmsifc::iAimmsW* getUnicodeInterface(bool usedInExternalAimmsDLL, const char *dllSearchPath)
    {
        return aimmsifc::iAimmsFactory::Singleton().GetUnicodeInterface(usedInExternalAimmsDLL, dllSearchPath);
    }

    aimmsifc::iAimmsA* getUTF8Interface(bool usedInExternalAimmsDLL, const char *dllSearchPath)
    {
        return aimmsifc::iAimmsFactory::Singleton().GetUTF8Interface(usedInExternalAimmsDLL, dllSearchPath);
    }

    aimmsifc::iAimmsA* getCCPInterface(bool usedInExternalAimmsDLL, const char *dllSearchPath)
    {
        return aimmsifc::iAimmsFactory::Singleton().GetCCPInterface(usedInExternalAimmsDLL, dllSearchPath);
    }

}

void InitializeDynamicLibrary()
{
}

void FinalizeDynamicLibrary()
{
}

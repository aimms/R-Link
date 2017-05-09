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
#define _CRT_NONSTDC_NO_DEPRECATE
#include "iAimmsFactory.h"
#include <Common/DynamicLib.h>
#include <Common/Array.h>
#include <sstream>
#include <string.h>
#include <iomanip>
#ifdef _TARGET_PLATFORM_WINDOWS
    #include <Psapi.h>
#else
    #include <dlfcn.h>
#endif


namespace {

	enum {
		FOUND_NONE,
		FOUND_LIB_314,
		FOUND_LIB_313ASC,
		FOUND_LIB_313UNI
	};

	#ifdef _TARGET_PLATFORM_WINDOWS
		#define OS_DLL_EXT ".dll"
		#define OTHER_DLL_EXT ".so"
        #define STRCOMPARE stricmp  // in windows it does not need to be exact 
	#else
		#define OS_DLL_EXT ".so"
		#define OTHER_DLL_EXT ".dll"
        #define STRCOMPARE strcmp
	#endif

	const char *AdjustDllExtension(std::string &path)
	{
		std::string::size_type period = path.rfind('.');
		if (period == std::string::npos) {
			path += OS_DLL_EXT;
		} else if ( STRCOMPARE(path.c_str()+period, OS_DLL_EXT) == 0 ) {
			// already correct
		} else if ( STRCOMPARE(path.c_str()+period, OTHER_DLL_EXT) == 0 ) {
			// replace .so by .dll (or v.v.)
			path.replace(period,std::string::npos,OS_DLL_EXT);
		} else {
			path += OS_DLL_EXT;
		}
		return path.c_str();
	}

#ifdef _TARGET_PLATFORM_WINDOWS
	std::string getLibProperty( const char* szFilename, const char* szPropertyName) 
	{
        // retrieve the size of the version info
        DWORD versionInfoSize = GetFileVersionInfoSizeA( szFilename, 0);
        if (versionInfoSize == 0 ){
            throw std::runtime_error("Unable to get dll version info size.");
        }

        // retrieve the version info
        Common::Array<char> memory(versionInfoSize);
        char* pData = memory.data();
        if (!GetFileVersionInfoA(szFilename, 0, versionInfoSize, pData )){
            throw std::runtime_error("Unable to get dll version info");
        }

        // construct the property for the version info value to retrieve,
        // assumed is the default english language & codepage
        unsigned short language = 1033;
        unsigned short codePage = 1200;
        std::ostringstream ossTmp;
        ossTmp << "\\StringFileInfo\\";
        ossTmp << std::hex << std::setfill ('0') << std::setw(4)  << language;
        ossTmp << std::hex << std::setfill ('0') << std::setw(4)  << codePage;
        ossTmp << "\\" << szPropertyName;

        // query the value from the version info
        void* pPropertyValue;
        UINT uiLen = 0;
        if (!VerQueryValueA(pData, ossTmp.str().c_str(), &pPropertyValue, &uiLen)){
            throw std::runtime_error("Unable to get property from dll version info.");
        }

        return std::string((char*)pPropertyValue, ((char*)pPropertyValue)+uiLen);
    }

	bool isUnicode(char *szImageName)
	{
		try {
			std::string fileDescription = getLibProperty(szImageName, "FileDescription" );
			return fileDescription.find("Unicode") != std::string::npos;
		} catch (...) {
			return false;
		}
	}

	bool isUnicodeFromHandle(void *dllHandle)
	{
		char szImageName[_MAX_PATH];
		::GetModuleFileNameA((HMODULE)dllHandle,szImageName,_MAX_PATH);
		return isUnicode(szImageName);
	}
#endif

	bool NameEndsWith(const char *fullPath, const char *dllName)
	{
		int offset = (int)strlen(fullPath) - (int)strlen(dllName);
		if (offset < 0) return false;
		return (STRCOMPARE(fullPath+offset,dllName) == 0);
	}

	void *TryLoadLibrary(const char *dllPath)
	{
#ifdef _TARGET_PLATFORM_WINDOWS
		return (void*)::LoadLibraryA(dllPath);
#else
		return ::dlopen(dllPath, RTLD_NOW | RTLD_GLOBAL);
#endif
	}
	void CloseLibrary(void *handle)
	{
		if (handle) {
#ifdef _TARGET_PLATFORM_WINDOWS
			FreeLibrary((HMODULE)handle);
#else
			::dlclose(handle);
#endif
		}
	}
}


namespace aimmsifc {

	/*static*/ iAimmsFactory &iAimmsFactory::Singleton()
	{
		static iAimmsFactory instance;
		return instance;
	}


	iAimmsFactory::iAimmsFactory()
		: m_Existing_iAimmsCCP(0)
		, m_Existing_iAimmsUTF8(0)
		, m_Existing_iAimmsUNI(0)
	{}

	iAimmsFactory::~iAimmsFactory()
	{
		if (m_Existing_iAimmsCCP) {
			(dynamic_cast<iFactoryManage*>(m_Existing_iAimmsCCP))->Delete();
		}
		if (m_Existing_iAimmsUTF8) {
			(dynamic_cast<iFactoryManage*>(m_Existing_iAimmsUTF8))->Delete();
		}
		if (m_Existing_iAimmsUNI) {
			(dynamic_cast<iFactoryManage*>(m_Existing_iAimmsUNI))->Delete();
		}

	}

	iAimmsA *iAimmsFactory::GetCCPInterface(bool usedInExternalAimmsDLL, const char *dllSearchPath)
	{
		if (m_Existing_iAimmsCCP) {
			return m_Existing_iAimmsCCP;
		}

		std::string dllPath;
		switch( GetAimmsDllPath(usedInExternalAimmsDLL, dllSearchPath, dllPath) ) {

		case FOUND_LIB_313ASC:
			m_Existing_iAimmsCCP = CreateiAimmsA_CCP_313asc(dllPath.c_str());
			break;

		case FOUND_LIB_313UNI:
			m_Existing_iAimmsCCP = CreateiAimmsA_CCP_313uni(dllPath.c_str());
			break;

		case FOUND_LIB_314:
			m_Existing_iAimmsCCP = CreateiAimmsA_CCP_314(dllPath.c_str());
			break;
		}

		return m_Existing_iAimmsCCP;
	}
	
	iAimmsA *iAimmsFactory::GetUTF8Interface(bool usedInExternalAimmsDLL, const char *dllSearchPath)
	{
		if (m_Existing_iAimmsUTF8) {
			return m_Existing_iAimmsUTF8;
		}

		std::string dllPath;
		switch( GetAimmsDllPath(usedInExternalAimmsDLL, dllSearchPath, dllPath) ) {

		case FOUND_LIB_313ASC:
			m_Existing_iAimmsUTF8 = CreateiAimmsA_UTF8_313asc(dllPath.c_str());
			break;

		case FOUND_LIB_313UNI:
			m_Existing_iAimmsUTF8 = CreateiAimmsA_UTF8_313uni(dllPath.c_str());
			break;

		case FOUND_LIB_314:
			m_Existing_iAimmsUTF8 = CreateiAimmsA_UTF8_314(dllPath.c_str());
			break;
		}

		return m_Existing_iAimmsUTF8;
	}

	iAimmsW *iAimmsFactory::GetUnicodeInterface(bool usedInExternalAimmsDLL, const char *dllSearchPath)
	{
		if (m_Existing_iAimmsUNI) {
			return m_Existing_iAimmsUNI;
		}

		std::string dllPath;
		switch( GetAimmsDllPath(usedInExternalAimmsDLL, dllSearchPath, dllPath) ) {

		case FOUND_LIB_313ASC:
			m_Existing_iAimmsUNI = CreateiAimmsW_uni_313asc(dllPath.c_str());
			break;

		case FOUND_LIB_313UNI:
			m_Existing_iAimmsUNI = CreateiAimmsW_uni_313uni(dllPath.c_str());
			break;

		case FOUND_LIB_314:
			m_Existing_iAimmsUNI = CreateiAimmsW_uni_314(dllPath.c_str());
			break;
		}

		return m_Existing_iAimmsUNI;
	}

	int iAimmsFactory::GetAimmsDllPath(bool usedInExternalAimmsDLL, const char *dllSearchPath, std::string &dllPath)
	{
		int rval = FOUND_NONE;
		if (usedInExternalAimmsDLL) {
			rval = FindLibAimmsInCurrentModules(dllPath);
			if (rval != FOUND_NONE) return rval;
		}

		return FindLibAimmsViaTrialAndError(dllSearchPath, dllPath);
	}


	int iAimmsFactory::FindLibAimmsInCurrentModules(std::string &dllPath)
	{
		int rVal = FOUND_NONE;
#ifdef _TARGET_PLATFORM_WINDOWS
		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId() );
		if (!hProcess) {
			return rVal;
		}

		// Get a list of all the modules in this process.

		HMODULE hMods[1024];
		DWORD cbNeeded;
		if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {

			for ( size_t i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ ) {

				char moduleName[MAX_PATH];

				// Get the full path to the module's file.
				if ( GetModuleFileNameExA( hProcess, hMods[i], moduleName, sizeof(moduleName) / sizeof(char))) {
					if (NameEndsWith(moduleName,"libaimms3" OS_DLL_EXT)) {
						dllPath = moduleName;
						rVal = FOUND_LIB_314;
						break;
					} else if (NameEndsWith(moduleName,"libaimms" OS_DLL_EXT)) {
						dllPath = moduleName;
						if ( isUnicode(moduleName) ) {
							rVal = FOUND_LIB_313UNI;
						} else {
							rVal = FOUND_LIB_313ASC;
						}
						break;
					}
				}

			}
		}

		// Release the handle to the process.
		CloseHandle( hProcess );
#else
                void *libHandle;
                dllPath = "libaimms3.so";
                libHandle = TryLoadLibrary(dllPath.c_str());
                if (libHandle) {
                    CloseLibrary(libHandle);
                    return FOUND_LIB_314;
                }
                // try 3.13, which on linux is always ASCII
                dllPath = "libaimms.so";
                libHandle = TryLoadLibrary(dllPath.c_str());
                if (libHandle) {
                    CloseLibrary(libHandle);
                    return FOUND_LIB_313ASC;
                }
                dllPath = "";
#endif
		return rVal;
	}

	int iAimmsFactory::FindLibAimmsViaTrialAndError(const char *dllSearchPath, std::string &dllPath)
	{
		void *libHandle;

		if (dllSearchPath && dllSearchPath[0]) {
#ifdef _TARGET_PLATFORM_WINDOWS
			::SetDllDirectoryA(dllSearchPath);
#endif
		}

		// try to find aimms3.14
		{ 
#ifdef _TARGET_PLATFORM_WINDOWS
			dllPath = "libaimms3" OS_DLL_EXT; // 3.14 and later, unicode only
#endif
#ifdef _TARGET_PLATFORM_LINUX
                        if (dllSearchPath){
                            dllPath = dllSearchPath;
                            dllPath += "/libaimms3" OS_DLL_EXT; // 3.14 and later, unicode only
                        }
                        else{
                            dllPath += "libaimms3" OS_DLL_EXT; // 3.14 and later, unicode only
                            
                        }
#endif
			libHandle = TryLoadLibrary(dllPath.c_str());
			if (libHandle) {
				CloseLibrary(libHandle);
				return FOUND_LIB_314;
			}
		}

		// try to find aimms3.13
		{ 
#ifdef _TARGET_PLATFORM_WINDOWS                    
			dllPath = "libaimms" OS_DLL_EXT; // 3.13 and earlier, unicode or ascii
#endif
#ifdef _TARGET_PLATFORM_LINUX
                        if (dllSearchPath){
                            dllPath = dllSearchPath;
                            dllPath = "/libaimms" OS_DLL_EXT; // 3.13 and earlier, unicode or ascii
                        }
                        else{
                            dllPath = "libaimms" OS_DLL_EXT; // 3.13 and earlier, unicode or ascii
                        }
#endif
			libHandle = TryLoadLibrary(dllPath.c_str());
			if (libHandle) {
				int rval;
#ifdef _TARGET_PLATFORM_WINDOWS
				if ( isUnicodeFromHandle(libHandle) ) {
					rval = FOUND_LIB_313UNI;
				} else {
					rval = FOUND_LIB_313ASC;
				}
#else
				// on Linux, aimms3.13 and earlier was *always* ascii
				rval = FOUND_LIB_313ASC;
#endif
				CloseLibrary(libHandle);
				return rval;
			}
		}

		return FOUND_NONE;
	}
	

}



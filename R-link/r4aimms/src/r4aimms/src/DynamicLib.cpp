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
#include "Common/DynamicLib.h"
#include "Common/GlobalInclude.h"

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

#include <windows.h>
#include <stdlib.h>

namespace Common
{
		DynamicLib::DynamicLib(const char* szFileName) : m_hModule(0)
		{
			if (szFileName) Open(szFileName);
		}

		DynamicLib::~DynamicLib()
		{
			Close();
		}

		bool DynamicLib::Open( const char* szFileName )
		{
			
			wchar_t wcsFilename[_MAX_PATH];
			size_t resultSize = 0;
			if ( mbstowcs_s( &resultSize, wcsFilename, szFileName, _TRUNCATE) !=0 ||
				resultSize != (strlen(szFileName)+1) ){
				return false;
			}

			m_hModule = (void*) ::LoadLibraryW( wcsFilename );
			
			//m_hModule = (void*) ::LoadLibrary( szFileName );
			return m_hModule != 0;
		}

		void DynamicLib::Close()
		{
			if (m_hModule){
				::FreeLibrary((HMODULE)m_hModule);
				m_hModule = 0;
			}
		}

		bool DynamicLib::Valid() const
		{
			return m_hModule != 0;
		}

		void* DynamicLib::GetProcAddress( const char* szProcName )
		{
			return ::GetProcAddress((HMODULE)m_hModule, szProcName );
		}
};
#endif

#ifdef _HOST_COMPILER_GCC

#include <dlfcn.h>

namespace Common
{
		DynamicLib::DynamicLib(const char* szFileName) : m_hModule(0)
		{
			// load any version we can
			if (szFileName) Open(szFileName);
		}

		DynamicLib::~DynamicLib()
		{
			Close();
		}
		bool DynamicLib::Open( const char* szFileName )
		{
			m_hModule = ::dlopen(szFileName, RTLD_NOW | RTLD_GLOBAL);
			return m_hModule != 0;
		}

		void DynamicLib::Close()
		{
			if (m_hModule){
				::dlclose(m_hModule);
				m_hModule = 0;
			}
		}
		bool DynamicLib::Valid() const
		{
			return m_hModule != 0;
		}

		void* DynamicLib::GetProcAddress( const char* szProcName )
		{
			return ::dlsym( m_hModule, szProcName );
		}
}
#endif

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
#ifndef _INCLUDED_Common_DynamicLib
#define _INCLUDED_Common_DynamicLib

#include "Common/CommonAPI.h"

namespace Common
{
	class COMMON_API DynamicLib
	{
	public:
		DynamicLib(const char* szFileName = 0);
		virtual ~DynamicLib();

		virtual bool Open( const char* szFileName );
		virtual void Close();

		virtual void* GetProcAddress( const char* szProcName );
		virtual bool Valid() const;

	private:
		void* m_hModule;
	};
}

#endif // end #ifndef _INCLUDED_Common_DynamicLib

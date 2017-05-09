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
#ifndef _INCLUDED__Common_ThreadLocalStorage
#define _INCLUDED__Common_ThreadLocalStorage

#include "Common/GlobalInclude.h"
#include "Common/CommonAPI.h"

#if defined(_TARGET_PLATFORM_WINDOWS)
  #include <windows.h>
#elif defined(_TARGET_PLATFORM_LINUX)
  #include <pthread.h>
#endif


namespace Common
{
	class COMMON_API ThreadLocalStorage{
	public:
		ThreadLocalStorage();
		~ThreadLocalStorage();
		
		void  set(void *p);
		void* get() const;
	private:
#if defined(_TARGET_PLATFORM_WINDOWS)
		DWORD m_Key;
#elif defined(_TARGET_PLATFORM_LINUX)
		pthread_key_t m_Key;
#endif
	};
} // end namespace Common

#endif // end #ifndef _INCLUDED__Common_ThreadLocalStorage

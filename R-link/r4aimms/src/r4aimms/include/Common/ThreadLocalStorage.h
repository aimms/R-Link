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

#include "Common/ThreadLocalStorage.h"


#ifdef _TARGET_PLATFORM_WINDOWS
namespace Common
{
	ThreadLocalStorage::ThreadLocalStorage() : m_Key(TLS_OUT_OF_INDEXES)
	{
		m_Key = TlsAlloc();
	}

	ThreadLocalStorage::~ThreadLocalStorage()
	{
		TlsFree(m_Key);
	}

	void  ThreadLocalStorage::set(void *p)
	{
		TlsSetValue(m_Key,p); 
	}

	void* ThreadLocalStorage::get() const
	{
		return TlsGetValue(m_Key); 
	}
} // end namespace Common
#define _ThreadLocalStorageImplemented
#endif// end #if defined(_TARGET_PLATFORM_WINDOWS)

#ifdef _TARGET_PLATFORM_LINUX
namespace Common
{
	ThreadLocalStorage::ThreadLocalStorage() : m_Key(0)
	{
		pthread_key_create(&m_Key,0);
	}

	ThreadLocalStorage::~ThreadLocalStorage()
	{
		pthread_key_delete(m_Key);
	}

	void  ThreadLocalStorage::set(void *p)
	{
		pthread_setspecific(m_Key, p);
	}

	void* ThreadLocalStorage::get() const
	{
		return pthread_getspecific(m_Key);
	}
} // end namespace Common


#define _ThreadLocalStorageImplemented
#endif // end #if defined(_TARGET_PLATFORM_LINUX)


#ifndef _ThreadLocalStorageImplemented
#error "No implementation for thread local storage on this platform"
#endif // end #ifndef _PlatformThreadImplemented


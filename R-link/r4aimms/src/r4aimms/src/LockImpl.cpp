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
#include "Common/LockImpl.h"

#if defined(_TARGET_PLATFORM_LINUX)
#include <errno.h>
#endif


namespace Common
{
#if defined(_TARGET_PLATFORM_LINUX)
	LockImpl::LockImpl() {
#ifndef NDEBUG
		// In debug, setup attributes for lock error checking.
		pthread_mutexattr_t mta;
		int rv = pthread_mutexattr_init(&mta);
		assert(rv==0);
		rv = pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_ERRORCHECK);
		assert(rv==0);
		rv = pthread_mutex_init(&os_lock_, &mta);
		assert(rv==0);
		rv = pthread_mutexattr_destroy(&mta);
		assert(rv==0);
#else
		// In release, go with the default lock attributes.
		pthread_mutex_init(&os_lock_, NULL);
#endif
	}

	LockImpl::~LockImpl() {
		int rv = pthread_mutex_destroy(&os_lock_);
		assert(rv == 0);
	}

	bool LockImpl::Try() {
		int rv = pthread_mutex_trylock(&os_lock_);
		assert(rv == 0 || rv == EBUSY);
		return rv == 0;
	}

	void LockImpl::Lock() {
		int rv = pthread_mutex_lock(&os_lock_);
		assert(rv==0);
	}

	void LockImpl::Unlock() {
		int rv = pthread_mutex_unlock(&os_lock_);
		assert(rv==0);
	}
#endif // defined(_TARGET_PLATFORM_LINUX)

#if defined(_TARGET_PLATFORM_WINDOWS)
	LockImpl::LockImpl() {
		// The second parameter is the spin count, for short-held locks it avoid the
		// contending thread from going to sleep which helps performance greatly.
		::InitializeCriticalSectionAndSpinCount(&os_lock_, 2000);
	}

	LockImpl::~LockImpl() {
		::DeleteCriticalSection(&os_lock_);
	}

	bool LockImpl::Try() {
		if (::TryEnterCriticalSection(&os_lock_) != FALSE) {
			return true;
		}
		return false;
	}

	void LockImpl::Lock() {
		::EnterCriticalSection(&os_lock_);
	}

	void LockImpl::Unlock() {
		::LeaveCriticalSection(&os_lock_);
	}
#endif // defined(_TARGET_PLATFORM_WINDOWS)

};
// end namespace Common



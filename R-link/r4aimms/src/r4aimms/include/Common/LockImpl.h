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
#ifndef _INCLUDED__Common_LockImpl
#define _INCLUDED__Common_LockImpl

#include "Common/GlobalInclude.h"
#include "Common/CommonAPI.h"
#include "Common/IntegerTypes.h"

#if defined(_TARGET_PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(_TARGET_PLATFORM_LINUX)
#include <pthread.h>
#endif

namespace Common
{
	// This class implements the underlying platform-specific spin-lock mechanism
	// used for the Lock class.  Most users should not use LockImpl directly, but
	// should instead use Lock.
	class COMMON_API LockImpl {
	public:
#if defined(_TARGET_PLATFORM_WINDOWS)
		typedef CRITICAL_SECTION OSLockType;
#elif defined(_TARGET_PLATFORM_LINUX)
		typedef pthread_mutex_t OSLockType;
#endif

		LockImpl();
		~LockImpl();

		// If the lock is not held, take it and return true.  If the lock is already
		// held by something else, immediately return false.
		bool Try();

		// Take the lock, blocking until it is available if necessary.
		void Lock();

		// Release the lock.  This must only be called by the lock's holder: after
		// a successful call to Try, or a call to Lock.
		void Unlock();

		// Return the native underlying lock.  Not supported for Windows builds.
		// TODO(awalker): refactor lock and condition variables so that this is
		// unnecessary.
#if !defined(_TARGET_PLATFORM_WINDOWS)
		OSLockType* os_lock() { return &os_lock_; }
#endif

	private:
		OSLockType os_lock_;

		DISALLOW_COPY_AND_ASSIGN(LockImpl);
	};

};
// end namespace Common

#endif // end #ifndef _INCLUDED__Common_LockImpl

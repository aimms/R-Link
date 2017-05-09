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
#include "Common/Lock.h"


namespace Common
{

#if !defined(NDEBUG)
	Lock::Lock() : lock_() {
		owned_by_thread_ = false;
		owning_thread_id_ = static_cast<PlatformThreadId>(0);
	}

	Lock::~Lock(){
        // if one of the assert below fails, you are trying to
        // destruct this lock while another thread is actually
        // owning this lock
        assert(!owned_by_thread_);
		assert(owning_thread_id_ == static_cast<PlatformThreadId>(0));
	}

	void Lock::AssertAcquired() const {
		assert(owned_by_thread_);
		assert(owning_thread_id_ == PlatformThread::CurrentId());
	}

	void Lock::CheckHeldAndUnmark() {
        // if one of the assert below fails, you are trying to
        // unlock a lock that was not locked by the current thread
		assert(owned_by_thread_);
		assert(owning_thread_id_ == PlatformThread::CurrentId());
		owned_by_thread_ = false;
		owning_thread_id_ = static_cast<PlatformThreadId>(0);
	}

	void Lock::CheckUnheldAndMark() {
        // if the assert below fails, you are trying to
        // lock a lock that is already locked by this thread
        // ie reentrant locking is not allowed
		assert(!owned_by_thread_);
		owned_by_thread_ = true;
		owning_thread_id_ = PlatformThread::CurrentId();
	}

#endif  // NDEBUG

};
// end namespace Common


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


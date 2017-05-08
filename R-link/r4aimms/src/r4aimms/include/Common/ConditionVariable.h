#ifndef _INCLUDED__Common_ConditionVariable
#define _INCLUDED__Common_ConditionVariable

#include "Common/GlobalInclude.h"
#include "Common/CommonAPI.h"
#include "Common/Lock.h"
#include "Common/Time.h"

#if defined(_TARGET_PLATFORM_WINDOWS)
#include <windows.h>
#endif

#if defined(_TARGET_PLATFORM_LINUX)
#include <pthread.h>
#endif

// USAGE NOTE 1: spurious signal events are possible with this and
// most implementations of condition variables.  As a result, be
// *sure* to retest your condition before proceeding.  The following
// is a good example of doing this correctly:
//
// while (!work_to_be_done()) Wait(...);
//
// In contrast do NOT do the following:
//
// if (!work_to_be_done()) Wait(...);  // Don't do this.
//
// Especially avoid the above if you are relying on some other thread only
// issuing a signal up *if* there is work-to-do.  There can/will
// be spurious signals.  Recheck state on waiting thread before
// assuming the signal was intentional. Caveat caller ;-).
//
// USAGE NOTE 2: Broadcast() frees up all waiting threads at once,
// which leads to contention for the locks they all held when they
// called Wait().  This results in POOR performance.  A much better
// approach to getting a lot of threads out of Wait() is to have each
// thread (upon exiting Wait()) call Signal() to free up another
// Wait'ing thread.  Look at condition_variable_unittest.cc for
// both examples.
//
// Broadcast() can be used nicely during teardown, as it gets the job
// done, and leaves no sleeping threads... and performance is less
// critical at that point.
//
// The semantics of Broadcast() are carefully crafted so that *all*
// threads that were waiting when the request was made will indeed
// get signaled.  Some implementations mess up, and don't signal them
// all, while others allow the wait to be effectively turned off (for
// a while while waiting threads come around).  This implementation
// appears correct, as it will not "lose" any signals, and will guarantee
// that all threads get signaled by Broadcast().
//
// This implementation offers support for "performance" in its selection of
// which thread to revive.  Performance, in direct contrast with "fairness,"
// assures that the thread that most recently began to Wait() is selected by
// Signal to revive.  Fairness would (if publicly supported) assure that the
// thread that has Wait()ed the longest is selected. The default policy
// may improve performance, as the selected thread may have a greater chance of
// having some of its stack data in various CPU caches.
//
// For a discussion of the many very subtle implementation details, see the FAQ
// at the end of the Windows implementation in ConditionVariable.cpp.

namespace Common
{
	class COMMON_API ConditionVariable
	{
	public:
		// Construct a cv for use with ONLY one user lock.
		explicit ConditionVariable(Lock* user_lock);

		~ConditionVariable();

		// Wait() releases the caller's critical section atomically as it starts to
		// sleep, and the reacquires it when it is signaled.
		void Wait();

        // this method is deprecated since in does not allow to compensate for spurious
        // wake-events that primarily *will* happen on linux
		DEPRECATED("use WaitForTimeout") void TimedWait(const int& msWait);
        
        // returns false when the condition got signalled before the specified
        // time has passed; returns true when the time has expired
        bool WaitForTimeout( const Time& absTime );

		// Broadcast() revives all waiting threads.
		void Broadcast();
		// Signal() revives one waiting thread.
		void Signal();

	private:

#if defined(_TARGET_PLATFORM_WINDOWS)

		// Define Event class that is used to form circularly linked lists.
		// The list container is an element with NULL as its handle_ value.
		// The actual list elements have a non-zero handle_ value.
		// All calls to methods MUST be done under protection of a lock so that links
		// can be validated.  Without the lock, some links might asynchronously
		// change, and the assertions would fail (as would list change operations).
		class COMMON_API Event {
		public:
			// Default constructor with no arguments creates a list container.
			Event();
			~Event();

			// InitListElement transitions an instance from a container, to an element.
			void InitListElement();

			// Methods for use on lists.
			bool IsEmpty() const;
			void PushBack(Event* other);
			Event* PopFront();
			Event* PopBack();

			// Methods for use on list elements.
			// Accessor method.
			HANDLE handle() const;
			// Pull an element from a list (if it's in one).
			Event* Extract();

			// Method for use on a list element or on a list.
			bool IsSingleton() const;

		private:
			// Provide pre/post conditions to validate correct manipulations.
			bool ValidateAsDistinct(Event* other) const;
			bool ValidateAsItem() const;
			bool ValidateAsList() const;
			bool ValidateLinks() const;

			HANDLE handle_;
			Event* next_;
			Event* prev_;
			DISALLOW_COPY_AND_ASSIGN(Event);
		};

		// Note that RUNNING is an unlikely number to have in RAM by accident.
		// This helps with defensive destructor coding in the face of user error.
		enum RunState { SHUTDOWN = 0, RUNNING = 64213 };

		// Internal implementation methods supporting Wait().
		Event* GetEventForWaiting();
		void RecycleEvent(Event* used_event);

		RunState run_state_;

		// Private critical section for access to member data.
		Lock internal_lock_;

		// Lock that is acquired before calling Wait().
		Lock& user_lock_;

		// Events that threads are blocked on.
		Event waiting_list_;

		// Free list for old events.
		Event recycling_list_;
		int recycling_list_size_;

		// The number of allocated, but not yet deleted events.
		int allocation_counter_;

#elif defined(_TARGET_PLATFORM_LINUX)

		pthread_cond_t condition_;
		pthread_mutex_t* user_mutex_;
#if !defined(NDEBUG)
		Lock* user_lock_;     // Needed to adjust shadow lock state on wait.
#endif

#endif

		DISALLOW_COPY_AND_ASSIGN(ConditionVariable);
	};
};
// end namespace Common

#endif // end #ifndef _INCLUDED__Common_ConditionVariable

#ifndef _INCLUDED__Common_PlatfomThread
#define _INCLUDED__Common_PlatfomThread

#include "Common/GlobalInclude.h"
#include "Common/CommonAPI.h"

#if defined(_TARGET_PLATFORM_WINDOWS)
  #include <windows.h>
#elif defined(_TARGET_PLATFORM_LINUX)
  #include <pthread.h>
  #include <unistd.h>
#endif


namespace Common
{
#if defined(_TARGET_PLATFORM_WINDOWS)
  typedef DWORD PlatformThreadId;
  typedef void* PlatformThreadHandle;  // HANDLE
  const PlatformThreadHandle kNullThreadHandle = NULL;
#elif defined(_TARGET_PLATFORM_LINUX)
  typedef pid_t PlatformThreadId;
  typedef pthread_t PlatformThreadHandle;
  const PlatformThreadHandle kNullThreadHandle = 0;
#endif

	// A namespace for low-level thread functions.
	class COMMON_API PlatformThread
	{
	public:
		// Gets the current thread id, which may be useful for logging purposes.
		static PlatformThreadId CurrentId();

		// Yield the current thread so another thread can be scheduled.
		static void YieldCurrentThread();

		// Sleeps for the specified duration (units are milliseconds).
		static void Sleep(int duration_ms);

		// Sets the thread name visible to a debugger.  This has no effect otherwise.
		static void SetName(const char* name);

		// Implement this interface to run code on a background thread.  Your
		// ThreadMain method will be called on the newly created thread.
		class IDelegate {
		public:
			virtual ~IDelegate() {}
			virtual void ThreadMain() = 0;
		};

		// Creates a new thread.  The |stack_size| parameter can be 0 to indicate
		// that the default stack size should be used.  Upon success,
		// |*thread_handle| will be assigned a handle to the newly created thread,
		// and |delegate|'s ThreadMain method will be executed on the newly created
		// thread.
		// NOTE: When you are done with the thread handle, you must call Join to
		// release system resources associated with the thread.  You must ensure that
		// the Delegate object outlives the thread.
		static bool Create(size_t stack_size, IDelegate* delegate,
			PlatformThreadHandle* thread_handle);

		// CreateNonJoinable() does the same thing as Create() except the thread
		// cannot be Join()'d.  Therefore, it also does not output a
		// PlatformThreadHandle.
		static bool CreateNonJoinable(size_t stack_size, IDelegate* delegate);

		// Joins with a thread created via the Create function.  This function blocks
		// the caller until the designated thread exits.  This will invalidate
		// |thread_handle|.
		static void Join(PlatformThreadHandle thread_handle);

	private:
		// DISALLOW_IMPLICIT_CONSTRUCTORS(PlatformThread);
	};

};
// end namespace Common

#endif // end #ifndef _INCLUDED__Common_PlatfomThread

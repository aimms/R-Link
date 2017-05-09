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
#include "Common/PlatformThread.h"


#ifdef _TARGET_PLATFORM_WINDOWS

#include <windows.h>
namespace Common {

    namespace {
        // The information on how to set the thread name comes from
        // a MSDN article: http://msdn2.microsoft.com/en-us/library/xcb2z8hs.aspx
        const DWORD kVCThreadNameException = 0x406D1388;

        typedef struct tagTHREADNAME_INFO {
            DWORD dwType; // Must be 0x1000.
            LPCSTR szName; // Pointer to name (in user addr space).
            DWORD dwThreadID; // Thread ID (-1=caller thread).
            DWORD dwFlags; // Reserved for future use, must be zero.
        } THREADNAME_INFO;

        DWORD __stdcall ThreadFunc(void* closure) {
            PlatformThread::IDelegate* delegate =
                    static_cast<PlatformThread::IDelegate*> (closure);
            delegate->ThreadMain();
            return NULL;
        }

    } // namespace

    // static

    PlatformThreadId PlatformThread::CurrentId() {
        return GetCurrentThreadId();
    }

    // static

    void PlatformThread::YieldCurrentThread() {
        ::Sleep(0);
    }

    // static

    void PlatformThread::Sleep(int duration_ms) {
        ::Sleep(duration_ms);
    }
    // static

    void PlatformThread::SetName(const char* name) {
        // The debugger needs to be around to catch the name in the exception.  If
        // there isn't a debugger, we are just needlessly throwing an exception.
        if (!::IsDebuggerPresent())
            return;

        THREADNAME_INFO info;
        info.dwType = 0x1000;
        info.szName = name;
        info.dwThreadID = CurrentId();
        info.dwFlags = 0;

        __try
        {
            RaiseException(kVCThreadNameException, 0, sizeof (info) / sizeof (DWORD),
                    reinterpret_cast<DWORD_PTR*> (&info));
        }

        __except(EXCEPTION_CONTINUE_EXECUTION) {
        }
    }
    // static

    bool PlatformThread::Create(size_t stack_size, IDelegate* delegate,
            PlatformThreadHandle* thread_handle) {
        unsigned int flags = 0;
        if (stack_size > 0) {
            flags = STACK_SIZE_PARAM_IS_A_RESERVATION;
        } else {
            stack_size = 0;
        }

        // Using CreateThread here vs _beginthreadex makes thread creation a bit
        // faster and doesn't require the loader lock to be available.  Our code will
        // have to work running on CreateThread() threads anyway, since we run code
        // on the Windows thread pool, etc.  For some background on the difference:
        //   http://www.microsoft.com/msj/1099/win32/win321099.aspx
        *thread_handle = CreateThread(
                NULL, stack_size, ThreadFunc, delegate, flags, NULL);
        return *thread_handle != NULL;
    }

    // static

    bool PlatformThread::CreateNonJoinable(size_t stack_size, IDelegate* delegate) {
        PlatformThreadHandle thread_handle;
        bool result = Create(stack_size, delegate, &thread_handle);
        CloseHandle(thread_handle);
        return result;
    }

    // static

    void PlatformThread::Join(PlatformThreadHandle thread_handle) {
        assert(thread_handle);

        // Wait for the thread to exit.  It should already have terminated but make
        // sure this assumption is valid.
        DWORD result = WaitForSingleObject(thread_handle, INFINITE);
        assert(WAIT_OBJECT_0 == result);

        CloseHandle(thread_handle);
    }
};
// end namespace Common

#define _PlatformThreadImplemented
#endif// end #if defined(_TARGET_PLATFORM_WINDOWS)

#ifdef _TARGET_PLATFORM_LINUX

#include "Common/Time.h"
//#include <dlfcn.h>
#include <errno.h>
#include <sched.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
//#include <unistd.h>

namespace Common {
    namespace {

        static void* ThreadFunc(void* closure) {
            PlatformThread::IDelegate* delegate =
                    static_cast<PlatformThread::IDelegate*> (closure);
            delegate->ThreadMain();
            return NULL;
        }
    }

    // static

    PlatformThreadId PlatformThread::CurrentId() {
        return syscall(__NR_gettid);
    }

    // static

    void PlatformThread::YieldCurrentThread() {
        sched_yield();
    }

    // static

    void PlatformThread::Sleep(int duration_ms) {

        struct timespec sleep_time, remaining;
        sleep_time = TimeDelta::FromMilliseconds(duration_ms).ToTimeSpec();

        while (nanosleep(&sleep_time, &remaining) == -1 && errno == EINTR) {
            sleep_time = remaining;
        }
    }

    // Linux SetName is currently disabled, as we need to distinguish between
    // helper threads (where it's ok to make this call) and the main thread
    // (where making this call renames our process, causing tools like killall
    // to stop working).
#if 0 && defined(_TARGET_PLATFORM_LINUX)
    // static

    void PlatformThread::SetName(const char* name) {
        // http://0pointer.de/blog/projects/name-your-threads.html

        // glibc recently added support for pthread_setname_np, but it's not
        // commonly available yet.  So test for it at runtime.
        int (*dynamic_pthread_setname_np)(pthread_t, const char*);
        *reinterpret_cast<void**> (&dynamic_pthread_setname_np) =
                dlsym(RTLD_DEFAULT, "pthread_setname_np");

        if (dynamic_pthread_setname_np) {
            // This limit comes from glibc, which gets it from the kernel
            // (TASK_COMM_LEN).
            const int kMaxNameLength = 15;
            std::string shortened_name = std::string(name).substr(0, kMaxNameLength);
            int err = dynamic_pthread_setname_np(pthread_self(),
                    shortened_name.c_str());
            if (err < 0)
                LOG(ERROR) << "pthread_setname_np: " << safe_strerror(err);
        } else {
            // Implementing this function without glibc is simple enough.  (We
            // don't do the name length clipping as above because it will be
            // truncated by the callee (see TASK_COMM_LEN above).)
            int err = prctl(PR_SET_NAME, name);
            if (err < 0)
                PLOG(ERROR) << "prctl(PR_SET_NAME)";
        }
    }
#else
    // static

    void PlatformThread::SetName(const char* name) {
        // Leave it unimplemented.

        // (This should be relatively simple to implement for the BSDs; I
        // just don't have one handy to test the code on.)
    }
#endif  // defined(OS_LINUX)

    namespace {

        bool CreateThread(size_t stack_size, bool joinable,
                PlatformThread::IDelegate* delegate,
                PlatformThreadHandle* thread_handle) {

            bool success = false;
            pthread_attr_t attributes;
            pthread_attr_init(&attributes);

            // Pthreads are joinable by default, so only specify the detached attribute if
            // the thread should be non-joinable.
            if (!joinable) {
                pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
            }


            if (stack_size > 0)
                pthread_attr_setstacksize(&attributes, stack_size);

            success = !pthread_create(thread_handle, &attributes, ThreadFunc, delegate);

            pthread_attr_destroy(&attributes);
            return success;
        }

    } // anonymous namespace

    // static

    bool PlatformThread::Create(size_t stack_size, IDelegate* delegate,
            PlatformThreadHandle* thread_handle) {
        return CreateThread(stack_size, true /* joinable thread */,
                delegate, thread_handle);
    }

    // static

    bool PlatformThread::CreateNonJoinable(size_t stack_size, IDelegate* delegate) {
        PlatformThreadHandle unused;

        bool result = CreateThread(stack_size, false /* non-joinable thread */,
                delegate, &unused);
        return result;
    }

    // static

    void PlatformThread::Join(PlatformThreadHandle thread_handle) {
        pthread_join(thread_handle, NULL);
    }

};

#define _PlatformThreadImplemented
#endif // end #if defined(_TARGET_PLATFORM_LINUX)

#ifndef _PlatformThreadImplemented
#error "No implementation for thread on this platform"
#endif // end #ifndef _PlatformThreadImplemented


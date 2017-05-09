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
#include "Common/Time.h"

#if defined(_TARGET_PLATFORM_WINDOWS)
#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include <mmsystem.h>
#include <math.h>

#include "Common/PlatformThread.h"
#include "Common/Lock.h"
#include "Common/CPU.h"

using Common::Time;
using Common::TimeDelta;
using Common::TimeTicks;

namespace {
#define abs(x) ((x)<0?-(x):(x))

    template <class Dest, class Source>
    inline Dest bit_cast(const Source& source) {
      // Compile time assertion: sizeof(Dest) == sizeof(Source)
      // A compile error here means your Dest and Source have different sizes.
      typedef char VerifySizesAreEqual [sizeof(Dest) == sizeof(Source) ? 1 : -1];

      Dest dest;
      memcpy(&dest, &source, sizeof(dest));
      return dest;
    }

// From MSDN, FILETIME "Contains a 64-bit value representing the number of
// 100-nanosecond intervals since January 1, 1601 (UTC)."
int64_t FileTimeToMicroseconds(const FILETIME& ft) {
  // Need to bit_cast to fix alignment, then divide by 10 to convert
  // 100-nanoseconds to milliseconds. This only works on little-endian
  // machines.
  return bit_cast<int64_t, FILETIME>(ft) / 10;
}


void MicrosecondsToFileTime(int64_t us, FILETIME* ft) {
    assert(us >= 0ULL);
  // Multiply by 10 to convert milliseconds to 100-nanoseconds. Bit_cast will
  // handle alignment problems. This only works on little-endian machines.
  *ft = bit_cast<FILETIME, int64_t>(us * 10);
}

int64_t CurrentWallclockMicroseconds() {
  FILETIME ft;
  ::GetSystemTimeAsFileTime(&ft);
  return FileTimeToMicroseconds(ft);
}

// Time between resampling the un-granular clock for this API.  60 seconds.
const int kMaxMillisecondsToAvoidDrift = 60 * Time::kMillisecondsPerSecond;

int64_t initial_time = 0;
TimeTicks initial_ticks;

void InitializeClock() {
  initial_ticks = TimeTicks::Now();
  initial_time = CurrentWallclockMicroseconds();
}

}  // namespace

// Time -----------------------------------------------------------------------

// The internal representation of Time uses FILETIME, whose epoch is 1601-01-01
// 00:00:00 UTC.  ((1970-1601)*365+89)*24*60*60*1000*1000, where 89 is the
// number of leap year days between 1601 and 1970: (1970-1601)/4 excluding
// 1700, 1800, and 1900.
// static
const int64_t Time::kTimeTToMicrosecondsOffset = GG_INT64_C(11644473600000000);

bool Time::high_resolution_timer_enabled_ = false;


struct timeval TimeDelta::ToTimeVal() const {
  struct timeval result;
  int64_t us = InMicroseconds();
  result.tv_sec = (long) us / Time::kMicrosecondsPerSecond;
  result.tv_usec = us % Time::kMicrosecondsPerSecond;
  return result;
}

// static
Time Time::Now() {
  if (initial_time == 0)
    InitializeClock();

  // We implement time using the high-resolution timers so that we can get
  // timeouts which are smaller than 10-15ms.  If we just used
  // CurrentWallclockMicroseconds(), we'd have the less-granular timer.
  //
  // To make this work, we initialize the clock (initial_time) and the
  // counter (initial_ctr).  To compute the initial time, we can check
  // the number of ticks that have elapsed, and compute the delta.
  //
  // To avoid any drift, we periodically resync the counters to the system
  // clock.
  while (true) {
    TimeTicks ticks = TimeTicks::Now();

    // Calculate the time elapsed since we started our timer
    TimeDelta elapsed = ticks - initial_ticks;

    // Check if enough time has elapsed that we need to resync the clock.
    if (elapsed.InMilliseconds() > kMaxMillisecondsToAvoidDrift) {
      InitializeClock();
      continue;
    }

    return Time(elapsed + Time(initial_time));
  }
}

// static
Time Time::NowFromSystemTime() {
  // Force resync.
  InitializeClock();
  return Time(initial_time);
}

// static
Time Time::FromFileTime(FILETIME ft) {
  return Time(FileTimeToMicroseconds(ft));
}

FILETIME Time::ToFileTime() const {
  FILETIME utc_ft;
  MicrosecondsToFileTime(us_, &utc_ft);
  return utc_ft;
}

// static
void Time::EnableHighResolutionTimer(bool enable) {
  // Test for single-threaded access.
  static PlatformThreadId my_thread = PlatformThread::CurrentId();
  assert(PlatformThread::CurrentId() == my_thread);

  if (high_resolution_timer_enabled_ == enable)
    return;

  high_resolution_timer_enabled_ = enable;
}

// static
bool Time::ActivateHighResolutionTimer(bool activate) {
  if (!high_resolution_timer_enabled_)
    return false;

  // Using anything other than 1ms makes timers granular
  // to that interval.
  const int kMinTimerIntervalMs = 1;
  MMRESULT result;
  if (activate)
    result = timeBeginPeriod(kMinTimerIntervalMs);
  else
    result = timeEndPeriod(kMinTimerIntervalMs);
  return result == TIMERR_NOERROR;
}

// static
Time Time::FromExploded(bool is_local, const Exploded& exploded) {
  // Create the system struct representing our exploded time. It will either be
  // in local time or UTC.
  SYSTEMTIME st;
  st.wYear = exploded.year;
  st.wMonth = exploded.month;
  st.wDayOfWeek = exploded.day_of_week;
  st.wDay = exploded.day_of_month;
  st.wHour = exploded.hour;
  st.wMinute = exploded.minute;
  st.wSecond = exploded.second;
  st.wMilliseconds = exploded.millisecond;

  // Convert to FILETIME.
  FILETIME ft;
  if (!SystemTimeToFileTime(&st, &ft)) {
    assert(false);
    return Time(0);
  }

  // Ensure that it's in UTC.
  if (is_local) {
    FILETIME utc_ft;
    LocalFileTimeToFileTime(&ft, &utc_ft);
    return Time(FileTimeToMicroseconds(utc_ft));
  }
  return Time(FileTimeToMicroseconds(ft));
}

void Time::Explode(bool is_local, Exploded* exploded) const {
  // FILETIME in UTC.
  FILETIME utc_ft;
  MicrosecondsToFileTime(us_, &utc_ft);

  // FILETIME in local time if necessary.
  BOOL success = TRUE;
  FILETIME ft;
  if (is_local)
    success = FileTimeToLocalFileTime(&utc_ft, &ft);
  else
    ft = utc_ft;

  // FILETIME in SYSTEMTIME (exploded).
  SYSTEMTIME st;
  if (!success || !FileTimeToSystemTime(&ft, &st)) {
    assert(false);
    ZeroMemory(exploded, sizeof(exploded));
    return;
  }

  exploded->year = st.wYear;
  exploded->month = st.wMonth;
  exploded->day_of_week = st.wDayOfWeek;
  exploded->day_of_month = st.wDay;
  exploded->hour = st.wHour;
  exploded->minute = st.wMinute;
  exploded->second = st.wSecond;
  exploded->millisecond = st.wMilliseconds;
}

// TimeTicks ------------------------------------------------------------------
namespace {

// We define a wrapper to adapt between the __stdcall and __cdecl call of the
// mock function, and to avoid a static constructor.  Assigning an import to a
// function pointer directly would require setup code to fetch from the IAT.
DWORD timeGetTimeWrapper() {
  return timeGetTime();
}

DWORD (*tick_function)(void) = &timeGetTimeWrapper;

// Accumulation of time lost due to rollover (in milliseconds).
int64_t rollover_ms = 0;

// The last timeGetTime value we saw, to detect rollover.
DWORD last_seen_now = 0;

// Lock protecting rollover_ms and last_seen_now.
// Note: this is a global object, and we usually avoid these. However, the time
// code is low-level, and we don't want to use Singletons here (it would be too
// easy to use a Singleton without even knowing it, and that may lead to many
// gotchas). Its impact on startup time should be negligible due to low-level
// nature of time code.
Common::Lock rollover_lock;

// We use timeGetTime() to implement TimeTicks::Now().  This can be problematic
// because it returns the number of milliseconds since Windows has started,
// which will roll over the 32-bit value every ~49 days.  We try to track
// rollover ourselves, which works if TimeTicks::Now() is called at least every
// 49 days.
TimeDelta RolloverProtectedNow() {
  Common::AutoLock locked(rollover_lock);
  // We should hold the lock while calling tick_function to make sure that
  // we keep last_seen_now stay correctly in sync.
  DWORD now = tick_function();
  if (now < last_seen_now)
    rollover_ms += 0x100000000I64;  // ~49.7 days.
  last_seen_now = now;
  return TimeDelta::FromMilliseconds(now + rollover_ms);
}

// Overview of time counters:
// (1) CPU cycle counter. (Retrieved via RDTSC)
// The CPU counter provides the highest resolution time stamp and is the least
// expensive to retrieve. However, the CPU counter is unreliable and should not
// be used in production. Its biggest issue is that it is per processor and it
// is not synchronized between processors. Also, on some computers, the counters
// will change frequency due to thermal and power changes, and stop in some
// states.
//
// (2) QueryPerformanceCounter (QPC). The QPC counter provides a high-
// resolution (100 nanoseconds) time stamp but is comparatively more expensive
// to retrieve. What QueryPerformanceCounter actually does is up to the HAL.
// (with some help from ACPI).
// According to http://blogs.msdn.com/oldnewthing/archive/2005/09/02/459952.aspx
// in the worst case, it gets the counter from the rollover interrupt on the
// programmable interrupt timer. In best cases, the HAL may conclude that the
// RDTSC counter runs at a constant frequency, then it uses that instead. On
// multiprocessor machines, it will try to verify the values returned from
// RDTSC on each processor are consistent with each other, and apply a handful
// of workarounds for known buggy hardware. In other words, QPC is supposed to
// give consistent result on a multiprocessor computer, but it is unreliable in
// reality due to bugs in BIOS or HAL on some, especially old computers.
// With recent updates on HAL and newer BIOS, QPC is getting more reliable but
// it should be used with caution.
//
// (3) System time. The system time provides a low-resolution (typically 10ms
// to 55 milliseconds) time stamp but is comparatively less expensive to
// retrieve and more reliable.
class HighResNowSingleton {
 public:
  static HighResNowSingleton* GetInstance() {
      static HighResNowSingleton g_HighResNowSingleton;
      return &g_HighResNowSingleton;
    // return Singleton<HighResNowSingleton>::get();
  }

  bool IsUsingHighResClock() {
    return ticks_per_microsecond_ != 0.0;
  }

  void DisableHighResClock() {
    ticks_per_microsecond_ = 0.0;
  }

  TimeDelta Now() {
    if (IsUsingHighResClock())
      return TimeDelta::FromMicroseconds(UnreliableNow());

    // Just fallback to the slower clock.
    return RolloverProtectedNow();
  }

  int64_t GetQPCDriftMicroseconds() {
    if (!IsUsingHighResClock())
      return 0;

    return abs((UnreliableNow() - ReliableNow()) - skew_);
  }

 private:
  HighResNowSingleton()
    : ticks_per_microsecond_(0.0),
      skew_(0) {
    InitializeClock();

    // On Athlon X2 CPUs (e.g. model 15) QueryPerformanceCounter is
    // unreliable.  Fallback to low-res clock.
    Common::CPU cpu;
    if (cpu.vendor_name() == "AuthenticAMD" && cpu.family() == 15)
      DisableHighResClock();
  }

  // Synchronize the QPC clock with GetSystemTimeAsFileTime.
  void InitializeClock() {
    LARGE_INTEGER ticks_per_sec = {0};
    if (!QueryPerformanceFrequency(&ticks_per_sec))
      return;  // Broken, we don't guarantee this function works.
    ticks_per_microsecond_ = static_cast<float>(ticks_per_sec.QuadPart) /
      static_cast<float>(Time::kMicrosecondsPerSecond);

    skew_ = UnreliableNow() - ReliableNow();
  }

  // Get the number of microseconds since boot in an unreliable fashion.
  int64_t UnreliableNow() {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return static_cast<int64_t>(now.QuadPart / ticks_per_microsecond_);
  }

  // Get the number of microseconds since boot in a reliable fashion.
  int64_t ReliableNow() {
    return RolloverProtectedNow().InMicroseconds();
  }

  // Cached clock frequency -> microseconds. This assumes that the clock
  // frequency is faster than one microsecond (which is 1MHz, should be OK).
  float ticks_per_microsecond_;  // 0 indicates QPF failed and we're broken.
  int64_t skew_;  // Skew between lo-res and hi-res clocks (for debugging).

  //friend struct DefaultSingletonTraits<HighResNowSingleton>;
};

}  // namespace

// static
TimeTicks::TickFunctionType TimeTicks::SetMockTickFunction(
    TickFunctionType ticker) {
  TickFunctionType old = tick_function;
  tick_function = ticker;
  return old;
}

// static
TimeTicks TimeTicks::Now() {
  return TimeTicks() + RolloverProtectedNow();
}

// static
TimeTicks TimeTicks::HighResNow() {
  return TimeTicks() + HighResNowSingleton::GetInstance()->Now();
}

// static
int64_t TimeTicks::GetQPCDriftMicroseconds() {
  return HighResNowSingleton::GetInstance()->GetQPCDriftMicroseconds();
}

// static
bool TimeTicks::IsHighResClockWorking() {
  return HighResNowSingleton::GetInstance()->IsUsingHighResClock();
}

#endif

#if defined(_TARGET_PLATFORM_LINUX)
#include <sys/time.h>
#include <time.h>

#include <limits>

namespace Common {
    
struct timeval TimeDelta::ToTimeVal() const {
  struct timeval result;
  int64_t us = InMicroseconds();
  result.tv_sec = us / Time::kMicrosecondsPerSecond;
  result.tv_usec = us % Time::kMicrosecondsPerSecond;
  return result;
}

struct timespec TimeDelta::ToTimeSpec() const {
  int64_t microseconds = InMicroseconds();
  time_t seconds = 0;
  if (microseconds >= Time::kMicrosecondsPerSecond) {
    seconds = InSeconds();
    microseconds -= seconds * Time::kMicrosecondsPerSecond;
  }
  struct timespec result =
      {seconds,
       microseconds * Time::kNanosecondsPerMicrosecond};
  return result;
}

// The Time routines in this file use standard POSIX routines, or almost-
// standard routines in the case of timegm.  We need to use a Mach-specific
// function for TimeTicks::Now() on Mac OS X.

// Time -----------------------------------------------------------------------

// Windows uses a Gregorian epoch of 1601.  We need to match this internally
// so that our time representations match across all platforms.  See bug 14734.
//   irb(main):010:0> Time.at(0).getutc()
//   => Thu Jan 01 00:00:00 UTC 1970
//   irb(main):011:0> Time.at(-11644473600).getutc()
//   => Mon Jan 01 00:00:00 UTC 1601
static const int64_t kWindowsEpochDeltaSeconds = GG_INT64_C(11644473600);
static const int64_t kWindowsEpochDeltaMilliseconds =
    kWindowsEpochDeltaSeconds * Time::kMillisecondsPerSecond;

// static
const int64_t Time::kWindowsEpochDeltaMicroseconds =
    kWindowsEpochDeltaSeconds * Time::kMicrosecondsPerSecond;

// Some functions in time.cc use time_t directly, so we provide an offset
// to convert from time_t (Unix epoch) and internal (Windows epoch).
// static
const int64_t Time::kTimeTToMicrosecondsOffset = kWindowsEpochDeltaMicroseconds;

// static
Time Time::Now() {
  struct timeval tv;
  struct timezone tz = { 0, 0 };  // UTC
  if (gettimeofday(&tv, &tz) != 0) {
      assert(false);// Could not determine time of day
  }
  // Combine seconds and microseconds in a 64-bit field containing microseconds
  // since the epoch.  That's enough for nearly 600 centuries.  Adjust from
  // Unix (1970) to Windows (1601) epoch.
  return Time((tv.tv_sec * kMicrosecondsPerSecond + tv.tv_usec) +
      kWindowsEpochDeltaMicroseconds);
}

// static
Time Time::NowFromSystemTime() {
  // Just use Now() because Now() returns the system time.
  return Now();
}

void Time::Explode(bool is_local, Exploded* exploded) const {
  // Time stores times with microsecond resolution, but Exploded only carries
  // millisecond resolution, so begin by being lossy.  Adjust from Windows
  // epoch (1601) to Unix epoch (1970);
  int64_t milliseconds = (us_ - kWindowsEpochDeltaMicroseconds) /
      kMicrosecondsPerMillisecond;
  time_t seconds = milliseconds / kMillisecondsPerSecond;

  struct tm timestruct;
  if (is_local)
    localtime_r(&seconds, &timestruct);
  else
    gmtime_r(&seconds, &timestruct);

  exploded->year         = timestruct.tm_year + 1900;
  exploded->month        = timestruct.tm_mon + 1;
  exploded->day_of_week  = timestruct.tm_wday;
  exploded->day_of_month = timestruct.tm_mday;
  exploded->hour         = timestruct.tm_hour;
  exploded->minute       = timestruct.tm_min;
  exploded->second       = timestruct.tm_sec;
  exploded->millisecond  = milliseconds % kMillisecondsPerSecond;
}

// static
Time Time::FromExploded(bool is_local, const Exploded& exploded) {
  struct tm timestruct;
  timestruct.tm_sec    = exploded.second;
  timestruct.tm_min    = exploded.minute;
  timestruct.tm_hour   = exploded.hour;
  timestruct.tm_mday   = exploded.day_of_month;
  timestruct.tm_mon    = exploded.month - 1;
  timestruct.tm_year   = exploded.year - 1900;
  timestruct.tm_wday   = exploded.day_of_week;  // mktime/timegm ignore this
  timestruct.tm_yday   = 0;     // mktime/timegm ignore this
  timestruct.tm_isdst  = -1;    // attempt to figure it out
#if !defined(OS_NACL)
  timestruct.tm_gmtoff = 0;     // not a POSIX field, so mktime/timegm ignore
  timestruct.tm_zone   = NULL;  // not a POSIX field, so mktime/timegm ignore
#endif

  time_t seconds;
  if (is_local)
    seconds = mktime(&timestruct);
  else
    seconds = timegm(&timestruct);

  int64_t milliseconds;
  // Handle overflow.  Clamping the range to what mktime and timegm might
  // return is the best that can be done here.  It's not ideal, but it's better
  // than failing here or ignoring the overflow case and treating each time
  // overflow as one second prior to the epoch.
  if (seconds == -1 &&
      (exploded.year < 1969 || exploded.year > 1970)) {
    // If exploded.year is 1969 or 1970, take -1 as correct, with the
    // time indicating 1 second prior to the epoch.  (1970 is allowed to handle
    // time zone and DST offsets.)  Otherwise, return the most future or past
    // time representable.  Assumes the time_t epoch is 1970-01-01 00:00:00 UTC.
    //
    // The minimum and maximum representible times that mktime and timegm could
    // return are used here instead of values outside that range to allow for
    // proper round-tripping between exploded and counter-type time
    // representations in the presence of possible truncation to time_t by
    // division and use with other functions that accept time_t.
    //
    // When representing the most distant time in the future, add in an extra
    // 999ms to avoid the time being less than any other possible value that
    // this function can return.
    if (exploded.year < 1969) {
      milliseconds = std::numeric_limits<time_t>::min() *
                     kMillisecondsPerSecond;
    } else {
      milliseconds = (std::numeric_limits<time_t>::max() *
                      kMillisecondsPerSecond) +
                     kMillisecondsPerSecond - 1;
    }
  } else {
    milliseconds = seconds * kMillisecondsPerSecond + exploded.millisecond;
  }

  // Adjust from Unix (1970) to Windows (1601) epoch.
  return Time((milliseconds * kMicrosecondsPerMillisecond) +
      kWindowsEpochDeltaMicroseconds);
}


// static
TimeTicks TimeTicks::Now() {
  uint64_t absolute_micro;

  struct timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
    // NOTREACHED() << "clock_gettime(CLOCK_MONOTONIC) failed.";
    return TimeTicks();
  }

  absolute_micro =
      (static_cast<int64_t>(ts.tv_sec) * Time::kMicrosecondsPerSecond) +
      (static_cast<int64_t>(ts.tv_nsec) / Time::kNanosecondsPerMicrosecond);

  return TimeTicks(absolute_micro);
}

// static
TimeTicks TimeTicks::HighResNow() {
  return Now();
}

struct timespec Time::ToTimeSpec() const
{
  struct timespec result;
  int64_t us = us_ - kTimeTToMicrosecondsOffset;
  result.tv_sec = us / Time::kMicrosecondsPerSecond;
  result.tv_nsec  = (us % Time::kMicrosecondsPerSecond);
  result.tv_nsec *= Time::kNanosecondsPerMicrosecond;
  return result;
}

struct timeval Time::ToTimeVal() const {
  struct timeval result;
  int64_t us = us_ - kTimeTToMicrosecondsOffset;
  result.tv_sec = us / Time::kMicrosecondsPerSecond;
  result.tv_usec = us % Time::kMicrosecondsPerSecond;
  return result;
}

}  // namespace Common

#endif // _TARGET_PLATFORM_LINUX


namespace Common {

// TimeDelta ------------------------------------------------------------------

int TimeDelta::InDays() const {
  return static_cast<int>(delta_ / Time::kMicrosecondsPerDay);
}

int TimeDelta::InHours() const {
  return static_cast<int>(delta_ / Time::kMicrosecondsPerHour);
}

int TimeDelta::InMinutes() const {
  return static_cast<int>(delta_ / Time::kMicrosecondsPerMinute);
}

double TimeDelta::InSecondsF() const {
  return static_cast<double>(delta_) / Time::kMicrosecondsPerSecond;
}

int64_t TimeDelta::InSeconds() const {
  return delta_ / Time::kMicrosecondsPerSecond;
}

double TimeDelta::InMillisecondsF() const {
  return static_cast<double>(delta_) / Time::kMicrosecondsPerMillisecond;
}

int64_t TimeDelta::InMilliseconds() const {
  return delta_ / Time::kMicrosecondsPerMillisecond;
}

int64_t TimeDelta::InMillisecondsRoundedUp() const {
  return (delta_ + Time::kMicrosecondsPerMillisecond - 1) /
      Time::kMicrosecondsPerMillisecond;
}

int64_t TimeDelta::InMicroseconds() const {
  return delta_;
}

// Time -----------------------------------------------------------------------

// static
Time Time::FromTimeT(time_t tt) {
  if (tt == 0)
    return Time();  // Preserve 0 so we can tell it doesn't exist.
  return Time((tt * kMicrosecondsPerSecond) + kTimeTToMicrosecondsOffset);
}

time_t Time::ToTimeT() const {
  if (us_ == 0)
    return 0;  // Preserve 0 so we can tell it doesn't exist.
  return (us_ - kTimeTToMicrosecondsOffset) / kMicrosecondsPerSecond;
}

// static
Time Time::FromDoubleT(double dt) {
  if (dt == 0)
    return Time();  // Preserve 0 so we can tell it doesn't exist.
  return Time(static_cast<int64_t>((dt *
                                  static_cast<double>(kMicrosecondsPerSecond)) +
                                 kTimeTToMicrosecondsOffset));
}

double Time::ToDoubleT() const {
  if (us_ == 0)
    return 0;  // Preserve 0 so we can tell it doesn't exist.
  return (static_cast<double>(us_ - kTimeTToMicrosecondsOffset) /
          static_cast<double>(kMicrosecondsPerSecond));
}

// static
Time Time::UnixEpoch() {
  Time time;
  time.us_ = kTimeTToMicrosecondsOffset;
  return time;
}

Time Time::LocalMidnight() const {
  Exploded exploded;
  LocalExplode(&exploded);
  exploded.hour = 0;
  exploded.minute = 0;
  exploded.second = 0;
  exploded.millisecond = 0;
  return FromLocalExploded(exploded);
}

/*
// static
bool Time::FromString(const wchar_t* time_string, Time* parsed_time) {
  assert((time_string != NULL) && (parsed_time != NULL));
  std::string ascii_time_string = SysWideToUTF8(time_string);
  if (ascii_time_string.length() == 0)
    return false;
  PRTime result_time = 0;
  PRStatus result = PR_ParseTimeString(ascii_time_string.c_str(), PR_FALSE,
                                       &result_time);
  if (PR_SUCCESS != result)
    return false;
  result_time += kTimeTToMicrosecondsOffset;
  *parsed_time = Time(result_time);
  return true;
}
*/

// Time::Exploded -------------------------------------------------------------

inline bool is_in_range(int value, int lo, int hi) {
  return lo <= value && value <= hi;
}

bool Time::Exploded::HasValidValues() const {
  return is_in_range(month, 1, 12) &&
         is_in_range(day_of_week, 0, 6) &&
         is_in_range(day_of_month, 1, 31) &&
         is_in_range(hour, 0, 23) &&
         is_in_range(minute, 0, 59) &&
         is_in_range(second, 0, 60) &&
         is_in_range(millisecond, 0, 999);
}

}  // namespace base

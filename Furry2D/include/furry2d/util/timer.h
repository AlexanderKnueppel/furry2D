#ifndef __FURRY_UTIL_TIMER_H__
#define __FURRY_UTIL_TIMER_H__ 

#if __cplusplus < 201103L && (!defined(_MSC_VER) || _MSC_VER < 1700)
#error furry2d::Timer class requires C++11 support
#else

#include <chrono>
#include <ostream>

FURRY_NS_BEGIN

/**
* \brief Simple class for timing issues
*
* \ingroup util
*/
class Timer {
	typedef std::chrono::high_resolution_clock HighResolutionClock;
	typedef std::chrono::milliseconds Milliseconds;
public:
	explicit Timer(bool run = false) {
		if (run)
			reset();
	}
	void reset() {
		start_ = HighResolutionClock::now();
	}
	Milliseconds elapsed() const {
		return std::chrono::duration_cast<Milliseconds>(HighResolutionClock::now() - start_);
	}
	template <typename T, typename Traits>
	friend std::basic_ostream<T, Traits>& operator<<(std::basic_ostream<T, Traits>& out, const Timer& timer) {
		return out << timer.Elapsed().count();
	}
private:
	HighResolutionClock::time_point start_;
};

FURRY_NS_END

#endif

#endif
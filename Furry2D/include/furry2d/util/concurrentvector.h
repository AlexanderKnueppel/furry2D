#ifndef __FURRY_UTIL_CONVECTOR_H__
#define __FURRY_UTIL_CONVECTOR_H__  

/*
* ****************************************
*
* This file is part of Furry2D, a simple gameframework for 2D desktop games.
*
* Copyright (c) 2015 Furry2D. All rights reserved.
*
* For the full copyright and license information, please view the LICENSE.txt
* file that was distributed with this source code.
*
* \Author Alexander Knueppel
*
* ****************************************
*/

#include <vector>
#include <mutex>
#include <condition_variable>

#include <furry2d/preprocessor.h>

FURRY_NS_BEGIN

/**
* \brief Vector for threadsafe operations
*
* \ingroup util
*/
template <typename T>
class ConcurrentVector {
public:
	typedef std::mutex Mutex;
	typedef std::lock_guard<Mutex> ScopedLock;
	typedef std::unique_lock<Mutex> UniqueLock;
	typedef std::condition_variable ConditionVariable;

	void push_back(const T& item) {
		ScopedLock lock(mutex_);
		internal_vector_.push_back(item);
		condition_variable_.notify_all();
	}

	void push_back(T&& item) {
		ScopedLock lock(mutex_);
		internal_vector_.push_back(std::move(item));
		condition_variable_.notify_all();
	}

	void pop_back(T& item) {
		UniqueLock lock(mutex_);

		condition_variable_.wait(lock, [&] { return !internal_vector_.empty(); });

		if (!internal_vector_.empty()) {
			item = std::move(internal_vector_.back());
			internal_vector_.pop_back();
		}
	}

	void swap(ConcurrentVector& other) {
		ScopedLock lockSelf(mutex_);
		ScopedLock lockOther(other.mutex_);

		std::swap(internal_vector_, other.internal_vector_);
	}

	const std::vector<T>& getInternalsUnsafe() const {
		return internal_vector_;
	}

	std::vector<T> copyInternals() const {
		ScopedLock lock(mutex_);
		return internal_vector_;
	}

	void remove_if(std::function<bool(const T& value)> condition) {
		ScopedLock lock(mutex_);

		auto it = std::remove_if(
			internal_vector_.begin(),
			internal_vector_.end(),
			condition
			);

		if (it != internal_vector_.end())
			internal_vector_.erase(it);
	}

private:
	std::vector<T> internal_vector_;
	mutable Mutex mutex_;
	ConditionVariable condition_variable_;
};

FURRY_NS_END

#endif

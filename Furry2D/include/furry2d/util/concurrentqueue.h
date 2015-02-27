#ifndef __FURRY_UTIL_CONCURRENTQUEUE_H__
#define __FURRY_UTIL_CONCURRENTQUEUE_H__  

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

#include <queue>
#include <mutex>
#include <condition_variable>

FURRY_NS_BEGIN

/**
* \brief Queue for threadsafe operations
*
* \ingroup util
*/
template<typename T>
class ConcurrentQueue {
private:
	typedef std::mutex Mutex;
	typedef std::lock_guard<Mutex> ScopedGuard;
	typedef std::unique_lock<Mutex> UniqueLock;
	typedef std::condition_variable ConditionVariable;
public:
	ConcurrentQueue() {}
	ConcurrentQueue(const ConcurrentQueue&) = delete;
	ConcurrentQueue& operator=(const ConcurrentQueue&) = delete;

	bool isEmpty() const {
		ScopedGuard sg(mutex_);
		return queue_.empty();
	}

	size_t size() const {
		ScopedGuard sg(mutex_);
		return queue_.size();
	}

	T pop() { //blocks the thread until more work is coming
		UniqueLock lock(mutex_);

		while (queue_.empty())
			condition_variable_.wait(lock);

		auto result = std::move(queue_.front());
		queue_.pop();
		return result;
	}

	bool try_pop(T& result) { 
		ScopedGuard sg(mutex_);

		if (queue_.empty())
			return false;

		result = std::move(queue_.front());
		queue_.pop();

		return true;
	}

	void push(const T& element) {
		{
			ScopedGuard sg(mutex_);
			queue_.push(element);
		}
		condition_variable_.notify_one();
	}
	void push(T&& element) {
		{
			ScopedGuard sg(mutex_);
			queue_.push(std::forward<T>(element));
		}
		condition_variable_.notify_one();
	}
private:
	std::queue<T> queue_;
	mutable Mutex mutex_;
	ConditionVariable condition_variable_;
};

FURRY_NS_END

#endif
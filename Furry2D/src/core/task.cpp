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


#include <furry2d/furry2d.h>
#include <thread>

FURRY_NS_BEGIN

namespace detail {
	/*** WrappedTask ***/
	WrappedTask::WrappedTask() {}

	WrappedTask::WrappedTask(Task task, bool repeating, bool background) :
		unwrapped_task_{ std::move(task) } {
		is_repeating_ = repeating;
		is_background_ = background;
	}

	void WrappedTask::operator()() const {

		try {
			// execute the task
			unwrapped_task_();
		}
		catch (const std::exception& ex) {
			gLogError << "Exception: " << ex.what();
		}
		catch (...) {
			gLogError << "Unknown exception";
		}
	}

	bool WrappedTask::isRepeating() const {
		return is_repeating_;
	}

	bool WrappedTask::isBackground() const {
		return is_background_;
	}

	void WrappedTask::setRepeating(bool enabled) {
		is_repeating_ = enabled;
	}

	void WrappedTask::setBackground(bool enabled) {
		is_background_ = enabled;
	}
}

detail::WrappedTask make_wrapped(
	Task task,
	bool repeating,
	bool background) {
	return detail::WrappedTask{ task, repeating, background };
}

FURRY_NS_END
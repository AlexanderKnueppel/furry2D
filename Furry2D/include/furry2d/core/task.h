#ifndef __FURRY_CORE_TASK_H__
#define __FURRY_CORE_TASK_H__

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

#include <cstdint>
#include <functional>

FURRY_NS_BEGIN

// The main Task typedef
typedef std::function<void()> Task;

namespace detail {
	struct FURRY_API WrappedTask {
	public:
		explicit WrappedTask();
		explicit WrappedTask(Task t, bool repeating, bool background);

		void operator()() const;

		bool isRepeating() const;
		bool isBackground() const;

		friend class TaskProcessor;

	private:
		void setRepeating(bool enabled);
		void setBackground(bool enabled); // modifying the background status does not change the execution style immediately - it only affects future executions of this task

		Task unwrapped_task_;
		uint32_t is_repeating_ : 1;
		uint32_t is_background_ : 1;
	};
}

//in the end, this became a trivial function
detail::WrappedTask FURRY_API make_wrapped(
	Task task,
	bool repeating = false,
	bool isBackground = false
);

FURRY_NS_END

#endif
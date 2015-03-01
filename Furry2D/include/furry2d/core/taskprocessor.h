#ifndef __FURRY_CORE_TASKPROCESSOR_H__
#define __FURRY_CORE_TASKPROCESSOR_H__

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
#include <thread>

FURRY_NS_BEGIN

class FURRY_API TaskProcessor {
private:
	typedef ConcurrentVector<detail::WrappedTask> TaskQueue;

public:
	TaskProcessor(size_t numWorkers = 0); //use 0 for autodetect

	void addWork(Task t, bool repeating = false, bool background = false);

	// explicit aliases for various types of work
	void addRepeatingWork(Task t, bool background = false);
	void addBackgroundWork(Task t, bool repeating = false);
	void addRepeatingBackgroundWork(Task t);

	void start();
	void stop();

private:
	void addWork(detail::WrappedTask t);	//places the task in the appropriate queue
	void execute(detail::WrappedTask t);

	TaskQueue main_tasks_;		//executed in this thread
	TaskQueue background_tasks_;	//launched in a separate thread (use this for lengthy operations, such as I/O)

	bool is_running_;

	size_t num_workers_;
	std::vector<std::thread> background_workers_;
};

FURRY_NS_END

#endif
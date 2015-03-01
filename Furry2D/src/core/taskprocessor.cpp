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

TaskProcessor::TaskProcessor(size_t numWorkers) :
	num_workers_(numWorkers),
	is_running_(false)
{
	if (num_workers_ == 0)
		num_workers_ = std::thread::hardware_concurrency() - 1; // look at the available number of hardware threads, but leave one for the main thread

	if (num_workers_ < 1)
		num_workers_ = 1; // keep at least 1 background thread
}

void TaskProcessor::addWork(Task t, bool repeating, bool background) {
	addWork(make_wrapped(std::move(t), repeating, background));
}

void TaskProcessor::addRepeatingWork(Task t, bool background) {
	addWork(std::move(t), true, background);
}

void TaskProcessor::addBackgroundWork(Task t, bool repeating) {
	addWork(std::move(t), repeating, true);
}

void TaskProcessor::addRepeatingBackgroundWork(Task t) {
	addWork(std::move(t), true, true);
}

void TaskProcessor::start() {
	if (is_running_)
		return; // already started

	is_running_ = true;
	gLog << "Starting " << num_workers_ << " background threads";

	// start the workers
	for (size_t i = 0; i < num_workers_; ++i) {
		background_workers_.push_back(std::thread([&] {
			while (is_running_) {
				detail::WrappedTask t;

				background_tasks_.pop_back(t);

				if (is_running_)
					execute(t);
			}
		}));
	}

	// start the main executor
	while (is_running_) {
		TaskQueue localQueue;
		main_tasks_.swap(localQueue);

		const auto& taskList = localQueue.getInternalsUnsafe();
		for (const auto& task : taskList)
			execute(task);
	}
}

void TaskProcessor::stop() {
	is_running_ = false;

	for (size_t i = 0; i < num_workers_; ++i)
		addBackgroundWork([&] { is_running_ = false; });

	for (auto &t : background_workers_)
		t.join();
}

void TaskProcessor::addWork(detail::WrappedTask t) {
	if (t.isBackground())
		background_tasks_.push_back(std::move(t));
	else
		main_tasks_.push_back(std::move(t));
}

void TaskProcessor::execute(detail::WrappedTask t) {
	t();

	if (t.isRepeating())
		addWork(std::move(t));
}

FURRY_NS_END
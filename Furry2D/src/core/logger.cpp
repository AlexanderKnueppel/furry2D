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

FURRY_NS_BEGIN

Logger::Logger(const std::string& filename) {
	active_ = Active::create();

	add(makeConsoleSink());
	add(makeFileSink(filename));
}

LogMessage Logger::operator()(
	LogLevel level,
	const std::string& filename,
	int line
	) {
	return LogMessage(
		level,
		filename,
		line,
		this
		);
}

void Logger::add(const LogSink& sink) {
	sinks_.push_back(sink); // perhaps check for duplicates?
}

void Logger::remove(const LogSink& sink) {
	auto it = std::find(sinks_.begin(), sinks_.end(), sink);

	if (it == sinks_.end())
		throw std::runtime_error("Tried to remove a sink that was not added yet");

	sinks_.erase(it);
}

void Logger::flush(const LogMessage& message) const {
	/*
	// This is the single-threaded version
	auto msg = message.buffer_.str();

	for (auto&& sink: sinks_)
	sink.forward(message.meta_, msg);
	*/

	// This is the Active Object (and threadsafe) version

	auto&& sinks = sinks_;
	auto&& meta = message.meta_;
	auto msg = message.buffer_.str();

	active_->send([=] {
		for (auto&& sink : sinks)
			sink.forward(meta, msg);
	});
}

FURRY_NS_END
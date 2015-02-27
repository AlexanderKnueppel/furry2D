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
#include <atomic>

FURRY_NS_BEGIN

LogMessage::LogMessage( LogLevel level,
						const std::string& file,
						int line,
						Logger* owner) : owner_(owner) {
	meta_ = { level, file, line };
}

LogMessage::~LogMessage() {
	if (owner_ && logLevel(meta_.level_))
		owner_->flush(*this);
}

LogMessage::LogMessage(LogMessage&& message) :  meta_(std::move(message.meta_)),
												owner_(std::move(message.owner_)),
												buffer_(std::move(message.buffer_)) {
	message.owner_ = nullptr;
}

LogMessage& LogMessage::operator << (std::ostream& (*fn)(std::ostream& os)) {
	fn(buffer_);
	return *this;
}

FURRY_NS_END
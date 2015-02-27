#ifndef __FURRY_CORE_LOGMESSAGE_H__
#define __FURRY_CORE_LOGMESSAGE_H__  

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

FURRY_NS_BEGIN

class LogSink;
/**
* \brief Logger class
*
* \ingroup core
*/
class FURRY_API LogMessage {
private:
	friend class Logger;
	friend class LogSink;

	LogMessage(
		LogLevel level,
		const std::string& file,
		int line,
		Logger* owner
		);

public:
	~LogMessage();

	LogMessage(LogMessage&& message);

	template <typename T>
	LogMessage& operator << (const T& message) {
		buffer_ << message;
		return *this;
	}
	LogMessage& operator << (std::ostream& (*fn)(std::ostream& os));

	struct Meta {
		LogLevel level_;
		std::string file_;
		int line_;
	};

private:
	Meta meta_;
	Logger* owner_; // the spawning object is guaranteed to outlive this one, so it's fine to use a raw pointer here
	std::ostringstream buffer_;
};

FURRY_NS_END

#endif
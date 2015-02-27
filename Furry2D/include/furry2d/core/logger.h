#ifndef __FURRY_CORE_LOGGER_H__
#define __FURRY_CORE_LOGGER_H__  

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
class LogMessage;
/**
* \brief Logger class
*
* \ingroup core
*/
class FURRY_API Logger {
public:
	//constructors for local scope.
	Logger(const std::string& filename);
	LogMessage operator()(LogLevel level, const std::string& filename, int line);

	//singleton interface for global scope. 
	static Logger& instance() {
		static Logger instance("Furry2d-dev.log");
		return instance;
	}

	void add(const LogSink& sink);
	void remove(const LogSink& sink);

	void flush(const LogMessage& message) const;

private:
	std::vector<LogSink> sinks_;
	std::unique_ptr<Active> active_;
};

#define gLogLevel(level) ::FURRY_NS::Logger::instance()( \
	::FURRY_NS::LogLevel::level, \
	__FILE__, \
	__LINE__ \
	)

#define gLog        gLogLevel(EMessage)

#define gLogDebug   gLogLevel(EDebug)
#define gLogMessage gLogLevel(EMessage)
#define gLogError   gLogLevel(EError)
#define gLogWarning gLogLevel(EWarning)
#define gLogFatal   gLogLevel(EFatal)

FURRY_NS_END

#endif
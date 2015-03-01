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
#include <fstream>
#include <memory>
#include <chrono>
#include <ctime>
#include <iomanip>

FURRY_NS_BEGIN

LogSink::LogSink(const LogSink& sink) :
wrapper_(sink.wrapper_->clone())
{
}

LogSink& LogSink::operator = (const LogSink& sink) FURRY_NOEXCEPT {
	wrapper_.reset(sink.wrapper_->clone());
	return *this;
}

bool LogSink::operator == (const LogSink& sink) const {
	return (wrapper_.get() == sink.wrapper_.get());
}

void LogSink::forward(
	const LogMessage::Meta& meta,
	const std::string& message
	) const {
	wrapper_->forward(meta, message);
}

std::ostream& operator<< (std::ostream& os, const LogLevel& level) {
	switch (level) {
	case LogLevel::EDebug:		os << "[dbg] ";		break;
	case LogLevel::EMessage:	os << "      ";		break;
	case LogLevel::EWarning:	os << ">wrn< ";		break;
	case LogLevel::EError:		os << "< ERROR >      ";	break;	// extra noticable
	case LogLevel::EFatal:		os << ">>>>FATAL<<<<  ";	break;	// extra noticable
	default:
		os << "Unknown";
	}

	return os;
}

LogSink makeConsoleSink() {
	return[](
		const LogMessage::Meta& meta,
		const std::string& message
		) {
		operator<<(std::cout, meta.level_);
		std::cout
			<< meta.level_
			<< message
			<< "\n";
	};
}
namespace {
	struct FileSink {
		FileSink(const std::string& filename) :
		file_(std::make_shared<std::ofstream>(filename))
		{
			if (!file_->good()) {
				std::string message = "Failed to open file sink: ";
				message.append(filename);
				throw std::runtime_error(message);
			}
		}

		void operator()(
			const LogMessage::Meta& meta,
			const std::string& message
			) const {
			using namespace std::chrono;

			auto now = system_clock::now();
			auto time_t = system_clock::to_time_t(now);
#ifdef FURRY_COMPILER_VC
			tm lt;
			localtime_s(&lt, &time_t);
			auto local_time = &lt;
#else
			auto local_time = std::localtime(&time_t);
#endif
			(*file_)
				<< std::put_time(local_time, "[%H:%M:%S] ")
				<< meta.level_
				<< message
				<< " ("
				<< meta.file_
				<< ":"
				<< meta.line_
				<< ")\n";
		}

		std::shared_ptr<std::ofstream> file_;
	};
}

LogSink makeFileSink(const std::string& filename) {
	return FileSink(filename);
}

FURRY_NS_END
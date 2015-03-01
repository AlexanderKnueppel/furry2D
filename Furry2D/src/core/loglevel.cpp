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

namespace { //make these atomics only accessible in this file
	std::atomic<bool> gLogLevelState[5]{
		{ true },	// Debug
		{ true },	// Message
		{ true },	// Warning
		{ true },	// Error
		{ true }	// Fatal
	};

	inline int selectLogState(LogLevel level) {
		switch (level) {
		case LogLevel::EDebug:		return 0;
		case LogLevel::EMessage:	return 1;
		case LogLevel::EWarning:	return 2;
		case LogLevel::EError:		return 3;
		case LogLevel::EFatal:		return 4;
		default:
			throw std::runtime_error("Unsupported loglevel");
		}
	}
}

void setLogLevel(LogLevel level, bool enabled) {
	int lvl = selectLogState(level);
	gLogLevelState[lvl].store(enabled, std::memory_order_release);
}

bool logLevel(LogLevel level) {
	int lvl = selectLogState(level);
	return gLogLevelState[lvl].load(std::memory_order_acquire);
}

FURRY_NS_END
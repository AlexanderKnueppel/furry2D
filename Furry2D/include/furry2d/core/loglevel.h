#ifndef __FURRY_CORE_LOGLEVEL_H__
#define __FURRY_CORE_LOGLEVEL_H__  

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

#include <ostream>

FURRY_NS_BEGIN

/**
* \brief Logger level
*
* \ingroup core
*/
enum class LogLevel {
	EDebug, EMessage, EWarning, EError, EFatal
};

void FURRY_API setLogLevel(LogLevel, bool enabled);
bool FURRY_API logLevel(LogLevel);

FURRY_NS_END

#endif
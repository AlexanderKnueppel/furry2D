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

Version::Version(const std::string &versionString) { 
	std::istringstream iss(versionString);
	std::string str[3]; 

	for (int i = 0; i < 3; ++i)
		std::getline(iss, str[i], '.');

	std::istringstream(str[0]) >> major_;
	std::istringstream(str[1]) >> minor_;
	std::istringstream(str[2]) >> release_;
}

std::string Version::toString() const {
	std::stringstream ss;
	ss << major_ << "." << minor_ << "." << release_;
	return ss.str();
}
std::string Version::toStringComplete() const {
	std::stringstream ss;
	ss << toString() << " (" << FURRY_PLATFORM_STRING;
#ifdef FURRY_64BIT
	ss << ", 64 Bit";
#else
	ss << ", 32 Bit";
#endif
	ss << ")";
	return ss.str();
}

FURRY_NS_END


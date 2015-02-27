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

System::System(std::string name) : name_{std::move(name)}
{}

bool System::initialize() {
#ifdef FURRY_DEBUG
	gLog << "Initializing " << getName();
#endif
	return true;
}

void System::update() {}

void System::shutdown() {
#ifdef FURRY_DEBUG
	gLog << "Shutting down " << getName();
#endif
}
FURRY_NS_END
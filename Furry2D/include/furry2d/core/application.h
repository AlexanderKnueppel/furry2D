#ifndef __FURRY_CORE_APPLICATION_H__
#define __FURRY_CORE_APPLICATION_H__

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
/*
* Conceptually an 'application' is very similar to an engine subsystem,
* only it can assume that all engine subsystems have been initialized already.
* This exists in an attempt to separate 'application logic' from 'library logic'
*/
class FURRY_API Application : public System {
public:
	Application(std::string name) : System(name) {}

	// While substems have optional implementations of these, Applications should always provide these
	//virtual bool initialize() {
	//	return true;
	//};
	//virtual void update() {};
	//virtual void shutdown() {};
			
	virtual void render(double elapsedTime) = 0;
	virtual void update(double elapsedTime) = 0;
	
	virtual bool initializeDependencies() {
		return true;
	}
	virtual void shutdownDependencies() {}
};

FURRY_NS_END

#endif
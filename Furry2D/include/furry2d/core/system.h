#ifndef __FURRY_CORE_SYSTEM_H__
#define __FURRY_CORE_SYSTEM_H__  

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

class Engine;

// Abstract 'system' (aka module)
class FURRY_API System {
public:
	friend class Engine;

	System(std::string name);

	System(const System&) = delete;
	System& operator = (const System&) = delete;

	virtual bool initialize();
	virtual void update();
	virtual void shutdown();

	const std::string& getName() const {
		return name_;
	}

protected:
	void setName(std::string name);

	Channel channel_;
	Engine* engine_;
	std::string name_;
};

FURRY_NS_END

#endif
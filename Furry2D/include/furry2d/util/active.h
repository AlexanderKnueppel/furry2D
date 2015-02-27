#ifndef __FURRY_UTIL_ACTIVE_H__
#define __FURRY_UTIL_ACTIVE_H__  

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

#include <memory>
#include <thread>

FURRY_NS_BEGIN

/**
* \brief Implements the active pattern 
* (see: http://www.drdobbs.com/parallel/prefer-using-active-objects-instead-of-n/225700095?pgno=1)
*
* \ingroup util
*/

class FURRY_API Active {
private:
	typedef std::function<void()> Callback;
	Active();
public:
	Active(const Active&) = delete;
	Active& operator = (const Active&) = delete;

	~Active();

	static std::unique_ptr<Active> create();
	void send(Callback message);

private:
	void run();

	bool done_;
	ConcurrentQueue<Callback> queue_;
	std::thread thread_;
};


FURRY_NS_END

#endif
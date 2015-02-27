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

std::unique_ptr<Active> Active::create() {
	std::unique_ptr<Active> res(new Active);
	//signature: void Active::run(Active *this) -> therefore bind res on method run
	res->thread_ = std::thread(&Active::run, res.get()); 
	return res;
}

Active::Active() : done_(false) {
}

Active::~Active() {
	send([this]{
		done_ = true;
	});
	thread_.join();
}

void Active::send(Callback message) {
	queue_.push(std::move(message));
}

void Active::run() {
	while (!done_) {
		Callback cb;
		cb = queue_.pop();
		cb();
	}
}

FURRY_NS_END
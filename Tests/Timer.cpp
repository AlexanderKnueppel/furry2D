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
#include <chrono>
#include <gmock/gmock.h>

using ::FURRY_NS::Timer;
using ::testing::Eq;
using ::testing::A;
using ::testing::HasSubstr;

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

TEST(Timer, DefaultConstructorHoldsClocksEpochAsTimePoint) {
	Timer timer;
	using Ms = std::chrono::milliseconds;
	 long long since_epoch = std::chrono::duration_cast<Ms>(Clock::now().time_since_epoch()).count();
	 long long t = timer.elapsed().count();

	EXPECT_NEAR(since_epoch, t, 5);
}

TEST(Timer, ResetWorksProperly) {
	std::chrono::seconds sec(2);
	int max_diff = 2; 
	Timer timer(true);
	EXPECT_NEAR(timer.elapsed().count(), 0, max_diff);
	std::this_thread::sleep_for(sec);
	timer.reset();
	EXPECT_NEAR(timer.elapsed().count(), 0, max_diff);
	std::this_thread::sleep_for(sec);
	timer.reset();
	EXPECT_NEAR(timer.elapsed().count(), 0, max_diff);
	std::this_thread::sleep_for(sec);
}

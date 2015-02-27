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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

/*
* Entry point for unit tests with google test
*/
int main(int argc, char** argv) {
	testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}

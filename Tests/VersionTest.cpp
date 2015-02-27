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

#include <gmock/gmock.h>

using ::FURRY_NS::Version;
using ::testing::Eq;
using ::testing::HasSubstr;

TEST(Version, HoldsZeroVersionIfUninitialized) {
	Version version;
	ASSERT_THAT(version.getMajorVersion(), Eq(0));
	ASSERT_THAT(version.getMinorVersion(), Eq(0));
	ASSERT_THAT(version.getRelease(), Eq(0));
}

TEST(Version, HoldsCorrectVersionIfInitialized) {
	Version version(1,2,3);
	ASSERT_THAT(version.getMajorVersion(), Eq(1));
	ASSERT_THAT(version.getMinorVersion(), Eq(2));
	ASSERT_THAT(version.getRelease(), Eq(3));
}

TEST(Version, TokenizesGivenVersionStringCorrectly) {
	Version version("1.2.3");
	ASSERT_THAT(version.getMajorVersion(), Eq(1));
	ASSERT_THAT(version.getMinorVersion(), Eq(2));
	ASSERT_THAT(version.getRelease(), Eq(3));
}

TEST(Version, ReturnsVersionAsString) {
	Version version(1,2,3);
	ASSERT_THAT(version.toString(), Eq("1.2.3"));
}

TEST(Version, ReturnsVersionAsStringWithArchitecture) {
	Version version(1, 2, 3);

	ASSERT_THAT(version.toStringComplete(), HasSubstr("1.2.3"));

#ifdef FURRY_PLATFORM_WINDOWS
	ASSERT_THAT(version.toStringComplete(), testing::HasSubstr("Windows"));
#elif defined(FURRY_PLATFORM_LINUX)
	ASSERT_THAT(version.toStringComplete(), testing::HasSubstr("Linux"));
#else
	ASSERT_THAT(version.toStringComplete(), testing::HasSubstr("Unknown"));
#endif

#if defined(_M_X64) || defined(__amd64__)
	ASSERT_THAT(version.toStringComplete(), testing::HasSubstr("64 Bit"));
#else
	ASSERT_THAT(version.toStringComplete(), testing::HasSubstr("32 Bit"));
#endif
}

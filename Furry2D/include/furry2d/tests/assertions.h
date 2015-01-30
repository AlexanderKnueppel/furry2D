#ifndef __FURRY_TEST_ASSERTIONS_H__
#define __FURRY_TEST_ASSERTIONS_H__

/*
* ****************************************
* Copyright (c) 2014 Furry Graphics/Game Engine. All rights reserved.
*
* For the full copyright and license information, please view the LICENSE.txt
* file that was distributed with this source code.
*
* \Author Alexander Knueppel
*
* ****************************************
*/

/*
* Minimalistic unit testing framework for the purpose of TDD.
* Intention: Test suits and cases, fixtures, mock objects... we'll see how complex it will get
*/
#include <string>

FURRY_NS_BEGIN

	namespace test {
		namespace detail {
			template <typename T>
			std::string _buildString(const detail::expression<T> &e) {
				return e.toString();
			}
		}

		class assert {
		public:
			template <typename T, typename E>
			static void that(const T& type1, const detail::expression<E> &expression, std::string file, int line) {
				if (!detail::_evaluate(type1, expression)) {
					std::stringstream sstr;
					sstr << '\n';
					sstr << "Actual value: " << type1 << std::endl;
					sstr << "Expected value " << detail::_buildString(expression) << std::endl; //to be implemented
					throw detail::assertion_exception(sstr.str(), file, line);
				}
			}

		};
	}

FURRY_NS_END

#define assert_that(p1,p2)\
	furry2d::test::assert::that((p1), (p2), __FILE__, __LINE__); \

#endif
#ifndef __FURRY_TEST_TEST_H__
#define __FURRY_TEST_TEST_H__

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
* Intention: Test suits and cases, fixtures, mock objects
*/

#include <furry2d/preprocessor.h>

#include <memory>
#include <functional>
#include <stdexcept>
#include <list>
#include <sstream>
#include <string>

FURRY_NS_BEGIN

namespace test {
	namespace detail {
		typedef std::function <void()> voidf_t;

		struct test_exception : public std::runtime_error {
			test_exception(const char* message) : std::runtime_error(message) {}
		};

		struct assertion_exception : public std::runtime_error {
			assertion_exception(const std::string& message,
			const std::string& filename, const unsigned int linenumber)
			: std::runtime_error(message), file_name_(filename), line_number_(linenumber) {}

			assertion_exception(const std::string& message)
				: std::runtime_error(message), line_number_(0) {}

			virtual ~assertion_exception() throw() {}

			const std::string& file_name() const {
				return file_name_;
			}

			unsigned int line_number() const {
				return line_number_;
			}

		private:
			std::string file_name_;
			unsigned int line_number_;
		};

			
		typedef std::list<voidf_t> test_registry;

		inline test_registry& specs() {
			static test_registry registry;
			return registry;
		}

		struct test_registrar {
			test_registrar(voidf_t func) {
				specs().push_back(func);
			}
		};
			
	}
}

FURRY_NS_END

#define furry_add_test \
	static furry2d::test::detail::test_registrar registrar

/*
* includes
*/
#include <furry2d/tests/context.h>
#include <furry2d/tests/reporter.h>
#include <furry2d/tests/grammar.h>
#include <furry2d/tests/expression.h>
#include <furry2d/tests/assertions.h>

/*
* main runner
*/
FURRY_NS_BEGIN

namespace test {
	inline int go(int &argc, char** argv) {
		detail::reg_reporter().start();

		detail::context global_context("");
		detail::context_stack().push_back(global_context);

		for (detail::voidf_t f : detail::specs()) {
			f();
		}
			
		detail::reg_reporter().complete();
		return detail::reg_reporter().pass() ? 0 : 1;
	}
}

FURRY_NS_END

#define FURRY_RUN_ALL_TESTS \
	int main(int argc, char** argv) { \
		int r = furry2d::test::go(argc, argv); \
		std::cin.get(); \
	}
	

#endif
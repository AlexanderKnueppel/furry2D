#ifndef __FURRY_TEST_GRAMMAR_H__
#define __FURRY_TEST_GRAMMAR_H__

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

FURRY_NS_BEGIN

namespace test {

	inline void describe(std::string desc, detail::voidf_t f, detail::reporter& reporter, detail::contextstack_t& context_stack) {
		reporter.ctx_start(desc);
			
		detail::context c(desc);
		context_stack.back().execute();

		context_stack.push_back(c);
		try {
			f();
		}
		catch (const detail::test_exception& e) {
			reporter.error(desc, e);
		}
		context_stack.pop_back();

		reporter.ctx_end();
	}

	inline void describe(std::string desc, detail::voidf_t f) {
		describe(desc, f, detail::reg_reporter(), detail::context_stack());
	}

	inline void setup(detail::voidf_t f, detail::contextstack_t& context_stack) {
		context_stack.back().register_setup(f);
	}
	inline void setup(detail::voidf_t f) {
		setup(f, detail::context_stack());
	}

	inline void teardown(detail::voidf_t f, detail::contextstack_t& context_stack) {
		context_stack.back().register_teardown(f);
	}
	inline void teardown(detail::voidf_t f) {
		teardown(f, detail::context_stack());
	}

	inline void it(std::string desc, detail::voidf_t f, detail::reporter& reporter, detail::contextstack_t& context_stack) {
		bool success = false;
			
		reporter.it_start(desc);
			
		context_stack.back().execute();
		auto run_setups = [&]() {
			for (auto e : context_stack) {
				e.run_setup();
			}
		};
		auto run_teardowns = [&]() {
			for (auto e : context_stack) {
				e.run_teardown();
			}
		};

		try { //handle assertions here...
			run_setups();
			f();
			success = true;
		} catch (const detail::assertion_exception& ex) {
			reporter.it_failure(desc, ex);
		}
		catch (const std::exception& ex) {
			std::string err = std::string("exception: ") + ex.what();
			reporter.it_failure(desc, detail::assertion_exception(err));
		} catch (...) {
			reporter.it_unknown_error(desc);
		}

		try {
			run_teardowns();
			if (success) {
				reporter.it_success(desc);
			}
		} catch (const detail::assertion_exception& ex) {
			reporter.it_failure(desc, ex);
		} catch (const std::exception& ex) {
			std::string err = std::string("exception: ") + ex.what();
			reporter.it_failure(desc, detail::assertion_exception(err));
		} catch (...) {
			reporter.it_unknown_error(desc);
		}
	}

	inline void it(std::string desc, detail::voidf_t func) {
		it(desc, func, detail::reg_reporter(), detail::context_stack());
	}
}
FURRY_NS_END

/*
furry_add_test([]() {
	describe("a calculator", []() {
		calc* calculator;
		setup([&]() {
			calculator = new calc;
		});
		teardown([&]() {
			delete calc;
		});

		it("can add", [&]() {
			Assert::That(...);
		});

		describe("in scientific mode", [&]() {
			it("can calculate angles", [&]() {
				Assert::That(calculator->sin(3.1415), AlmostEquals(0, [0.01]));
			});
		});
	});
});


*/

#endif
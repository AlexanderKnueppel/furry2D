#ifndef __FURRY_TEST_CONTEXT_H__
#define __FURRY_TEST_CONTEXT_H__

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

#include <stack>

FURRY_NS_BEGIN

namespace test {
	namespace detail {
		class context {
		public:
			context(std::string desc) : mDesc{ std::move(desc) }, mExecuting{ false } {}
			const std::string& description() {
				return mDesc;
			}
			void execute() {
				mExecuting = true;
			}
			void register_setup(voidf_t func) {
				if (mExecuting)
					throw test_exception("Please make sure , that 'setup' is called before any nested 'describe' or 'it'.");
				mSetup.push_back(func);
			}
			void register_teardown(voidf_t func) {
				if (mExecuting)
					throw test_exception("Please make sure , that 'teardown' is called before any nested 'describe' or 'it'.");
				mTeardown.push_back(func);
			}
			void run_setup() {
				run(mSetup);
			}
			void run_teardown() {
				run(mTeardown);
			}
		private:
			void run(const std::list<voidf_t>& funcs) {
				auto call_func = [](voidf_t f){ f(); };
				for (auto e : funcs) call_func(e);
			}

			std::string mDesc;
			bool mExecuting;
			std::list<voidf_t> mSetup;
			std::list<voidf_t> mTeardown;
		};
		typedef std::deque<context> contextstack_t;

		inline contextstack_t& context_stack() {
			static contextstack_t contexts;
			return contexts;
		}
	}
}

FURRY_NS_END

#endif
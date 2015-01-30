#ifndef __FURRY_TEST_REPORTER_H__
#define __FURRY_TEST_REPORTER_H__

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

#include <iostream>

FURRY_NS_BEGIN

namespace test {
	namespace detail {
		struct reporter {
			virtual ~reporter() {}
			virtual void start() = 0;
			virtual void complete() = 0;

			virtual void ctx_start(std::string desc) = 0;
			virtual void ctx_end() = 0;
			virtual void error(std::string desc, const test_exception& error) = 0;

			virtual void it_start(std::string desc) = 0;
			virtual void it_success(std::string desc) = 0;
			virtual void it_failure(std::string desc, const assertion_exception& ex) = 0;
			virtual void it_unknown_error(std::string desc) = 0;

			virtual bool pass() const = 0;
		};

		struct defaultReporter : public reporter{

			defaultReporter() : mOutput(std::cout), mIndentation(0) {}

			virtual void start() {
				mNumberTests = 0;
				mNumberSucceeded = 0;
				mNumberFailed = 0;
				mIndentation = 0;
				mFailures.clear();
				mContexts.clear();
			}
			virtual void complete() { 
				mOutput << std::endl;
				summary();
				mOutput.flush();
			}

			virtual void ctx_start(std::string desc) {
				mContexts.push_back(std::string(desc));
				mOutput << indent();
				mOutput << "describe " << desc << std::endl;
				increase_indent();
				mOutput.flush();
			}
			virtual void ctx_end() {
				mContexts.pop_back();
				decrease_indent();
			}

			virtual void error(std::string desc, const  test_exception& error) {
				std::stringstream ss;
				ss << std::endl;
				ss << "Failed to run \"" << current_context() << "\": error \"" << error.what() << "\"" << std::endl;

				mMsg.push_back(ss.str());
			}

			virtual void it_start(std::string desc) {
				mNumberTests++;  
				mOutput << indent() << "- it " << desc << " ... ";
				mOutput.flush();
			}
			virtual void it_success(std::string desc) { 
				mNumberSucceeded++; 
				mOutput << "[OK]";
				mOutput << std::endl;
				mOutput.flush();
			}
			virtual void it_failure(std::string desc, const assertion_exception& ex) {
				mNumberFailed++; 
				std::stringstream ss;
				ss << std::endl;
				ss << current_context() << " " << desc << ":" << std::endl;
				if (ex.file_name().size()) {
					ss << ex.file_name();
					if (ex.line_number()) {
						ss << ":" << ex.line_number();
					}
					ss << ": ";
				}
				ss << ex.what();

				mFailures.push_back(ss.str());

				mOutput << "[FAILED]";
				mOutput << std::endl;
				mOutput.flush();
			}
			virtual void it_unknown_error(std::string desc) {
				mNumberFailed++;

				std::stringstream ss;
				ss << std::endl;
				ss << current_context() << " " << desc << ":" << std::endl;
				ss << "Unknown exception";
				ss << std::endl;

				mFailures.push_back(ss.str());

				mOutput << "[UKN. ERROR]";
				mOutput << std::endl;
				mOutput.flush();
			}

			virtual bool pass() const {
				return mNumberTests > 0 && mNumberFailed == 0 && mMsg.size() == 0;
			}

		protected:
			std::string current_context() {
				std::string name;

				for (std::string str : mContexts) {
					if (name.size() > 0) {
						name += " ";
					}

					name += str;
				}

				return name;
			}
			void increase_indent() {
				mIndentation++;
			}

			void decrease_indent() {
				mIndentation--;
			}

			void summary() {
				if (mNumberTests == 0 && mMsg.size() == 0) {
					mOutput << "No tests found!" << std::endl;
				}
				if (mNumberFailed == 0 && mMsg.size() == 0) {
					mOutput << "Succeeded!" << std::endl;
				}
				if (mMsg.size() > 0) {
					for (std::string str : mMsg)
						mOutput << str << std::endl;
				}
				if (mNumberFailed > 0) {
					mOutput << "Failures detected!" << std::endl;
					for (std::string str : mFailures)
						mOutput << str << std::endl;
				}
				mOutput << "Test run complete. " << mNumberTests << " tests run. " << mNumberSucceeded << " succeeded.";
				if (mNumberFailed > 0) {
					mOutput << " " << mNumberFailed << " failed.";
				}
				if (mMsg.size() > 0) {
					mOutput << " " << mMsg.size() << " test run errors.";
				}

				mOutput << std::endl;
			}

			std::string indent() {
				return std::string(mIndentation, '\t');
			}
			std::list<std::string> mContexts;
			std::list<std::string> mMsg;
			std::list<std::string> mFailures;
			int mNumberTests;
			int mNumberSucceeded;
			int mNumberFailed;

			std::ostream& mOutput;
			int mIndentation;
		};


		inline reporter& reg_reporter() {
			static std::unique_ptr<defaultReporter> reporter = nullptr;
			if (reporter == nullptr) {
				reporter = std::unique_ptr<defaultReporter>(new defaultReporter());
			}
			return *reporter;
		}
	}
}

FURRY_NS_END

#endif
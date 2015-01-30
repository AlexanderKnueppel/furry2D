#ifndef __FURRY_TEST_EXPRESSION_H__
#define __FURRY_TEST_EXPRESSION_H__

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

/**
* TODO:
* - expressions: GreaterThen, LessThan, CloseTo, StartsWith, EndsWith, isNull, throwsException, isEmpty, Contains...
*
*/

FURRY_NS_BEGIN

namespace test {
	namespace detail {

		//expression is like a node
		template <typename U>
		struct expression;

		struct generateString {
			virtual std::string toString() const {
				return "";
			}
		};

		template <typename left, typename right>
		struct orExpression : expression<orExpression<left, right> > {
			left mLeft;
			right mRight;
			orExpression(const left& l, const right& r) : mLeft(l), mRight(r) {}

			template<typename T>
			bool operator()(const T& value) const {
				return (mLeft(value)) || (mRight(value));
			}

			std::string toString()  const {
				std::stringstream sstr;
				sstr << "(" << mLeft.toString() << " OR " << mRight.toString() << ")";
				return sstr.str();
			}
		};

		template <typename left, typename right>
		struct andExpression : expression<andExpression<left, right> > {
			left mLeft;
			right mRight;
			andExpression(const left& l, const right& r) : mLeft(l), mRight(r) {}

			template<typename T>
			bool operator()(const T& value) const {
				return (mLeft(value)) && (mRight(value));
			}

			std::string toString() const {
				std::stringstream sstr;
				sstr << "(" << mLeft.toString() << " AND " << mRight.toString() << ")";
				return sstr.str();
			}
		};

		template <typename T>
		struct notExpression : expression<notExpression<T> > {
			T mExpression;
			notExpression(const T& e) : mExpression(e) {}

			template<typename T>
			bool operator()(const T& value) const {
				return !mExpression(value);
			}

			std::string toString() const {
				std::stringstream sstr;
				sstr << "NOT (" << mExpression.toString() << ")";
				return sstr.str();
			}
		};

		template <typename U>
		struct expression : public generateString {
			//U mExpression;

			notExpression<U> operator!() const {
				return notExpression<U>(static_cast<const U&>(*this));
			}
			template< typename right >
			andExpression<U, right> operator&&(const right& r) const {
				return andExpression<U, right>(static_cast<const U&>(*this), r);
			}

			template< typename right >
			orExpression<U, right> operator||(const right& r) const {
				return orExpression<U, right>(static_cast<const U&>(*this), r);
			}

			// need more overlaoded operators to encapsulate objects (==, !=, >, <, >=, <=...)
		};


		template< typename ExpectedType >
		struct equalsExpression : expression< equalsExpression<ExpectedType> > {
			ExpectedType m_expected;
			equalsExpression(const ExpectedType& expected) : m_expected(expected)
			{}

			template<typename ActualType>
			bool operator()(const ActualType& actual) const {
				return (m_expected == actual);
			}

			std::string toString() const {
				std::stringstream sstr;
				sstr << " == " << m_expected;
				return sstr.str();
			}

		};

		template< typename ExpectedType>
		struct greaterThanExpression : expression< greaterThanExpression<ExpectedType> > {
			ExpectedType m_expected;
			bool m_equal;
			greaterThanExpression(const ExpectedType& expected, bool eq = false) : m_expected(expected), m_equal(eq)
			{}

			template<typename ActualType>
			bool operator()(const ActualType& actual) const {
				return (m_equal) ? (actual >= m_expected) : (actual > m_expected);
			}

			std::string toString() const {
				std::stringstream sstr;
				if (m_equal)
					sstr << " >= " << m_expected;
				else
					sstr << " > " << m_expected;
				return sstr.str();
			}

		};

		template< typename ExpectedType>
		struct lessThanExpression : expression< lessThanExpression<ExpectedType> > {
			ExpectedType m_expected;
			bool m_equal;
			lessThanExpression(const ExpectedType& expected, bool eq = false) : m_expected(expected), m_equal(eq)
			{}

			template<typename ActualType>
			bool operator()(const ActualType& actual) const {
				return (m_equal) ? (actual <= m_expected) : (actual < m_expected);
			}

			std::string toString() const {
				std::stringstream sstr;
				if (m_equal)
					sstr << " <= " << m_expected;
				else
					sstr << " < " << m_expected;
				return sstr.str();
			}

		};

		//template <typename T>
		//struct expression<equals<T> > {
		//	bool evaulate(T val) {
		//		static_cast<equals<T>*>(this)->operator(val);
		//	}
		//};

		//operators
		template< typename left, typename right >
		inline orExpression<left, right> _or(const left& l, const right& r) {
			return orExpression<left, right>(l, r);
		}

		template< typename left, typename right >
		inline andExpression<left, right> _and(const left& l, const right& r) {
			return andExpression<left, right>(l, r);
		}

		template< typename expression>
		inline notExpression<expression> _not(const expression& e) {
			return notExpression<expression>(e);
		}

		//constraints
		template< typename ExpectedType >
		inline equalsExpression<ExpectedType> _equals(const ExpectedType& expected) {
			return equalsExpression<ExpectedType>(expected);
		}

		template< typename ExpectedType>
		inline greaterThanExpression<ExpectedType> _greaterThan(const ExpectedType& expected) {
			return greaterThanExpression<ExpectedType>(expected);
		}

		template< typename ExpectedType>
		inline greaterThanExpression<ExpectedType> _greaterOrEqualThan(const ExpectedType& expected) {
			return greaterThanExpression<ExpectedType>(expected, true);
		}

		template< typename ExpectedType>
		inline lessThanExpression<ExpectedType> _lessThan(const ExpectedType& expected) {
			return lessThanExpression<ExpectedType>(expected);
		}

		template< typename ExpectedType>
		inline lessThanExpression<ExpectedType> _lessOrEqualThan(const ExpectedType& expected) {
			return lessThanExpression<ExpectedType>(expected, true);
		}

		//evaluation
		template< typename T1, typename T2 >
		inline bool _evaluate(const T1& t, const expression<T2>& e) {
			return (static_cast<const T2&>(e))(t);
		}

	}

}

FURRY_NS_END

#define equals(e)\
	furry2d::test::detail::_equals((e)) 

#define gt(e)\
	furry2d::test::detail::_greaterThan((e)) 
#define geq(e)\
	furry2d::test::detail::_greaterOrEqualThan((e)) 

#define lt(e)\
	furry2d::test::detail::_lessThan((e))
#define leq(e)\
	furry2d::test::detail::_lessOrEqualThan((e))

#define or(e1, e2)\
	furry2d::test::detail::_or((e1), (e2)) 

#define and(e1, e2)\
	furry2d::test::detail::_and((e1), (e2)) 

#define not(e)\
	furry2d::test::detail::_not((e)) 

//#define evaulate(e1, e2)\
//	furry::test::detail::_evaluate((e1), (e2)) \

#endif
#ifndef __FURRY_UTIL_VARIANT_H__
#define __FURRY_UTIL_VARIANT_H__  

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

#include <iostream>
#include <typeinfo>
#include <type_traits>

#include <furry2d/preprocessor.h>

FURRY_NS_BEGIN

/**
* \brief Implements a class for variants (similiar to boost.variant)
*
* \ingroup util
*/

namespace internal {
	//
	// Helper to determine position of a type in a list of types
	//
	template <typename X, typename... Types>
	struct position_impl;

	template <typename X>
	struct position_impl<X> { static size_t const value = 1; };

	template <typename X, typename... Types>
	struct position_impl<X, X, Types...> { static size_t const value = 0; };

	template <typename X, typename T, typename... Types>
	struct position_impl<X, T, Types...> { static size_t const value = 1 + position_impl<X, Types...>::value; };

	template <typename X, typename... Types>
	struct position {
		static size_t const value = position_impl<X, Types...>::value;
		static_assert(value < sizeof...(Types), "Unknown Type");
	}; // struct position

	//
	// Apply
	//
	template <typename T, typename F, typename S>
	void apply_unary_impl(F& functor, S& storage) {
		T& t = reinterpret_cast<T&>(storage);
		functor(t);
	} // apply_unary_impl

	template <typename... Types, typename F, typename S>
	void apply_unary(F& functor, S& storage, size_t const tag) {
		using Applier = void(*)(F&, S&);
		static Applier const appliers[] = { &apply_unary_impl<Types, F, S>... };

		Applier const a = appliers[tag];
		a(functor, storage);
	} // apply_unary

	template <typename T, typename F, typename S>
	void apply_binary_impl(F& functor, S& left, S& right) {
		T& typed_left = reinterpret_cast<T&>(left);
		T& typed_right = reinterpret_cast<T&>(right);
		functor(typed_left, typed_right);
	} // apply_binary_impl

	template <typename... Types, typename F, typename S>
	void apply_binary(F& functor, S& left, S& right, size_t const tag) {
		using Applier = void(*)(F&, S&);
		static Applier const appliers[] = { &apply_binary_impl<Types, F, S>... };

		Applier const a = appliers[tag];
		a(functor, left, right);
	} // apply_binary

	//
	// Some Visitors
	//
	template <typename T>
	struct MoveInitializer {
		MoveInitializer(T&& t) : item(std::move(t)) {}
		T item;

		void operator()(T& t) { new (&t) T(std::move(item)); }

		template <typename U>
		void operator()(U&) {}
	}; // struct MoveInitializer

	template <typename T>
	struct CopyInitializer {
		CopyInitializer(T const& t) : ref(t) {}
		T const& ref;

		void operator()(T& t) { new (&t) T(ref); }

		template <typename U>
		void operator()(U&) {}
	}; // struct CopyInitializer

	struct MoveConstructer {
		template <typename T>
		void operator()(T& left, T& right) { new (&left) T(std::move(right)); }
	}; // struct MoveConstructer

	struct CopyConstructer {
		template <typename T>
		void operator()(T& left, T& right) { new (&left) T(right); }
	}; // struct CopyConstructer

	struct Deleter {
		template <typename T>
		void operator()(T& t) { t.~T(); }
	}; // struct Deleter

} // namespace internal

template <typename H, typename... Tail>
class variant {
public:
	variant() : tag(0) { new (&storage) H{}; }

	template <typename T>
	variant(T&& t) : tag(internal::position<T, H, Tail...>::value) {
		internal::MoveInitializer<T> m{ std::move(t) }; internal::apply_unary<H, Tail...>(m, storage, tag);
	}

	template <typename T>
	variant(T const& t) : tag(internal::position<T, H, Tail...>::value) {
		internal::CopyInitializer<T> c{ t }; internal::apply_unary<H, Tail...>(c, storage, tag);
	}
	template <typename T>
	variant& operator=(T const& t) {
		tag = internal::position<T, H, Tail...>::value;
		internal::CopyInitializer<T> c{ t }; internal::apply_unary<H, Tail...>(c, storage, tag);
		return *this;
	} // operator=

	variant(variant&& other) : tag(other.tag) { internal::MoveConstructer m; internal::apply_binary<H, Tail...>(m, storage, other.storage, tag); }

	variant(variant const& other) : tag(other.tag) { internal::CopyConstructer c; internal::apply_binary<H, Tail...>(c, storage, other.storage, tag); }

	variant& operator=(variant other) {
		using std::swap;
		swap(*this, other);
		return *this;
	} // operator=

	~variant() { internal::Deleter d; internal::apply_unary<H, Tail...>(d, storage, tag); }

	size_t which() const { return tag; }

	template <typename T>
	T* get() {
		static size_t const pos = internal::position<T, H, Tail...>::value;
		return pos == tag ? reinterpret_cast<T*>(&storage) : nullptr;
	}

	template <typename T>
	T const* get() const {
		static size_t const pos = internal::position<T, H, Tail...>::value;
		return pos == tag ? reinterpret_cast<T const*>(&storage) : nullptr;
	}

private:
	using Storage = typename std::aligned_union<0, H, Tail...>::type;

	size_t tag;
	Storage storage;
}; // class variant

FURRY_NS_END

#endif
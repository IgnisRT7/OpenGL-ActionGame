/**
*	@file EnumOperator.h
*/
#pragma once

#include <type_traits>

namespace enum_concept {

	template<typename T>
	struct has_bitwise_operators : std::false_type {};

	template<typename T>
	struct has_and_or_operators : has_bitwise_operators<T> {};
}

namespace type_traits {

	template<bool con> using concept_t = typename std::enable_if<con, std::nullptr_t>::type;
	template<typename T> using underlying_type_t = typename std::underlying_type<T>::type;
}

namespace detail {

	using namespace type_traits;
	template<typename T, concept_t<std::is_enum<T>::value> = nullptr>
	constexpr underlying_type_t<T> underlying_cast(T e) { return static_cast<underlying_type_t<T>>(e); }
}

// & operator
template<typename T, type_traits::concept_t<enum_concept::has_and_or_operators<T>::value> = nullptr>
constexpr T operator&(T l, T r) {
	return static_cast<T>(detail::underlying_cast(l) & detail::underlying_cast(r));
}

// &= operator
template<typename T, type_traits::concept_t<enum_concept::has_and_or_operators<T>::value> = nullptr>
T& operator&=(T& l, T r) {
	l = static_cast<T>(detail::underlying_cast(l) | detail::underlying_cast(r));
	return l;
}

// | operator
template<typename T, type_traits::concept_t<enum_concept::has_and_or_operators<T>::value> = nullptr>
constexpr T operator|(T l, T r) { return static_cast<T>(detail::underlying_cast(l) | detail::underlying_cast(r)); }

// |= operator
template<typename T, type_traits::concept_t<enum_concept::has_and_or_operators<T>::value> = nullptr>
T& operator|=(T& l, T r) {
	l = static_cast<T>(detail::underlying_cast(l) | detail::underlying_cast(r));
	return r;
}

// ^ operator
template<typename T, type_traits::concept_t<enum_concept::has_and_or_operators<T>::value> = nullptr>
constexpr T operator^(T l, T r) { return static_cast<T>(detail::underlying_cast(l) ^ detail::underlying_cast(r)); }

// ^= operator
template<typename T, type_traits::concept_t<enum_concept::has_and_or_operators<T>::value> = nullptr>
constexpr T operator^=(T& l, T r) {
	l = static_cast<T>(detail::underlying_cast(l) ^ detail::underlying_cast(r));
	return l;
}

// ~ operator
template<typename T, type_traits::concept_t<enum_concept::has_and_or_operators<T>::value> = nullptr>
constexpr T operator~(T op) { return static_cast<T>(~detail::underlying_cast(op)); }
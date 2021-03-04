#pragma once
#include <ostream>

#define CREATE_NEW_INDEX_TYPE(type, name) using name = prim::STType<type, struct name##TagType>;

namespace prim {


	template <typename Type, typename Tag>
	class StrictType
	{
	private:
		typedef StrictType<Type, Tag> ST;
	public:
		constexpr StrictType() : m_value(Type()) {}
		constexpr explicit StrictType(const Type value) : m_value(value) {}

		constexpr Type value() const { return m_value; }

		constexpr bool operator==(const ST &rhs) const { return m_value == rhs.m_value; }
		constexpr bool operator!=(const ST &rhs) const { return m_value != rhs.m_value; }
		constexpr bool operator<(const ST &rhs) const { return m_value < rhs.m_value; }
		constexpr bool operator>(const ST &rhs) const { return m_value > rhs.m_value; }
		constexpr bool operator<=(const ST &rhs) const { return m_value <= rhs.m_value; }
		constexpr bool operator>=(const ST &rhs) const { return m_value >= rhs.m_value; }
		constexpr ST operator+(const ST &rhs) const { return m_value + rhs.m_value; }
		constexpr ST operator-(const ST &rhs) const { return m_value - rhs.m_value; }
		constexpr ST &operator+=(const ST &rhs) const { m_value += rhs.m_value; return *this; }
		constexpr ST &operator-=(const ST &rhs) const { m_value -= rhs.m_value; return *this; }

		inline ST &operator+=(const ST &rhs) { m_value = m_value + rhs; return *this; }
		inline ST &operator-=(const ST &rhs) { m_value = m_value - rhs; return *this; }
		inline Type &operator++() { m_value++;  return *this; }
		inline Type &operator--() { m_value--;  return *this; }
		inline Type operator++(int) { const Type ret(m_value); m_value++; return ret; }
		inline Type operator--(int) { const Type ret(m_value); m_value--; return ret; }

	private:
		Type m_value;
	};

	template <class Type, class Tag>
	std::ostream &operator<<(std::ostream &os, StrictType<Type, Tag> index) {
		os << index.value();
		return os;
	}
}

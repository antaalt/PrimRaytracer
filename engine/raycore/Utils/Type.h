#pragma once

#define CREATE_NEW_INDEX_TYPE(type, name) using name = Utils::IndexType<type, struct name##TagType>;

namespace raycore {


	template <typename Type, typename Tag>
	class IndexType
	{
	private:
		typedef IndexType<Type, Tag> Index;
	public:
		constexpr IndexType() : m_value(Type()) {}
		constexpr explicit IndexType(const Type value) : m_value(value) {}

		constexpr Type value() const { return m_value; }

		constexpr operator Type() const { return m_value; }

		constexpr bool operator==(const Index &rhs) const { return m_value == rhs.m_value; }
		constexpr bool operator!=(const Index &rhs) const { return m_value != rhs.m_value; }
		constexpr bool operator<(const Index &rhs) const { return m_value < rhs.m_value; }
		constexpr bool operator>(const Index &rhs) const { return m_value > rhs.m_value; }
		constexpr bool operator<=(const Index &rhs) const { return m_value <= rhs.m_value; }
		constexpr bool operator>=(const Index &rhs) const { return m_value >= rhs.m_value; }
		constexpr Index operator+(const Index &rhs) const { return m_value + rhs.m_value; }
		constexpr Index operator-(const Index &rhs) const { return m_value - rhs.m_value; }

		inline Index &operator+=(const Index &rhs) { m_value = m_value + rhs; return *this; }
		inline Index &operator-=(const Index &rhs) { m_value = m_value - rhs; return *this; }
		inline Type &operator++() { m_value++;  return *this; }
		inline Type &operator--() { m_value--;  return *this; }
		inline Type operator++(int) { const Type ret(m_value); m_value++; return ret; }
		inline Type operator--(int) { const Type ret(m_value); m_value--; return ret; }


	private:
		Type m_value;
	};

	template <class Type, class Tag>
	std::ostream &operator<<(std::ostream &os, IndexType<Type, Tag> index) {
		return os << index.value();
	}
}

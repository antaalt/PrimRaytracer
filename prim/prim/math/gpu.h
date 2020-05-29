#pragma once

namespace geometry {

template <typename T>
struct gpu {
	gpu(const T &type) {}
	T &operator()() { return m_value; }
	const T &operator()() const { return m_value; }
	operator T() const { return m_value; }
private:
	alignas(16) T m_value;
};

}
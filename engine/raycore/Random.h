#pragma once

#include <random>

namespace raycore {

struct Rand {
	template <typename T>
	static T sample();

	template <typename T>
	static T sample(T min, T max);
};


template <>
inline float Rand::sample()
{
	static thread_local std::mt19937 generator;
	std::uniform_real_distribution<float> distribution(0.f, 1.f);
	return distribution(generator);
}

template <>
inline float Rand::sample(float min, float max)
{
	static thread_local std::mt19937 generator;
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(generator);
}

template <>
inline uint32_t Rand::sample(uint32_t min, uint32_t max)
{
	static thread_local std::mt19937 generator;
	std::uniform_int_distribution<uint32_t> distribution(min, max);
	return distribution(generator);
}


}
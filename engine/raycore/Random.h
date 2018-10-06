#pragma once

#include <random>

namespace raycore {

	namespace rand {
		float rnd(float min = 0.f, float max = 1.f);
		unsigned int uirnd(unsigned int min, unsigned int max);
	}
}
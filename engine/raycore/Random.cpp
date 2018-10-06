#include "Random.h"
#include "RandomLib.h"

namespace raycore {
	namespace rand {

		float rnd(float min, float max)
		{
			static thread_local std::mt19937 generator;
			std::uniform_real_distribution<float> distribution(min, max);
			return distribution(generator);
		}

		unsigned int uirnd(unsigned int min, unsigned int max)
		{
			static thread_local std::mt19937 generator;
			std::uniform_int_distribution<unsigned int> distribution(min, max);
			return distribution(generator);
		}
	}
}
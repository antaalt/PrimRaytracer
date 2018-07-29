#include "Sampler.h"


namespace app {
	namespace rand {

		Sampler::Sampler()
		{
		}


		Sampler::~Sampler()
		{
		}


		float Sampler::r1() const
		{
			return 1.f;// Random::get(0.f, 1.f);
		}
		float Sampler::r2() const
		{
			return 1.f; //Random::get(0.f, 1.f);
		}

	}
}
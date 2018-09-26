#pragma once

#include "Random.h"


namespace raycore {
	namespace rand {
		using Random = ::effolkronium::random_static;

		class Sampler
		{
		public:
			Sampler();
			~Sampler();

			float r1() const;
			float r2() const;
		};

	}
}
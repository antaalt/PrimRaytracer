#pragma once
#include "Types.h"
#include "Ray.h"
#include "Hitable.h"

namespace raycore {

	enum BxDFType {
		BSDF_REFLECTION,
		BSDF_DIFFUSE,
		BSDF_SPECULAR,
		BSDF_TRANSMISSION,
		BSDF_GLOSSY,
		NB_BSDF
	};

	class BSDF
	{
	public:
		BSDF();
		~BSDF();
		virtual vec3 sample(const tracer::Ray &ray, const prim::HitInfo &info, float &pdf) const = 0;
		virtual colorHDR evaluate() = 0;
	};

	class MicroFacetReflection : BSDF
	{
	public:

	};

}
#include "Sampling.h"
#include "transform.h"


namespace raycore {
namespace sample {
vec3f unitSphere(float r1, float r2)
{
	return transform::SphericalCoordinates(
		geometry::radianf(2.f * geometry::pi<float>() * r1),
		geometry::arccos(1.f - 2.f * r2),
		1.f
	);
}
vec3f unitHemisphere(float r1, float r2)
{
	return vec3f(
		transform::PolarCoordinates(
			geometry::radianf(2.f * geometry::pi<float>() * r1),
			r2
		), 
		geometry::sqrt(1.f - r2 * r2)
	);
}
vec3f unitMicrofacet(float roughness, float r1, float r2)
{
	return transform::SphericalCoordinates(
		geometry::arctan(roughness * geometry::sqrt(r1) / geometry::sqrt(1.f - r1)),
		geometry::radianf(2.f * geometry::pi<float>() * r2),
		1.f
	);
}
vec2f unitDisk(float r1, float r2)
{
	return transform::PolarCoordinates(
		geometry::radianf(2.f * geometry::pi<float>() * r2),
		geometry::sqrt(r1)
	);
}
}
}
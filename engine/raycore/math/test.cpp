#include "geometry.h"

void test() {
	using namespace geometry;

	uv2f uv;
	vec2f vec2(0.f, 1.f);

	vec3f vec3(1.f, 2.f, 3.f);
	point3f point;
	norm3f norm;

	vec4f vec4(2.f, 3.f, 4.f, 5.f);
	quatf quat;

	mat3f mat3;
	mat4f mat4;

	vec2 = vec2f(vec3);
	vec2 = vec2f(vec4);

	vec3 = vec3f(vec2, 1.f);
	vec3 = vec3f(vec4);

	vec4 = vec4f(vec2, 1.f, 2.f);
	vec4 = vec4f(vec3, 1.f);

	//

	vec2 = vec2f(uv);
	uv = uv2f(vec2);

	vec3 = vec3f(point);
	vec3 = vec3f(norm);
	vec3 = vec3f(vec4);
	vec3 = vec3f(mat3[0]);
	point = point3f(vec3);
	//point = point3f(mat3[0]);
	norm = norm3f(vec3);
	//norm = norm3f(mat3[0]);

	vec4 = vec4f(mat4[0]);

	//mat3[0] = col3f(vec3);

	mat4[0] = col4f(vec4);

	gpu<vec3f> gpu(vec3);
	gpu = vec3;
	vec3 = gpu;

}
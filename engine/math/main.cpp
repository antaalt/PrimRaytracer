
#define USE_VECTOR
#define USE_MATRIX
#define USE_QUATERNION
#include "math\types.h"
#include <iostream>

int main(int argc, char *argv[])
{
	Vector3f vec3(0.f, 1.f, 0.f);
	Vector3f vec32(0.f, 1.f, 1.f);
	float dot = Type::dot(vec3, vec32);
	std::cout << "dot : " << dot << std::endl;
	//Vector2f vec2 = Type::toVec2(vec3);
	return 0;
}
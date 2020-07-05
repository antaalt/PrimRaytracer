#pragma once

#include <stdint.h>

namespace geometry {

struct half {
	half() : data(0) {}
	half(float value) : data(value) {}

	operator float() { return data; }
private:
	uint16_t data;
};

}
#pragma once
#include <vector>

#include "Types.h"

namespace app {

	struct Pixel {
		union {
			float data[4];
			struct {
				float r, g, b, a;
			};
		};
		void clamp();
		void applyGamma();

		operator colorHDR();

		Pixel operator+(const Pixel &rhs);
		friend Pixel operator*(float lhs, const Pixel &rhs);
		friend Pixel operator*(const Pixel &lhs, float rhs);

		Pixel();
		Pixel(float val);
		Pixel(const colorHDR &color);
		Pixel(float r, float g, float b, float a);
	};

	class PixelBuffer {
	public:
		PixelBuffer(unsigned int w, unsigned int h);
		void resize(unsigned int w, unsigned int h);

		Pixel &operator()(unsigned int x, unsigned int y);
		const Pixel &operator()(unsigned int x, unsigned int y) const;

		Pixel *data();
		const Pixel *data() const;
		unsigned int width() const;
		unsigned int height() const;
	private:

		std::vector<Pixel> m_data;
		unsigned int m_width, m_height;
	};
}
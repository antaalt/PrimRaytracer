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

		operator Color32();
		operator ColorHDR();
		/*void operator=(const Color32 &color);
		void operator=(const ColorHDR &color);*/

		Pixel operator+(const Pixel &rhs);

		Pixel();
		Pixel(float val);
		Pixel(const Color32 &color);
		Pixel(const ColorHDR &color);
		Pixel(float r, float g, float b, float a);
	};

	class PixelBuffer {
	public:
		PixelBuffer();
		PixelBuffer(unsigned int w, unsigned int h);
		~PixelBuffer();
		void resize(unsigned int w, unsigned int h);

		Pixel &operator()(unsigned int x, unsigned int y);
		const Pixel &operator()(unsigned int x, unsigned int y) const;

		Pixel *data();
		unsigned int width() const;
		unsigned int height() const;
	private:

		std::vector<Pixel> m_data;
		unsigned int m_width, m_height;
	};
}
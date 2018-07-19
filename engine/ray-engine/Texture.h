#pragma once
#include <string>
#include <vector>

#include "Types.h"

namespace Application::RayTracer {


	struct Pixel {
		union {
			float data[4];
			struct {
				float r, g, b, a;
			};
		};
		void clamp();
		void applyGamma();

		Color32 color32();
		ColorHDR colorHDR();

		Pixel(const Color32 &color);
		Pixel(const ColorHDR &color);

		Pixel();
	};

	// 4 components texture as float (
	class Texture
	{
	public:
		Texture();
		~Texture();

		void create(unsigned int w, unsigned int h);

		bool saveToFile(std::string path) const;
		bool loadFromFile(std::string path);

		void set(unsigned int w, unsigned int h, Pixel pixel);
		Pixel get(unsigned int w, unsigned int h) const;

		const float *data() const;
		unsigned int width() const;
		unsigned int height() const;

	private:
		std::vector<Pixel> m_data;
		unsigned int m_width, m_height;
	};

}
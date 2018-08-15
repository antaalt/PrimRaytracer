#include "Texture.h"
#include "Config.h"
#include "Math.h"

namespace app {
	Texture::Texture()
	{
	}
	Texture::Texture(const std::vector<unsigned char>& data, unsigned int width, unsigned int height, unsigned int components) :
		m_width(width),
		m_height(height),
		m_component(4)
	{
		bool hasAlpha = (components == 4);
		m_data.resize(width * height * 4);
		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				const unsigned int index = y * width * components + x * components;
				m_data[index + 0] = powf(static_cast<float>(data[index + 0]) / 255.f, 2.2f);
				m_data[index + 1] = powf(static_cast<float>(data[index + 1]) / 255.f, 2.2f);
				m_data[index + 2] = powf(static_cast<float>(data[index + 2]) / 255.f, 2.2f);
				if (hasAlpha)
					m_data[index + 3] = powf(static_cast<float>(data[index + 3]) / 255.f, 2.2f);
				else
					m_data[index + 3] = 1.f;
			}
		}
	}


	Texture::Texture(const std::vector<float> &data, unsigned int width, unsigned int height, unsigned int components) :
		m_width(width),
		m_height(height),
		m_component(4)
	{
		bool hasAlpha = (components == 4);
		m_data.resize(width * height * 4);
		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				const unsigned int index = y * width * components + x * components;
				m_data[index + 0] = data[index + 0];
				m_data[index + 1] = data[index + 1];
				m_data[index + 2] = data[index + 2];
				if (hasAlpha)
					m_data[index + 3] = data[index + 3];
				else
					m_data[index + 3] = 1.f;
			}
		}
	}

	color4 Texture::texture2D(float u, float v)
	{
		float ui = u * m_width;
		float vi = v * m_height;
		unsigned int uPixel = static_cast<unsigned int>(ui) % m_width;
		unsigned int vPixel = static_cast<unsigned int>(vi) % m_height;
#if defined(BILINEAR_FILTER_TEXTURE)
		float uf = ui - floorf(ui);
		float vf = vi - floorf(vi);
		return lerp(
			lerp(
				at(uPixel, vPixel),
				at(uPixel + 1, vPixel),
				uf
			),
			lerp(
				at(uPixel, vPixel + 1),
				at(uPixel + 1, vPixel + 1),
				uf
			),
			vf
		);
#else
		unsigned int index = vPixel * m_width * m_component + uPixel * m_component;
		return color4(&m_data[index]);
#endif
	}

	unsigned int Texture::stride()
	{
		return sizeof(float) * m_component;
	}
	color4 Texture::at(unsigned int x, unsigned int y)
	{
#if defined(TEXTURE_REPEAT)
		x = x % m_width;
		y = y % m_height;
#else
		if (x >= m_width || x < 0 || y >= m_height || m_height == 0)
			return color4(0.f);
#endif
		return color4(&m_data[y * m_width * m_component + x * m_component]);
	}
}

#include "Texture.h"
#include "Config.h"


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

	ColorHDR Texture::texture2D(float u, float v)
	{
		if (u > 1.f)
			u = u - std::floorf(u);
		else if (u < 0.f)
			u = u - std::ceilf(u) + 1.f;
		if (v > 1.f)
			v = v - std::floorf(v);
		else if (v < 0.f)
			v = v - std::ceilf(v) + 1.f;
		unsigned int uPixel = (m_width - 1) * u;
		unsigned int vPixel = (m_height - 1) * v;
		unsigned int index = vPixel * m_width * m_component + uPixel * m_component;
		ASSERT(index < m_data.size(), "OUt of range");
		if(m_component == 4)
			return ColorHDR(m_data[index], m_data[index + 1], m_data[index + 2], m_data[index + 3]);
		else
			return ColorHDR(m_data[index], m_data[index + 1], m_data[index + 2], 1.f);
	}

	unsigned int Texture::stride()
	{
		return sizeof(float) * m_component;
	}
}

#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "stb_image.h"
#include "stb_image_write.h"
#include "Config.h"
#include <iostream>
#include <fstream>

namespace app {
	Image::Image()
	{
	}


	Image::Image(std::string path)
	{
		load(path);
	}

	Image::Image(const std::vector<unsigned char> &data)
	{
		loadFromRawBytes(data);
	}


	Image::~Image()
	{
	}

	Format Image::getFormatFromExtension(std::string path)
	{

		std::string extension = path.substr(path.find_last_of('.') + 1);
		if (extension == "jpg" || extension == "jpeg")
			return Format::JPEG;
		else if (extension == "png")
			return Format::PNG;
		else if (extension == "bmp")
			return Format::BMP;
		else if (extension == "gif")
			return Format::GIF;
		else if (extension == "psd")
			return Format::PSD;
		else if (extension == "tga")
			return Format::TGA;
		return Format::UNSUPPORTED_FORMAT;
	}

	Format Image::getFormatFromBytes(const std::vector<unsigned char>& bytes)
	{
		if (memcmp(bytes.data(), "BM", 2) == 0)
			return Format::BMP;
		else if (memcmp(bytes.data(), "\x89PNG", 4) == 0)
			return Format::PNG;
		else if (memcmp(bytes.data(), "\xFF\xD8\xFF", 3) == 0)
			return Format::JPEG;
		else if (memcmp(bytes.data(), "GIF8", 4) == 0)
			return Format::GIF;
		else if (memcmp(bytes.data(), "8BPS", 4) == 0)
			return Format::PSD;
		else if (memcmp(bytes.data(), "TRUE", 4) == 0)
			return Format::TGA;
		return Format::UNSUPPORTED_FORMAT;
	}

	bool Image::load(std::string path)
	{
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		if (!file.is_open())
			return false;
		std::streampos size = file.tellg();
		file.seekg(0);
		std::vector<unsigned char> data;
		data.resize(size);
		file.read(reinterpret_cast<char*>(data.data()), size);

		loadFromRawBytes(data);
		return true;
	}


	bool Image::loadFromRawBytes(const std::vector<unsigned char> &bytes)
	{
		int w, h, comp;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load_from_memory(bytes.data(), static_cast<int>(bytes.size()), &w, &h, &comp, STBI_default);
		if (!data) {
			std::cerr << "Unknown image format." << std::endl;
			return false;
		}

		if (w < 1 || h < 1) {
			free(data);
			std::cerr << "Invalid image data." << std::endl;
			return false;
		}
		m_width = w;
		m_height = h;
		m_data.resize(static_cast<size_t>(w * h * comp));
		m_component = comp;
		std::copy(data, data + w * h * comp, m_data.begin());

		free(data);
		return true;
	}

	bool Image::loadFromTexture(const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height)
	{
		if ((bytes.size() % (width * height)) != 0)
			return false;
		m_component = static_cast<unsigned int>(bytes.size()) / (width * height);
		m_data = bytes;
		m_width = width;
		m_height = height;
		return true;
	}

	static void writeToMemorySTBI(void *context, void *data, int size) {
		std::vector<unsigned char> *buffer =
			reinterpret_cast<std::vector<unsigned char> *>(context);

		unsigned char *pData = reinterpret_cast<unsigned char *>(data);

		buffer->insert(buffer->end(), pData, pData + size);
	}

	std::vector<unsigned char> Image::saveToBytes(Format format)
	{
		std::vector<unsigned char> data;
		ASSERT(m_data.size() > 0, "Size null");
		stbi_flip_vertically_on_write(true);
		switch (format)
		{
		case Format::JPEG:
			stbi_write_jpg_to_func(writeToMemorySTBI, &data, m_width, m_height, m_component, m_data.data(), 100);
			break;
		case Format::PNG:
			stbi_write_png_to_func(writeToMemorySTBI, &data, m_width, m_height, m_component, m_data.data(), 0);
			break;
		default:
			return data;
		}
		return data;
	}

	bool Image::save(std::string path, Format format)
	{
		std::vector<unsigned char> data = saveToBytes(format);
		assert(getFormatFromBytes(data) == format);

		std::ofstream file(path, std::ios::binary);
		if (!file.is_open())
			return false;
		file.write(reinterpret_cast<char*>(data.data()), data.size());

		return !file.bad();
	}
	const unsigned char * Image::data() const
	{
		return m_data.data();
	}
	size_t Image::size() const
	{
		return m_width * m_height * m_component;
	}
	unsigned int Image::width() const
	{
		return m_width;
	}
	unsigned int Image::height() const
	{
		return m_height;
	}
	unsigned int Image::componentSize() const
	{
		return m_component;
	}
	bool Image::isValid() const
	{
		return m_data.size() == m_width * m_height * m_component;
	}
}
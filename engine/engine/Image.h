#pragma once

#include <string>
#include <vector>

namespace Application {
	enum Format {
		BMP = 0,
		JPEG = 2,
		PNG = 13
	};

	struct Pixel {
		union {
			unsigned char r, g, b, a;
			unsigned char data[4];
		};
	};

	class Image
	{
	public:
		Image();
		~Image();

		bool loadFromFile(std::string path);
		bool loadFromBytes(unsigned char *data, size_t size);
		bool saveToFile(std::string path, Format format = BMP);
		Pixel get(unsigned int x, unsigned int y) const;

		const unsigned char *data() const;

		unsigned char * leak();

		unsigned int stride() const;
		unsigned int width() const;
		unsigned int height() const;
		unsigned int size() const;

	private:
		unsigned int m_width, m_height;
		unsigned char * m_data;
		Format m_format;
	};
}

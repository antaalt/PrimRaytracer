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

		bool load(std::string path);
		bool save(std::string path, Format format = BMP);

		Pixel get(unsigned int x, unsigned int y) const;

		const unsigned char *data() const;

	private:
		unsigned int m_width, m_height;
		std::vector<unsigned char> m_data;
	};
}

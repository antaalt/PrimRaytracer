#pragma once

#include <string>
#include <vector>


namespace app {

	// Format	Extensions		Magic Number
	// JPEG		.jpg / .jpeg	ff d8 ff e0
	// PNG		.png			.PNG
	// BMP		.bmp			BM
	// GIF		.gif			GIF8
	// PSD		.psd			
	// PIC
	// PNM
	// HDR
	// TGA		.tga			[variable]
	enum class Format {
		JPEG,
		PNG,
		BMP,
		GIF,
		PSD,
		PIC,
		PNM,
		HDR,
		TGA,
		UNSUPPORTED_FORMAT
	};


	class Image
	{
	public:
		Image();
		Image(std::string path);
		Image(const std::vector<unsigned char> &data);
		~Image();

		// -- Get image format
		Format getFormatFromExtension(std::string path);
		Format getFormatFromBytes(const std::vector<unsigned char> &bytes);

		// -- Load Image
		// Load an image from the given path
		bool load(std::string path);
		// Load an image from raw image bytes
		bool loadFromRawBytes(const std::vector<unsigned char> &bytes);
		// Load an image from texture
		bool loadFromTexture(const std::vector<unsigned char> &bytes, unsigned int width, unsigned int height);

		// -- Save Image
		// get raw bytes of an image converted to the image format
		std::vector<unsigned char> saveToBytes(Format format);
		// save an image at path
		bool save(std::string path, Format format);


		// -- Image data
		const unsigned char *data() const;
		size_t size() const;

		unsigned int width() const;
		unsigned int height() const;

		unsigned int componentSize() const;

		bool isValid() const;

	private:
		std::vector<unsigned char> m_data;
		unsigned int m_width, m_height;
		unsigned int m_component;
	};
}
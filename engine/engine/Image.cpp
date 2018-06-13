#include "Image.h"

#include "Utils\Log.h"
#include "FreeImage.h"

namespace Application {
	Image::Image()
	{
	}


	Image::~Image()
	{

	}

	bool Image::load(std::string path)
	{
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str());
		if (format == FIF_UNKNOWN)
		{
			format = FreeImage_GetFIFFromFilename(path.c_str());
			if (format == FIF_UNKNOWN)
			{
				Log::error("File format not supported");
				return false;
			}
		}

		FIBITMAP* bitmap = FreeImage_Load(format, path.c_str());
		FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
		FreeImage_Unload(bitmap);
		m_width = FreeImage_GetWidth(bitmap32);
		m_height = FreeImage_GetHeight(bitmap32);
		m_data = std::vector<unsigned char>(m_width * m_height * 4);
		FreeImage_ConvertToRawBits((BYTE*)m_data.data(), bitmap32, m_width * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, true);

		FreeImage_Unload(bitmap32);
		return true;
	}

	bool Image::save(std::string path, Format format)
	{
		FIBITMAP* image = FreeImage_ConvertFromRawBits(m_data.data(), m_width, m_height, m_width * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
		if (FreeImage_Save(FIF_JPEG, image, path.c_str(), 0))
		{
			Log::info("New file saved successfully");
			FreeImage_Unload(image);
			return true;
		}
		else
		{
			Log::error("Save failed");
			FreeImage_Unload(image);
			return false;
		}
	}
	Pixel Image::get(unsigned int x, unsigned int y) const
	{
		Pixel pixel;
		const int index = y * m_width * 4 + x * 4;
		for (int i = 0; i < 4; i++)
			pixel.data[i] = m_data[index + i];
		
		return pixel;
	}
	const unsigned char * Image::data() const
	{
		return m_data.data();
	}
}

#include "image.h"


namespace me::core
{
	Image::Image() : m_Size(0)
	{

	}
	Image::~Image()
	{
		m_Pixels.clear();
	}

	void Image::create(const glm::uint32& width, const glm::uint32& height, const glm::u8vec4& color)
	{
		if (width && height)
		{
			std::vector<glm::uint8> newPixels(width*height * 4);
			glm::uint8* ptr = &newPixels.at(0);
			glm::uint8* end = ptr + newPixels.size();

			while (ptr < end)
			{
				*ptr++ = color.x;
				*ptr++ = color.y;
				*ptr++ = color.z;
				*ptr++ = color.w;
			}

			m_Pixels.swap(newPixels);

			m_Size.x = width;
			m_Size.y = height;
		}
		else
		{
			std::vector<glm::uint8>().swap(m_Pixels);

			m_Size.x = 0;
			m_Size.y = 0;
		}
	}
	void Image::create(const glm::uint32& width, const glm::uint32& height, const glm::u8vec3& color)
	{
		create(width, height, glm::u8vec4(color.x, color.y, color.z, 255));
	}
	void Image::create(const glm::uint32& width, const glm::uint32& height, const glm::uint8& r, const glm::uint8& g, const glm::uint8& b, const glm::uint8& a)
	{
		create(width, height, glm::u8vec4(r, g, b, a));
	}
	void Image::create(const glm::uint32& width, const glm::uint32& height, const glm::uint8* pixels)
	{
		if (pixels && width && height)
		{
			std::vector<glm::uint8> newPixels(pixels, pixels + width*height * 4);
			
			m_Pixels.swap(newPixels);
			m_Size.x = width;
			m_Size.y = height;
		}
		else
		{
			std::vector<glm::uint8>().swap(m_Pixels);

			m_Size.x = 0;
			m_Size.y = 0;
		}
	}

	bool Image::loadFromFile(const std::string& file)
	{
		m_Pixels.clear();

		glm::int32 width = 0;
		glm::int32 height = 0;
		glm::int32 channels = 0;
		glm::uint8* ptr = SOIL_load_image(file.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);


		if (ptr)
		{
			m_File = file;

			if (width && height)
			{
				std::vector<glm::uint8> newPixels(ptr, ptr + width*height * 4);
				m_Pixels.swap(newPixels);

				m_Size.x = width;
				m_Size.y = height;
			}

			SOIL_free_image_data(ptr);

			return true;
		}
		else
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Read, std::string("Image: ") + file);
			return false;
		}
	}
	bool Image::saveToFile(const std::string& file)
	{
		if (!m_Pixels.empty() && (m_Size.x > 0) && (m_Size.y > 0))
		{
			const glm::size_t dot = file.find_last_of('.');
			const std::string extension = dot != std::string::npos ? util::to_lower(file.substr(dot + 1)) : "";

			if (extension == "bmp") return SOIL_save_image(file.c_str(), SOIL_SAVE_TYPE_BMP, m_Size.x, m_Size.y, 4, &m_Pixels[0]);
			if (extension == "tga") return SOIL_save_image(file.c_str(), SOIL_SAVE_TYPE_TGA, m_Size.x, m_Size.y, 4, &m_Pixels[0]);
			if (extension == "png") return SOIL_save_image(file.c_str(), SOIL_SAVE_TYPE_PNG, m_Size.x, m_Size.y, 4, &m_Pixels[0]);
			if (extension == "dds") return SOIL_save_image(file.c_str(), SOIL_SAVE_TYPE_DDS, m_Size.x, m_Size.y, 4, &m_Pixels[0]);
			if (extension == "jpg" || extension == "jpeg") return SOIL_save_image_quality(file.c_str(), SOIL_SAVE_TYPE_JPG, m_Size.x, m_Size.y, 4, &m_Pixels[0], 99);
		}
		else
		{
			std::string reason;
			if (m_Pixels.empty())
			{
				reason = "it was empty";
			}
			else if (m_Size.x == 0)
			{
				reason = "width == 0";
			}
			else if (m_Size.y == 0)
			{
				reason = "height == 0";
			}
			else
			{
				reason = "an unknown error occured";
			}
			throw util::RuntimeError(util::RuntimeError::ErrorType::Create, std::string("Image: ") + file, std::string("while saveToFile(), ") + reason);
			return false;
		}
	}

	const glm::uvec2 Image::getSize()
	{
		return m_Size;
	}

	void Image::setPixel(const glm::uint32& x, const glm::uint32& y, const glm::u8vec4& color)
	{
		glm::uint8* pixel = &m_Pixels.at((x + y*m_Size.x) * 4);
		*pixel++ = color.x;
		*pixel++ = color.y;
		*pixel++ = color.z;
		*pixel++ = color.w;
	}
	void Image::setPixel(const glm::uint32& x, const glm::uint32& y, const glm::u8vec3& color)
	{
		setPixel(x, y, glm::u8vec4(color.x, color.y, color.z, 255));
	}
	void Image::setPixel(const glm::uint32& x, const glm::uint32& y, const glm::uint8& r, const glm::uint8& g, const glm::uint8& b, const glm::uint8& a)
	{
		setPixel(x, y, glm::u8vec4(r, g, b, a));
	}

	const glm::u8vec4& Image::getPixel(const glm::uint32& x, const glm::uint32& y)
	{
		const glm::uint8* pixel = &m_Pixels.at((x + y*m_Size.x) * 4);
		return glm::u8vec4(pixel[0], pixel[1], pixel[2], pixel[3]);
	}

	const glm::uint8* Image::getPixelPtr()
	{
		if (!m_Pixels.empty())
		{
			return &m_Pixels.at(0);
		}
		else
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Find, (m_File.empty() ? "Image Pixels" : std::string("Image Pixels: ") + m_File), "it is empty");
			return 0;
		}
	}
	void Image::flipX()
	{
		if (!m_Pixels.empty())
		{
			glm::size_t rowSize = m_Size.x * 4;

			std::vector<glm::uint8>::iterator top = m_Pixels.begin();
			std::vector<glm::uint8>::iterator bottem = m_Pixels.end() - rowSize;

			for (glm::size_t y = 0; y < m_Size.y / 2; ++y)
			{
				std::vector<glm::uint8>::iterator left = m_Pixels.begin() + y * rowSize;
				std::vector<glm::uint8>::iterator right = m_Pixels.begin() + (y + 1) * rowSize - 4;

				for (glm::size_t x = 0; x < m_Size.x / 2; ++x)
				{
					std::swap_ranges(left, left + 4, right);

					left += 4;
					right -= 4;
				}
			}
		}
		else
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Find, (m_File.empty() ? "Image Pixels" : std::string("Image Pixels: ") + m_File), "while flipX(), it was empty");
		}
	}
	void Image::flipY()
	{
		if (!m_Pixels.empty())
		{
			glm::size_t rowSize = m_Size.x * 4;

			std::vector<glm::uint8>::iterator top = m_Pixels.begin();
			std::vector<glm::uint8>::iterator bottom = m_Pixels.end() - rowSize;

			for (std::size_t y = 0; y < m_Size.y / 2; ++y)
			{
				std::swap_ranges(top, top + rowSize, bottom);

				top += rowSize;
				bottom -= rowSize;
			}
		}
		else
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Find, (m_File.empty() ? "Image Pixels" : std::string("Image Pixels: ") + m_File), "while flipY(), it was empty");
		}
	}

}
#include "texture.h"


namespace me::core
{
	Texture::Texture()
		:
		m_Size(0, 0),
		m_Texture(0),
		m_Smooth(false),
		m_SRGB(false),
		m_Repeated(false),
		m_Flipped(false),
		m_FboAttached(false),
		m_MipMap(false)
	{

	}
	Texture::~Texture()
	{
		if (m_Texture)
		{
			glDeleteTextures(1, &m_Texture);
		}
	}

	const bool Texture::create(const glm::uvec2& size)
	{
		const glm::uint32 maxSize = getMaximumSize();
		if ((size.x == 0) || (size.y == 0) || (size.x > maxSize) || (size.y > maxSize))
		{
			std::string reason;
			if (size.x == 0)
			{
				reason = "width == 0";
			}
			else if (size.y == 0)
			{
				reason = "height == 0";
			}
			else if (size.x > maxSize)
			{
				reason = "width too big (" + std::to_string(size.x) + "). Max size is " + std::to_string(maxSize);
			}
			else if (size.y > maxSize)
			{
				reason = "height too big (" + std::to_string(size.y) + "). Max size is " + std::to_string(maxSize);
			}
			else
			{
				reason = "Something happened";
			}

			throw util::RuntimeError(util::RuntimeError::ErrorType::Create, std::string("Texture", "while create(), ") + reason);
			return false;
		}

		m_Size = size;
		m_Flipped = false;
		m_FboAttached = false;

		if (!m_Texture) glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, (m_SRGB ? GL_SRGB8_ALPHA8 : GL_RGBA), m_Size.x, m_Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Smooth ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Smooth ? GL_LINEAR : GL_NEAREST);

		m_MipMap = false;
	}
	const bool Texture::create(const glm::uint32& width, const glm::uint32& height)
	{
		return create(glm::uvec2(width, height));
	}
	const bool Texture::loadFromFile(const std::string& file)
	{
		Image image;
		return image.loadFromFile(file) && loadFromImage(image);
	}
	const bool Texture::loadFromImage(const Image& image)
	{
		if (create(const_cast<Image&>(image).getSize()))
		{
			update(image);
		}
		else
		{
			return false;
		}
	}
	const glm::uvec2& Texture::getSize()
	{
		return m_Size;
	}
	Image Texture::copyToImage()
	{
		if (m_Texture)
		{
			std::vector<glm::uint8> pixels(m_Size.x * m_Size.y * 4);
			if (!m_Flipped)
			{
				glBindTexture(GL_TEXTURE_2D, m_Texture);
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
			}
			else
			{
				std::vector<glm::uint8> allPixels(m_Size.x * m_Size.y * 4);
				glBindTexture(GL_TEXTURE_2D, m_Texture);
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &allPixels[0]);

				const glm::uint8* src = &allPixels[0];
				glm::uint8* dst = &pixels[0];
				glm::int32 srcPitch = m_Size.x * 4;
				glm::int32 dstPitch = m_Size.x * 4;

				if (m_Flipped)
				{
					src += srcPitch * (m_Size.y - 1);
					srcPitch = -srcPitch;
				}
				for (glm::uint32 i = 0; i < m_Size.y; ++i)
				{
					std::memcpy(dst, src, dstPitch);
					src += srcPitch;
					dst += dstPitch;
				}
			}

			Image image;
			image.create(m_Size.x, m_Size.y, &pixels[0]);

			return image;
		}
		else
		{
			return Image();
		}
	}
	void Texture::update(const glm::uint8* pixels, const glm::uvec2& size, const glm::uvec2& position)
	{
		if (pixels && m_Texture)
		{
			glBindTexture(GL_TEXTURE_2D, m_Texture);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Smooth ? GL_LINEAR : GL_NEAREST);
			m_MipMap = false;
			m_Flipped = false;
			glFlush();
		}
	}
	void Texture::update(const glm::uint8* pixels, const glm::uint32& width, const glm::uint32& height, const glm::uint32& x, const glm::uint32& y)
	{
		update(pixels, glm::uvec2(width, height), glm::uvec2(x, y));
	}
	void Texture::update(const glm::uint8* pixels)
	{
		update(pixels, m_Size, glm::uvec2(0, 0));
	}
	void Texture::update(const Texture& texture, const glm::uvec2& position)
	{
		if (m_Texture && texture.m_Texture)
		{
			update(const_cast<Texture&>(texture).copyToImage(), position.x, position.y);
		}
	}
	void Texture::update(const Texture& texture, const glm::uint32& x, const glm::uint32& y)
	{
		update(texture, glm::uvec2(x, y));
	}
	void Texture::update(const Texture& texture)
	{
		update(texture, glm::uvec2(0, 0));
	}
	void Texture::update(const Image& image, glm::uvec2& position)
	{
		update(const_cast<Image&>(image).getPixelPtr(), const_cast<Image&>(image).getSize(), position);
	}
	void Texture::update(const Image& image, const glm::uint32& x, const glm::uint32& y)
	{
		update(image, glm::uvec2(x, y));
	}
	void Texture::update(const Image& image)
	{
		update(image, glm::uvec2(0, 0));
	}
	void Texture::setSmooth(const bool& smooth)
	{
		if (m_Smooth != smooth)
		{
			m_Smooth = smooth;

			glBindTexture(GL_TEXTURE_2D, m_Texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Smooth ? GL_LINEAR : GL_NEAREST);
			
			if (m_MipMap)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Smooth ? GL_LINEAR : GL_NEAREST);
			}
		}
	}
	const bool Texture::isSmooth()
	{
		return m_Smooth;
	}
	void Texture::setSRGB(const bool& srgb)
	{
		m_SRGB = srgb;
	}
	const bool Texture::isSRGB()
	{
		return m_SRGB;
	}
	void Texture::setRepeated(const bool& repeated)
	{
		if (m_Repeated != repeated)
		{
			m_Repeated = repeated;

			if (m_Texture)
			{
				glBindTexture(GL_TEXTURE_2D, m_Texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE);
			}
		}
	}
	const bool Texture::isRepeated()
	{
		return m_Repeated;
	}
	void Texture::setMipmaps(const bool& mipmap)
	{
		if (m_Texture)
		{
			glBindTexture(GL_TEXTURE_2D, m_Texture);

			if (mipmap)
			{
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);

				m_MipMap = true;
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Smooth ? GL_LINEAR : GL_NEAREST);

				m_MipMap = false;
			}
		}
	}
	const bool Texture::hasMipmaps()
	{
		return m_MipMap;
	}
	void Texture::swap(Texture& texture)
	{
		std::swap(m_Size, texture.m_Size);
		std::swap(m_Texture, texture.m_Texture);
		std::swap(m_Smooth, texture.m_Smooth);
		std::swap(m_SRGB, texture.m_SRGB);
		std::swap(m_Repeated, texture.m_Repeated);
		std::swap(m_Flipped, texture.m_Flipped);
		std::swap(m_FboAttached, texture.m_FboAttached);
		std::swap(m_MipMap, texture.m_MipMap);
	}
	const glm::uint32 Texture::getID(GLenum id)
	{
		glBindTexture(id, m_Texture);
		return m_Texture;
	}
	glm::uint32 Texture::getMaximumSize()
	{
		static bool checked = false;
		static glm::int32 size = 0;

		if (!checked)
		{
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);

			checked = true;
		}

		return static_cast<glm::uint32>(size);
	}
}
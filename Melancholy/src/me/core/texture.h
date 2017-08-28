#pragma once

#include "../util/util.h"
#include "image.h"


namespace me::core
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		const bool create(const glm::uint32& width, const glm::uint32& height);
		const bool create(const glm::uvec2& size);
		const bool loadFromFile(const std::string& file);
		const bool loadFromImage(const Image& image);
		const glm::uvec2& getSize();
		Image copyToImage();
		void update(const glm::uint8* pixels, const glm::uvec2& size, const glm::uvec2& position);
		void update(const glm::uint8* pixels, const glm::uint32& width, const glm::uint32& height, const glm::uint32& x, const glm::uint32& y);
		void update(const glm::uint8* pixels);
		void update(const Texture& texture, const glm::uvec2& position);
		void update(const Texture& texture, const glm::uint32& x, const glm::uint32& y);
		void update(const Texture& texture);
		void update(const Image& image, glm::uvec2& position);
		void update(const Image& image, const glm::uint32& x, const glm::uint32& y);
		void update(const Image& image);
		void setSmooth(const bool& smooth);
		const bool isSmooth();
		void setSRGB(const bool& srgb);
		const bool isSRGB();
		void setRepeated(const bool& repeated);
		const bool isRepeated();
		void setMipmaps(const bool& mipmap);
		const bool hasMipmaps();
		void swap(Texture& texture);
		const glm::uint32 getID(GLenum type = GL_TEXTURE0);
		static glm::uint32 getMaximumSize();

	private:
		glm::uint32 m_Texture;
		std::string m_File;
		glm::uvec2 m_Size;
		bool m_Flipped;
		bool m_Repeated;
		bool m_SRGB;
		bool m_Smooth;
		bool m_FboAttached;
		bool m_MipMap;

	};
}
#pragma once

#include "../util/util.h"
#include "image.h"


namespace me::core
{

	class Texture
	{
	public:
		enum class Textures
		{
			Grass01,
			Grass02,
			Grass03,
			Grass04,
			Tile01_s01,
			Tile01_s02,
			Tile01_s03
		};
		static std::map<Textures, std::string> TextureMap;

	public:
		Texture();
		~Texture();

		bool create(const glm::uint32& width, const glm::uint32& height);
		bool create(const glm::uvec2& size);
		bool loadFromFile(const std::string& file);
		bool loadFromImage(const Image& image);
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
		void setSmooth(bool smooth);
		bool isSmooth();
		void setSRGB(bool srgb);
		bool isSRGB();
		void setRepeated(bool repeated);
		bool isRepeated();
		void setMipmaps(bool mipmap);
		bool hasMipmaps();
		void swap(Texture& texture);
		const glm::uint32 getID();
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
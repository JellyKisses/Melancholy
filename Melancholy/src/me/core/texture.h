#pragma once

#include "../util/util.h"
#include "image.h"


namespace me::core
{
	class Texture
	{
	public:
		enum class Type
		{
			Diffuse,
			Normal
		};

	public:
		Texture();
		~Texture();

		bool create(const glm::uint32& width, const glm::uint32& height, bool unique = true);
		bool create(const glm::uvec2& size, bool unique = true);
		bool loadFromFile(const std::string& file, bool unique = true, Type type = Type::Diffuse);
		bool loadFromImage(const Image& image, bool unique = true, Type type = Type::Diffuse);
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
		void setType(Type type);
		Type getType();
		void setSmooth(bool smooth);
		bool isSmooth();
		void setSRGB(bool srgb);
		bool isSRGB();
		void setRepeated(bool repeated);
		bool isRepeated();
		void setMipmaps(bool mipmap);
		bool hasMipmaps();
		void swap(Texture& texture);
		//Set to 0 or lower to not modify active textures
		const glm::uint32 getID(int id = 0);
		const std::string& getFile();
		static glm::uint32 getMaximumSize();

	private:
		glm::uint32 m_Texture;
		std::string m_File;
		glm::uvec2 m_Size;
		Type m_Type;
		bool m_Flipped;
		bool m_Repeated;
		bool m_SRGB;
		bool m_Smooth;
		bool m_FboAttached;
		bool m_MipMap;

	};
}
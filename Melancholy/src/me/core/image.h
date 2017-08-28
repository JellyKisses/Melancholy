#pragma once

#include "../util/util.h"


namespace me::core
{
	class Image
	{
	public:
		Image();
		~Image();

		void create(const glm::uint32& width, const glm::uint32& height, const glm::u8vec4& color);
		void create(const glm::uint32& width, const glm::uint32& height, const glm::u8vec3& color);
		void create(const glm::uint32& width, const glm::uint32& height, const glm::uint8& r, const glm::uint8& g, const glm::uint8& b, const glm::uint8& a = 255);
		void create(const glm::uint32& width, const glm::uint32& height, const glm::uint8* pixels);

		const bool loadFromFile(const std::string& file);
		const bool saveToFile(const std::string& file);

		const glm::uvec2 getSize();

		void setPixel(const glm::uint32& x, const glm::uint32& y, const glm::u8vec4& color);
		void setPixel(const glm::uint32& x, const glm::uint32& y, const glm::u8vec3& color);
		void setPixel(const glm::uint32& x, const glm::uint32& y, const glm::uint8& r, const glm::uint8& g, const glm::uint8& b, const glm::uint8& a = 255);

		const glm::u8vec4& getPixel(const glm::uint32& x, const glm::uint32& y);

		const glm::uint8* getPixelPtr();
		void flipX();
		void flipY();


	private:
		std::string m_File;
		glm::uvec2 m_Size;
		std::vector<glm::uint8> m_Pixels;

	};
}
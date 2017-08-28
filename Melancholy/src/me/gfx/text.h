#pragma once

#include "../util/util.h"
#include "../core/texture.h"
#include "../core/transformable.h"
#include "../core/shader.h"

namespace me::gfx
{
	struct Glyph
	{
		Glyph();

		glm::float32 m_Advance;
		glm::vec4 m_Bounds;
		glm::ivec4 m_TextureRect;
	};

	class Font
	{
	public:
		class Info
		{
		public:
			Info();

		public:
			std::string m_Family;
		};

	public:
		Font();
		~Font();

		const bool loadFromFile(const std::string& file);
		const Info& getInfo();
		const Glyph& getGlyph(glm::uint32 point, glm::uint32 size = 11, bool bold = false, float thickness = 0);
		const glm::float32 getKerning(const glm::uint32& first, const glm::uint32& second, const glm::uint32& size);
		const glm::float32 getLineSpacing(const const glm::uint32& size);
		const glm::float32 getUnderlinePosition(const glm::uint32& size);
		const glm::float32 getUnderlineThickness(const glm::uint32& size);
		core::Texture getTexture(const glm::uint32& size);

	private:
		struct Row
		{
			Row(const glm::uint32& top, const glm::uint32& height);

			glm::uint32 m_Width;
			glm::uint32 m_Height;
			glm::uint32 m_Top;
		};
		struct Page
		{
			Page();

			std::map<glm::uint64, Glyph> m_Glyphs;
			core::Texture m_Texture;
			glm::uint32 m_NextRow;
			std::vector<Row> m_Rows;
		};
		friend class Text;

	private:
		void* m_Library;
		void* m_Face;
		void* m_Stroker;
		glm::int32* m_RefCount;
		Info m_Info;
		std::map<glm::uint32, Page> m_Pages;
		std::vector<glm::uint8> m_PixelBuffer;
		std::string m_File;

	private:
		Glyph loadGlyph(glm::uint32 point, glm::uint32 size, bool bold, glm::float32 thickness);
		const glm::ivec4& findGlyphRect(Page& page, const glm::uint32& width, const glm::uint32& height);
		const bool setCurrentSize(const glm::uint32 size);
	};

	class Text : public core::Transformable2D
	{
	public:
		const enum Style
		{
			Regular = 0,
			Bold = 1 << 0,
			Italic = 1 << 1,
			Underlined = 1 << 2,
			StrikeThrough = 1 << 3
		};
		class Vertex
		{
		public:
			Vertex();
			Vertex(const glm::vec2& position, const glm::vec2& texCoord, const glm::vec4& color);
			Vertex(const glm::float32& px, const glm::float32& py, const glm::float32& tx, const glm::float32& ty, const glm::float32& r, const glm::float32& g, const glm::float32& b, const glm::float32& a);
			~Vertex();

		public:
			glm::vec2 m_Position;
			glm::vec2 m_TexCoord;
			glm::vec4 m_Color;
		};

	public:
		Text();
		Text(const std::string& str, const Font& font, const glm::uint32& size = 12);
		~Text();

		void setString(const std::string& str);
		void setFont(const Font& font);
		void setStyle(const glm::uint32& style);
		void setCharacterSize(const glm::uint32& size);
		void setFillColor(const glm::u8vec4& color);
		void setFillColor(const glm::u8vec3& color);
		void setFillColor(const glm::uint8& r, const glm::uint8& g, const glm::uint8& b, const glm::uint8& a = 255);
		void setOutlineThickness(const glm::float32& thickness);
		void setOutlineColor(const glm::u8vec4& color);
		void setOutlineColor(const glm::u8vec3& color);
		void setOutlineColor(const glm::uint8& r, const glm::uint8& g, const glm::uint8& b, const glm::uint8& a = 255);
		const std::string& getString();
		const Font* getFont();
		const glm::uint32& getCharacterSize();
		const glm::uint32& getOutlineThickness();
		const glm::uint32& getStyle();
		const glm::u8vec4& getFillColor();
		const glm::u8vec4& getOutlineColor();
		const glm::vec2& findChracterPos(glm::size_t index);
		const glm::vec4& getLocalBounds();
		const glm::vec4& getGlobalBounds();

		void draw();

	private:
		std::string m_String;
		const Font* m_Font;
		glm::uint32 m_CharSize;
		glm::uint32 m_Style;
		glm::u8vec4 m_FillColor;
		glm::u8vec4 m_OutlineColor;
		glm::float32 m_OutlineThickness;
		std::vector<Text::Vertex> m_Vertices;
		std::vector<Text::Vertex> m_OutlineVertices;
		glm::uint32 m_Vbo[2];
		static glm::uint32 m_Vao;
		static core::Shader m_Shader;
		glm::vec4 m_Bounds;
		bool m_Update;

	private:
		friend class App;

	private:
		void update();
		void initialize();
		void addLine(std::vector<Text::Vertex>& vertices, const glm::float32& lineLength, const glm::float32& lineTop, const glm::u8vec4& color, const glm::float32& offset, const glm::float32& thickness, const glm::float32& outlineThickness = 0);
		void addQuad(std::vector<Text::Vertex>& vertices, const glm::vec2& position, const glm::u8vec4& color, const Glyph& glyph, const glm::float32& italic, const glm::float32& outlineThickness = 0);

	};
}
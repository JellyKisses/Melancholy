#pragma once

#include "../util/util.h"
#include "../core/texture.h"
#include "../core/transformable.h"
#include "../core/shader.h"
#include "../core/image.h"

namespace me::gfx
{
	namespace priv
	{
		enum class RenderMode
		{
			RENDER_NORMAL,
			RENDER_OUTLINE_EDGE,
			RENDER_OUTLINE_POSITIVE,
			RENDER_OUTLINE_NEGATIVE,
			RENDER_SIGNED_DISTANCE_FIELD
		};
		class Kerning
		{
		public:
			Kerning();

		public:
			glm::uint32 m_CodePoint;
			glm::float32 m_Kerning;

		};
		class Glyph
		{
		public:
			Glyph();
			~Glyph();

			glm::float32 getKerning(const char* codePoint);

		public:
			glm::uint32 m_CodePoint;
			glm::uint32 m_Width;
			glm::uint32 m_Height;
			glm::int32  m_OffsetX;
			glm::int32  m_OffsetY;
			glm::float32 m_AdvanceX;
			glm::float32 m_AdvanceY;
			glm::float32 m_s0, m_s1, m_t0, m_t1;
			glm::float32 m_OutlineThickness;
			std::vector<Kerning> m_Kernings;
			RenderMode m_RenderMode;
		};
		class Font
		{
		private:
			static glm::int32 loadFace(glm::float32 size, FT_Library* library, FT_Face* face);
			void genKerning(FT_Library* library, FT_Face* face);

		public:
			Font();
			~Font();

			bool loadFromFile(const std::string& file);

			Glyph* getGlyph(const char* codePoint);
			Glyph* loadGlyph(const char* codePoint);
			glm::size_t loadGlyphs(const char* codePoints);
			void enlarge(const glm::uint32 width, const glm::uint32 height);

		public:
			std::vector<Glyph> m_Glyphs;
			std::string m_File;
			core::Image m_Image;
			glm::float32 m_Size;
			glm::float32 m_Height;
			glm::float32 m_LineGap;
			glm::float32 m_Ascender;
			glm::float32 m_Descender;
			glm::float32 m_OutlineThickness;
			glm::float32 m_UnderlinePosition;
			glm::float32 m_UnderlineThickness;
			RenderMode m_RenderMode;
			bool m_Hinting;
			bool m_Filtering;
			bool m_Kerning;
			glm::uint8* m_LcdWeights;
		};
	}


}
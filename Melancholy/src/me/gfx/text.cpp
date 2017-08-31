#include "text.h"
#include "../core/app.h"


namespace me::gfx
{
	namespace priv
	{
	
		glm::uint32	utf8_to_utf32(const char* character)
		{
			uint32_t result = -1;

			if (!character)
			{
				return result;
			}

			if ((character[0] & 0x80) == 0x0)
			{
				result = character[0];
			}

			if ((character[0] & 0xC0) == 0xC0)
			{
				result = ((character[0] & 0x3F) << 6) | (character[1] & 0x3F);
			}

			if ((character[0] & 0xE0) == 0xE0)
			{
				result = ((character[0] & 0x1F) << (6 + 6)) | ((character[1] & 0x3F) << 6) | (character[2] & 0x3F);
			}

			if ((character[0] & 0xF0) == 0xF0)
			{
				result = ((character[0] & 0x0F) << (6 + 6 + 6)) | ((character[1] & 0x3F) << (6 + 6)) | ((character[2] & 0x3F) << 6) | (character[3] & 0x3F);
			}

			if ((character[0] & 0xF8) == 0xF8)
			{
				result = ((character[0] & 0x07) << (6 + 6 + 6 + 6)) | ((character[1] & 0x3F) << (6 + 6 + 6)) | ((character[2] & 0x3F) << (6 + 6)) | ((character[3] & 0x3F) << 6) | (character[4] & 0x3F);
			}

			return result;
		}

		Kerning::Kerning() : m_CodePoint(0), m_Kerning(0.f)
		{
		
		}

		Glyph::Glyph()
			:
			m_CodePoint(-1),
			m_Width(0),
			m_Height(0),
			m_OffsetX(0),
			m_OffsetY(0),
			m_AdvanceX(0.f),
			m_AdvanceY(0.f),
			m_s0(0.f),
			m_s1(0.f),
			m_t0(0.f),
			m_t1(0.f),
			m_OutlineThickness(),
			m_Kernings(),
			m_RenderMode(RenderMode::RENDER_NORMAL)
		{

		}
		Glyph::~Glyph()
		{
			m_Kernings.clear();
		}
		glm::float32 Glyph::getKerning(const char* codePoint)
		{
			glm::uint32 ucodepoint = utf8_to_utf32(codePoint);

			for (glm::size_t i = 0; i < m_Kernings.size(); i++)
			{
				if (m_Kernings.at(i).m_CodePoint == ucodepoint)
				{
					return m_Kernings.at(i).m_Kerning;
				}
			}
			return 0;
		}

		void Font::genKerning(FT_Library* library, FT_Face* face)
		{
			FT_UInt glyph_index, prev_index;
			FT_Vector kerning;

			for (glm::uint32 i = 1; i < m_Glyphs.size(); ++i)
			{
				glyph_index = FT_Get_Char_Index(*face, m_Glyphs.at(i).m_CodePoint);
				m_Glyphs.at(i).m_Kernings.clear();

				for (glm::uint32 j = 1; j < m_Glyphs.size(); ++j)
				{
					prev_index = FT_Get_Char_Index(*face, m_Glyphs.at(j).m_CodePoint);

					if (kerning.x)
					{
						Kerning k;
						k.m_CodePoint = m_Glyphs.at(j).m_CodePoint;
						k.m_Kerning = kerning.x / static_cast<glm::float32>(core::g_AppInstance->m_AppInfo.getResolution().y * core::g_AppInstance->m_AppInfo.getResolution().y);
						m_Glyphs.at(i).m_Kernings.push_back(k);
					}
				}
			}
		}
		glm::int32 Font::loadFace(glm::float32 size, FT_Library* library, FT_Face* face)
		{

		}
		Font::Font()
			:
			m_Height(0.f),
			m_Ascender(0.f),
			m_Descender(0.f),
			m_RenderMode(RenderMode::RENDER_NORMAL),
			m_OutlineThickness(0.f),
			m_Hinting(true),
			m_Kerning(true),
			m_Filtering(true)
		{
			m_LcdWeights = new glm::uint8[5]{ 0x10, 0x40, 0x70, 0x40, 0x10 };
		}
		Font::~Font()
		{

		}
		const bool Font::loadFromFile(const std::string& file)
		{
			static FT_Library library;
			static FT_Face face;
			static FT_Size_Metrics metrics;

			if (!loadFace(m_Size * 100.f, &library, &face))
				throw util::RuntimeError(util::RuntimeError::ErrorType::Initialize, "FreeType Font Face");

			m_UnderlinePosition = face->underline_position / static_cast<glm::float32>(core::g_AppInstance->m_AppInfo.getResolution().y * core::g_AppInstance->m_AppInfo.getResolution().y) * m_Size;
			m_UnderlinePosition = roundf(m_UnderlinePosition);
			if (m_UnderlinePosition > -2.f)
				m_UnderlinePosition = -2.f;

			m_UnderlineThickness = face->underline_thickness / static_cast<glm::float32>(core::g_AppInstance->m_AppInfo.getResolution().y * core::g_AppInstance->m_AppInfo.getResolution().y) * m_Size;
			m_UnderlineThickness = roundf(m_UnderlineThickness);
			if (m_UnderlineThickness > 1.f)
				m_UnderlineThickness = 1.f;

			metrics = face->size->metrics;
			m_Ascender = (metrics.ascender >> 6) / 100.f;
			m_Descender = (metrics.descender >> 6) / 100.f;
			m_Height = (metrics.height >> 6) / 100.f;
			m_LineGap = m_Height - m_Ascender + m_Descender;
			FT_Done_Face(face);
			FT_Done_FreeType(library);

			getGlyph(0);

			return true;
		}
		Glyph* Font::getGlyph(const char* codePoint)
		{

		}
		Glyph* Font::loadGlyph(const char* codePoint)
		{

		}
		glm::size_t Font::loadGlyphs(const char* codePoints)
		{

		}
		void Font::enlarge(const glm::uint32 width, const glm::uint32 height)
		{

		}
	}
}
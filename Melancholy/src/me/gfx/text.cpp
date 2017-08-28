#include "text.h"
#include "../core/app.h"


namespace me::gfx
{
	glm::uint32 Text::m_Vao = 0;
	core::Shader Text::m_Shader;

	Text::Vertex::Vertex() : m_Position(0.f, 0.f), m_TexCoord(0.f, 0.f), m_Color(0.f, 0.f, 0.f, 0.f)
	{
		
	}
	Text::Vertex::Vertex(const glm::vec2& position, const glm::vec2& texCoord, const glm::vec4& color) : m_Position(position), m_TexCoord(texCoord), m_Color(color)
	{

	}
	Text::Vertex::Vertex(const glm::float32& px, const glm::float32& py, const glm::float32& tx, const glm::float32& ty, const glm::float32& r, const glm::float32& g, const glm::float32& b, const glm::float32& a) : m_Position(px, py), m_TexCoord(tx, ty), m_Color(r, g, b, a)
	{

	}
	Text::Vertex::~Vertex()
	{

	}

	Glyph::Glyph() : m_Advance(0.f) {}

	Font::Info::Info() : m_Family() {}

	Font::Font() :
		m_Face(0),
		m_Stroker(0),
		m_RefCount(0),
		m_Info()
	{}
	Font::~Font() 
	{ 
		if (m_RefCount)
		{
			(*m_RefCount)--;

			if (*m_RefCount == 0)
			{
				delete m_RefCount;

				if (m_Stroker) FT_Stroker_Done(static_cast<FT_Stroker>(m_Stroker));
				if (m_Face) FT_Done_Face(static_cast<FT_Face>(m_Face));
				if (m_Library) FT_Done_FreeType(static_cast<FT_Library>(m_Library));
			}
		}
		
		m_Library = 0;
		m_Face = 0;
		m_Stroker = 0;
		m_RefCount = 0;
		m_Pages.clear();
		std::vector<glm::uint8>().swap(m_PixelBuffer);
	}
	const bool Font::loadFromFile(const std::string& file)
	{
		m_RefCount = new int(1);
		
		FT_Library library;
		FT_Error error = FT_Init_FreeType(&library);
		if (error != 0)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Initialize, std::string("Font Library: ") + file, FT_GetErrorString(error));
			return false;
		}
		m_Library = library;

		FT_Face face;
		error = FT_New_Face(static_cast<FT_Library>(m_Library), file.c_str(), 0, &face);
		if (error != 0)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Read, std::string("Font Face: ") + file, FT_GetErrorString(error));
			return false;
		}

		FT_Stroker stroker;
		error = FT_Stroker_New(static_cast<FT_Library>(m_Library), &stroker);
		if (error != 0)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Initialize, std::string("Font Stroker: ") + file, FT_GetErrorString(error));
			return false;
		}

		error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
		if (error != 0)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Initialize, std::string("Font Face: ") + file, FT_GetErrorString(error));
			return false;
		}
		
		m_Face = face;
		m_Stroker = stroker;

		m_Info.m_Family = face->family_name ? face->family_name : std::string();

		m_File = file;

		return true;
	}
	const Font::Info& Font::getInfo()
	{
		return m_Info;
	}
	const Glyph& Font::getGlyph(glm::uint32 point, glm::uint32 size, bool bold, float thickness)
	{
		std::map<glm::uint64, Glyph>& glyphs = m_Pages[size].m_Glyphs;

		glm::uint64 key = (static_cast<glm::uint64>(*reinterpret_cast<glm::uint32*>(&thickness)) << 32)
						| (static_cast<glm::uint64>(bold ? 1 : 0) << 31)
						|  static_cast<glm::uint64>(point);

		std::map<glm::uint64, Glyph>::const_iterator it = glyphs.find(key);
		if (it != glyphs.end())
		{
			return it->second;
		}
		else
		{
			Glyph glyph = loadGlyph(point, size, bold, thickness);
			return glyphs.insert(std::make_pair(key, glyph)).first->second;
		}
	}
	const glm::float32 Font::getKerning(const glm::uint32& first, const glm::uint32& second, const glm::uint32& size)
	{
		if (first == 0 || second == 0)
			return 0.f;

		FT_Face face = static_cast<FT_Face>(m_Face);

		if (face && FT_HAS_KERNING(face) && setCurrentSize(size))
		{
			FT_UInt index1 = FT_Get_Char_Index(face, first);
			FT_UInt index2 = FT_Get_Char_Index(face, second);
			FT_Vector kerning;

			FT_Get_Kerning(face, index1, index2, FT_KERNING_DEFAULT, &kerning);

			if (!FT_IS_SCALABLE(face)) return static_cast<glm::float32>(kerning.x);

			return static_cast<glm::float32>(kerning.x) / static_cast<glm::float32>(1 << 6);
		}
		else
		{
			return 0.f;
		}
	}
	const glm::float32 Font::getLineSpacing(const glm::uint32& size)
	{
		FT_Face face = static_cast<FT_Face>(m_Face);
		
		if (face && setCurrentSize(size))
		{
			return static_cast<glm::float32>(face->size->metrics.height) / static_cast<glm::float32>(1 << 6);
		}
		else
		{
			return 0.f;
		}
	}
	const glm::float32 Font::getUnderlinePosition(const glm::uint32& size)
	{
		FT_Face face = static_cast<FT_Face>(m_Face);

		if (face && setCurrentSize(size))
		{
			if (!FT_IS_SCALABLE(face))
				return size / 10.f;

			return -static_cast<glm::float32>(FT_MulFix(face->underline_position, face->size->metrics.y_scale)) / static_cast<glm::float32>(1 << 6);
		}
		else
		{
			return 0.f;
		}
	}
	const glm::float32 Font::getUnderlineThickness(const glm::uint32& size)
	{
		FT_Face face = static_cast<FT_Face>(m_Face);

		if (face && setCurrentSize(size))
		{
			if (!FT_IS_SCALABLE(face))
				return size / 14.f;

			return static_cast<glm::float32>(FT_MulFix(face->underline_thickness, face->size->metrics.y_scale)) / static_cast<glm::float32>(1 << 6);
		}
		else
		{
			return 0.f;
		}
	}
	core::Texture Font::getTexture(const glm::uint32& size)
	{
		return m_Pages.at(size).m_Texture;
	}
	Glyph Font::loadGlyph(glm::uint32 point, glm::uint32 size, bool bold, glm::float32 thickness)
	{
		Glyph glyph;

		FT_Face face = static_cast<FT_Face>(m_Face);
		if (!face) return glyph;
		if (!setCurrentSize(size)) return glyph;
		
		FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;
		if (thickness != 0) flags |= FT_LOAD_NO_BITMAP;
	
		if (FT_Load_Char(face, point, flags) != 0) return glyph;

		FT_Glyph glyphDesc;
		if (FT_Get_Glyph(face->glyph, &glyphDesc) != 0) return glyph;

		FT_Pos weight = 1 << 6;
		bool outline = (glyphDesc->format == FT_GLYPH_FORMAT_OUTLINE);
		if (outline)
		{
			if (bold)
			{
				FT_OutlineGlyph outlineGlyph = (FT_OutlineGlyph)glyphDesc;
				FT_Outline_Embolden(&outlineGlyph->outline, weight);
			}
			if (thickness != 0)
			{
				FT_Stroker stroker = static_cast<FT_Stroker>(m_Stroker);

				FT_Stroker_Set(stroker, static_cast<FT_Fixed>(thickness * static_cast<glm::float32>(1 << 6)), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
				FT_Glyph_Stroke(&glyphDesc, stroker, false);
			}
		}

		FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_Bitmap& bitmap = reinterpret_cast<FT_BitmapGlyph>(glyphDesc)->bitmap;

		if (!outline)
		{
			if (bold) FT_Bitmap_Embolden(static_cast<FT_Library>(m_Library), &bitmap, weight, weight);
			if (thickness != 0) throw util::RuntimeError(util::RuntimeError::ErrorType::Create, std::string("Glyph Outline: ") + m_File);
		}

		glyph.m_Advance = static_cast<glm::float32>(face->glyph->metrics.horiAdvance) / static_cast<glm::float32>(1 << 6);
		if (bold) glyph.m_Advance += static_cast<glm::float32>(weight) / static_cast<glm::float32>(1 << 6);

		glm::int32 width = bitmap.width;
		glm::int32 height = bitmap.rows;

		if ((width > 0) && (height > 0))
		{
			const glm::uint32 padding = 1;

			width += 2 * padding;
			height += 2 * padding;

			Page& page = m_Pages.at(size);
			glyph.m_TextureRect = findGlyphRect(page, width, height);

			glyph.m_TextureRect.x += padding;
			glyph.m_TextureRect.y += padding;
			glyph.m_TextureRect.z -= 2 * padding;
			glyph.m_TextureRect.w -= 2 * padding;

			glyph.m_Bounds.x = static_cast<glm::float32>(face->glyph->metrics.horiBearingX) / static_cast<glm::float32>(1 << 6);
			glyph.m_Bounds.y = static_cast<glm::float32>(face->glyph->metrics.horiBearingY) / static_cast<glm::float32>(1 << 6);
			glyph.m_Bounds.z = static_cast<glm::float32>(face->glyph->metrics.width) / static_cast<glm::float32>(1 << 6) + thickness * 2;
			glyph.m_Bounds.w = static_cast<glm::float32>(face->glyph->metrics.height) / static_cast<glm::float32>(1 << 6) + thickness * 2;

			m_PixelBuffer.resize(width*height * 4);
			glm::uint8* current = &m_PixelBuffer.at(0);
			glm::uint8* end = current + width * height * 4;

			while (current != end)
			{
				(*current++) = 255;
				(*current++) = 255;
				(*current++) = 255;
				(*current++) = 0;
			}

			const glm::uint8* pixels = bitmap.buffer;
			if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
			{
				for (glm::int32 y = padding; y < height - padding; ++y)
				{
					for (glm::int32 x = padding; x < width - padding; ++x)
					{
						glm::size_t index = x + y * width;
						m_PixelBuffer.at(index * 4 + 3) = ((pixels[(x - padding) / 8]) & (1 << (7 - ((x - padding) % 8)))) ? 255 : 0;
					}
					pixels += bitmap.pitch;
				}
			}
			else
			{
				for (glm::int32 y = padding; y < height - padding; ++y)
				{
					for (glm::int32 x = padding; x < width - padding; ++x)
					{
						glm::size_t index = x + y * width;
						m_PixelBuffer[index * 4 + 3] = pixels[x - padding];
					}
					pixels += bitmap.pitch;
				}
			}

			glm::int32 x = glyph.m_TextureRect.x - padding;
			glm::int32 y = glyph.m_TextureRect.y - padding;
			glm::int32 w = glyph.m_TextureRect.z + 2 * padding;
			glm::int32 h = glyph.m_TextureRect.w + 2 * padding;
			
			page.m_Texture.update(&m_PixelBuffer[0], w, h, x, y);
		}

		FT_Done_Glyph(glyphDesc);

		return glyph;
	}
	const glm::ivec4& Font::findGlyphRect(Page& page, const glm::uint32& width, const glm::uint32& height)
	{
		Row* row = 0;
		glm::float32 bestRatio = 0;
		for (std::vector<Row>::iterator it = page.m_Rows.begin(); it != page.m_Rows.end() && !row; ++it)
		{
			float ratio = static_cast<glm::float32>(height) / it->m_Height;

			if ((ratio < 0.7f) || (ratio > 1.f)) continue;
			if (width > const_cast<Page&>(page).m_Texture.getSize().x - it->m_Width) continue;
			if (ratio < bestRatio) continue;

			row = &*it;
			bestRatio = ratio;
		}

		if (!row)
		{
			int rowHeight = height + height / 10;
			while ((page.m_NextRow + rowHeight >= page.m_Texture.getSize().y) || (width >= page.m_Texture.getSize().x))
			{
				glm::uint32 textureWidth = page.m_Texture.getSize().x;
				glm::uint32 textureHeight = page.m_Texture.getSize().y;
				if ((textureHeight * 2 <= core::Texture::getMaximumSize()) && (textureHeight * 2 <= core::Texture::getMaximumSize()))
				{
					core::Texture newTexture;
					newTexture.create(textureWidth * 2, textureHeight * 2);
					newTexture.update(page.m_Texture);
					page.m_Texture.swap(newTexture);
				}
				else
				{
					throw util::RuntimeError(util::RuntimeError::ErrorType::Create, std::string("Glyph Rect: ") + m_File, "while findGlyphRect(), the max texture size was reached");
					return glm::ivec4(0, 0, 2, 2);
				}
			}

			page.m_Rows.push_back(Row(page.m_NextRow, rowHeight));
			page.m_NextRow += rowHeight;
			row = &page.m_Rows.back();
		}

		glm::ivec4 rect(row->m_Width, row->m_Top, width, height);
		row->m_Width += width;

		return rect;
	}
	const bool Font::setCurrentSize(const glm::uint32 size)
	{
		FT_Face face = static_cast<FT_Face>(m_Face);
		FT_UShort currentSize = face->size->metrics.x_ppem;

		if (currentSize != size)
		{
			FT_Error error = FT_Set_Pixel_Sizes(face, 0, size);

			if (error == FT_Err_Invalid_Pixel_Size)
			{
				if (!FT_IS_SCALABLE(face))
				{
					throw util::RuntimeError(util::RuntimeError::ErrorType::Set, std::string("Font Current Size: ") + m_File, std::to_string(size) + " is too large");
				}
			}

			return error == FT_Err_Ok;
		}
		else
		{
			return true;
		}
	}

	Font::Row::Row(const glm::uint32& top, const glm::uint32& height) : m_Width(0), m_Top(top), m_Height(height) {}

	Font::Page::Page() : m_NextRow(3)
	{
		core::Image image;
		image.create(128, 128, 255, 255, 255, 0);

		for (glm::int32 x = 0; x < 2; ++x)
			for (glm::int32 y = 0; y < 2; ++y)
				image.setPixel(x, y, 255, 255, 255, 255);
			
		m_Texture.loadFromImage(image);
		m_Texture.setSmooth(true);
	}

	Text::Text() 
		:
		m_String(),
		m_Font(0),
		m_CharSize(12),
		m_Style(Style::Regular),
		m_FillColor(255, 255, 255, 255),
		m_OutlineColor(0, 0, 0, 255),
		m_OutlineThickness(0),
		m_Bounds(),
		m_Update(false)
	{
		initialize();
	}
	Text::Text(const std::string& str, const Font& font, const glm::uint32& size)
		:
		m_String(str),
		m_Font(&font),
		m_CharSize(size),
		m_Style(Style::Regular),
		m_FillColor(255, 255, 255, 255),
		m_OutlineColor(0, 0, 0, 255),
		m_OutlineThickness(0),
		m_Bounds(),
		m_Update(true)
	{
		initialize();
	}
	Text::~Text() 
	{
	
	}

	void Text::setString(const std::string& str)
	{
		if (m_String != str)
		{
			m_String = str;
			m_Update = true;
		}
	}
	void Text::setFont(const Font& font)
	{
		if (m_Font != &font)
		{
			m_Font = &font;
			m_Update = true;
		}
	}
	void Text::setStyle(const glm::uint32& style)
	{
		if (m_Style != style)
		{
			m_Style = style;
			m_Update = true;
		}
	}
	void Text::setCharacterSize(const glm::uint32& size)
	{
		if (m_CharSize != size)
		{
			m_CharSize = size;
			m_Update = true;
		}
	}
	void Text::setFillColor(const glm::u8vec4& color)
	{
		if (m_FillColor != color)
		{
			m_FillColor = color;

			if (!m_Update)
				for (glm::size_t i = 0; i < m_Vertices.size(); ++i)
					m_Vertices.at(i).m_Color = m_FillColor;
			
		}
	}
	void Text::setFillColor(const glm::u8vec3& color)
	{
		setFillColor(glm::u8vec4(color.x, color.y, color.z, 255));
	}
	void Text::setFillColor(const glm::uint8& r, const glm::uint8& g, const glm::uint8& b, const glm::uint8& a)
	{
		setFillColor(glm::u8vec4(r, g, b, a));
	}
	void Text::setOutlineThickness(const glm::float32& thickness)
	{
		if (m_OutlineThickness != thickness)
		{
			m_OutlineThickness = thickness;
			m_Update = true;
		}
	}
	void Text::setOutlineColor(const glm::u8vec4& color)
	{
		if (m_OutlineColor != color)
		{
			m_OutlineColor = color;

			if (!m_Update)
				for (glm::size_t i = 0; i < m_Vertices.size(); ++i)
					m_Vertices.at(i).m_Color = m_FillColor;
		}
	}
	void Text::setOutlineColor(const glm::u8vec3& color)
	{
		setOutlineColor(glm::u8vec4(color.x, color.y, color.z, 255));
	}
	void Text::setOutlineColor(const glm::uint8& r, const glm::uint8& g, const glm::uint8& b, const glm::uint8& a)
	{
		setOutlineColor(glm::u8vec4(r, g, b, a));
	}
	const std::string& Text::getString()
	{
		return m_String;
	}
	const Font* Text::getFont()
	{
		return m_Font;
	}
	const glm::uint32& Text::getCharacterSize()
	{
		return m_CharSize;
	}
	const glm::uint32& Text::getOutlineThickness()
	{
		return m_OutlineThickness;
	}
	const glm::uint32& Text::getStyle()
	{
		return m_Style;
	}
	const glm::u8vec4& Text::getFillColor()
	{
		return m_FillColor;
	}
	const glm::u8vec4& Text::getOutlineColor()
	{
		return m_OutlineColor;
	}
	const glm::vec2& Text::findChracterPos(glm::size_t index)
	{
		if (!m_Font) return glm::vec2(0.f, 0.f);

		if (index > m_String.size()); index = m_String.size();
		
		bool bold = (m_Style & Style::Bold) != 0;
		glm::float32 hspace = const_cast<Font*>(m_Font)->getGlyph(L' ', m_CharSize, bold).m_Advance;
		glm::float32 vspace = const_cast<Font*>(m_Font)->getLineSpacing(m_CharSize);

		glm::vec2 position;
		glm::uint32 prevChar = 0;
		for (glm::size_t i = 0; i < index; ++i)
		{
			glm::uint32 curChar = m_String.at(i);
			position.x += const_cast<Font*>(m_Font)->getKerning(prevChar, curChar, m_CharSize);
			prevChar = curChar;

			switch (curChar)
			{
			case ' ': position.x += hspace; continue;
			case '\t': position.x += hspace * 4; continue;
			case '\n': position.y += vspace; position.x = 0; continue;
			}

			position.x += const_cast<Font*>(m_Font)->getGlyph(curChar, m_CharSize, bold).m_Advance;
		}
		position = getTransformPoint(position);

		return position;
	}
	const glm::vec4& Text::getLocalBounds()
	{
		update();
		return m_Bounds;
	}
	const glm::vec4& Text::getGlobalBounds()
	{
		update();
		return glm::vec4(getTransformPoint(0.f, 0.f), getTransformPoint(m_Bounds.z, m_Bounds.w));
	}
	void Text::draw()
	{
		if (m_Font)
		{
			update();
			glDisable(GL_DEPTH_TEST);

			m_Shader.bind();
			m_Shader.addUniformI("u_Tex", const_cast<Font*>(m_Font)->getTexture(m_CharSize).getID(GL_TEXTURE1));
			m_Shader.addUniformF("u_Projection", glm::ortho(0.f, static_cast<glm::float32>(core::g_AppInstance->m_AppInfo.getResolution().x), 0.f, static_cast<glm::float32>(core::g_AppInstance->m_AppInfo.getResolution().y)));

			glBindVertexArray(m_Vao);

			if (m_OutlineThickness != 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_Vbo[1]);
				glDrawArrays(GL_TRIANGLES, 0, m_OutlineVertices.size());
			}
			glBindBuffer(GL_ARRAY_BUFFER, m_Vbo[0]);
			glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());

			glEnable(GL_DEPTH_TEST);
		}
	}
	void Text::update()
	{
			if (!m_Update)
				return;

			m_Vertices.clear();
			m_OutlineVertices.clear();
			m_Bounds = glm::vec4(0.f);

			if (!m_Font || m_String.empty())
				return;

			bool bold = (m_Style & bold) != 0;
			bool underlined = (m_Style & Underlined) != 0;
			bool strikeThrough = (m_Style & StrikeThrough) != 0;
			bool italic = (m_Style & Italic) != 0;

			glm::float32 underlineOffset = const_cast<Font*>(m_Font)->getUnderlinePosition(m_CharSize);
			glm::float32 underlineThickness = const_cast<Font*>(m_Font)->getUnderlineThickness(m_CharSize);
			glm::vec4 xBounds = const_cast<Font*>(m_Font)->getGlyph(L'x', m_CharSize, bold).m_Bounds;
			glm::float32 strikeThroughOffset = xBounds.y + xBounds.w / 2.f;
			glm::float32 hspace = const_cast<Font*>(m_Font)->getGlyph(L' ', m_CharSize, bold).m_Advance;
			glm::float32 vspace = const_cast<Font*>(m_Font)->getLineSpacing(m_CharSize);
			glm::float32 x = 0.f;
			glm::float32 y = static_cast<glm::float32>(m_CharSize);
			glm::float32 minX = static_cast<glm::float32>(m_CharSize);
			glm::float32 minY = static_cast<glm::float32>(m_CharSize);
			glm::float32 maxX = 0.f;
			glm::float32 maxY = 0.f;
			glm::uint32 prevChar = 0;
			for (glm::size_t i = 0; i < m_String.size(); ++i)
			{
				glm::uint32 curChar = m_String.at(i);
				x += const_cast<Font*>(m_Font)->getKerning(prevChar, curChar, m_CharSize);
				prevChar = curChar;

				if (underlined && (curChar == L'\n'))
				{
					addLine(m_Vertices, x, y, m_FillColor, underlineOffset, underlineThickness);

					if (m_OutlineThickness != 0)
						addLine(m_OutlineVertices, x, y, m_OutlineColor, underlineOffset, underlineThickness, m_OutlineThickness);
				}
				if (strikeThrough && (curChar == L'\n'))
				{
					addLine(m_Vertices, x, y, m_FillColor, strikeThroughOffset, underlineThickness);

					if (m_OutlineThickness != 0)
						addLine(m_OutlineVertices, x, y, m_OutlineColor, strikeThroughOffset, underlineThickness, m_OutlineThickness);
				}
				if ((curChar == ' ') || (curChar == '\t') || (curChar == '\n'))
				{
					minX = glm::min(minX, x);
					minY = glm::min(minY, y);

					switch (curChar)
					{
					case ' ': x += hspace; break;
					case '\t': x += hspace * 4; break;
					case '\n': y += vspace; x = 0; break;
					}

					maxX = glm::max(maxX, x);
					maxY = glm::max(maxY, y);

					continue;
				}

				if (m_OutlineThickness != 0)
				{
					const Glyph& glyph = const_cast<Font*>(m_Font)->getGlyph(curChar, m_CharSize, bold, m_OutlineThickness);
					
					glm::float32 left = glyph.m_Bounds.x;
					glm::float32 top = glyph.m_Bounds.y;
					glm::float32 right = glyph.m_Bounds.x + glyph.m_Bounds.z;
					glm::float32 bottom = glyph.m_Bounds.y + glyph.m_Bounds.w;
					
					addQuad(m_OutlineVertices, glm::vec2(x, y), m_OutlineColor, glyph, italic, m_OutlineThickness);

					minX = glm::min(minX, x + left - italic * bottom - m_OutlineThickness);
					maxX = glm::max(maxX, x + right - italic * top - m_OutlineThickness);
					minY = glm::min(minY, y + top - m_OutlineThickness);
					maxX = glm::max(maxY, y + bottom - m_OutlineThickness);
				}

				const Glyph& glyph = const_cast<Font*>(m_Font)->getGlyph(curChar, m_CharSize, bold);
				addQuad(m_Vertices, glm::vec2(x, y), m_FillColor, glyph, italic);

				if (m_OutlineThickness == 0)
				{
					glm::float32 left = glyph.m_Bounds.x;
					glm::float32 top = glyph.m_Bounds.y;
					glm::float32 right = glyph.m_Bounds.x + glyph.m_Bounds.z;
					glm::float32 bottom = glyph.m_Bounds.y + glyph.m_Bounds.w;
					
					minX = glm::min(minX, x + left - italic * bottom);
					maxX = glm::max(maxX, x + right - italic * bottom);
					minY = glm::min(minY, y + top);
					maxY = glm::max(maxY, y + bottom);
				}

				x += glyph.m_Advance;
			}

			if (underlined && (x > 0))
			{
				addLine(m_Vertices, x, y, m_FillColor, underlineOffset, underlineThickness);

				if (m_OutlineThickness != 0)
					addLine(m_OutlineVertices, x, y, m_OutlineColor, underlineOffset, underlineThickness, m_OutlineThickness);
			}

			if (strikeThrough && (x > 0))
			{
				addLine(m_Vertices, x, y, m_FillColor, strikeThroughOffset, underlineThickness);

				if (m_OutlineThickness != 0)
					addLine(m_OutlineVertices, x, y, m_OutlineColor, strikeThroughOffset, underlineThickness, m_OutlineThickness);
			}

			m_Bounds.x = minX;
			m_Bounds.y = minY;
			m_Bounds.z = maxX - minX;
			m_Bounds.w = maxY - minY;


			if (m_OutlineThickness != 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_Vbo[1]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(Text::Vertex) * m_OutlineVertices.size(), 0, GL_DYNAMIC_DRAW);
			}
			glBindBuffer(GL_ARRAY_BUFFER, m_Vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Text::Vertex) * m_Vertices.size(), 0, GL_DYNAMIC_DRAW);

	
			m_Update = false;
	}
	void Text::initialize()
	{
		if (!m_Vao)
		{
			glGenVertexArrays(1, &m_Vao);
			glBindVertexArray(m_Vao);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Text::Vertex), (GLvoid*)(0 * sizeof(glm::float32)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Text::Vertex), (GLvoid*)(2 * sizeof(glm::float32)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Text::Vertex), (GLvoid*)(4 * sizeof(glm::float32)));
		}
		if (!m_Vbo)
		{
			glGenBuffers(2, m_Vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_Vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Text::Vertex) * m_Vertices.size(), 0, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, m_Vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Text::Vertex) * m_OutlineVertices.size(), 0, GL_DYNAMIC_DRAW);
		}
		if (!m_Shader.m_Program)
		{
			m_Shader.loadFromFile("data/glsl/v_text_shader.glsl", core::Shader::Type::Vertex);
			m_Shader.loadFromFile("data/glsl/f_text_shader.glsl", core::Shader::Type::Fragment);
		}
	}
	void Text::addLine(std::vector<Text::Vertex>& vertices, const glm::float32& lineLength, const glm::float32& lineTop, const glm::u8vec4& color, const glm::float32& offset, const glm::float32& thickness, const glm::float32& outlineThickness)
	{
		glm::float32 top = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
		glm::float32 bottom = top + std::floor(thickness + 0.5f);

		vertices.push_back(Text::Vertex(glm::vec2(-outlineThickness, top - outlineThickness),				 glm::vec2(1, 1), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
		vertices.push_back(Text::Vertex(glm::vec2(lineLength + outlineThickness, top - outlineThickness),	 glm::vec2(1, 1), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
		vertices.push_back(Text::Vertex(glm::vec2(-outlineThickness, bottom + outlineThickness),			 glm::vec2(1, 1), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
		vertices.push_back(Text::Vertex(glm::vec2(-outlineThickness, bottom + outlineThickness),			 glm::vec2(1, 1), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
		vertices.push_back(Text::Vertex(glm::vec2(lineLength + outlineThickness, top - outlineThickness),	 glm::vec2(1, 1), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
		vertices.push_back(Text::Vertex(glm::vec2(lineLength + outlineThickness, bottom + outlineThickness), glm::vec2(1, 1), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
	}
	void Text::addQuad(std::vector<Text::Vertex>& vertices, const glm::vec2& position, const glm::u8vec4& color, const Glyph& glyph, const glm::float32& italic, const glm::float32& outlineThickness)
	{
		glm::float32 left = glyph.m_Bounds.x;
		glm::float32 top = glyph.m_Bounds.y;
		glm::float32 right = glyph.m_Bounds.x + glyph.m_Bounds.z;
		glm::float32 bottom = glyph.m_Bounds.y + glyph.m_Bounds.w;

		glm::float32 u1 = static_cast<float>(glyph.m_TextureRect.x);
		glm::float32 v1 = static_cast<float>(glyph.m_TextureRect.y);
		glm::float32 u2 = static_cast<float>(glyph.m_TextureRect.x + glyph.m_TextureRect.z);
		glm::float32 v2 = static_cast<float>(glyph.m_TextureRect.y + glyph.m_TextureRect.w);

		vertices.push_back(Text::Vertex(glm::vec2(position.x + left - italic * top - outlineThickness, position.y + top - outlineThickness),		 glm::vec2(u1, v1), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
		vertices.push_back(Text::Vertex(glm::vec2(position.x + right - italic * top - outlineThickness, position.y + top - outlineThickness),		 glm::vec2(u2, v1), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
		vertices.push_back(Text::Vertex(glm::vec2(position.x + left - italic * bottom - outlineThickness, position.y + bottom - outlineThickness),	 glm::vec2(u1, v2), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
		vertices.push_back(Text::Vertex(glm::vec2(position.x + left - italic * bottom - outlineThickness, position.y + bottom - outlineThickness),	 glm::vec2(u1, v2), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
		vertices.push_back(Text::Vertex(glm::vec2(position.x + right - italic * top - outlineThickness, position.y + top - outlineThickness),		 glm::vec2(u2, v1), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
		vertices.push_back(Text::Vertex(glm::vec2(position.x + right - italic * bottom - outlineThickness, position.y + bottom - outlineThickness),	 glm::vec2(u2, v2), glm::vec4(color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f)));
	}

}
#include "util.h"
#include "../core/image.h"


namespace me::util
{
	const core::Image perlin(const glm::float32 baseFreq, const glm::float32 persistence, const glm::int32 w, const glm::int32 h)
	{
		glm::int32 width = w;
		glm::int32 height = h;
		core::Image  data;
		data.create(w, h, glm::u8vec4(0, 0, 0, 255));
		glm::float32 xFactor = 1.f / (width - 1);
		glm::float32 yFactor = 1.f / (height - 1);

		for (glm::int32 row = 0; row < height; ++row)
		{
			for (glm::int32 col = 0; col < width; ++col)
			{
				glm::float32 x = xFactor * col;
				glm::float32 y = yFactor * row;
				glm::float32 sum = 0.f;
				glm::float32 freq = baseFreq;
				glm::float32 persist = persistence;
				for (glm::int32 oct = 0; oct < 4; ++oct)
				{
					glm::vec2 p(x * freq, y * freq);
					glm::float32 val = glm::perlin(p) * persist;

					sum += val;
					
					glm::float32 result = (sum + 1.f) / 2.f;
					result = glm::clamp(result, 0.f, 1.f);
					
					data.setPixel(row, col, glm::u8vec4(result * 255, result * 255, result * 255, 255));
					freq *= 2.f;
					persist *= persistence;
				}
			}
		}

		return data;
	}

	Vertex::Vertex()
		:
		m_Position(0.f, 0.f, 0.f),
		m_TexCoord(0.f, 0.f),
		m_Normal(0.f, 0.f, 0.f)
	{

	}
	Vertex::Vertex(const glm::vec3& position, const glm::vec2& texCoord, const glm::vec3& normal)
		:
		m_Position(position),
		m_TexCoord(texCoord),
		m_Normal(normal)
	{

	}
	Vertex::Vertex(const glm::float32& px, const glm::float32& py, const glm::float32& pz, const glm::float32& tx, const glm::float32& ty, const glm::float32& nx, const glm::float32& ny, const glm::float32& nz)
		:
		m_Position(px, py, pz),
		m_TexCoord(tx, ty),
		m_Normal(nx, ny, nz)
	{

	}
	Vertex::~Vertex()
	{

	}

	VertexArray::VertexArray()
	{
	}
	VertexArray::~VertexArray()
	{
		glDeleteBuffers(1, &m_VBO);
	}
	glm::size_t VertexArray::getSize()
	{
		return m_Vertices.size();
	}
	GLuint VertexArray::getVbo(bool bind)
	{
		if (bind)
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		return m_VBO;
	}
	const Vertex& VertexArray::const_at(glm::size_t position) const
	{
		if (position > m_Vertices.size()) return Vertex();
		return m_Vertices.at(position);
	}
	Vertex& VertexArray::at(glm::size_t position)
	{
		if (position > m_Vertices.size()) return Vertex();

		m_Loaded = false;
		return m_Vertices.at(position);
	}
	std::vector<Vertex>& VertexArray::getVertices()
	{
		m_Loaded = false;
		return m_Vertices;
	}
	bool VertexArray::isLoaded()
	{
		return m_Loaded;
	}
	bool VertexArray::initialize()
	{
		if(!m_VBO) glGenBuffers(1, &m_VBO);
		
		if (!m_Loaded && !m_Vertices.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(util::Vertex) * m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW);
			m_Loaded = true;
		}

		return true;
	}
	bool VertexArray::draw()
	{
		if (m_Vertices.empty())	return true;
		if (!m_Loaded) initialize();

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());

		return true;
	}

	RuntimeError::RuntimeError(const ErrorType type, const std::string& msg, const std::string& because) :
		m_ErrorTitles({ "Oh S***!", "Not Again...", "I was so close!", "Please don't worry.", "I'll be back soon",":(", "D:", "f***" }),
		m_ErrorType(type),
		m_Message(msg),
		m_Because(because)
	{

	}
	RuntimeError::RuntimeError(const ErrorType type, const std::string& msg) :
		m_ErrorTitles({ "Oh S***!", "Not Again...", "I was so close!", "Please don't worry.", "I'll be back soon",":(", "D:", "f***" }),
		m_ErrorType(type),
		m_Message(msg),
		m_Because("")
	{

	}
	RuntimeError::RuntimeError(const ErrorType type) : 
		m_ErrorTitles({ "Oh S***!", "Not Again...", "I was so close!", "Please don't worry.", "I'll be back soon",":(", "D:", "f***" }),
		m_ErrorType(type),
		m_Message(""),
		m_Because("")
	{

	}
	const std::string RuntimeError::errorTitle()
	{
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> ui(1, static_cast<int>(m_ErrorTitles.size()) - 1);

		return m_ErrorTitles.at(ui(rng));
	}
	const std::string RuntimeError::errorMessage()
	{
		std::string error_msg = "Failed to ";

		switch (m_ErrorType)
		{
		case ErrorType::Read:
			if (!m_Message.empty() && m_Because.empty()) error_msg += "Read '" + m_Message + "'!";
			else if (!m_Message.empty() && !m_Because.empty()) error_msg += "Read '" + m_Message + "' because '" + m_Because + "'!";
			else error_msg += "Read Something!";
			break;
		case ErrorType::Create:
			if (!m_Message.empty() && m_Because.empty()) error_msg += "Create '" + m_Message + "'!";
			else if (!m_Message.empty() && !m_Because.empty()) error_msg += "Create '" + m_Message + "' because '" + m_Because + "'!";
			else error_msg += "Create Something!";
			break;
		case ErrorType::Initialize:
			if (!m_Message.empty() && m_Because.empty()) error_msg += "Initialize '" + m_Message + "'!";
			else if (!m_Message.empty() && !m_Because.empty()) error_msg += "Initialize '" + m_Message + "' because '" + m_Because + "'!";
			else error_msg += "Initialize Something!";
			break;
		case ErrorType::Find:
			if (!m_Message.empty() && m_Because.empty()) error_msg += "Find '" + m_Message + "'!";
			else if (!m_Message.empty() && !m_Because.empty()) error_msg += "Find '" + m_Message + "' because '" + m_Because + "'!";
			else error_msg += "Find Something!";
			break;
		case ErrorType::Set:
			if (!m_Message.empty() && m_Because.empty()) error_msg += "Set '" + m_Message + "'!";
			else if (!m_Message.empty() && !m_Because.empty()) error_msg += "Set '" + m_Message + "' because '" + m_Because + "'!";
			else error_msg += "Set Something!";
			break;
		case ErrorType::Draw:
			if (!m_Message.empty() && m_Because.empty()) error_msg += "Draw '" + m_Message + "'!";
			else if (!m_Message.empty() && !m_Because.empty()) error_msg += "Draw '" + m_Message + "' because '" + m_Because + "'!";
			else error_msg += "Draw Something!";
			break;
		case ErrorType::Other:
		default:
			error_msg = "Melancholy has exited because of an unknown problem.";
			break;
		}
		error_msg += "\n\nTo Report a Problem, contact jellykisses260@gmail.com\nI'll do my best to respond as soon as possible!\n";

		return error_msg;
	}
	void RuntimeError::messageBox()
	{
		MessageBox(0, errorMessage().c_str(), errorTitle().c_str(), MB_OK | MB_ICONERROR);
	}

}
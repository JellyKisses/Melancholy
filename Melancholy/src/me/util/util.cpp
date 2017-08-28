#include "util.h"


namespace me::util
{
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
			else error_msg += "Read!";
			break;
		case ErrorType::Create:
			if (!m_Message.empty() && m_Because.empty()) error_msg += "Create '" + m_Message + "'!";
			else if (!m_Message.empty() && !m_Because.empty()) error_msg += "Create '" + m_Message + "' because '" + m_Because + "'!";
			else error_msg += "Create!";
			break;
		case ErrorType::Initialize:
			if (!m_Message.empty() && m_Because.empty()) error_msg += "Initialize '" + m_Message + "'!";
			else if (!m_Message.empty() && !m_Because.empty()) error_msg += "Initialize '" + m_Message + "' because '" + m_Because + "'!";
			else error_msg += "Initialize!";
			break;
		case ErrorType::Find:
			if (!m_Message.empty() && m_Because.empty()) error_msg += "Find '" + m_Message + "'!";
			else if (!m_Message.empty() && !m_Because.empty()) error_msg += "Find '" + m_Message + "' because '" + m_Because + "'!";
			else error_msg += "Find!";
			break;
		case ErrorType::Set:
			if (!m_Message.empty() && m_Because.empty()) error_msg += "Set '" + m_Message + "'!";
			else if (!m_Message.empty() && !m_Because.empty()) error_msg += "Set '" + m_Message + "' because '" + m_Because + "'!";
			else error_msg += "Set!";
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
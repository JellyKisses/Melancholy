#include "shader.h"


namespace me::core
{
	Shader::Shader()
	{

	}
	Shader::~Shader()
	{
		glDeleteProgram(m_Program);
	}

	const bool Shader::loadFromFile(std::string file, Type type)
	{
		switch (type)
		{
		case Type::Vertex:	 m_VertexCode = read(file); break;
		case Type::Fragment: m_FragmentCode = read(file); break;
		case Type::Geometry: m_GeometryCode = read(file); break;
		}

		if (!create())
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Shader: " + file);
			return false;
		}
		return true;

	}
	const bool Shader::loadFromMemory(std::string code, Type type)
	{
		switch (type)
		{
		case Type::Vertex: m_VertexCode = code; break;
		case Type::Fragment: m_FragmentCode = code; break;
		case Type::Geometry: m_GeometryCode = code; break;
		}

		if (!create())
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Shader from Memory");
			return false;
		}
		return true;

	}

	void Shader::addUniformI(const std::string& name,  glm::int32 x)
	{
		 const GLchar* _name = name.c_str();
		glUniform1i(glGetUniformLocation(m_Program, _name), x);
	}
	void Shader::addUniformU(const std::string& name,  glm::uint32 x)
	{
		const GLchar* _name = name.c_str();
		glUniform1ui(glGetUniformLocation(m_Program, _name), x);
	}
	void Shader::addUniformF(const std::string& name,  glm::mat4 mat4)
	{
		const GLchar* _name = name.c_str();
		glUniformMatrix4fv(glGetUniformLocation(m_Program, _name), 1, GL_FALSE, glm::value_ptr(mat4));
	}
	void Shader::addUniformF(const std::string& name,  glm::float32 x)
	{
		const GLchar* _name = name.c_str();
		glUniform1f(glGetUniformLocation(m_Program, _name), x);
	}
	void Shader::addUniformF(const std::string& name,  glm::vec2 vec2)
	{
		const GLchar* _name = name.c_str();
		glUniform2f(glGetUniformLocation(m_Program, _name), vec2.x, vec2.y);
	}
	void Shader::addUniformF(const std::string& name,  glm::float32 x, glm::float32 y)
	{
		const GLchar* _name = name.c_str();
		glUniform2f(glGetUniformLocation(m_Program, _name), x, y);
	}
	void Shader::addUniformF(const std::string& name,  glm::vec3 vec3)
	{
		const GLchar* _name = name.c_str();
		glUniform3f(glGetUniformLocation(m_Program, _name), vec3.x, vec3.y, vec3.z);
	}
	void Shader::addUniformF(const std::string& name,  glm::float32 x, glm::float32 y, glm::float32 z)
	{
		const GLchar* _name = name.c_str();
		glUniform3f(glGetUniformLocation(m_Program, _name), x, y, z);
	}
	void Shader::addUniformF(const std::string& name,  glm::vec4 vec4)
	{
		const GLchar* _name = name.c_str();
		glUniform4f(glGetUniformLocation(m_Program, _name), vec4.x, vec4.y, vec4.z, vec4.w);
	}
	void Shader::addUniformF(const std::string& name,  glm::float32 x, glm::float32 y, glm::float32 z, glm::float32 w)
	{
		const GLchar* _name = name.c_str();
		glUniform4f(glGetUniformLocation(m_Program, _name), x, y, z, w);
	}
	void Shader::addUniformsI(const std::string& name,  std::vector<glm::int32> x)
	{
		const GLchar* _name = name.c_str();
		glUniform1iv(glGetUniformLocation(m_Program, _name), x.size(), reinterpret_cast< glm::int32*>(x.data()));
	}
	void Shader::addUniformsU(const std::string& name,  std::vector<glm::uint32> x)
	{
		const GLchar* _name = name.c_str();
		glUniform1uiv(glGetUniformLocation(m_Program, _name), x.size(), reinterpret_cast< glm::uint32*>(x.data()));
	}
	void Shader::addUniformsF(const std::string& name,  std::vector<glm::float32> x)
	{
		const GLchar* _name = name.c_str();
		glUniform1fv(glGetUniformLocation(m_Program, _name), x.size(), reinterpret_cast< glm::float32*>(x.data()));
	}
	void Shader::addUniformsF(const std::string& name,  std::vector<glm::vec2> vec2)
	{
		const GLchar* _name = name.c_str();
		glUniform2fv(glGetUniformLocation(m_Program, _name), vec2.size(), reinterpret_cast< glm::float32*>(vec2.data()));
	}
	void Shader::addUniformsF(const std::string& name,  std::vector<glm::vec3> vec3)
	{
		const GLchar* _name = name.c_str();
		glUniform3fv(glGetUniformLocation(m_Program, _name), vec3.size(), reinterpret_cast< glm::float32*>(vec3.data()));
	}
	void Shader::addUniformsF(const std::string& name,  std::vector<glm::vec4> vec4)
	{
		const GLchar* _name = name.c_str();
		glUniform4fv(glGetUniformLocation(m_Program, _name), vec4.size(), reinterpret_cast< glm::float32*>(vec4.data()));
	}


	std::string Shader::read(std::string file)
	{
		std::stringstream buffer;
		std::ifstream in;

		in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			in.open(file);
			buffer << in.rdbuf();
			in.close();
		}
		catch (std::ifstream::failure e)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Read, file, e.what());
		}

		return buffer.str();
	}
	const bool Shader::create()
	{
		m_Program = 0;
		m_Vertex = 0;
		m_Fragment = 0;
		m_Geometry = 0;

		m_Program = glCreateProgram();
		if (!m_Program)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Shader Program");
			return false;
		}

		if (!m_VertexCode.empty())
		{
			m_Vertex = glCreateShader(GL_VERTEX_SHADER);
			if (!m_Vertex)
			{
				throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Vertex Shader");
				return false;
			}

			const GLchar* buf = m_VertexCode.c_str();
			glShaderSource(m_Vertex, 1, &buf, 0);
			glCompileShader(m_Vertex);

			GLint result;
			glGetShaderiv(m_Vertex, GL_COMPILE_STATUS, &result);
			if (!result)
			{
				GLchar log[1024];
				glGetShaderInfoLog(m_Vertex, 1024, 0, log);
				throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Vertex Shader", log);
				return false;
			}

			glAttachShader(m_Program, m_Vertex);
		}
		if (!m_FragmentCode.empty())
		{
			m_Fragment = glCreateShader(GL_FRAGMENT_SHADER);
			if (!m_Fragment)
			{
				throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Fragment Shader");
				return false;
			}

			const GLchar* buf = m_FragmentCode.c_str();
			glShaderSource(m_Fragment, 1, &buf, 0);
			glCompileShader(m_Fragment);

			GLint result;
			glGetShaderiv(m_Fragment, GL_COMPILE_STATUS, &result);
			if (!result)
			{
				GLchar log[1024];
				glGetShaderInfoLog(m_Fragment, 1024, 0, log);
				throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Fragment Shader", log);
				return false;
			}

			glAttachShader(m_Program, m_Fragment);
		}
		if (!m_GeometryCode.empty())
		{
			m_Geometry = glCreateShader(GL_GEOMETRY_SHADER);
			if (!m_Geometry)
			{
				throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Geometry Shader");
				return false;
			}

			const GLchar* buf = m_GeometryCode.c_str();
			glShaderSource(m_Geometry, 1, &buf, 0);
			glCompileShader(m_Geometry);

			GLint result;
			glGetShaderiv(m_Geometry, GL_COMPILE_STATUS, &result);
			if (!result)
			{
				GLchar log[1024];
				glGetShaderInfoLog(m_Geometry, 1024, 0, log);
				throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Geometry Shader", log);
				return false;
			}

			glAttachShader(m_Program, m_Geometry);
		}

		glLinkProgram(m_Program);
		int result;
		glGetProgramiv(m_Program, GL_LINK_STATUS, &result);
		if (!result)
		{
			throw util::RuntimeError(util::RuntimeError::ErrorType::Create, "Shader Program");
			return false;
		}

		glDetachShader(m_Program, m_Vertex);
		glDetachShader(m_Program, m_Fragment);
		glDetachShader(m_Program, m_Geometry);
		glDeleteShader(m_Vertex);
		glDeleteShader(m_Fragment);
		glDeleteShader(m_Geometry);

		unbind();

		return true;

	}
	void Shader::bind()
	{
		glUseProgram(m_Program);
	}
	void Shader::unbind()
	{
		glUseProgram(0);
	}

}
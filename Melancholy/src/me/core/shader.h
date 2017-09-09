#pragma once

#include "../util/util.h"
#include "texture.h"


namespace me::core
{
	class Shader
	{
	public:
		enum class Type
		{
			Vertex,
			Fragment,
			Geometry
		};

	public:
		GLuint m_Program;
		GLuint m_Vertex;
		GLuint m_Fragment;
		GLuint m_Geometry;
		std::string m_VertexCode;
		std::string m_FragmentCode;
		std::string m_GeometryCode;

	public:
		Shader();
		~Shader();

		bool loadFromFile(std::string file, Type type);
		bool loadFromMemory(std::string code, Type type);

		void addUniformI(const std::string& name, glm::int32 x);
		void addUniformU(const std::string& name, glm::uint32 x);
		void addUniformF(const std::string& name, glm::mat4 mat4);
		void addUniformF(const std::string& name, glm::float32 x);
		void addUniformF(const std::string& name, glm::vec2 vec2);
		void addUniformF(const std::string& name, glm::float32 x, glm::float32 y);
		void addUniformF(const std::string& name, glm::vec3 vec3);
		void addUniformF(const std::string& name, glm::float32 x, glm::float32 y, glm::float32 z);
		void addUniformF(const std::string& name, glm::vec4 vec4);
		void addUniformF(const std::string& name, glm::float32 x, glm::float32 y, glm::float32 z, glm::float32 w);
		void addUniformsI(const std::string& name,  std::vector<glm::int32> x);
		void addUniformsU(const std::string& name,  std::vector<glm::uint32> x);
		void addUniformsF(const std::string& name,  std::vector<glm::float32> x);
		void addUniformsF(const std::string& name,  std::vector<glm::vec2> vec2);
		void addUniformsF(const std::string& name,  std::vector<glm::vec3> vec3);
		void addUniformsF(const std::string& name,  std::vector<glm::vec4> vec4);

	public:
		void bind();
		void unbind();

	private:
		std::string read(std::string file);
		bool create();

	};
}
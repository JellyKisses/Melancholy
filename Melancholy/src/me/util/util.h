#pragma once

#include <windows.h>

#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/std_based_type.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H

static const std::string& FT_GetErrorString(FT_Error error)
{
#undef __FTERRORS_H_
#define FT_ERRORDEF( e, v, s ) case e: return s;
#define FT_ERROR_START_LIST switch(error) {
#define FT_ERROR_END_LIST }
#include FT_ERRORS_H
	return "Unknown";
}

#include <SOIL2/SOIL2.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <functional>
#include <algorithm>
#include <cctype>
#include <random>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <map>


namespace me::util
{
	static const std::string& to_lower(const std::string& str)
	{
		std::string data = str;
		std::transform(data.begin(), data.end(), data.begin(), tolower);
		return data;
	}

	class Vertex
	{
	public:
		Vertex();
		Vertex(const glm::vec3& position, const glm::vec2& texCoord, const glm::vec3& normal);
		Vertex(const glm::float32& px, const glm::float32& py, const glm::float32& pz, const glm::float32& tx, const glm::float32& ty, const glm::float32& nx, const glm::float32& ny, const glm::float32& nz);
		~Vertex();
	
	public:
		glm::vec3 m_Position;
		glm::vec2 m_TexCoord;
		glm::vec3 m_Normal;
	};

	class RuntimeError
	{
	public:
		const enum class ErrorType
		{
			Read,
			Create,
			Initialize,
			Find,
			Set,
			Other
		};

	private:
		const std::array<std::string, 8> m_ErrorTitles;
		const ErrorType m_ErrorType;
		const std::string m_Message;
		const std::string m_Because;

	public:
		RuntimeError(const ErrorType type, const std::string& msg, const std::string& because);
		RuntimeError(const ErrorType type, const std::string& msg);
		RuntimeError(const ErrorType type);

		void messageBox();

	private:
		const std::string errorTitle();
		const std::string errorMessage();

	};
}
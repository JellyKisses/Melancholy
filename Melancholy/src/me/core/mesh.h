#pragma once

#include "../util/util.h"


namespace me::core
{
	class Texture;
	class Shader;

	class Mesh
	{
	public:
		Mesh();
		Mesh(std::vector<util::Vertex> vertices, std::vector<glm::uint32> indices, std::vector<std::string> textures);
		~Mesh();

		bool draw(const glm::float64& delta);
		bool initialize();
		bool isLoaded();
		// Use if you do NOT wish to change the Vertices (faster)
		std::vector<util::Vertex> const_getVertices() const;
		// Use if you do NOT wish to change the Indices (faster)
		std::vector<glm::uint32> const_getIndices() const;
		// Use if you do NOT wish to change the Textures (faster)
		std::vector<std::string> const_getTextures() const;

		// Use if you DO wish to change the Vertices (slower)
		std::vector<util::Vertex>& getVertices();
		// Use if you DO wish to change the Indices (slower)
		std::vector<glm::uint32>& getIndices();
		// Use if you DO wish to change the Textures (slower)
		std::vector<std::string>& getTextures();

	private:
		GLuint m_VAO, m_VBO, m_EBO;
		std::vector<util::Vertex> m_Vertices;
		std::vector<glm::uint32> m_Indices;
		std::vector<std::string> m_Textures;
	
		bool m_Loaded;

	};
}
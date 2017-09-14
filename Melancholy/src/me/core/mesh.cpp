#include "mesh.h"
#include "texture.h"
#include "app.h"


namespace me::core
{
	Mesh::Mesh() : m_Loaded(false)
	{

	}
	Mesh::Mesh(std::vector<util::Vertex> vertices, std::vector<glm::uint32> indices, std::vector<std::string> textures) : m_Vertices(vertices), m_Indices(indices), m_Textures(textures), m_Loaded(false)
	{

	}
	Mesh::~Mesh()
	{
		m_Vertices.clear();
		m_Indices.clear();
		m_Textures.clear();

		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}

	bool Mesh::draw(const glm::float64& delta)
	{
		if (m_Vertices.empty() || m_Indices.empty()) return true;
		if (!m_Loaded) initialize();
		if (!m_VAO || !m_VBO || !m_EBO) return false;

		std::shared_ptr<Shader> shader = g_AppInstance->getScene()->getShader("world_shader");
		shader->bind();

		glm::size_t diffuseNr = 0;
		glm::size_t normalNr = 0;
		for (glm::size_t i = 0; i < m_Textures.size(); i++)
		{
			if (!g_Textures->exists(m_Textures[i]))
			{
				Texture tex;
				tex.loadFromFile(m_Textures[i], false, m_Textures[i].back == 'n' ? Texture::Type::Normal : Texture::Type::Diffuse);
				g_Textures->add(m_Textures[i], tex);
			}
			if (m_Textures[i].back == 'n')
			{
				shader->addUniformI("u_tex_norm[" + std::to_string(normalNr++) + "]", g_Textures->const_get(m_Textures[i]).getID());
			}
			//Diffuse
			else
			{
				shader->addUniformI("u_tex_diff[" + std::to_string(diffuseNr++) + "]", g_Textures->const_get(m_Textures[i]).getID());
			}
		}

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//glActiveTexture(GL_TEXTURE0);
		return true;
	}
	bool Mesh::initialize()
	{
		if (!m_Loaded)
		{
			if (!m_VAO) glGenVertexArrays(1, &m_VAO);
			if (!m_VBO) glGenBuffers(1, &m_VBO);
			if (!m_EBO) glGenBuffers(1, &m_EBO);

			glBindVertexArray(m_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(util::Vertex), &m_Vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(glm::uint32), &m_Indices[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(util::Vertex), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(util::Vertex), (GLvoid*)offsetof(util::Vertex, util::Vertex::m_TexCoord));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(util::Vertex), (GLvoid*)offsetof(util::Vertex, util::Vertex::m_Normal));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(util::Vertex), (GLvoid*)offsetof(util::Vertex, util::Vertex::m_Tangent));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(util::Vertex), (GLvoid*)offsetof(util::Vertex, util::Vertex::m_Bitangent));

			glBindVertexArray(0);

			m_Loaded = true;
		}
		return true;
	}
	bool Mesh::isLoaded()
	{
		return m_Loaded;
	}
	std::vector<util::Vertex> Mesh::const_getVertices() const
	{
		return m_Vertices;
	}
	std::vector<glm::uint32> Mesh::const_getIndices() const
	{
		return m_Indices;
	}
	std::vector<std::string> Mesh::const_getTextures() const
	{
		return m_Textures;
	}
	std::vector<util::Vertex>& Mesh::getVertices()
	{
		m_Loaded = false;
		return m_Vertices;
	}
	std::vector<glm::uint32>& Mesh::getIndices()
	{
		m_Loaded = false;
		return m_Indices;
	}
	std::vector<std::string>& Mesh::getTextures()
	{
		m_Loaded = false;
		return m_Textures;
	}
}
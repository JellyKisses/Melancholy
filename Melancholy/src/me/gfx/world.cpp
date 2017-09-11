#include "world.h"


namespace me::gfx
{
	World::World() : m_Loaded(false)
	{
		glGenVertexArrays(1, &m_VAO);
	}
	World::~World()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	bool World::draw(const glm::float64& delta)
	{
		if (!m_Loaded)
		{
			initialize();
		}

		glBindVertexArray(m_VAO);
		m_Floor.draw();
		//m_Walls.draw();
		
		return true;
	}
	bool World::initialize()
	{
		if (!m_Loaded)
		{
			glBindVertexArray(m_VAO);
			m_Floor.getVertices() = {
				{ { -3.f, 0.f, -3.f },{ 0.f, 0.f },{ 0.f, 1.f, 0.f } },
				{ {  3.f, 0.f, -3.f },{ 1.f, 0.f },{ 0.f, 1.f, 0.f } },
				{ {  3.f, 0.f,  3.f },{ 1.f, 1.f },{ 0.f, 1.f, 0.f } },

				{ {  3.f, 0.f,  3.f },{ 1.f, 1.f },{ 0.f, 1.f, 0.f } },
				{ { -3.f, 0.f,  3.f },{ 0.f, 1.f },{ 0.f, 1.f, 0.f } },
				{ { -3.f, 0.f, -3.f },{ 0.f, 0.f },{ 0.f, 1.f, 0.f } }
			};
			m_Floor.initialize();
			m_Walls.initialize();

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(util::Vertex), (GLvoid*)(0 * sizeof(glm::float32)));
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(util::Vertex), (GLvoid*)(3 * sizeof(glm::float32)));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(util::Vertex), (GLvoid*)(5 * sizeof(glm::float32)));


			m_Loaded = true;
		}

		return true;
	}
	bool World::isLoaded()
	{
		return m_Loaded;
	}
}
